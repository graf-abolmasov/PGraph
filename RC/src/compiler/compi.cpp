#include <QtCore>
#include <QPixmap>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compi.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/qgraphsettings.h"

QSet<QString> globalVirtualGraphs;

GraphCompiler::GraphCompiler(const Graph &graph) :
    myGraph(graph)
{
    initDirectories();
}

void GraphCompiler::initDirectories()
{
    myOutputDirectory = QGraphSettings::getOutputDirectory();
    myBaseDirectory = QGraphSettings::getBaseDirectory();
    myTemplateDirectory = QGraphSettings::getTemplateDirectory();
    myOtherFilesList = globalDBManager->getOtherFilesDB();
}

GraphCompiler::GraphCompiler(const Graph &graph, QSet<QString> &skip) :
    myGraph(graph), mySkipList(skip)
{
    initDirectories();
}

QStringList GraphCompiler::compileRecursively(const Graph &graph)
{
    QStringList res;
    if (compiledGraphs.contains(graph.name))
        return res;

    globalLogger->log(QObject::tr("Компиляция агрегата %1:%2.....").arg(graph.name).arg(graph.extName), Logger::Compile);
    QStringList errors = graph.validate();
    if (!errors.isEmpty()) {
        res.append(errors);
        return res;
    }

    compileStruct(graph);
    compiledGraphs.insert(graph.name);
    actors.insert(globalDBManager->getGraph(graph.name));

    QList<Top> topList = graph.topList;
    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        if (top.actor->type == Actor::GraphType)
            res.append(compileRecursively(globalDBManager->getGraphDB(top.actor->name)));
    }
    return res;
}


void GraphCompiler::copyUsedFiles()
{
    // копируем файлы используемых акторов и предикатов в выходной каталог
    foreach (const Actor *actor, actors) {
        if (actor->type == Actor::GraphType)
            continue;
        actor->build();
        const QString f = myBaseDirectory + "/" + actor->name + ".cpp";
        Q_ASSERT(QFile::exists(f));
        const QString nf = myOutputDirectory + "/" + actor->name + ".cpp";
        QFile::remove(nf);
        QFile::copy(f, nf);
    }
    foreach (const Predicate *predicate, predicates) {
        predicate->build();
        const QString f = myBaseDirectory + "/" + predicate->name + ".cpp";
        Q_ASSERT(QFile::exists(f));
        const QString nf = myOutputDirectory + "/" + predicate->name + ".cpp";
        QFile::remove(nf);
        QFile::copy(f, nf);
    }
    foreach (const BaseModule *baseModule, baseModules) {
        const QString f1 = myBaseDirectory + "/" + baseModule->name + ".c";
        const QString f2 = myBaseDirectory + "/" + baseModule->uniqName + ".cpp";
        Q_ASSERT(QFile::exists(f1));
        Q_ASSERT(QFile::exists(f2));
        const QString nf1 = myOutputDirectory + "/" + baseModule->name + ".c";
        const QString nf2 = myOutputDirectory + "/" + baseModule->uniqName + ".cpp";
        QFile::remove(nf1);
        QFile::remove(nf2);
        QFile::copy(f1, nf1);
        QFile::copy(f2, nf2);
    }
}

void GraphCompiler::copyStaticTemplates()
{
    // копируем необходимые файлы
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graph.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/memman.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graph.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/property.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/memman.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graphmv.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/Makefile.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/runme.bat.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/stype.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/defines.h.template"));

    QFile::remove(myOutputDirectory + "/graph.h");
    QFile::remove(myOutputDirectory + "/memman.h");
    QFile::remove(myOutputDirectory + "/graph.cpp");
    QFile::remove(myOutputDirectory + "/property.h");
    QFile::remove(myOutputDirectory + "/memman.cpp");
    QFile::remove(myOutputDirectory + "/graphmv.cpp");
    QFile::remove(myOutputDirectory + "/Makefile");
    QFile::remove(myOutputDirectory + "/runme.bat");
    QFile::remove(myOutputDirectory + "/stype.h");
    QFile::copy(myTemplateDirectory + "/graph.h.template", myOutputDirectory + "/graph.h");
    QFile::copy(myTemplateDirectory + "/memman.h.template", myOutputDirectory + "/memman.h");
    QFile::copy(myTemplateDirectory + "/graph.cpp.template", myOutputDirectory + "/graph.cpp");
    QFile::copy(myTemplateDirectory + "/property.h.template", myOutputDirectory + "/property.h");
    QFile::copy(myTemplateDirectory + "/memman.cpp.template", myOutputDirectory + "/memman.cpp");
    QFile::copy(myTemplateDirectory + "/graphmv.cpp.template", myOutputDirectory + "/graphmv.cpp");
    QFile::copy(myTemplateDirectory + "/Makefile.template", myOutputDirectory + "/Makefile");
    QFile::copy(myTemplateDirectory + "/runme.bat.template", myOutputDirectory + "/runme.bat");
    QFile::copy(myTemplateDirectory + "/stype.h.template", myOutputDirectory + "/stype.h");
    QFile::copy(myTemplateDirectory + "/defines.h.template", myOutputDirectory + "/defines.h");

    foreach (const QString  name, myOtherFilesList)
        QFile::copy(myBaseDirectory + "/" + name, myOutputDirectory + "/" + name);
}

bool GraphCompiler::compile()
{
    QTime t;
    t.start();
    QStringList errors = compileRecursively(myGraph);
    if (!errors.isEmpty()) {
        globalLogger->log(errors, Logger::Warning);
        globalLogger->log(QObject::tr("Компиляция провалилась. Всего %1 ошибок.").arg(QString::number(errors.count())), Logger::Warning);
        return false;
    }
    compileMakefile("debug");
    compileMakefile("release");
    compileMain();
    copyUsedFiles();
    copyStaticTemplates();
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
    return true;
}

void GraphCompiler::compileStruct(const Graph &graph)
{
    // Делаем <#graphname>.cpp
    QList<Top> topList = graph.topList;
    QList<Arc> arcList = graph.arcList;

    //Только мои предикаты
    QSet<const Predicate *> myPredicatesSet;
    foreach (Arc arc, arcList) {
        Q_ASSERT(arc.predicate != NULL);
        myPredicatesSet.insert(arc.predicate);
        if (arc.predicate->type == Predicate::NormalType) {
            Q_ASSERT(arc.predicate->baseModule != NULL);
            baseModules.insert(arc.predicate->baseModule);
        }
    }
    const QList<const Predicate *> myPredicatesList = myPredicatesSet.toList();
    //добавим мои ко всем
    predicates.unite(myPredicatesSet);

    //Только мои акторы
    QSet<const Actor *> myActorsSet;
    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        myActorsSet.insert(top.actor);
        if (top.actor->type == Actor::NormalType) {
            Q_ASSERT(top.actor->baseModule != NULL);
            baseModules.insert(top.actor->baseModule);
        }
    }
    const QList<const Actor *> myActorsList = myActorsSet.toList();
    //добавим мои ко всем
    actors.unite(myActorsSet);

    //делаем заголовки для предикатов
    QString predicateStr;
    foreach (const Predicate *predicate, myPredicatesList)
        predicateStr.append("int " + predicate->name + "(TPOData *D);\r\n");

    // делаем заголовки файл для акторов
    QString actorStr;
    foreach (const Actor *actor, myActorsList)
        actorStr.append("int " + actor->name + "(TPOData *D);\r\n");

    // список _ListP
    QString _ListP;
    _ListP.append("static DefinePredicate ListPred[" + QString::number(myPredicatesList.size()) + "] = {\r\n");
    QStringList list;
    foreach (const Predicate *predicate, myPredicatesList)
        list << "\tDefinePredicate(\"" + predicate->name + "\", &" + predicate->name + ")";
    _ListP.append(list.join(",\r\n"));
    _ListP.append("\r\n};\r\n");

    // Список _ListP и _ListGraph
    QString _ListGraph;
    QStringList listGraph;
    int maxTopNum = 0;
    foreach (Top top, topList)
        maxTopNum = top.number > maxTopNum ? top.number : maxTopNum;

    QVector<QString> vec(maxTopNum+1);
    QStringList virtualTops;
    QStringList virtualGraphs;
    _ListGraph.append("static DefineGraph ListGraf[" + QString::number(arcList.size()) + "] = {\r\n");
    foreach (Top top, topList) {
        QList<Arc> outArcs = graph.getOutArcs(top.number);
        qSort(outArcs.begin(), outArcs.end(), orderArcByPriorityAsc);
        const bool isTailTop = outArcs.count() == 0;
        const int first = isTailTop ? -77 : listGraph.count();
        const int last = isTailTop ? -77 : first + outArcs.count()-1;

        foreach (Arc arc, outArcs) {
            int endTop = arc.endTop;
            if (arc.type == Arc::ParallelArc) {
                const QString virtualGraphName = "V" + graph.name + "_" + QString::number(arc.endTop);
                globalVirtualGraphs.insert(virtualGraphName);
                const QString virtualGraphExtName = QString("Virtual graph for %1 top").arg(QString::number(arc.endTop));
                virtualTops << "\tDefineTop(\"" + virtualGraphName + "\", " + QString::number(-77) + ", " + QString::number(-77) + ", &" + virtualGraphName + ")";
                virtualGraphs << buildGraph(virtualGraphName, virtualGraphExtName, arc.endTop);
                actorStr.append("int " + virtualGraphName + "(TPOData *D);\r\n");
                endTop = maxTopNum+virtualTops.size();
            }
            listGraph << "\tDefineGraph(" + QString::number(myPredicatesList.indexOf(arc.predicate)) + ", " + QString::number(endTop) + ", " + QString::number(arc.type) + ")";
        }
        vec[top.number] = "\tDefineTop(\"" + top.actor->name + "\", " + QString::number(first) + ", " + QString::number(last) + ", &" + top.actor->name + ")";
    }
    _ListGraph.append(listGraph.join(",\r\n"));
    _ListGraph.append("\r\n};\r\n");

    QStringList listTop;
    foreach (QString virtualTop, virtualTops)
        vec.append(virtualTop);
    foreach (QString deftop, vec)
        listTop.append(deftop.isEmpty() ? "\tDefineTop(\"GHOST TOP\", -77, -77, NULL)" : deftop);
    QString _ListT;
    _ListT.append("static DefineTop ListTop[" + QString::number(vec.size()) + "] = {\r\n");
    _ListT.append(listTop.join(",\r\n"));
    _ListT.append("\r\n};\r\n");

    QString main;
    main.append("#include \"graph.h\"\r\n");
    main.append("PROJECT_BEGIN_NAMESPACE\r\n");
    main.append(predicateStr);
    main.append(actorStr);

    main.append(_ListP);
    main.append(_ListT);
    main.append(_ListGraph);

    main.append(buildGraph(graph.name, graph.extName, graph.getRootTop()));
    main.append(virtualGraphs.join("\r\n"));
    main.append("PROJECT_END_NAMESPACE\r\n");

    QFile f(myOutputDirectory + "/" + graph.name + ".cpp");
    f.open(QFile::WriteOnly);
    f.write(main.toUtf8());
    f.close();
}

QString GraphCompiler::buildGraph(const QString &name, const QString &extName, int root) const
{
    QString result;
    result.append("int " + name + "(TPOData *D)\r\n");
    result.append("{\r\n");
    result.append("\t//" + extName + "\r\n");
    result.append("\t//printf(\"" + extName + "\\r\\n\");\r\n");
    result.append("\tint topCount = " + QString::number(0) + "; //never used parameter\r\n");
    result.append("\tint rootTop = " + QString::number(root) + ";\r\n");
    result.append("\tGraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);\r\n");
    result.append("\treturn 1;\r\n");
    result.append("}\r\n");
    return result;
}

void GraphCompiler::compileMakefile(QString target) const
{
    target = target.toLower();
    QStringList objects;
    QStringList sources;
    QStringList targets;

    QStringList names;
    names << "main" << "graphmv" << "tpodata" << "graph" << "memman";
    foreach (const Predicate *predicate, predicates)
        names.append(predicate->name);
    foreach (const Actor *actor, actors)
        names.append(actor->name);
    foreach (const BaseModule *baseModule, baseModules)
        names.append(baseModule->uniqName);
    foreach (QString name, names) {
        objects.append(target + "/" + name + ".o");
        sources.append(name + ".cpp");
        targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".cpp" );
    }
    foreach (const BaseModule *baseModule, baseModules) {
        objects.append(target + "/" + baseModule->name + ".o");
        sources.append(baseModule->name + ".c");
        targets.append(target + "/" + baseModule->name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + QDir::separator() + baseModule->name + ".o " + baseModule->name + ".c" );
    }

    foreach (QString  name, myOtherFilesList) {
        if (name.endsWith(".c")) {
            name = name.left(name.lastIndexOf(".c"));
            objects.append(target + "/" + name + ".o");
            targets.append(target + "/" + name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".c" );
            continue;
        }
        if (name.endsWith(".cpp")) {
            name = name.left(name.lastIndexOf(".cpp"));
            objects.append(target + "/" + name + ".o");
            targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".cpp" );
            continue;
        }
    }

    QString result = getTemplate(myTemplateDirectory + "/" + "Makefile." + target + ".template");
    if (result.isEmpty())
        globalLogger->log(QObject::tr(ERR_GRAPHCOMPI_EMPTY_TEMPL).arg("Makefile." + target), Logger::Warning);
    result.replace("<#dateTime>", QDateTime::currentDateTime().toString());
    result.replace("<#project>", "Project" + QString::number(globalDBManager->getProjectId()));
    result.replace("<#objects>", objects.join(" \\\r\n\t\t"));
    result.replace("<#sources>", sources.join(" \\\r\n\t\t"));
    result.replace("<#targets>", targets.join("\r\n\r\n"));

    QFile f(myOutputDirectory + "/" + "Makefile." + target);
    f.open(QFile::WriteOnly);
    f.write(result.toUtf8());
    f.close();
}

void GraphCompiler::compileMain() const
{
    QString result = getTemplate(myTemplateDirectory + "/" + "main.cpp.template");
    if (result.isEmpty())
        globalLogger->log(QObject::tr(ERR_GRAPHCOMPI_EMPTY_TEMPL).arg("main.cpp"), Logger::Warning);
    QStringList allActors;
    QStringList getFuncAddrByName;
    foreach (const Actor *actor, actors) {
        allActors.append("int " + actor->name + "(TPOData *D);");
        getFuncAddrByName.append(QString("if (strcmp(\"%1\", name) == 0) return &%1;").arg(actor->name));
    }

    foreach (QString virtualGraphName, globalVirtualGraphs) {
        allActors.append("int " + virtualGraphName + "(TPOData *D);");
        getFuncAddrByName.append(QString("if (strcmp(\"%1\", name) == 0) return &%1;").arg(virtualGraphName));
    }

    result.replace("<#getFuncAddrByName>", getFuncAddrByName.join("\r\n"));
    result.replace("<#allActors>", allActors.join("\r\n"));
    result.replace("<#graphname>", myGraph.name);

    QFile f(myOutputDirectory + "/" + "main.cpp");
    f.open(QFile::WriteOnly);
    f.write(result.toUtf8());
    f.close();
}


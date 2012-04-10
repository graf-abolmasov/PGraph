#include <QtCore>
#include <QPixmap>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compi.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/qgraphsettings.h"

#include "../../src/compiler/vcompi/vcompiwrapper.h"

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

/*!
  Рекурсивный подъем и компиляция всех используемых графов.
  */
void GraphCompiler::unpackGraph(const Graph &graph, QSet<const Predicate *> &predicates, QSet<const Actor *> &actors, QSet<QString> &exrtractedGraphs)
{
    QList<Top> topList = graph.topList;
    QList<Arc> arcList = graph.arcList;
    exrtractedGraphs.insert(graph.name);
    actors.insert(globalDBManager->getGraph(graph.name));
    foreach (Arc arc, arcList) {
        Q_ASSERT(arc.predicate != NULL);
        predicates.insert(arc.predicate);
    }
    // только используемые акторы
    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        actors.insert(top.actor);
    }
    foreach (const Actor *actor, actors) {
        if (actor->type == Actor::GraphType && !exrtractedGraphs.contains(actor->name)) {
            Graph newGraph = globalDBManager->getGraphDB(actor->name);
            GraphCompiler c(newGraph, mySkipList);
            if (c.compile())
                unpackGraph(newGraph, predicates, actors, exrtractedGraphs);
        }
    }
}

void GraphCompiler::collectUsedData()
{
    QSet<const Predicate *> predicates;
    QSet<const Actor *> actors;
    QSet<QString> exrtractedGraphs;
    unpackGraph(myGraph, predicates, actors, exrtractedGraphs);
    usedPredicateList = predicates.toList();
    usedActorList = actors.toList();
    foreach (const Predicate *predicate, usedPredicateList) {
        Q_ASSERT(predicate != NULL);
        if (predicate->type == Predicate::InlineType)
            continue;
        if (!usedBaseModuleList.contains(predicate->baseModule))
            usedBaseModuleList.append(predicate->baseModule);
    }
    foreach (const Actor *actor, usedActorList) {
        Q_ASSERT(actor != NULL);
        if (actor->type == Actor::InlineType || actor->type == Actor::GraphType)
            continue;
        Q_ASSERT(actor->baseModule != NULL);
        if (!usedBaseModuleList.contains(actor->baseModule))
            usedBaseModuleList.append(actor->baseModule);
    }
}

void GraphCompiler::copyUsedFiles()
{
    // копируем файлы используемых акторов и предикатов в выходной каталог
    foreach (const Actor *actor, usedActorList) {
        if (actor->type == Actor::GraphType)
            continue;
        const QString f = myBaseDirectory + "/" + actor->name + ".cpp";
        if (!QFile::exists(f))
            actor->build();
        Q_ASSERT(QFile::exists(f));
        const QString nf = myOutputDirectory + "/" + actor->name + ".cpp";
        QFile::remove(nf);
        QFile::copy(f, nf);
    }
    foreach (const Predicate *predicate, usedPredicateList) {
        const QString f = myBaseDirectory + "/" + predicate->name + ".cpp";
        if (!QFile::exists(f))
            predicate->build();
        Q_ASSERT(QFile::exists(f));
        const QString nf = myOutputDirectory + "/" + predicate->name + ".cpp";
        QFile::remove(nf);
        QFile::copy(f, nf);
    }
    foreach (const BaseModule *baseModule, usedBaseModuleList) {
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
    globalLogger->log(QObject::tr("Компиляция агрегата %1:%2.....").arg(myGraph.name).arg(myGraph.extName), Logger::Compile);
    QStringList errors = myGraph.validate();
    if (!errors.isEmpty()) {
        foreach (QString error, errors)
            globalLogger->log(error, Logger::Warning);
        globalLogger->log(QObject::tr("Компиляция провалилась. Всего %1 ошибок.").arg(QString::number(errors.count())), Logger::Warning);
        return false;
    }
    if (mySkipList.contains(myGraph.name))
        return true;
    mySkipList.insert(myGraph.name);

    collectUsedData();
    compileStruct();
    compileMakefile("debug");
    compileMakefile("release");
    compileMain();
    copyUsedFiles();
    copyStaticTemplates();
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
    return true;
}

void GraphCompiler::compileStruct() const
{
    // Делаем <#graphname>.cpp
    QList<Top> topList = myGraph.topList;
    QList<Arc> arcList = myGraph.arcList;

    // делаем заголовки для предикатов
    QString predicateStr;
    foreach (const Predicate *predicate, usedPredicateList)
        predicateStr.append("int " + predicate->name + "(TPOData *D);\r\n");

    // делаем заголовки файл для акторов
    QString actorStr;
    foreach (const Actor *actor, usedActorList)
        actorStr.append("int " + actor->name + "(TPOData *D);\r\n");

    // список _ListP
    QString ListPredString;
    ListPredString.append("static DefinePredicate ListPred[" + QString::number(usedPredicateList.size()) + "] = {\r\n");
    QStringList _ListP;
    foreach (const Predicate *predicate, usedPredicateList)
        _ListP << "\tDefinePredicate(\"" + predicate->name + "\", &" + predicate->name + ")";
    ListPredString.append(_ListP.join(",\r\n"));
    ListPredString.append("\r\n};\r\n");

    // Список _ListT и _ListGraph
    QList<DefGrf> _listGraph;
    QStringList listGraph;
    QVector<QString> vec(topList.size());
    QVector<CompTop> _vec(topList.size());
    int index = 0;
    int maxLT = 0;
    foreach (Top top, topList) {
        QList<Arc> outArcs = myGraph.getOutArcs(top.number);
        qSort(outArcs.begin(), outArcs.end(), orderArcByPriorityAsc);
        const bool isTailTop = outArcs.count() == 0;
        const int first = isTailTop ? -77 : listGraph.count();
        const int last = isTailTop ? -77 : first + outArcs.count()-1;
        bool isV = false;
        foreach (Arc arc, outArcs) {
            listGraph << "\tDefineGraph(" + QString::number(usedPredicateList.indexOf(arc.predicate)) + ", " + QString::number(arc.endTop) + ")";

            DefGrf lg;

            //Заполняем структуру
            lg.ArcType_ = arc.type;
            lg.NambPred = usedPredicateList.indexOf(arc.predicate);
            lg.NambTop = arc.endTop;
            lg.NTop = -1;
            lg.Fl = -1;
            lg.F = 0;
            lg.I = -1;
            lg.Name = myGraph.name;
            lg.CodeTr = "0.I";

            _listGraph.append(lg);

            if (arc.type == Arc::ParallelArc)
                isV = true;
        }
        if (vec.size() <= top.number)
            vec.resize(top.number + 1);

        if (_vec.size() <= top.number)
            _vec.resize(top.number + 1);

        vec[top.number] = "\tDefineTop(\"" + top.actor->name + "\", " + QString::number(first) + ", " + QString::number(last) + ", &" + top.actor->name + ")";

        _vec[top.number].Name = myGraph.name;
        _vec[top.number].NameProt = top.actor->name;
        _vec[top.number].FirstDef = first;
        _vec[top.number].LastDef = last;
        _vec[top.number].F = 0;
        _vec[top.number].Faz = -1;
        _vec[top.number].back = 0;
        _vec[top.number].rankT = 0;
        _vec[top.number].Top = top.number;
        _vec[top.number].CodeTr = "0.I";

        maxLT++;
        if (isV) maxLT++;
    }

    QList<CompTop> _listTop = _vec.toList();
    vcompi::VcompyWrapper::vcompy(_listTop, _listGraph, maxLT, _listGraph.size());

    QStringList listTop;
    foreach (QString deftop, vec)
        listTop.append(deftop.isEmpty() ? "\tDefineTop(\"GHOST TOP\", -77, -77, NULL)" : deftop);

    QString ListTopString;
    ListTopString.append("static DefineTop ListTop[" + QString::number(vec.size()) + "] = {\r\n");
    ListTopString.append(listTop.join(",\r\n"));
    ListTopString.append("\r\n};\r\n");

    QString ListGrafString;
    ListGrafString.append("static DefineGraph ListGraf[" + QString::number(arcList.size()) + "] = {\r\n");
    ListGrafString.append(listGraph.join(",\r\n"));
    ListGrafString.append("\r\n};\r\n");

    QString main;
    main.append("#include \"graph.h\"\r\n");
    main.append("PROJECT_BEGIN_NAMESPACE\r\n");
    main.append(predicateStr);
    main.append(actorStr);

    main.append(ListPredString);
    main.append(ListTopString);
    main.append(ListGrafString);

    main.append("int " + myGraph.name + "(TPOData *D)\r\n");
    main.append("{\r\n");
    main.append("\t//" + myGraph.extName + "\r\n");
    main.append("\t//printf(\"" + myGraph.extName + "\\r\\n\");\r\n");
    main.append("\tint topCount = " + QString::number(myGraph.topList.count()) + ";\r\n");
    main.append("\tint rootTop = " + QString::number(myGraph.getRootTop()) + ";\r\n");
    main.append("\tGraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);\r\n");
    main.append("\treturn 1;\r\n");
    main.append("}\r\n");
    main.append("PROJECT_END_NAMESPACE\r\n");

    QFile f(myOutputDirectory + "/" + myGraph.name + ".cpp");
    f.open(QFile::WriteOnly);
    f.write(main.toUtf8());
    f.close();
}

void GraphCompiler::compileMakefile(QString target) const
{
    target = target.toLower();
    QStringList objects;
    QStringList sources;
    QStringList targets;

    QStringList names;
    names << "main" << "graphmv" << "tpodata" << "graph" << "memman";
    foreach (const Predicate *predicate, usedPredicateList)
        names.append(predicate->name);
    foreach (const Actor *actor, usedActorList)
        names.append(actor->name);
    foreach (const BaseModule *baseModule, usedBaseModuleList)
        names.append(baseModule->uniqName);
    foreach (QString name, names) {
        objects.append(target + "/" + name + ".o");
        sources.append(name + ".cpp");
        targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".cpp" );
    }
    foreach (const BaseModule *baseModule, usedBaseModuleList) {
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
    result.replace("<#graphname>", myGraph.name);

    QFile f(myOutputDirectory + "/" + "main.cpp");
    f.open(QFile::WriteOnly);
    f.write(result.toUtf8());
    f.close();
}


#include <QtCore>
#include <QtGui>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compi.h"
#include "globalvariables.h"
#include "commonutils.h"
#include "qgraphsettings.h"

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
    foreach (Arc arc, arcList)
        predicates.insert(arc.predicate);
    // только используемые акторы
    foreach (Top top, topList)
        actors.insert(top.actor);
    foreach (const Actor *actor, actors)
        if (actor->type == Actor::GraphType && !exrtractedGraphs.contains(actor->name)) {
            Graph newGraph = globalDBManager->getGraphDB(actor->name);
            GraphCompiler c(newGraph, mySkipList);
            c.compile();
            unpackGraph(newGraph, predicates, actors, exrtractedGraphs);
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
        if (!usedBaseModuleList.contains(actor->baseModule))
            usedBaseModuleList.append(actor->baseModule);
    }
}

void GraphCompiler::copyUsedFiles()
{
    // копируем файлы используемых акторов и предикатов в выходной каталог
    foreach (const Actor *actor, usedActorList) {
        QFile::copy(myBaseDirectory + "/" + actor->name + ".cpp", myOutputDirectory + "/" + actor->name + ".cpp");
    }
    foreach (const Predicate *predicate, usedPredicateList) {
        QFile::copy(myBaseDirectory + "/" + predicate->name + ".cpp", myOutputDirectory + "/" + predicate->name + ".cpp");
    }
    foreach (const BaseModule *baseModule, usedBaseModuleList) {
        QFile::copy(myBaseDirectory + "/" + baseModule->name + ".c", myOutputDirectory + "/" + baseModule->name + ".c");
        QFile::copy(myBaseDirectory + "/" + baseModule->uniqName + ".cpp", myOutputDirectory + "/" + baseModule->uniqName + ".cpp");
    }
}

void GraphCompiler::copyStaticTemplates()
{
    // копируем необходимые файлы
    QFile::copy(myTemplateDirectory + "/graph.h.template", myOutputDirectory + "/graph.h");
    QFile::copy(myTemplateDirectory + "/memman.h.template", myOutputDirectory + "/memman.h");
    QFile::copy(myTemplateDirectory + "/graph.cpp.template", myOutputDirectory + "/graph.cpp");
    QFile::copy(myTemplateDirectory + "/property.h.template", myOutputDirectory + "/property.h");
    QFile::copy(myTemplateDirectory + "/memman.cpp.template", myOutputDirectory + "/memman.cpp");
    QFile::copy(myTemplateDirectory + "/graphmv.cpp.template", myOutputDirectory + "/graphmv.cpp");
    QFile::copy(myTemplateDirectory + "/Makefile.template", myOutputDirectory + "/Makefile");
    QFile::copy(myTemplateDirectory + "/runme.bat.template", myOutputDirectory + "/runme.bat");
}

void GraphCompiler::compile()
{
    if (mySkipList.contains(myGraph.name))
        return;
    mySkipList.insert(myGraph.name);
    QTime t;
    t.start();
    globalLogger->log(QObject::tr("Компиляция агрегата %1:%2.....").arg(myGraph.name).arg(myGraph.extName), Logger::Compile);
    QStringList errors = myGraph.validate();
    if (!errors.isEmpty()) {
        foreach (QString error, errors)
            globalLogger->log(error, Logger::Warning);
        globalLogger->log(QObject::tr("Компиляция провалилась. Всего %1 ошибок.").arg(QString::number(errors.count())), Logger::Warning);
        return;
    }

    collectUsedData();
    compileStruct();
    compileMakefile("debug");
    compileMakefile("release");
    compileMain();
    copyUsedFiles();
    copyStaticTemplates();
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
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
    QString _ListP;
    _ListP.append("static DefinePredicate ListPred[" + QString::number(usedPredicateList.size()) + "] = {\r\n");
    QStringList list;
    foreach (const Predicate *predicate, usedPredicateList)
        list << "DefinePredicate(\"" + predicate->name + "\", &" + predicate->name + ")";
    _ListP.append(list.join(",\r\n"));
    _ListP.append("\r\n};\r\n");

    // Список _ListP и _ListGraph
    QString _ListT;
    QString _ListGraph;
    QStringList listTop;
    QStringList listGraph;
    _ListT.append("static DefineTop ListTop[" + QString::number(topList.size()) + "] = {\r\n");
    _ListGraph.append("static DefineGraph ListGraf[" + QString::number(arcList.size()) + "] = {\r\n");
    foreach (Top top, topList) {
        QList<Arc> outArcs = myGraph.getOutArcs(top.number);
        qSort(outArcs.begin(), outArcs.end(), orderArcByPriorityAsc);
        const bool isTailTop = outArcs.count() == 0;
        const int first = isTailTop ? -77 : listGraph.count();
        const int last = isTailTop ? -77 : first + outArcs.count();
        foreach (Arc arc, outArcs) {
            listGraph << "DefineGraph(" + QString::number(usedPredicateList.indexOf(arc.predicate)) + ", " + QString::number(arc.endTop) + ")";
        }
        listTop << "DefineTop(\"" + top.actor->name + "\", " + QString::number(first) + ", " + QString::number(last) + ", &" + top.actor->name + ")";
    }
    _ListT.append(listTop.join(",\r\n"));
    _ListT.append("\r\n};\r\n");
    _ListGraph.append(listGraph.join(",\r\n"));
    _ListGraph.append("\r\n};\r\n");

    QString main;
    main.append("#include \"graph.h\"\r\n");
    main.append(predicateStr);
    main.append(actorStr);

    main.append(_ListP);
    main.append(_ListT);
    main.append(_ListGraph);

    main.append("int " + myGraph.name + "(TPOData *D)\r\n");
    main.append("{\r\n");
    main.append("int topCount = " + QString::number(myGraph.topList.count()) + ";\r\n");
    main.append("int rootTop = " + QString::number(myGraph.getRootTop()) + ";\r\n");
    main.append("GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);\r\n");
    main.append("return 1;\r\n");
    main.append("}\r\n");

    QFile f(myOutputDirectory + "/" + myGraph.name + ".cpp");
    f.open(QFile::WriteOnly);
    f.write(main.toUtf8());
    f.close();
}

void GraphCompiler::compileMakefile(const QString target) const
{
    target.toLower();
    QString buildTypeU = target;
    buildTypeU[0] = buildTypeU[0].toUpper();
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
        targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + "\\" + name + ".o " + name + ".cpp" );
    }
    foreach (const BaseModule *baseModule, usedBaseModuleList) {
        objects.append(target + "/" + baseModule->name + ".o");
        sources.append(baseModule->name + ".c");
        targets.append(target + "/" + baseModule->name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + "\\" + baseModule->name + ".o " + baseModule->name + ".c" );
    }

    QString result = getTemplate(myTemplateDirectory + "/" + "Makefile." + buildTypeU + ".template");
    if (result.isEmpty())
        globalLogger->log(QObject::tr(ERR_GRAPHCOMPI_EMPTY_TEMPL).arg("Makefile." + buildTypeU), Logger::Warning);
    result.replace("<#dateTime>", QDateTime::currentDateTime().toString());
    result.replace("<#project>", "Project" + QString::number(globalDBManager->getProjectId()));
    result.replace("<#objects>", objects.join(" \\\r\n\t\t"));
    result.replace("<#sources>", sources.join(" \\\r\n\t\t"));
    result.replace("<#targets>", targets.join("\r\n\r\n"));

    QFile f(myOutputDirectory + "/" + "Makefile." + buildTypeU);
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


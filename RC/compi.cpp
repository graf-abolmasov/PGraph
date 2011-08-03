#include <QtCore>
#include <QtGui>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shab.h"
#include "compi.h"
#include "globalvariables.h"
#include "commonutils.h"

GraphCompiler::GraphCompiler(const Graph &graph) :
    myGraph(graph)
{
}

void GraphCompiler::compile()
{
    QList<Top> topList = myGraph.topList;
    QList<Arc> arcList = myGraph.arcList;

    // только используемые предикаты
    QList<const Predicate *> usedPredicateList;
    foreach (Arc arc, arcList) {
        if (!usedPredicateList.contains(arc.predicate))
            usedPredicateList.append(arc.predicate);
    }

    // только используемые акторы
    QList<const Actor *> usedActorList;
    foreach (Top top, topList) {
        if (!usedActorList.contains(top.actor))
            usedActorList.append(top.actor);
    }

    // только используемые базовые модули
    QList<const BaseModule *> usedBaseModuleList;
    foreach (const Predicate *predicate, usedPredicateList) {
        Q_ASSERT(predicate != NULL);
        if (predicate->type == Predicate::InlineType)
            continue;
        if (!usedBaseModuleList.contains(predicate->baseModule))
            usedBaseModuleList.append(predicate->baseModule);
    }
    foreach (const Actor *actor, usedActorList) {
        if (actor->type == Actor::InlineType || actor->type == Actor::GraphType)
            continue;
        if (!usedBaseModuleList.contains(actor->baseModule))
            usedBaseModuleList.append(actor->baseModule);
    }

    QSettings settings("graph.ini", QSettings::IniFormat);
    const QString myOutputDirectory = QFileInfo(settings.value("Location/OutputDir", "./Out/").toString()).canonicalFilePath();
    const QString myBaseDirectory = QFileInfo(settings.value("Location/BaseDir", "./BaseDir/").toString()).canonicalFilePath();
    const QString myTempalteDirectory = QFileInfo(settings.value("Location/TemplateDir", "./Templates/").toString()).canonicalFilePath();

    // копируем файлы используемых акторов и предикатов в выходной каталог
    foreach (const Actor *actor, usedActorList) {
        QFile::copy(myBaseDirectory + "/" + actor->name + ".cpp", myOutputDirectory + "/" + actor->name + ".cpp");
    }
    foreach (const Predicate *predicate, usedPredicateList) {
        QFile::copy(myBaseDirectory + "/" + predicate->name + ".cpp", myOutputDirectory + "/" + predicate->name + ".cpp");
    }

    // делаем заголовочный файл для базовых модулей
    foreach (const BaseModule *baseModule, usedBaseModuleList) {
        QFile::copy(myBaseDirectory + "/" + baseModule->name + ".c", myOutputDirectory + "/" + baseModule->name + ".c");
        QFile::copy(myBaseDirectory + "/" + baseModule->uniqName + ".cpp", myOutputDirectory + "/" + baseModule->uniqName + ".cpp");
    }

    // делаем заголовки для предикатов
    QString predicateStr;
    foreach (const Predicate *predicate, usedPredicateList) {
        predicateStr.append("int " + predicate->name + "(TPOData *D);\r\n");
    }

    // делаем заголовки файл для акторов
    QString actorStr;
    foreach (const Actor *actor, usedActorList) {
        actorStr.append("int " + actor->name + "(TPOData *D);\r\n");
    }


    // Делаем main.cpp
    // список _ListP
    QString _ListP;
    _ListP.append("static DefinePredicate ListPred[" + QString::number(usedPredicateList.size()) + "] = {\r\n");
    QStringList list;
    foreach (const Predicate *predicate, usedPredicateList) {
        list << "DefinePredicate(\"" + predicate->name + "\", &" + predicate->name + ")";
    }
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
    //main.append("int predCount = " + QString::number(usedPredicateList.count()) + ";\r\n");
    main.append("int rootTop = " + QString::number(myGraph.getRootTop()) + ";\r\n");
    main.append("GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);\r\n");
    main.append("return 1;\r\n");
    main.append("}\r\n");

    QFile f(myOutputDirectory + "/" + myGraph.name + ".cpp");
    f.open(QFile::WriteOnly);
    f.write(main.toUtf8());
    f.close();

    // копируем необходимые файлы
    QFile::copy(myTempalteDirectory + "/graph.h.template", myOutputDirectory + "/graph.h");
    QFile::copy(myTempalteDirectory + "/memman.h.template", myOutputDirectory + "/memman.h");
    QFile::copy(myTempalteDirectory + "/graph.cpp.template", myOutputDirectory + "/graph.cpp");
    QFile::copy(myTempalteDirectory + "/property.h.template", myOutputDirectory + "/property.h");
    QFile::copy(myTempalteDirectory + "/memman.cpp.template", myOutputDirectory + "/memman.cpp");
    QFile::copy(myTempalteDirectory + "/graphmv.cpp.template", myOutputDirectory + "/graphmv.cpp");
}



#include <QtCore>
#include <QPixmap>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphcompiler.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/qgraphsettings.h"

GraphCompiler::GraphCompiler()
{
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

    QList<Top> topList = graph.topList;
    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        if (top.actor->type == Actor::GraphType)
            res.append(compileRecursively(globalDBManager->getGraphDB(top.actor->name)));
    }
    return res;
}

QList<GraphStruct> GraphCompiler::compile(const Graph &graph)
{
    mySkipList.clear();
    result.clear();
    compiledGraphs.clear();
    procsMax = QGraphSettings::isParallel() ?  2 : 1;
    procsCounter = procsMax;
    QTime t;
    t.start();
    QStringList errors = compileRecursively(graph);
    if (!errors.isEmpty()) {
        globalLogger->log(errors, Logger::Warning);
        globalLogger->log(QObject::tr("Компиляция провалилась. Всего %1 ошибок.").arg(QString::number(errors.count())), Logger::Warning);
        return QList<GraphStruct>();
    }
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
    result[0].usedActors.append(new Actor(graph));
    globalLogger->log(QObject::tr("Нужно %1 процов").arg(QString::number(procsMax)), Logger::Compile);
    return result;
}

void GraphCompiler::compileStruct(const Graph &graph)
{
    GraphStruct graphStruct;

    // Делаем <#graphname>.cpp
    QList<Top> topList = graph.topList;
    QList<Arc> arcList = graph.arcList;

    QSet<const Actor *> myActorsSet;
    QSet<const Predicate *> myPredicatesSet;
    QSet<const BaseModule *> myBasemodulesSet;

    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        myActorsSet.insert(top.actor);
    }
    graphStruct.usedActors = myActorsSet.toList();
    foreach (const Actor *actor, graphStruct.usedActors) {
        if (actor->type == Actor::NormalType) {
            Q_ASSERT(actor->baseModule != NULL);
            myBasemodulesSet.insert(actor->baseModule);
        }
    }

    foreach (Arc arc, graph.arcList) {
        Q_ASSERT(arc.predicate != NULL);
        myPredicatesSet.insert(arc.predicate);
    }
    graphStruct.usedPredicates = myPredicatesSet.toList();
    foreach (const Predicate *predicate, graphStruct.usedPredicates) {
        graphStruct.predicatesTable.append(DefinePredicate(predicate->name));
        if (predicate->type == Predicate::NormalType) {
            Q_ASSERT(predicate->baseModule != NULL);
            myBasemodulesSet.insert(predicate->baseModule);
        }
    }

    graphStruct.usedBasemodules = myBasemodulesSet.toList();

    int maxTopNum = 0;
    foreach (Top top, graph.topList) {
        Q_ASSERT(top.actor != NULL);
        maxTopNum = top.number > maxTopNum ? top.number : maxTopNum;
    }

    QVector<DefineTop> vecTopsDefine(maxTopNum+1);
    QList<DefineTop> virtualTopsDefine;
    foreach (Top top, topList) {
        QList<Arc> outArcs = graph.getOutArcs(top.number);
        qSort(outArcs.begin(), outArcs.end(), orderArcByPriorityAsc);
        const bool isTailTop = outArcs.count() == 0;
        const int first = isTailTop ? -77 : graphStruct.graphsTable.count();
        const int last = isTailTop ? -77 : first + outArcs.count()-1;

        foreach (Arc arc, outArcs) {
            int endTop = arc.endTop;
            if (arc.type == Arc::ParallelArc) {
                const QString virtualGraphName = "V" + graph.name + "_" + QString::number(arc.endTop);
                graphStruct.entries[virtualGraphName] = arc.endTop;
                virtualTopsDefine.append(DefineTop(virtualGraphName, -77, -77));
                endTop = maxTopNum+virtualTopsDefine.size();
            }
            int predicateIndex = graphStruct.usedPredicates.indexOf(arc.predicate);
            graphStruct.graphsTable.append(DefineGraph(predicateIndex, endTop, arc.type));
        }
        vecTopsDefine[top.number] = DefineTop(top.actor->name, first, last);
    }

    foreach (DefineTop virtualTop, virtualTopsDefine)
        vecTopsDefine.append(virtualTop);

    graphStruct.topsTable = vecTopsDefine.toList();
    graphStruct.entries[graph.name] = graph.getRootTop();
    graphStruct.namepr = graph.name;
    result.append(graphStruct);
}

DefineGraph::DefineGraph(int predicateIndex, int topIndex)
{
    this->predicateIndex = predicateIndex;
    this->topIndex = topIndex;
    this->type = 1;
}

DefineGraph::DefineGraph(int predicateIndex, int topIndex, int type)
{
    this->predicateIndex = predicateIndex;
    this->topIndex = topIndex;
    this->type = type;
}

DefineTop::DefineTop(QString actor, int firstIndex, int lastIndex)
{
    this->actor = actor;
    this->firstIndex = firstIndex;
    this->lastIndex = lastIndex;
}

DefinePredicate::DefinePredicate(QString namepr)
{
    this->namepr = namepr;
}

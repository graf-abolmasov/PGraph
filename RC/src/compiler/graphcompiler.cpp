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

    compiledGraphs.insert(graph.name);

    QList<Top> topList = graph.topList;
    foreach (Top top, topList) {
        Q_ASSERT(top.actor != NULL);
        if (top.actor->type == Actor::GraphType)
            res.append(compileRecursively(globalDBManager->getGraphDB(top.actor->name)));
    }
    compileStruct(graph);
    return res;
}

QList<GraphStruct> GraphCompiler::compile(const Graph &graph)
{
    mySkipList.clear();
    result.clear();
    compiledGraphs.clear();
    QTime t;
    t.start();
    QStringList errors = compileRecursively(graph);
    if (!errors.isEmpty()) {
        globalLogger->log(errors, Logger::Warning);
        globalLogger->log(QObject::tr("Компиляция провалилась. Всего %1 ошибок.").arg(QString::number(errors.count())), Logger::Warning);
        return QList<GraphStruct>();
    }
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
    result[0].usedActors.insert(new Actor(graph));
    if (QGraphSettings::isParallel())
        globalLogger->log(QObject::tr("Нужно %1 процов").arg(QString::number(procsMax[graph.name]+2)), Logger::Compile);
    return result;
}

void GraphCompiler::compileStruct(const Graph &graph)
{
    procsMax[graph.name] = 0;
    procsCounter[graph.name] = 0;


    //reorder tops: terminated tops always after all tops of one level
    QList<Top> topList;
    QMap<int, Top> topMap;
    QQueue<Top> topQueue;

    int maxTopNum = 0;
    foreach (Top top, graph.topList) {
        topMap[top.number] = top;
        maxTopNum = top.number > maxTopNum ? top.number : maxTopNum;
    }

    const Top rootTop = topMap[graph.getRootTop()];
    topQueue.append(rootTop);
    while(!topMap.isEmpty()) {
        const Top currTop = topQueue.dequeue();
        bool skip = false;
        foreach (Top top, topQueue) {
            if (currTop.terminated && ((top.level > currTop.level) || (top.level == currTop.level && !top.terminated))) {
                skip = true;
                break;
            }
        }
        if (skip) {
            topQueue.enqueue(currTop);
            continue;
        }

        if (!topMap.contains(currTop.number))
            continue;
        topList.append(currTop);
        topMap.remove(currTop.number);
        QList<Arc> arcs = graph.getOutArcs(currTop.number);
        foreach (Arc arc, arcs) {
            if (topMap.contains(arc.endTop)) {
                Top newTop = topMap[arc.endTop];
                if (arc.type == Arc::SerialArc)
                    newTop.level = currTop.level;
                else if (arc.type == Arc::ParallelArc)
                    newTop.level = currTop.level+1;
                else if (arc.type == Arc::TerminateArc)
                    newTop.level = currTop.level-1;
                topQueue.enqueue(newTop);
            }
        }
    }

    GraphStruct graphStruct;

    foreach (Top top, topList)
        graphStruct.usedActors.insert(top.actor);

    foreach (const Actor *actor, graphStruct.usedActors) {
        if (actor->type == Actor::NormalType) {
            Q_ASSERT(actor->baseModule != NULL);
            graphStruct.usedBasemodules.insert(actor->baseModule);
        }
    }

    foreach (Arc arc, graph.arcList) {
        Q_ASSERT(arc.predicate != NULL);
        graphStruct.usedPredicates.insert(arc.predicate);
    }
    foreach (const Predicate *predicate, graphStruct.usedPredicates) {
        if (predicate->type == Predicate::NormalType) {
            Q_ASSERT(predicate->baseModule != NULL);
            graphStruct.usedBasemodules.insert(predicate->baseModule);
        }
    }

    QVector<DefineTop> vecTopsDefine(maxTopNum+1);
    QList<DefineTop> virtualTopsDefine;
    foreach (Top top, topList) {
        QList<Arc> outArcs = graph.getOutArcs(top.number);
        qSort(outArcs.begin(), outArcs.end(), orderArcByPriorityAsc);
        const bool isTailTop = outArcs.count() == 0;
        const int first = isTailTop ? -77 : graphStruct.graphsTable.count();
        const int last = isTailTop ? -77 : first + outArcs.count()-1;

        if (top.actor->type == Actor::GraphType)
            procsMax[graph.name]+=procsMax[top.actor->name];

        foreach (Arc arc, outArcs) {
            int endTop = arc.endTop;
            if (arc.type == Arc::ParallelArc) {
                const QString virtualGraphName = "V" + graph.name + "_" + QString::number(arc.endTop);
                graphStruct.entries[virtualGraphName] = arc.endTop;
                virtualTopsDefine.append(DefineTop(virtualGraphName, -77, -77));
                endTop = maxTopNum+virtualTopsDefine.size();
                procsCounter[graph.name]++;
            }
            if (procsCounter[graph.name] > procsMax[graph.name])
                procsMax[graph.name] = procsCounter[graph.name];
            if (arc.type == Arc::TerminateArc)
                procsCounter[graph.name]--;
            graphStruct.graphsTable.append(DefineGraph(arc.predicate->name, endTop, arc.type));
        }
        vecTopsDefine[top.number] = DefineTop(top.actor->name, first, last);
    }

    foreach (DefineTop virtualTop, virtualTopsDefine)
        vecTopsDefine.append(virtualTop);

    graphStruct.topsTable = vecTopsDefine.toList();
    graphStruct.entries[graph.name] = graph.getRootTop();
    graphStruct.namepr = graph.name;
    graphStruct.procsMax = procsMax[graph.name];
    result.append(graphStruct);
}

DefineGraph::DefineGraph(QString predicate, int topIndex)
{
    this->predicate = predicate;
    this->topIndex = topIndex;
    this->type = Arc::SerialArc;
}

DefineGraph::DefineGraph(QString predicate, int topIndex, int type)
{
    this->predicate = predicate;
    this->topIndex = topIndex;
    this->type = type;
}

DefineTop::DefineTop(QString actor, int firstIndex, int lastIndex)
{
    this->actor = actor;
    this->firstIndex = firstIndex;
    this->lastIndex = lastIndex;
}

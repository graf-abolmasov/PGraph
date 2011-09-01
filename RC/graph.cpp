#include "graph.h"
#include "actor.h"

Graph::Graph(QString name, QString extName, QList<Top> topList, QList<Arc> arcList, QList<Comment> commentList, QList<SyncArc> syncArcList) :
    Actor(name, extName, Actor::GraphType)
{
    this->topList          = topList;
    this->arcList          = arcList;
    this->commentList      = commentList;
    this->syncArcList      = syncArcList;
}

QList<Arc> Graph::getOutArcs(int topNumber) const
{
    QList<Arc> result;
    foreach (Arc arc, arcList) {
        if (arc.startTop == topNumber)
            result.append(arc);
    }
    return result;
}

/*!
  Номер кореновой вершины.
  \return Номер кореневой вершины или -1, если вершина не задана
*/
int Graph::getRootTop() const
{
    int rootTop = -1;
    foreach (Top top, topList) {
        if (top.isRoot) {
            rootTop = top.number;
            break;
        }
    }
    return rootTop;
}

QStringList Graph::validate() const
{
    QStringList msgs;
    msgs.append(Actor::validate());
    foreach (Arc arc, arcList)
        msgs.append(arc.validate());
    foreach (Top top, topList)
        msgs.append(top.validate());
    foreach (SyncArc sync, syncArcList)
        msgs.append(sync.validate());
    if (getRootTop() == -1)
        msgs << QObject::tr(ERR_GRAPH_UNDEF_ROOT).arg(name);
    return msgs;
}

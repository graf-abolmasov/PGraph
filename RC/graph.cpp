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

int Graph::getRootTop() const
{
    int rootTop = -1;
    foreach (Top top, topList) {
        if (top.isRoot) {
            rootTop = top.number;
            break;
        }
    }
    Q_ASSERT(rootTop != -1);
    return rootTop;
}

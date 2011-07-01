#include "graph.h"

Graph::Graph(QString name, QString extName, QList<Top> topList, QList<Arc> arcList, QList<Comment> commentList, QList<SyncArc> syncArcList) :
    Actor(name, extName, Actor::GraphType)
{
//    this->name             = name;
//    this->extName          = extName;
    this->topList          = topList;
    this->arcList          = arcList;
    this->commentList      = commentList;
    this->syncArcList      = syncArcList;
}

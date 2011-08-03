#ifndef GRAPH_H
#define GRAPH_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QFont>
#include "actor.h"
#include "predicate.h"

class SyncArc
{
    SyncArc(QString startGraph, int startTop, QString endGraph, int endTop);
    QString startGraph;
    int startTop;
    QString endGraph;
    int endTop;
};

class Comment
{
public:
    Comment(float x, float y, QString text, QFont font);
    float x;
    float y;
    QString text;
    QFont font;
};

class Arc
{
public:
    enum ArcType { SerialArc, ParallelArc, TerminateArc };
    Arc(ArcType type, int priority, int startTop, int endTop, const Predicate *predicate, QStringList &lines);
    ArcType type;
    int priority;
    int startTop;
    int endTop;
    QStringList lines;
    const Predicate *predicate;
};

class Top
{
public:
    enum TopType { NormalTop, MultiProcTop };
    Top(float x, float y, float sizeX, float sizeY, int number, int procCount, bool isRoot, const Actor *actor, TopType type);
    float x;
    float y;
    float sizeX;
    float sizeY;
    int number;
    int procCount;
    bool isRoot;
    const Actor *actor;
    TopType type;
};

class Graph  : public Actor
{
public:
    Graph(QString name, QString extName, QList<Top> topList, QList<Arc> arcList, QList<Comment> commentList, QList<SyncArc> syncArcList);
    QList<Top>           topList;
    QList<Arc>           arcList;
    QList<Comment>       commentList;
    QList<SyncArc>       syncArcList;

    QList<Arc> getOutArcs(int topNumber) const;
    int getRootTop() const;
};

#endif // GRAPH_H

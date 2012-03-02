#ifndef GRAPH_H
#define GRAPH_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QFont>
#include "actor.h"
#include "predicate.h"
#include "errors.h"

class SyncArc
{
public:
    SyncArc(QString startGraph, int startTop, QString endGraph, int endTop);
    QString startGraph;
    int startTop;
    QString endGraph;
    int endTop;
    QStringList validate() const {return QStringList();}
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

    QStringList validate() const;
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

    QStringList validate() const;
};

class Project
{
public:
    int id;
    QString name;
    QString description;
    QString author;
    Project(int id, QString name);
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
    QStringList validate() const;
};

#endif // GRAPH_H

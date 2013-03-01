#ifndef GRAPH_H
#define GRAPH_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QFont>
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/common/errors.h"

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

class GraphArc
{
public:
    enum ArcType { SerialArc = 1, ParallelArc = 2, TerminateArc = 3};
    GraphArc(ArcType type, int priority, int startTop, int endTop, const Predicate *predicate, const QStringList &lines);
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
    Top();
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
    int level;
    bool terminated;

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
    Graph(const QString &name, const QString &extName, const QList<Top> &topList, const QList<GraphArc> &arcList, const QList<Comment> &commentList, const QList<SyncArc> &syncArcList);
    Graph(const QString &name, const QString &extName, const QList<Top> &topList, const QList<GraphArc> &arcList, const QList<Comment> &commentList, const QList<SyncArc> &syncArcList, const QPixmap &icon);
    QList<Top>           topList;
    QList<GraphArc>           arcList;
    QList<Comment>       commentList;
    QList<SyncArc>       syncArcList;

    QList<GraphArc> getOutArcs(int topNumber) const;
    QList<SyncArc> getSyncSend(int topNumber) const;
    QList<SyncArc> getSyncRecv(int topNumber) const;
    int getRootTop() const;
    QStringList validate() const;
};

#endif // GRAPH_H

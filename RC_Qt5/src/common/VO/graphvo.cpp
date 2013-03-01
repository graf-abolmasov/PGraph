#include "graphvo.h"
#include "actor.h"

Graph::Graph(const QString &name, const QString &extName, const QList<Top> &topList, const QList<GraphArc> &arcList, const QList<Comment> &commentList, const QList<SyncArc> &syncArcList) :
    Actor(name, extName, Actor::GraphType)
{
    this->arcList          = arcList;
    this->commentList      = commentList;
    this->syncArcList      = syncArcList;

    QMap<int, Top> topMap;
    foreach (Top top, topList)
        topMap[top.number] = top;
    foreach (GraphArc arc, arcList)
        topMap[arc.startTop].terminated = arc.type == GraphArc::TerminateArc;
    this->topList = topMap.values();
}

Graph::Graph(const QString &name, const QString &extName, const QList<Top> &topList, const QList<GraphArc> &arcList, const QList<Comment> &commentList, const QList<SyncArc> &syncArcList, const QPixmap &icon) :
    Actor(name, extName, Actor::GraphType, NULL, QList<const Variable *>(), QStringList(), icon)
{
    this->arcList          = arcList;
    this->commentList      = commentList;
    this->syncArcList      = syncArcList;

    QMap<int, Top> topMap;
    foreach (Top top, topList)
        topMap[top.number] = top;
    foreach (GraphArc arc, arcList)
        topMap[arc.startTop].terminated = arc.type == GraphArc::TerminateArc;
    this->topList = topMap.values();
}

QList<GraphArc> Graph::getOutArcs(int topNumber) const
{
    QList<GraphArc> result;
    foreach (GraphArc arc, arcList) {
        if (arc.startTop == topNumber)
            result.append(arc);
    }
    return result;
}


QList<SyncArc> Graph::getSyncRecv(int topNumber) const
{
    QList<SyncArc> result;
    foreach (SyncArc arc, syncArcList) {
        if (arc.endTop == topNumber)
            result.append(arc);
    }
    return result;
}


QList<SyncArc> Graph::getSyncSend(int topNumber) const
{
    QList<SyncArc> result;
    foreach (SyncArc arc, syncArcList) {
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
    foreach (GraphArc arc, arcList)
        msgs.append(arc.validate());
    foreach (Top top, topList) {
        msgs.append(top.validate());
        QList<GraphArc> outArcs = getOutArcs(top.number);
        if (outArcs.isEmpty())
            continue;
        bool outArcsCorrect = true;
        const int outArcType = outArcs.first().type;
        foreach (GraphArc arc, outArcs) {
            if (outArcType != arc.type) {
                outArcsCorrect = false;
                break;
            }
        }
        if (!outArcsCorrect) {
            msgs << QObject::tr(ERR_TOP_DIFFERENT_OUT_ARCS).arg(QString::number(top.number));
        }
    }
    foreach (SyncArc sync, syncArcList)
        msgs.append(sync.validate());
    if (getRootTop() == -1)
        msgs << QObject::tr(ERR_GRAPH_UNDEF_ROOT).arg(name);
    return msgs;
}

Project::Project(int id, QString name)
{
    this->id = id;
    this->name = name;
}

Top::Top()
{

}

Top::Top(float x, float y, float sizeX, float sizeY, int number, int procCount, bool isRoot, const Actor *actor, TopType type)
{
    this->x = x;
    this->y = y;
    this->actor = actor;
    this->isRoot = isRoot;
    this->number = number;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->type = type;
    this->procCount = procCount;
    this->level = 0;
    this->terminated = false;
}

QStringList Top::validate() const
{
    QStringList msgs;
    if (sizeX == 0 || sizeY == 0)
        msgs << QObject::tr(ERR_TOP_ZERO_SIZE).arg(QString::number(number)).arg(QString::number(sizeX)).arg(QString::number(sizeY));
    if (number < 0)
        msgs << QObject::tr(ERR_TOP_WRONG_NUMBER).arg(QString::number(number));
    if (actor == NULL)
        msgs << QObject::tr(ERR_TOP_NULL_ACTOR).arg(QString::number(number));
    if (type == MultiProcTop && procCount < 2)
        msgs << QObject::tr(ERR_TOP_WRONG_PROCNUM).arg(QString::number(number)).arg(QString::number(procCount));
    return msgs;
}

SyncArc::SyncArc(QString startGraph, int startTop, QString endGraph, int endTop)
{
    this->startGraph = startGraph;
    this->endTop = endTop;
    this->endGraph = endGraph;
    this->startTop = startTop;
}

GraphArc::GraphArc(ArcType type, int priority, int startTop, int endTop, const Predicate *predicate, const QStringList &lines)
{
    this->type = type;
    this->priority = priority;
    this->startTop = startTop;
    this->endTop = endTop;
    this->predicate = predicate;
    this->lines = lines;
}

QStringList GraphArc::validate() const
{
    QStringList msgs;
    if (startTop == -1)
        msgs << QObject::tr(ERR_ARC_UNDEF_STARTOP).arg(QString::number(startTop)).arg(QString::number(endTop));
    if (endTop == -1)
        msgs << QObject::tr(ERR_ARC_UNDEF_ENDTOP).arg(QString::number(startTop)).arg(QString::number(endTop));
    if (predicate == NULL)
        msgs << QObject::tr(ERR_ARC_UNDEF_PREDICATE).arg(QString::number(startTop)).arg(QString::number(endTop));
    if (lines.count() == 0)
        msgs << QObject::tr(ERR_ARC_EMPTY_LINES).arg(QString::number(startTop)).arg(QString::number(endTop));
    return msgs;
}

Comment::Comment(float x, float y, QString text, QFont font)
{
    this->x = x;
    this->y = y;
    this->text = text;
    this->font = font;
}

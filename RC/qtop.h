#ifndef QTOP_H
#define QTOP_H

#include <QtGui>
#include <QList>
#include "qsyncarc.h"
#include "qarc.h"
#include "actor.h"

class QArc;
class QSyncArc;

#define TOP_TYPE UserType+15

class Top
{
public:
    Top(float x, float y, float sizeX, float sizeY, int number, int procCount, bool isRoot, QString actor, QString type);
    float x;
    float y;
    float sizeX;
    float sizeY;
    int number;
    int procCount;
    bool isRoot;
    QString actor;
    QString type;
};

class QTop : public QGraphicsRectItem
{
public:
    enum { Type = TOP_TYPE};
    QTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~QTop();
    int number;
    bool isRoot;
    Actor* actor;
    void removeArc(QArc *arc);
    void removeArcs();
    void removeSync(QSyncArc *arc);
    void removeSyncs();
    void addArc(QArc *arc);
    void addSync(QSyncArc *arc);
    QLineF getIntersectBound(QLineF line) const;
    int type() const
        { return Type; }
    QList<QArc *> allArcs() const
        { return arcs; }
    QList<QSyncArc *> allSync() const
        { return sync; }
    QList<QArc *> inArcs() const;
    QList<QArc *> outArcs() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPainterPath opaqueArea() const;
    virtual Top* toTop() const = 0;
    QList<QArc *> getArcsAtBound(int i) const;
    bool moveBy(qreal dx, qreal dy);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    QList<QArc *> arcs;
    QList<QSyncArc *> sync;
    QMenu *myContextMenu;
};

#endif

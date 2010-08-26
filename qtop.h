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
    QLineF getIntersectBound(QLineF line);
    int type() const
        { return Type; }
    QList<QArc *> allArcs()
        { return arcs; }
    QList<QArc *> inArcs();
    QList<QArc *> outArcs();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPainterPath opaqueArea() const;
    virtual Top* toTop() = 0;
    QList<QArc *> getArcsAtBound(int i);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QList<QArc *> arcs;
    QList<QSyncArc *> sync;
    QMenu *myContextMenu;
    //QList<QArc *> getArcsAtBound(QLineF bound);
    static int counter;
};

#endif

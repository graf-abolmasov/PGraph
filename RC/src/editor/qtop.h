#ifndef QTOP_H
#define QTOP_H

#include <QtGui/QGraphicsRectItem>
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/graphvo.h"

class QArc;
class QSyncArc;
class QDiagramScene;

#define TOP_TYPE UserType+15

class QTop : public QGraphicsRectItem
{
public:
    enum { Type = TOP_TYPE};
    QTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~QTop();
    int number;
    bool isRoot;

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
    virtual Top toTop() const = 0;
    QList<QArc *> getArcsAtBound(int i) const;
    bool moveBy(qreal dx, qreal dy);

    const Actor *actor;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QList<QArc *> arcs;
    QList<QSyncArc *> sync;
private:
    QMenu *myContextMenu;
};

#endif

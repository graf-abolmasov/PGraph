#ifndef QSYNCARC_H
#define QSYNCARC_H

#include <QGraphicsLineItem>
#include "qtop.h"
QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

#define SYNC_ARC_TYPE      UserType+7

class QTop;

class QSyncArc : public QGraphicsLineItem
{
public:
    enum { Type = SYNC_ARC_TYPE };
    QSyncArc(QTop *startItem, QTop *endItem, QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    int type() const
        { return Type;}
    QTop* startItem() const
        { return myStartItem; }
    QTop* endItem() const
        { return myEndItem; }
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QTop* myStartItem;
    QTop* myEndItem;
    QPolygonF arrowHead;
    QColor myColor;
    QMenu *myContextMenu;
};

#endif // QSYNCARC_H


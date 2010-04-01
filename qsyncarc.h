#ifndef QSYNCARC_H
#define QSYNCARC_H

#include <QGraphicsLineItem>
#include "qdiagramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

#define SYNC_ARC_TYPE      UserType+7

class TTop;

class QSyncArc : public QGraphicsLineItem
{
public:
    enum { Type = SYNC_ARC_TYPE };
    QSyncArc(TTop *startItem, TTop *endItem, QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    int type() const
        { return Type;}
    TTop* startItem() const
        { return myStartItem; }
    TTop* endItem() const
        { return myEndItem; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

public slots:
    void updatePosition();

private:
    TTop* myStartItem;
    TTop* myEndItem;
    QPolygonF arrowHead;
    QColor myColor;
    QMenu *myContextMenu;
};

#endif // QSYNCARC_H


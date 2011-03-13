#ifndef QDATAITEM_H
#define QDATAITEM_H

#include <QtGui>
#include "variable.h"

#define DATA_ITEM_TYPE UserType+16

class QDataItem : public QGraphicsRectItem
{
public:
    enum { Type = DATA_ITEM_TYPE };
    QDataItem(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~QDataItem();
    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    Variable *variable;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
};

#endif // QDATAITEM_H

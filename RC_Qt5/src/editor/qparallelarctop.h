#ifndef QPARALLELARCTOP_H
#define QPARALLELARCTOP_H

#include <QGraphicsEllipseItem>

#define ARC_TOP_TYPE  UserType+5

class QParallelArcTop : public QGraphicsEllipseItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    QParallelArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QMenu *myContextMenu;
};

#endif // QPARALLELARCTOP_H

#ifndef QSERIALARCTOP_H
#define QSERIALARCTOP_H

#include <QtGui/QGraphicsRectItem>

#define ARC_TOP_TYPE  UserType+5

class QSerialArcTop : public QGraphicsRectItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    QSerialArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
    QPainterPath shape() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QMenu *myContextMenu;
};

#endif // QSERIALARCTOP_H

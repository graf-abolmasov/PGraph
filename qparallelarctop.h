#ifndef QPARALLELARCTOP_H
#define QPARALLELARCTOP_H

#include <QtGui>

#define ARC_TOP_TYPE  UserType+5

class QParallelArcTop : public QGraphicsEllipseItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    QParallelArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QMenu *myContextMenu;
};

#endif // QPARALLELARCTOP_H

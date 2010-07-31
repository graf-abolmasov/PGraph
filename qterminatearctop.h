#ifndef QTERMINATEARCTOP_H
#define QTERMINATEARCTOP_H

#include <QtGui>

#define ARC_TOP_TYPE  UserType+5

class QTerminateArcTop : public QGraphicsLineItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    QTerminateArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
    QPainterPath shape() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QMenu *myContextMenu;
};



#endif // QTERMINATEARCTOP_H

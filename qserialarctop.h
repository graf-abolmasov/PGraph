#ifndef QSERIALARCTOP_H
#define QSERIALARCTOP_H

#include <QtGui>

#define ARC_TOP_TYPE  UserType+5

class QSerialArcTop : public QGraphicsPolygonItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    QSerialArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QMenu *myContextMenu;
};

#endif // QSERIALARCTOP_H

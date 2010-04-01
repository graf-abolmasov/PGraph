#include <QtGui>
#include "qarctop.h"
#include "qarc.h"

void QArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QArcTop::QArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myContextMenu = contextMenu;
    QPolygonF myPolygon;
    myPolygon << QPointF(-8, 8) << QPointF(8, 8)
              << QPointF(8,-8) << QPointF(-8, -8)
              << QPointF(-8, 8);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setPolygon(myPolygon);
}

QRectF QArcTop::boundingRect() const {
    return QGraphicsPolygonItem::boundingRect().adjusted(-2, -2, 2, 2);
}

QVariant QArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        TArc* arc = qgraphicsitem_cast<TArc* >(parentItem());
        QPen pen = arc->pen();
        if (isSelected())
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::black);
        arc->setPen(pen);
    }

    return value;
}

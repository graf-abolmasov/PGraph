#include "qparallelarctop.h"
#include <QtGui>
#include "qarc.h"

void QParallelArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QParallelArcTop::QParallelArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsEllipseItem(parent, scene)
{
    myContextMenu = contextMenu;
    setRect(-9, -9, 18, 18);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setBrush(QBrush(Qt::gray, Qt::SolidPattern));
}

QRectF QParallelArcTop::boundingRect() const {
    return QGraphicsEllipseItem::boundingRect().adjusted(-1, -1, 1, 1);
}

QVariant QParallelArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        QArc* arc = qgraphicsitem_cast<QArc* >(parentItem());
        QPen pen = arc->pen();
        if (isSelected())
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::black);
        arc->setPen(pen);
    }

    return value;
}

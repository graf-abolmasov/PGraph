#include "qterminatearctop.h"
#include <QtGui>
#include "qarc.h"

void QTerminateArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QTerminateArcTop::QTerminateArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myContextMenu = contextMenu;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setLine(9,9,-9,-9);
}

QRectF QTerminateArcTop::boundingRect() const {
    return QGraphicsLineItem::boundingRect().adjusted(-1, -1, 1, 1);
}

QPainterPath QTerminateArcTop::shape() const{
    QPainterPath path;

    path.addRect(-9, -9, 18, 18);

    return path;
}

QVariant QTerminateArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
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

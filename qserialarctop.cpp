#include <QtGui>
#include "qserialarctop.h"
#include "qarc.h"
#include "arcpropertydialog.h"

void QSerialArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QSerialArcTop::QSerialArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
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

QRectF QSerialArcTop::boundingRect() const {
    return QGraphicsPolygonItem::boundingRect().adjusted(-2, -2, 2, 2);
}

QVariant QSerialArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        /*QArc* arc = qgraphicsitem_cast<QArc* >(parentItem());
        QPen pen = arc->pen();
        if (isSelected())
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::black);
        arc->setPen(pen);*/
    }

    return value;
}

void QSerialArcTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ArcPropertyDialog dlg;
    QArc* arc = qgraphicsitem_cast<QArc *>(parentItem());
    dlg.prepareForm(arc);
    if (dlg.exec())
        dlg.getResult();
}

#include "qparallelarctop.h"
#include <QtGui>
#include "qarc.h"
#include "arcpropertydialog.h"
#include "globalvariables.h"

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

void QParallelArcTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ArcPropertyDialog dlg;
    QArc* arc = qgraphicsitem_cast<QArc *>(parentItem());
    dlg.prepareForm(arc);
    if (dlg.exec())
        dlg.getResult();
}

void QParallelArcTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
    QArc* myArc = qgraphicsitem_cast<QArc *>(parentItem());
    if (myArc->predicate != NULL)
        painter->drawText(rect().toRect(), Qt::AlignCenter, QString::number(globalPredicateList.indexOf(myArc->predicate->extName) + 1));
}

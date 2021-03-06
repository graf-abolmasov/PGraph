#include <QtGui>
#include "../../src/editor/qserialarctop.h"
#include "../../src/editor/qarc.h"
#include "../../src/editor/dialogs/arcpropertydialog.h"
#include "../../src/common/globalvariables.h"

void QSerialArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    if (myContextMenu != NULL)
        myContextMenu->exec(event->screenPos());
}

QSerialArcTop::QSerialArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem(parent, scene)
{
    myContextMenu = contextMenu;
    setRect(-8, -8, 16, 16);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setZValue(2);
}

QRectF QSerialArcTop::boundingRect() const {
    return rect().adjusted(-2, -2, 2, 2);
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

QPainterPath QSerialArcTop::shape() const
{
    QPainterPath result;
    result.addRect(rect().adjusted(-1,-1,1,1));
    return result;
}

void QSerialArcTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    QArc* arc = qgraphicsitem_cast<QArc *>(parentItem());
    ArcPropertyDialog *dlg = ArcPropertyDialog::getDialog(arc);
    if (dlg->exec())
        arc = dlg->getResult();
    delete dlg;
}

void QSerialArcTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    QArc* myArc = qgraphicsitem_cast<QArc *>(parentItem());
    if (myArc->predicate != NULL)
        painter->drawText(rect().toRect(), Qt::AlignCenter, QString::number(globalPredicateList.indexOf(myArc->predicate->name) + 1));
}


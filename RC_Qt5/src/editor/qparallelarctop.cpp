#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QMenu>

#include "qparallelarctop.h"
#include "qarc.h"
#include "qtop.h"
#include "dialogs/arcpropertydialog.h"
#include "../../src/common/globalvariables.h"

void QParallelArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect();
    eff->setOffset(3);
    eff->setBlurRadius(15);
    setGraphicsEffect(eff);
    if (myContextMenu != NULL)
        myContextMenu->exec(event->screenPos());
}

QParallelArcTop::QParallelArcTop(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    myContextMenu = contextMenu;
    setRect(-9, -9, 18, 18);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setZValue(2);
}

QRectF QParallelArcTop::boundingRect() const {
    return QGraphicsEllipseItem::boundingRect().adjusted(-2, -2, 2, 2);
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

void QParallelArcTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    QArc* arc = qgraphicsitem_cast<QArc *>(parentItem());
    ArcPropertyDialog *dlg = ArcPropertyDialog::getDialog(arc);
    if (dlg->exec()) {
        arc = dlg->getResult();
        QList<QArc *> outarcs = arc->startItem()->outArcs();
        QList<QArc *> inarcs = arc->endItem()->inArcs();
        foreach(QArc *qarc, outarcs)
            qarc->setArcType(arc->arcType());
        foreach(QArc *qarc, inarcs)
            qarc->setArcType(arc->arcType());
    }
    delete dlg;
}

void QParallelArcTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
//    QArc* myArc = qgraphicsitem_cast<QArc *>(parentItem());
//    if (myArc->predicate != NULL)
//        painter->drawText(rect().toRect(), Qt::AlignCenter, QString::number(globalPredicateList.indexOf(myArc->predicate->name) + 1));
}

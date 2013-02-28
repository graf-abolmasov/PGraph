#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QMenu>

#include "qterminatearctop.h"
#include "qarc.h"
#include "qtop.h"
#include "dialogs/arcpropertydialog.h"

void QTerminateArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect();
    eff->setOffset(3);
    eff->setBlurRadius(15);
    setGraphicsEffect(eff);
    if (myContextMenu != NULL)
        myContextMenu->exec(event->screenPos());
}

QTerminateArcTop::QTerminateArcTop(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myContextMenu = contextMenu;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setLine(9,-9,-9,9);
    setZValue(2);
}

QRectF QTerminateArcTop::boundingRect() const {
    return QGraphicsLineItem::boundingRect().adjusted(-2, -2, 2, 2);
}

QPainterPath QTerminateArcTop::shape() const{
    QPainterPath path;

    path.addRect(-9, -9, 18, 18);

    return path;
}

QVariant QTerminateArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        //QArc* arc = qgraphicsitem_cast<QArc* >(parentItem());
        /*QPen pen = arc->pen();
        if (isSelected())
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::black);
        arc->setPen(pen);*/
    }

    return value;
}

void QTerminateArcTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
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

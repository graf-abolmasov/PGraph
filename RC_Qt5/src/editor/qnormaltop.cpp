#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

#include "qnormaltop.h"
#include "dialogs/toppropertydialog.h"
#include "qarcline.h"
#include "../../src/common/VO/actor.h"
#include "qarc.h"
#include "qsyncarc.h"
#include "../../src/common/globalvariables.h"

QNormalTop::QNormalTop(QMenu *contextMenu, QGraphicsItem *parent)
        : QTop(contextMenu, parent)
{
    setRect(-35, -25, 70, 50);
    runInMemman = false;
}

void QNormalTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::TextAntialiasing |
                            QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing);


    painter->setPen(pen());
    painter->setBrush(brush());
    QGraphicsRectItem::paint(painter, option, widget);

    //рисуем иконку
    if (actor != NULL && !actor->icon.isNull()) {
        if ((actor->icon.width() >= boundingRect().width()) ||
            (actor->icon.height() >= boundingRect().height())) {
            painter->drawImage(boundingRect().adjusted(5, 5, -5, -5), actor->icon.toImage());
        } else {
            painter->drawImage(-actor->icon.width()/2, -actor->icon.height()/2, actor->icon.toImage());
        }
    }
    //пишем текст
    else if (actor != NULL) {
        QTextOption opt;
        opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        opt.setAlignment(Qt::AlignLeft);
        QFont f = painter->font();
        f.setPointSize(8);
        painter->setFont(f);
        painter->drawText(boundingRect().adjusted(8, 8, -8, -8), actor->extName, opt);
    }
}

float QNormalTop::getMinWidth() const
{
    QList<QArc* > arcListAtBound2 = getArcsAtBound(1);
    QList<QArc* > arcListAtBound1 = getArcsAtBound(3);
    QList<QArc* > arcList;
    foreach (QArc* arc, arcListAtBound1)
        arcList.append(arc);
    foreach (QArc* arc, arcListAtBound2)
        arcList.append(arc);
    float xMax = 0;
    float xMin = 0;
    if (arcList.count() > 0){
        if (this == arcList.first()->startItem()){
            xMax = arcList.first()->lines.first()->line().x1();
            xMin = arcList.first()->lines.first()->line().x1();
        } else {
            xMax = arcList.first()->lines.last()->line().x1();
            xMin = arcList.first()->lines.last()->line().x1();
        }

        foreach (QArc* arc, arcList){
            if (this == arcList.first()->startItem()){
                if(arc->lines.first()->line().x1() > xMax)
                    xMax = arc->lines.first()->line().x1();
                if(arc->lines.first()->line().x1() < xMin)
                    xMin = arc->lines.first()->line().x1();
            } else {
                if(arc->lines.last()->line().x2() > xMax)
                    xMax = arc->lines.last()->line().x2();
                if(arc->lines.last()->line().x2() < xMin)
                    xMin = arc->lines.last()->line().x2();
            }
        }

        xMin = fabs(scenePos().x() - xMin);
        xMax = fabs(xMax - scenePos().x());
    }
    return xMax > xMin ? xMax*2 : xMin*2;
}

float QNormalTop::getMinHeight() const
{
    QList<QArc* > arcListAtBound2 = getArcsAtBound(2);
    QList<QArc* > arcListAtBound1 = getArcsAtBound(4);
    QList<QArc* > arcList;
    foreach (QArc* arc, arcListAtBound1)
        arcList.append(arc);
    foreach (QArc* arc, arcListAtBound2)
        arcList.append(arc);
    float yMax = 0;
    float yMin = 0;
    if (arcList.count() > 0){
        if (this == arcList.first()->startItem()){
            yMax = arcList.first()->lines.first()->line().y1();
            yMin = arcList.first()->lines.first()->line().y1();
        } else {
            yMax = arcList.first()->lines.last()->line().y1();
            yMin = arcList.first()->lines.last()->line().y1();
        }


        foreach (QArc* arc, arcList){
            if (this == arcList.first()->startItem()){
                if(arc->lines.first()->line().y1() > yMax)
                    yMax = arc->lines.first()->line().y1();
                if(arc->lines.first()->line().y1() < yMin)
                    yMin = arc->lines.first()->line().y1();
            } else {
                if(arc->lines.last()->line().y2() > yMax)
                    yMax = arc->lines.last()->line().y2();
                if(arc->lines.last()->line().x2() < yMin)
                    yMin = arc->lines.last()->line().y2();
            }
        }

        yMin = fabs(scenePos().y() - yMin);
        yMax = fabs(yMax - scenePos().y());
    }

    return yMax > yMin ? yMax*2 : yMin*2;
}

/*!
  Устанавливает иконку.
  @param icon - иконка
*/
void QNormalTop::setIcon(QPixmap &icon){
    if (actor != NULL) {
        const_cast<Actor *>(actor)->icon = icon;
        globalDBManager->saveActorPicture(actor->name, icon);
    }
}

/*!
  Устанавливает флаг корневой вершины.
  @param flag - true если корневая
*/
void QNormalTop::setAsRoot(bool flag)
{
    isRoot = flag;
    if (isRoot){
        QPen p = pen();// painter->pen();
        p.setWidth(3);
        setPen(p);
    } else {
        QPen p = pen();// painter->pen();
        p.setWidth(2);
        setPen(p);
    }
}

void QNormalTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    TopPropertyDialog dlg;
    const QList<QGraphicsItem *>selectedItems = scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    QNormalTop* top = qgraphicsitem_cast<QNormalTop *>(selectedItems.first());
    dlg.prepareForm(top);
    if (dlg.exec())
        top = dlg.getResult();
    event->accept();
}

void QNormalTop::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QList<QGraphicsItem *> otherItems = scene()->items();
    otherItems.removeAll(this);

    QPen myPen = pen();
    myPen.setColor(Qt::red);
    setPen(myPen);

    foreach(QArc *arc, arcs) {
        QPen arcPen = arc->pen();
        arcPen.setColor(arc->startItem() == this ? Qt::blue : Qt::green);
        arc->setPen(arcPen);
        foreach(QArcLine *line, arc->lines)
            otherItems.removeAll(line);
        otherItems.removeAll(arc->arcTop);
        otherItems.removeAll(arc);
        otherItems.removeAll(arc->startItem());
        otherItems.removeAll(arc->endItem());
    }

    foreach(QSyncArc *syncArc, sync) {
        otherItems.removeAll(syncArc);
        otherItems.removeAll(syncArc->startItem());
        otherItems.removeAll(syncArc->endItem());
        otherItems.removeAll(syncArc);
    }

    foreach(QGraphicsItem *item, otherItems)
        item->setOpacity(0.55);

    event->accept();
}

void QNormalTop::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QList<QGraphicsItem *> allItems = scene()->items();

    QPen myPen = pen();
    myPen.setColor(Qt::black);
    setPen(myPen);

    foreach(QArc *arc, arcs) {
        QPen arcPen = arc->pen();
        arcPen.setColor(Qt::black);
        arc->setPen(arcPen);
    }

    foreach(QGraphicsItem *item, allItems)
        item->setOpacity(1.0);

    event->accept();
}

Top QNormalTop::toTop() const
{
    QPointF pos = scenePos();
    float sizeX = rect().width();
    float sizeY = rect().height();
    return Top(pos.x(), pos.y(), sizeX, sizeY, number, -1, isRoot, actor, Top::NormalTop);
}

void QNormalTop::setRunInMemman(bool runInMemman)
{
    setBrush(runInMemman ? QBrush(QColor::fromRgb(245,245,20), Qt::SolidPattern) : QBrush(QColor::fromRgb(230,230,230), Qt::SolidPattern));
    this->runInMemman = runInMemman;
}

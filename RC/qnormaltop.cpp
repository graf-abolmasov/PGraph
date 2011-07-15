#include <QtCore>
#include <QtGui>

#include "qnormaltop.h"
#include "toppropertydialog.h"
#include "qarcline.h"
#include "actor.h"
#include "qarc.h"

QNormalTop::QNormalTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
        : QTop(contextMenu, parent, scene)
{
    setRect(-35, -25, 70, 50);
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
            painter->drawImage(boundingRect().adjusted(5, 5, -5, -5), actor->icon);
        } else {
            painter->drawImage(-actor->icon.width()/2, -actor->icon.height()/2, actor->icon);
        }
    }
    //пишем текст
    else if (actor != NULL) {
        QTextOption opt;
        opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        opt.setAlignment(Qt::AlignLeft);
        painter->drawText(boundingRect().adjusted(7, 6, -6, -6), actor->extName, opt);
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
void QNormalTop::setIcon(QImage &icon){
    if (actor != NULL)
        QMessageBox::critical(0, QObject::tr("Ошибка"), QObject::tr("Функция пока недостуна"));
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

void QNormalTop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    TopPropertyDialog dlg;
    QNormalTop* top = qgraphicsitem_cast<QNormalTop *>(scene()->selectedItems().first());
    dlg.prepareForm(top);
    if (dlg.exec())
        top = dlg.getResult();
}

void QNormalTop::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::red);
    setPen(myPen);
    event->accept();
}

void QNormalTop::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::black);
    setPen(myPen);
    event->accept();
}

void QNormalTop::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::red);
    setPen(myPen);
    event->accept();
}

Top QNormalTop::toTop() const
{
    QPointF pos = scenePos();
    float sizeX = rect().width();
    float sizeY = rect().height();
    return Top(pos.x(), pos.y(), sizeX, sizeY, number, -1, isRoot, actor == NULL ? "" : actor->name, "T");
}

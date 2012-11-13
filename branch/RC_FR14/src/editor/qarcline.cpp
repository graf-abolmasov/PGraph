#include <QtCore>
#include <QtGui>

#include "qarcline.h"
#include "qarc.h"

QArcLine::QArcLine(QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene)
{
    QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect();
    eff->setOffset(2);
    eff->setBlurRadius(15);
    setGraphicsEffect(eff);
}

QPainterPath QArcLine::shape() const {
    QPainterPath path;
    int width = pen().width() + 2;
    path.addRect(QRectF(line().p1(), line().p2()).normalized().adjusted(-width, -width, width, width));
    return path;
}

QRectF QArcLine::boundingRect() const
{
    return QGraphicsLineItem::boundingRect().adjusted(-10, -10, 10, 10);
}

void QArcLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen myPen = pen();
    painter->setPen(myPen);
    painter->drawLine(line());
    if (isSelected()) {
        painter->setBrush(Qt::green);
        int lineWidth = myPen.width();
        myPen.setWidth(1);
        painter->setPen(myPen);
        painter->drawEllipse(line().p1(), lineWidth + 1.2, lineWidth + 1.2);
        painter->drawEllipse(line().p2(), lineWidth + 1.2, lineWidth + 1.2);
        painter->setBrush(Qt::red);
        painter->drawEllipse((line().p1() + line().p2()) / 2, lineWidth + 1.2, lineWidth + 1.2);
    }
}

QVariant QArcLine::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        setZValue(value.toBool());

    return value;
}

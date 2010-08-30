#include "qarcline.h"
#include "qarc.h"

QArcLine::QArcLine(QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene)
{

};

QPainterPath QArcLine::shape() const {
    QPainterPath path;
    path.addRect(QGraphicsLineItem::shape().boundingRect().adjusted(-5, -5, 5, 5));
    return path;
}

void QArcLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->drawLine(line());
    if (isSelected()){
        painter->setBrush(Qt::green);
        QPen myPen = pen();
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
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (isSelected())
            setZValue(1);
        else
            setZValue(0);
    }

    return value;
}

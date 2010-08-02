#include "qmultiproctop.h"

QMultiProcTop::QMultiProcTop(QMenu *contextMenu,
                             QGraphicsItem *parent, QGraphicsScene *scene)
                                 : QGraphicsRectItem(parent, scene)
{
    myContextMenu = contextMenu;
    setRect(-70, -50, 140, 100);

    setBrush(QBrush(Qt::white,Qt::SolidPattern));
    setZValue(1);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    procCount = 100;

}

void QMultiProcTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = painter->pen();
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    //верзний прямоугольник
    painter->drawRoundedRect(QRect(-25, -50, 50, 30), 10, 10);
    painter->drawLine(-25, -35, -50, -35);
    painter->drawLine(25, -35, 50, -35);
    painter->drawLine(-50, -35, -50, 35);
    painter->drawLine(50, -35, 50, 35);
    painter->drawLine(-50, 35, -25, 35);
    painter->drawLine(50, 35, 25, 35);
    //нижний прямоуголник
    painter->drawRoundedRect(QRect(-25, 20, 50, 30), 10, 10);
    //левый прямоугольник
    painter->drawRect(QRect(-70, -15, 40, 30));
    //правый прямоуголник
    painter->drawRect(QRect(30, -15, 40, 30));

    //левый штрих
    painter->drawLine(-47, 18, -53, 24);
    //правый штрих
    painter->drawLine(53, 18, 47, 24);

    //Стрелки
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    pen.setWidth(1);
    painter->setPen(pen);
    QPolygonF arrow;
    arrow << QPointF(-50, -16) << QPointF(-53, -20) <<  QPointF(-47, -20) << QPointF(-50, -16);
    painter->drawPolygon(arrow);
    arrow.clear();
    arrow << QPointF(50, -16) << QPointF(53, -20) <<  QPointF(47, -20) << QPointF(50, -16);
    painter->drawPolygon(arrow);
    arrow.clear();
    arrow << QPointF(-25, 35) << QPointF(-29, 32) <<  QPointF(-29, 38) << QPointF(-25, 35);
    painter->drawPolygon(arrow);
    arrow.clear();
    arrow << QPointF(25, 35) << QPointF(29, 32) <<  QPointF(29, 38) << QPointF(25, 35);
    painter->drawPolygon(arrow);
    arrow.clear();

    //левый кружочек
    painter->setBrush(QBrush(Qt::gray,Qt::SolidPattern));
    painter->drawEllipse(QPoint(-35, -35), 7, 7);
    //правый кружочек
    painter->drawEllipse(QPoint(35, -35), 7, 7);

    //текст
    QFont old_font = painter->font();
    old_font.setPointSize(16);
    old_font.setBold(true);
    painter->setFont(old_font);
    painter->drawText(QRect(-25, -15, 50, 30), Qt::AlignCenter, QString::number(procCount));

    if (isSelected()) {
        painter->setBrush(QBrush(Qt::white,Qt::NoBrush));
        painter->setPen(Qt::DashLine);
        painter->drawRect(boundingRect());
    }

    //QGraphicsRectItem::paint(painter, option, widget);
}

QRectF QMultiProcTop::boundingRect() const
{
    return QGraphicsRectItem::boundingRect().adjusted(-5, -5, 5, 5);
}

#include "qdataitem.h"

QDataItem::QDataItem(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem(parent, scene)
{
    QPen p = pen();
    p.setWidth(2);
    setPen(p);
    setBrush(QBrush(QColor::fromRgb(120, 120, 120), Qt::SolidPattern));
    setZValue(3);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);

    variable = NULL;

    setRect(-35, -10, 70, 20);
}

void QDataItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::TextAntialiasing |
                            QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    QGraphicsRectItem::paint(painter, option, widget);

    //пишем текст
    if (variable != NULL) {
        QTextOption opt;
        opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        opt.setAlignment(Qt::AlignLeft);
        painter->drawText(boundingRect().adjusted(7, 6, -6, -6), variable->name, opt);
    }
}

QDataItem::~QDataItem()
{

}

void QDataItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::red);
    setPen(myPen);
    event->accept();
}

void QDataItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::black);
    setPen(myPen);
    event->accept();
}

void QDataItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPen myPen = pen();
    myPen.setColor(Qt::red);
    setPen(myPen);
    event->accept();
}

QRectF QDataItem::boundingRect() const
{
    return QGraphicsRectItem::boundingRect().adjusted(-3, -3, 3, 3);
}

QPainterPath QDataItem::shape() const
{
    QPainterPath result;
    result.addRect(rect().adjusted(-1,-1,1,1));
    return result;
}

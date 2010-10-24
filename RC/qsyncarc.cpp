#include <QtGui>
#include <math.h>
#include "qsyncarc.h"

const qreal Pi = 3.14;

QSyncArc::QSyncArc(QTop *startItem, QTop *endItem, QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene){
    myStartTop = startItem;
    myEndTop = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::red;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    myContextMenu = contextMenu;
};

void QSyncArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QPainterPath QSyncArc::shape() const {
    QPainterPath path;
    path.addRect(QGraphicsLineItem::shape().boundingRect().adjusted(-5, -5, 5, 5));
    return path;
}


QRectF QSyncArc::boundingRect() const {
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void QSyncArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget){

    if (myStartTop->collidesWithItem(myEndTop))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);

    myPen.setStyle(Qt::DashLine);
    QVector<qreal> dashes;
    qreal space = 4;
    dashes << 6 << space;
    myPen.setDashPattern(dashes);
    qreal arrowSize = 12;
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QLineF centerLine(line());
    QPointF intersectPoint;
    myStartTop->getIntersectBound(centerLine).intersect(centerLine, &intersectPoint);

    setLine(QLineF(intersectPoint, myStartTop->pos()));

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected()) {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

QSyncArc::~QSyncArc()
{
    if (myStartTop != NULL)
        myStartTop->removeSync(this);
    if (myEndTop != NULL)
        myEndTop->removeSync(this);
}

bool QSyncArc::remake(QTop *aMovedTop, float dx, float dy)
{
    if (aMovedTop == myStartTop) {
        QPointF newStartP = line().p1() + QPointF(dx, dy);
        setLine(QLineF(newStartP, line().p2()));
    } else if (aMovedTop == myEndTop) {
        QPointF newEndP = line().p2() + QPointF(dx, dy);
        setLine(QLineF(line().p1(), newEndP));
    }
    return true;
}

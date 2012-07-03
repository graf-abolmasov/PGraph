#include <QtGui>
#include <math.h>
#include "qsyncarc.h"
#include "qtop.h"

const qreal Pi = 3.14;

QSyncArc::QSyncArc(QTop *startItem, QTop *endItem, QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene){
    myStartTop = startItem;
    myEndTop = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    eff->setOffset(2);
    eff->setBlurRadius(15);
    setGraphicsEffect(eff);

    //делаем нужный пунктир
    QPen myPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QVector<qreal> dashes;
    qreal space = 4; //расстояние между штрихами
    dashes << 6 << space; //длина штриха
    myPen.setDashPattern(dashes);

    setPen(myPen);
    myContextMenu = contextMenu;
}

void QSyncArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    if (myContextMenu != NULL)
        myContextMenu->exec(event->screenPos());
}

QPainterPath QSyncArc::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arcHead);
    return path;
}


QRectF QSyncArc::boundingRect() const {
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void QSyncArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){

    if (myStartTop->collidesWithItem(myEndTop))
        return;

    if ((myStartTop == NULL) || (myEndTop == NULL)) {
        //пока рисуем(создаем) дугу
    } else {
        qreal arrowSize = 12; // размер стрелки
        painter->setPen(pen());
        painter->setBrush(pen().color());
        painter->setRenderHints(QPainter::TextAntialiasing |
                                QPainter::Antialiasing |
                                QPainter::HighQualityAntialiasing);

        //рисуем линию
        painter->drawLine(line());

        //вычисляем пложение стрелки
        QPointF endIntersectPoint;
        myEndTop->getIntersectBound(line()).intersect(line(), &endIntersectPoint);
        QPointF startIntersectPoint;
        myStartTop->getIntersectBound(line()).intersect(line(), &startIntersectPoint);
        float koeff2 = QLineF(line().p2(), endIntersectPoint).length(); //ЫЫЫ =)
        QPointF t = line().p2() - QPointF(cos(line().angle() * Pi / 180) * koeff2, -sin(line().angle() * Pi / 180) * koeff2);
        double angle = acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;
        QPointF arcP1 = t - QPointF(sin(angle + Pi / 3) * arrowSize,
                                    cos(angle + Pi / 3) * arrowSize);
        QPointF arcP2 = t - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                    cos(angle + Pi - Pi / 3) * arrowSize);
        arcHead.clear();
        arcHead << t << arcP1 << arcP2;
        painter->drawPolygon(arcHead);

        if (isSelected()){
            painter->setBrush(Qt::green);
            QPen myPen = pen();
            int lineWidth = myPen.width();
            myPen.setWidth(1);
            myPen.setColor(Qt::black);
            painter->setPen(myPen);
            painter->drawEllipse(startIntersectPoint, lineWidth + 3, lineWidth + 3);
            painter->drawEllipse(endIntersectPoint, lineWidth + 3, lineWidth + 3);
            painter->setBrush(Qt::darkBlue);
            painter->drawEllipse((line().p1() + line().p2()) / 2, lineWidth + 2, lineWidth + 2);
        }
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

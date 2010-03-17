#include <QtGui>

#include "qdiagramitem.h"
#include "qarc.h"

int TTop::counter = 0;

TTop::TTop(QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myContextMenu = contextMenu;

    QPainterPath path;
    QPolygonF myPolygon;
    myPolygon << QPointF(-40, 30) << QPointF(40, 30)
            << QPointF(40,-30) << QPointF(-40, -30)
            << QPointF(-40, 30);

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAsRoot(false);
    number = counter++;
}

QRectF TTop::boundingRect() const{
    return QGraphicsPolygonItem::boundingRect().adjusted(-5, -5, 5, 5);
}


/*!
  Удаляет дугу. Совсем.
  @param arc - дуга
*/
void TTop::removeArc(TArc *arc)
{
    int index = arcs.indexOf(arc);

    if (index != -1) {
        arcs.removeAt(index);

        QList<TArc *> outArcsList = outArcs();

        if (arc->startItem() == this){
            for (int i = 0; i < outArcsList.count(); i++){
                if (outArcsList.at(i)->priority() > arc->priority()){
                    outArcsList.at(i)->setPriority(outArcsList.at(i)->priority() - 1);
                }
            }
        }
    }
}

/*!
  Удаляет все дуги, входящие и выходящие из вершины
*/
void TTop::removeArcs()
{
    foreach (TArc *arc, arcs) {
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
        scene()->removeItem(arc);
        delete arc;
    }
}
/*!
  Добавляет дугу в список дуг текущей вершины
  @param arc - дуга
*/
void TTop::addArc(TArc *arc)
{
    arcs.append(arc);
}

void TTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant TTop::itemChange(GraphicsItemChange change, const QVariant &value){
    /*if (change == QGraphicsItem::ItemPositionChange) {
        foreach (TArc *arc, arcs) {
            arc->updatePosition();
        }
    }*/

    return value;
}

float TTop::getMinWidth(){
    QList<TArc* > arcListAtBound2 = getArcsAtBound(1);
    QList<TArc* > arcListAtBound1 = getArcsAtBound(3);
    QList<TArc* > arcList;
    foreach (TArc* arc, arcListAtBound1)
        arcList.append(arc);
    foreach (TArc* arc, arcListAtBound2)
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

        foreach (TArc* arc, arcList){
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

float TTop::getMinHeight(){
    QList<TArc* > arcListAtBound2 = getArcsAtBound(2);
    QList<TArc* > arcListAtBound1 = getArcsAtBound(4);
    QList<TArc* > arcList;
    foreach (TArc* arc, arcListAtBound1)
        arcList.append(arc);
    foreach (TArc* arc, arcListAtBound2)
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


        foreach (TArc* arc, arcList){
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

QList<TArc *> TTop::getArcsAtBound(int i)
{
    QList<TArc *> result;
    QPointF p1 = polygon().at(i-1) + pos();
    QPointF p2 = polygon().at(i) + pos();
    QPointF intersectPoint;
    QLineF polyLine = QLineF(p1, p2);
    foreach (TArc *arc, arcs) {
        QLineF arcLine;
        if (arc->lines.count() == 0)
            continue;
        if (this == arc->startItem())
            arcLine = arc->lines.first()->line();
        else
            arcLine = arc->lines.last()->line();
        QLineF::IntersectType intersectType = polyLine.intersect(arcLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) {
            result.append(arc);
        }
    }

    return result;
}

QList<TArc *> TTop::getArcsAtBound(QLineF bound)
{
    QPointF intersectPoint;
    QList<TArc *> result;
    foreach (TArc *arc, arcs) {
        QLineF arcLine;
        if (arc->lines.count() == 0)
            continue;
        if (this == arc->startItem())
            arcLine = arc->lines.first()->line();
        else
            arcLine = arc->lines.last()->line();
        QLineF::IntersectType intersectType = bound.intersect(arcLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) {
            result.append(arc);
        }
    }
    return result;
}

void TTop::autoArrangeArcsAtBound(QLineF bound){

    QList<TArc*> arcs = getArcsAtBound(bound);
    float dist = bound.length()/(arcs.count() + 1);
    int i = 1;
    
    foreach (TArc* arc, arcs){
        if (arc->startItem() == this){
            if (arc->lines.first()->line().y1() == arc->lines.first()->line().y2()){
                arc->lines.first()->setLine(arc->lines.first()->line().p1().x(), sceneBoundingRect().topLeft().y() + i*dist, arc->lines.first()->line().p2().x(), sceneBoundingRect().topLeft().y() + i*dist);
            }

            if (arc->lines.first()->line().x1() == arc->lines.first()->line().x2()){
                arc->lines.first()->setLine(sceneBoundingRect().topLeft().x() + i*dist, arc->lines.first()->line().p1().y(), sceneBoundingRect().topLeft().x() + i*dist, arc->lines.first()->line().p2().y());
            }
        }
        if (arc->endItem() == this){
            if (arc->lines.last()->line().y1() == arc->lines.last()->line().y2()){
                arc->lines.last()->setLine(arc->lines.last()->line().p1().x(), sceneBoundingRect().topLeft().y() + i*dist, arc->lines.last()->line().p2().x(), sceneBoundingRect().topLeft().y() + i*dist);
            }

            if (arc->lines.last()->line().x1() == arc->lines.last()->line().x2()){
                arc->lines.last()->setLine(sceneBoundingRect().topLeft().x() + i*dist, arc->lines.last()->line().p1().y(), sceneBoundingRect().topLeft().x() + i*dist, arc->lines.last()->line().p2().y());
            }
        }
        i++;
    }
}

/*!
  Равномерно распределяет дуги вдоль границ
*/
void TTop::autoArrangeArcs(QList<TArc* > brokenLines){

    for (int i = 1; i < 5; i++){
        QList<TArc* > arcList;
        QList<TArc* > arcsAtBoundList = getArcsAtBound(i);
        foreach (TArc* arc, arcsAtBoundList){
            if (brokenLines.contains(arc))
                arcList.append(arc);
        };

        QLineF bound =  QLineF(polygon().at(i-1) + scenePos(), polygon().at(i) + scenePos());
        float dist = bound.length() / (arcList.count() + 1);
        //если i нечетная - то горизонтальная
        //иначе вертикальная граница
        int j = 0;
        QPointF intersectPoint;
        switch(i){
        case 3://верхняя граница
            foreach (TArc* arc, arcList){
                j++;
                if (arc->startItem() == this){
                    arc->lines.first()->setLine(QLineF(QPointF(bound.x1(), arc->lines.first()->line().y1()) - QPointF(dist*j, 0),
                                                       QPointF(bound.x1(), arc->lines.first()->line().y2()) - QPointF(dist*j, 0)));
                    if (arc->lines.count() > 1){
                        arc->lines.first()->line().intersect(arc->lines.at(1)->line(), &intersectPoint);
                        arc->lines.first()->setLine(QLineF(arc->lines.first()->line().p1(), intersectPoint));
                        arc->lines.at(1)->setLine(QLineF(intersectPoint, arc->lines.at(1)->line().p2()));
                    }
                } else {
                    arc->lines.last()->setLine(QLineF(QPointF(bound.x1(), arc->lines.last()->line().y1()) - QPointF(dist*j, 0),
                                                      QPointF(bound.x1(), arc->lines.last()->line().y2()) - QPointF(dist*j, 0)));
                    if (arc->lines.count() > 1){
                        arc->lines.last()->line().intersect(arc->lines.at(arc->lines.count()-1)->line(), &intersectPoint);
                        arc->lines.last()->setLine(QLineF(intersectPoint, arc->lines.last()->line().p2()));
                        arc->lines.at(arc->lines.count()-1)->setLine(QLineF(arc->lines.at(arc->lines.count()-1)->line().p1(), intersectPoint));
                    }
                }

            }
            break;
        case 2://правая граница
            foreach (TArc* arc, arcList){
                j++;
                if (arc->startItem() == this){
                    arc->lines.first()->setLine(QLineF(QPointF(arc->lines.first()->line().x1(), bound.y2()) + QPointF(0, dist*j),
                                                       QPointF(arc->lines.first()->line().x2(), bound.y2()) + QPointF(0, dist*j)));
                    if (arc->lines.count() > 1){
                        arc->lines.first()->line().intersect(arc->lines.at(1)->line(), &intersectPoint);
                        arc->lines.first()->setLine(QLineF(arc->lines.first()->line().p1(), intersectPoint));
                        arc->lines.at(1)->setLine(QLineF(intersectPoint, arc->lines.at(1)->line().p2()));
                    }
                } else {
                    arc->lines.last()->setLine(QLineF(QPointF(arc->lines.last()->line().x1(), bound.y2()) + QPointF(0, dist*j),
                                                      QPointF(arc->lines.last()->line().x2(), bound.y2()) + QPointF(0, dist*j)));
                    if (arc->lines.count() > 1){
                        arc->lines.last()->line().intersect(arc->lines.at(arc->lines.count()-1)->line(), &intersectPoint);
                        arc->lines.last()->setLine(QLineF(intersectPoint, arc->lines.last()->line().p2()));
                        arc->lines.at(arc->lines.count()-1)->setLine(QLineF(arc->lines.at(arc->lines.count()-1)->line().p1(), intersectPoint));
                    }
                }
            }
            break;
        case 1://нижняя граница
            foreach (TArc* arc, arcList){
                j++;
                if (arc->startItem() == this){
                    arc->lines.first()->setLine(QLineF(QPointF(bound.x1(), arc->lines.first()->line().y1()) + QPointF(dist*j, 0),
                                                       QPointF(bound.x1(), arc->lines.first()->line().y2()) + QPointF(dist*j, 0)));
                    if (arc->lines.count() > 1){
                        arc->lines.first()->line().intersect(arc->lines.at(1)->line(), &intersectPoint);
                        arc->lines.first()->setLine(QLineF(arc->lines.first()->line().p1(), intersectPoint));
                        arc->lines.at(1)->setLine(QLineF(intersectPoint, arc->lines.at(1)->line().p2()));
                    }
                } else {
                    arc->lines.last()->setLine(QLineF(QPointF(bound.x1(), arc->lines.last()->line().y1()) + QPointF(dist*j, 0),
                                                      QPointF(bound.x1(), arc->lines.last()->line().y2()) + QPointF(dist*j, 0)));
                    if (arc->lines.count() > 1){
                        arc->lines.last()->line().intersect(arc->lines.at(arc->lines.count()-1)->line(), &intersectPoint);
                        arc->lines.last()->setLine(QLineF(intersectPoint, arc->lines.last()->line().p2()));
                        arc->lines.at(arc->lines.count()-1)->setLine(QLineF(arc->lines.at(arc->lines.count()-1)->line().p1(), intersectPoint));
                    }
                }
            }
            break;
        case 4://левая граница
            foreach (TArc* arc, arcList){
                j++;
                if (arc->startItem() == this){
                    arc->lines.first()->setLine(QLineF(QPointF(arc->lines.first()->line().x1(), bound.y2()) - QPointF(0, dist*j),
                                                       QPointF(arc->lines.first()->line().x2(), bound.y2()) - QPointF(0, dist*j)));
                    if (arc->lines.count() > 1){
                        arc->lines.first()->line().intersect(arc->lines.at(1)->line(), &intersectPoint);
                        arc->lines.first()->setLine(QLineF(arc->lines.first()->line().p1(), intersectPoint));
                        arc->lines.at(1)->setLine(QLineF(intersectPoint, arc->lines.at(1)->line().p2()));
                    }
                } else {
                    arc->lines.last()->setLine(QLineF(QPointF(arc->lines.last()->line().x1(), bound.y2()) - QPointF(0, dist*j),
                                                      QPointF(arc->lines.last()->line().x2(), bound.y2()) - QPointF(0, dist*j)));
                    if (arc->lines.count() > 1){
                        arc->lines.last()->line().intersect(arc->lines.at(arc->lines.count()-1)->line(), &intersectPoint);
                        arc->lines.last()->setLine(QLineF(intersectPoint, arc->lines.last()->line().p2()));
                        arc->lines.at(arc->lines.count()-1)->setLine(QLineF(arc->lines.at(arc->lines.count()-1)->line().p1(), intersectPoint));
                    }
                }
            }
            break;
        }
    }
}

/*!
  Возвращает границу с которой пересекается линия
  @param line - линия
*/
QLineF TTop::getIntersectBound(QLineF line)
{
    int i;
    QLineF result;
    QPointF intersectPoint;
    QPointF p1, p2;

    p1 = polygon().first() + pos();
    for (i = 1; i < polygon().count(); ++i) {
        p2 = polygon().at(i) + pos();
        result.setPoints(p1, p2);
        QLineF::IntersectType intersectType = result.intersect(line, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
            p1 = p2;
        }
   return result;
}
/*!
  Устанавливает иконку.
  @param icon - иконка
*/
void TTop::setIcon(QImage icon)
{
    myIcon = icon;
}

void TTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
          QWidget *widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);
    if (!myIcon.isNull()) {
        if ((myIcon.width() >= boundingRect().width()) ||
            (myIcon.height() >= boundingRect().height())) {
            painter->drawImage(boundingRect(), myIcon);
        } else {
            painter->drawImage(-myIcon.width()/2, -myIcon.height()/2, myIcon);
        }
    }
}

/*!
  Устанавливает флаг корневой вершины.
  @param flag - true если корневая
*/
void TTop::setAsRoot(bool flag){
    isRoot = flag;
    if (isRoot){
        QPen p = pen();// painter->pen();
        p.setWidth(5);
        setPen(p);
    } else {
        QPen p = pen();// painter->pen();
        p.setWidth(2);
        setPen(p);
    }
}
/*!
  Возвращает список исходящих дуг
*/
QList<TArc *> TTop::inArcs(){
    QList<TArc *> result;
    foreach(TArc *arc, arcs){
        if (arc->endItem() == this)
            result.append(arc);
    }
    return result;
}

/*!
  Возвращяет список входящих дуг
*/
QList<TArc *> TTop::outArcs(){
    QList<TArc *> result;
    foreach(TArc *arc, arcs){
        if (arc->startItem() == this)
            result.append(arc);
    }
    return result;
}

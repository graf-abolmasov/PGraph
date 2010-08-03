#include <QtCore>
#include <QtGui>

#include "qdiagramitem.h"

int QTop::counter = 0;

QTop::QTop(QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myContextMenu = contextMenu;

    setBrush(QBrush(Qt::white,Qt::SolidPattern));
    setZValue(1);

    QPainterPath path;
    QPolygonF myPolygon;
    myPolygon << QPointF(-35, 25) << QPointF(35, 25)
            << QPointF(35,-25) << QPointF(-35, -25)
            << QPointF(-35, 25);

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAsRoot(false);
    number = counter++;

    actor = NULL;

}

QRectF QTop::boundingRect() const{
    return QGraphicsPolygonItem::boundingRect().adjusted(-5, -5, 5, 5);
}

/*!
  Добавляет дугу синхронизации
*/

void QTop::addSync(QSyncArc *arc){
    sync.append(arc);
}

/*!
  Удаляет дугу cby[hjybpfwbb. Совсем.
  @param arc - дуга
*/
void QTop::removeSync(QSyncArc *arc){
    int index = sync.indexOf(arc);
    if (index != -1)
        sync.removeAt(index);
}

/*!
  Удаляет дугу. Совсем.
  @param arc - дуга
*/
void QTop::removeArc(QArc *arc){
    int index = arcs.indexOf(arc);

    if (index != -1) {
        arcs.removeAt(index);

        QList<QArc *> outArcsList = outArcs();

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
void QTop::removeArcs(){
    foreach (QArc *arc, arcs) {
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
        scene()->removeItem(arc);
        delete arc;
    }
}

/*!
  Удаляет все дуги синхронизации, входящие и выходящие из вершины
*/
void QTop::removeSyncs(){
    foreach (QSyncArc *arc, sync) {
        arc->startItem()->removeSync(arc);
        arc->endItem()->removeSync(arc);
        scene()->removeItem(arc);
        delete arc;
    }
}

/*!
  Добавляет дугу в список дуг текущей вершины
  @param arc - дуга
*/
void QTop::addArc(QArc *arc){
    arcs.append(arc);
}

void QTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant QTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (QSyncArc *arc, sync) {
            arc->updatePosition();
        }
    }

    return value;
}

float QTop::getMinWidth(){
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

float QTop::getMinHeight(){
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

QList<QArc *> QTop::getArcsAtBound(int i){
    QList<QArc *> result;
    QPointF p1 = polygon().at(i-1) + pos();
    QPointF p2 = polygon().at(i) + pos();
    QPointF intersectPoint;
    QLineF polyLine = QLineF(p1, p2);
    foreach (QArc *arc, arcs) {
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

QList<QArc *> QTop::getArcsAtBound(QLineF bound){
    QPointF intersectPoint;
    QList<QArc *> result;
    foreach (QArc *arc, arcs) {
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

/*!
  Возвращает границу с которой пересекается линия
  @param line - линия
*/
QLineF QTop::getIntersectBound(QLineF line){
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
void QTop::setIcon(QImage icon){
    myIcon = icon;
}

void QTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsPolygonItem::paint(painter, option, widget);
    //рисуем иконку
    if (!myIcon.isNull()) {
        if ((myIcon.width() >= boundingRect().width()) ||
            (myIcon.height() >= boundingRect().height())) {
            painter->drawImage(boundingRect().adjusted(5, 5, -5, -5), myIcon);
        } else {
            painter->drawImage(-myIcon.width()/2, -myIcon.height()/2, myIcon);
        }
    }
    //пишем текст
    else if (actor != NULL) {
        QTextOption opt;
        opt.setWrapMode(QTextOption::WrapAnywhere);
        opt.setAlignment(Qt::AlignLeft);
        painter->drawText(boundingRect().adjusted(8, 8, -8, -8), actor->extName, opt);
    }

}

/*!
  Устанавливает флаг корневой вершины.
  @param flag - true если корневая
*/
void QTop::setAsRoot(bool flag){
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
QList<QArc *> QTop::inArcs(){
    QList<QArc *> result;
    foreach(QArc *arc, arcs){
        if (arc->endItem() == this)
            result.append(arc);
    }
    return result;
}

/*!
  Возвращяет список входящих дуг
*/
QList<QArc *> QTop::outArcs(){
    QList<QArc *> result;
    foreach(QArc *arc, arcs){
        if (arc->startItem() == this)
            result.append(arc);
    }
    return result;
}

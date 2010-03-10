#include <QtGui>

#include "qdiagramitem.h"
#include "qarc.h"

TTop::TTop(DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;

    QPainterPath path;
    switch (myDiagramType) {
        case Top:
            myPolygon << QPointF(-40, 30) << QPointF(40, 30)
                      << QPointF(40,-30) << QPointF(-40, -30)
                      << QPointF(-40, 30);
            break;
        default:
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAsRoot(false);
}

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
  @Param arc - дуга
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

QVariant TTop::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (TArc *arc, arcs) {
            arc->updatePosition();
        }
    }

    return value;
}
/*!
  РІРѕР·РІСЂР°С‰Р°РµС‚ СЃРїРёСЃРѕРє РґСѓРі, РїСЂРёР»РµРїР»РµРЅРЅС‹С… Рє РґР°РЅРЅРѕР№ РіСЂР°РЅРёС†Рµ.
  i - РЅРѕРјРµСЂ РІРµСЂС€РёРЅС‹ РїРѕР»РёРіРѕРЅР°, РєРѕРЅРµС‡РЅР°СЏ С‚РѕС‡РєР° РѕС‚СЂРµР·РєР°
*/
QList<TArc *> TTop::getArcsAtBound(int i)
{
    QList<TArc *> result;
    QPointF p1 = polygon().at(i-1) + pos();
    QPointF p2 = polygon().at(i) + pos();
    QPointF intersectPoint;
    QLineF polyLine = QLineF(p1, p2);
    foreach (TArc *arc, arcs) {
        QLineF arcLine = arc->line();
        QLineF::IntersectType intersectType = polyLine.intersect(arcLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) {
            result.append(arc);
        }
    }
    return result;
}

/*!
  РІРѕР·РІСЂР°С‰Р°РµС‚ СЃРїРёСЃРѕРє РґСѓРі, РїСЂРёР»РµРїР»РµРЅРЅС‹С… Рє РґР°РЅРЅРѕР№ РіСЂР°РЅРёС†Рµ.
  bound - РіСЂР°РЅРёС†Р°
*/
QList<TArc *> TTop::getArcsAtBound(QLineF bound)
{
    QPointF intersectPoint;
    QList<TArc *> result;
    foreach (TArc *arc, arcs) {
        QLineF arcLine = arc->line();
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
  РІРѕР·РІСЂР°С‰Р°РµС‚ РіСЂР°РЅРёС†Сѓ РїРѕ РєРѕС‚РѕСЂРѕР№ РїРµСЂРµСЃРµРєР°РµС‚СЃСЏ СЃ Р»РёРЅРёРµР№
  line - Р»РёРЅРёСЏ
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
  @Param icon - иконка
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
  @Param flag - true если корневая
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

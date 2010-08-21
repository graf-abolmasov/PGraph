#include <QtCore>
#include <QtGui>

#include "qtop.h"

int QTop::counter = 0;

QTop::QTop(QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem(parent, scene)
{
    myContextMenu = contextMenu;

    QPen p = pen();
    p.setWidth(2);
    setPen(p);
    setBrush(QBrush(QColor::fromRgb(220, 220, 220), Qt::SolidPattern));
    setZValue(1);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    isRoot  = false;
    number = counter++;
    actor = NULL;
}

QRectF QTop::boundingRect() const
{
    return QGraphicsRectItem::boundingRect().adjusted(-3, -3, 3, 3);
}

QPainterPath QTop::shape() const
{
    QPainterPath result;
    result.addRect(rect().adjusted(-1,-1,1,1));
    return result;
}

QPainterPath QTop::opaqueArea() const
{
    QPainterPath result;
    result.addRect(rect());
    return result;
}

/*!
  Добавляет дугу синхронизации
*/

void QTop::addSync(QSyncArc *arc){
    sync.append(arc);
}

/*!
  Удаляет дугу синхронизации. Совсем.
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
    if (!arcs.contains(arc))
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

QList<QArc *> QTop::getArcsAtBound(int i){
    QList<QArc *> result;
    QPolygonF myPolygon(rect());

    QPointF p1 = myPolygon.at(i-1) + pos();
    QPointF p2 = myPolygon.at(i) + pos();
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

/*QList<QArc *> QTop::getArcsAtBound(QLineF bound){
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
}*/

/*!
  Возвращает границу с которой пересекается линия
  @param line - линия
*/
QLineF QTop::getIntersectBound(QLineF line){
    int i;
    QLineF result;
    QPointF intersectPoint;
    QPointF p1, p2;
    QPolygonF myPolygon(rect());
    p1 = myPolygon.first() + scenePos();
    for (i = 1; i < myPolygon.count(); i++) {
        p2 = myPolygon.at(i) + scenePos();
        result.setPoints(p1, p2);
        QLineF::IntersectType intersectType = result.intersect(line, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) break;
        p1 = p2;
    }
    return result;
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

Top::Top(float x, float y, float sizeX, float sizeY, int number, int procCount, bool isRoot, QString actor, QString type)
{
    this->x = x;
    this->y = y;
    this->actor = actor;
    this->isRoot = isRoot;
    this->number = number;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->type = type;
    this->procCount = procCount;
}

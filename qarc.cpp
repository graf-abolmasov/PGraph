#include <QtGui>
#include "qarc.h"
#include "math.h"
#include "time.h"

const qreal Pi = 3.14;
const int MINDELTA = 40;

int dvec2log(float dx, float dy){
    int res = 0;

    if (dx > 0) res |= RIGHT;
    else if (dx < 0) res |= LEFT;
    if (dy > 0) res |= DOWN;
    else if (dy < 0) res |= UP;
    return res;
}

/*!
  Упрощенный алгоритм перестройки дуги
*/
bool TArc::autoBuild(TTop* top, float dx, float dy){
    QPointF startPoint;
    QPointF endPoint;
    if (top == startItem()){
        startPoint = lines.first()->line().p1() + QPointF(dx, dy);
        endPoint = lines.last()->line().p2();
    } else {
        startPoint = lines.first()->line().p1();
        endPoint = lines.last()->line().p2() + QPointF(dx, dy);
    }

    foreach(TArcLine* line, lines){
        delete line;
        line = NULL;
    }
    lines.clear();

    /*высчитываем главный вектор дуги*/
    QLineF centerLine(startPoint, endPoint);

    QPointF startIntersectPoint;
    QLineF polyLineStart;
    for (int i = 1; i < 5; i++){
        QPointF p1 = startItem()->polygon().at(i-1) + startItem()->scenePos();
        QPointF p2 = startItem()->polygon().at(i) + startItem()->scenePos();
        polyLineStart = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLineStart.intersect(centerLine, &startIntersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
    }
    QPointF endIntersectPoint;
    QLineF polyLineEnd;
    for (int i = 1; i < 5; i++){
        QPointF p1 = endItem()->polygon().at(i-1) + endItem()->scenePos();
        QPointF p2 = endItem()->polygon().at(i) + endItem()->scenePos();
        polyLineEnd = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLineEnd.intersect(centerLine, &endIntersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
    }

    double deltaY = endPoint.y() - startPoint.y();
    double deltaX = endPoint.x() - startPoint.x();
    QPointF P1, P2;
    //vertical border start
    if ((polyLineStart.angle() == 90) ||
        (polyLineStart.angle() == 270)) {

        //parallel border end
        if ((polyLineEnd.angleTo(polyLineStart) == 0) ||
            (polyLineEnd.angleTo(polyLineStart) == 180)) {
            P1.setX(startPoint.x() + deltaX/2);
            P1.setY(startPoint.y());
            P2.setX(P1.x());
            P2.setY(endPoint.y());
            newLine(startPoint, P1);
            newLine(P1, P2);
            newLine(P2, endPoint);
        }
        //perpendicular border end
        if ((polyLineEnd.angleTo(polyLineStart) == 90) ||
            (polyLineEnd.angleTo(polyLineStart) == 270)) {
            P1.setX(endPoint.x());
            P1.setY(startPoint.y());
            newLine(startPoint, P1);
            newLine(P1, endPoint);
        }
    }
    //horizontal border start
    if ((polyLineStart.angle() == 0) ||
        (polyLineStart.angle() == 180)) {
        //perpendicular border end
        if ((polyLineEnd.angleTo(polyLineStart) == 90) ||
            (polyLineEnd.angleTo(polyLineStart) == 270)) {
            P1.setX(startPoint.x());
            P1.setY(endPoint.y());
            newLine(startPoint, P1);
            newLine(P1, endPoint);
        }
        //parallel border end
        if ((polyLineEnd.angleTo(polyLineStart) == 0) ||
            (polyLineEnd.angleTo(polyLineStart) == 180)) {
            P1.setX(startPoint.x());
            P1.setY(startPoint.y() + deltaY/2);
            P2.setX(endPoint.x());
            P2.setY(P1.y());
            newLine(startPoint, P1);
            newLine(P1, P2);
            newLine(P2, endPoint);
        }
    }
    addLine(currentLine);

    currentLine = NULL;
    return true;
}

/*!
  Хитрый алгоритм перерисовки дуги при перемещении вершины.
  @param aMOvedTop - указатель на перетаскиваемую вершину
  @param dx - перемещение по X
  @param dy - перемещение по Y
*/
bool TArc::remake(TTop* aMovedTop, float dx, float dy)
{
    bool result = true;

    int mx, my, j;
    QPointF pnts[4];
    bool flag; //false - если надо все напрочь переделать
    int lgdir, lgolddir;

    if ((myStartTop == aMovedTop) && (myEndTop == aMovedTop)){
        foreach (TArcLine* line, lines){
            line->setLine(line->line().translated(dx/2, dy/2));
        }
        return true;
    }

    if (lines.count() < 4){
        //старый алгоритм
        //переработано, дополнено, прокоментировано
        //заполняем структуры, необходимые для старого алгоритма
        int otr = lines.count();
        QPointF pts[4];
        int i;
        for(i =0; i < otr; i++){
            pts[i] = lines.at(i)->line().p1();
            pts[i+1] = lines.at(i)->line().p2();
        }

        QLineF startBorder = startItem()->getIntersectBound(lines.first()->line());
        QLineF endBorder = endItem()->getIntersectBound(lines.last()->line());

        QPointF startIntersectPoint;
        QPointF endIntersectPoint;

        startBorder.intersect(lines.first()->line(), &startIntersectPoint);
        endBorder.intersect(lines.last()->line(), &endIntersectPoint);

        QPointF deltaStartPoint = lines.first()->line().p1() - startIntersectPoint;
        QPointF deltaEndPoint = lines.last()->line().p2() - endIntersectPoint;

        pts[0] = startIntersectPoint;
        pts[i] = endIntersectPoint;

        bool fMovedTo = (aMovedTop == myEndTop);
        if (fMovedTo) //двигали конечную вершину
            for (j = 0; j <= otr; j++)
                pnts[j] = pts[j];
        else //двигали начальную вершину
            for (j = 0; j <= otr; j++)
                pnts[otr-j] = pts[j];
        flag = true;
        switch(otr) {
        case 1:
            lgolddir = dvec2log(pnts[1].x() - pnts[0].x(), pnts[1].y() - pnts[0].y());
            lgdir    = dvec2log(pnts[1].x() + dx - pnts[0].x(), pnts[1].y() + dy - pnts[0].y());
            if (lgolddir & lgdir){
                if (!(pnts[1].x() - pnts[0].x()) && !dx ||
                    !(pnts[1].y() - pnts[0].y()) && !dy){
                    pnts[1] += QPointF(dx, dy);
                } else {
                    pnts[3] = pnts[1] + QPointF(dx, dy);
                    pnts[1].setX(pnts[0].x() + !(!(pnts[1].x() - pnts[0].x()))*(pnts[3].x() - pnts[0].x())*(4-fMovedTo)/7);
                    pnts[1].setY(pnts[0].y() + !(!(pnts[1].y() - pnts[0].y()))*(pnts[3].y() - pnts[0].y())*(4-fMovedTo)/7);
                    if ((lgdir^lgolddir) & (LEFT | RIGHT)){
                        mx=1;
                        my=0;
                    } else {
                        mx=0;
                        my=1;
                    }
                    pnts[2] = pnts[1] + QPointF(dx*mx, dy*my);
                    otr=3;
                }
            }
            else flag = false;
            break;
            case 2:
            lgolddir = dvec2log(pnts[2].x() - pnts[0].x(), pnts[2].y() - pnts[0].y());
            lgdir    = dvec2log(pnts[2].x() + dx - pnts[0].x(), pnts[2].y() + dy - pnts[0].y());
            if (lgolddir == lgdir){
                pnts[1] += QPointF(dx*!(!(pnts[1].x() - pnts[0].x())), dy*!(!(pnts[1].y()-pnts[0].y())));
                pnts[2] += QPointF(dx, dy);
            }
            else flag = false;
            break;
            case 3:
            lgolddir = dvec2log(pnts[3].x() - pnts[2].x(), pnts[3].y() - pnts[2].y());
            lgdir    = dvec2log(pnts[3].x() + dx - pnts[1].x(), pnts[3].y() + dy - pnts[1].y());
            if (lgolddir & lgdir){
                pnts[2] += QPointF(dx*!(!(pnts[2].x() - pnts[1].x())), dy*!(!(pnts[2].y() - pnts[1].y())));
                pnts[3] += QPointF(dx, dy);
                if (pnts[1].x() == pnts[2].x() && pnts[1].y() == pnts[2].y()){
                    pnts[1] = pnts[3];
                    otr = 1;
                }
            }
            else flag = false;
            break;
        }
        if (flag) {
            if (fMovedTo){
                for (j = 0; j <= otr; j++)
                    pts[j] = pnts[j];
            } else {
                for (j = 0; j <= otr; j++)
                    pts[j] = pnts[otr-j];
            }
        } else {
            //нужна полная перерисвка
            result = false;
            //otr = 0;
        }

        //теперь в pts новые точки =) якобы. проверим это.
        foreach(TArcLine* line, lines){
            delete line;
            line = NULL;
        }
        lines.clear();
        currentLine = NULL;

        newLine(pts[0] + deltaStartPoint, pts[1]);
        for (j = 1; j < otr - 1; j++){
            newLine(pts[j], pts[j+1]);
        }
        newLine(pts[otr-1], pts[otr] + deltaEndPoint);
        addLine(currentLine);
        currentLine = NULL;

        return result;
    }
    
    //если передвинутая вершина - начало
    if (aMovedTop == myStartTop){
        //вертикальную линию двигаем только вправо/влево или удлинняем/укорачиваем
        if (lines.first()->line().p1().x() == lines.first()->line().p2().x()){
            //можно двигать пока остается место для серого квадратика
            if ((lines.first()->line().length() < MINDELTA) || (lines.last()->line().length() < MINDELTA)){
                return false;
            }
            lines.first()->setLine(QLineF(QPointF(lines.first()->line().p1().x() + dx,
                                                  lines.first()->line().p1().y() + dy),
                                          QPointF(lines.first()->line().p2().x() + dx,
                                                  lines.first()->line().p2().y())
                                          )
                                   );
            //перерисуем следующий кусочек тоже (если он есть)
            if (lines.count() > 1){
                lines.at(1)->setLine(QLineF(QPointF(lines.at(1)->line().p1().x() + dx,
                                                    lines.at(1)->line().p1().y()),
                                            lines.at(1)->line().p2()
                                            )
                                     );
            //если дуга соединяет вершины напрямую, то ограничим перемещение шириной конечной вершины
            } else {
                if ((lines.first()->line().p2().x() < myEndTop->sceneBoundingRect().bottomLeft().x()) ||
                    (lines.first()->line().p2().x() > myEndTop->sceneBoundingRect().bottomRight().x())){
                    lines.first()->setLine(QLineF(QPointF(lines.first()->line().p1().x() - dx,
                                                          lines.first()->line().p1().y() - dy),
                                                  QPointF(lines.first()->line().p2().x() - dx,
                                                          lines.first()->line().p2().y())
                                                  )
                                           );
                    return false;
                }
            }

            return true;
        }
        //горизонтальную линию можем двигать только вверх/вниз или удлиннять/укорачивать
        if (lines.first()->line().p1().y() == lines.first()->line().p2().y()){
            if ((lines.first()->line().length() < MINDELTA) || (lines.last()->line().length() < MINDELTA)){
                return false;
            }

            lines.first()->setLine(QLineF(QPointF(lines.first()->line().p1().x() + dx,
                                                  lines.first()->line().p1().y() + dy),
                                          QPointF(lines.first()->line().p2().x(),
                                                  lines.first()->line().p2().y() + dy)
                                          )
                                   );
            //перерисуем следующий кусочек тоже (если он есть)
            if (lines.count() > 1){
                lines.at(1)->setLine(QLineF(QPointF(lines.at(1)->line().p1().x(),
                                                    lines.at(1)->line().p1().y() + dy),
                                            lines.at(1)->line().p2()
                                            )
                                     );
            //если дуга соединяет вершины напрямую, то ограничим перемещение шириной конечной вершины
            } else {
                if ((lines.first()->line().p2().y() > myEndTop->sceneBoundingRect().bottomLeft().y()) ||
                    (lines.first()->line().p2().y() < myEndTop->sceneBoundingRect().topLeft().y())){
                    lines.first()->setLine(QLineF(QPointF(lines.first()->line().p1().x() - dx,
                                                          lines.first()->line().p1().y() - dy),
                                                  QPointF(lines.first()->line().p2().x(),
                                                          lines.first()->line().p2().y() - dy)
                                                  )
                                           );
                    return false;
                }
            }
            return true;
        }
    }
    //для тех у кого эта вершина - конец
    if (aMovedTop == myEndTop){
        //вертикальную линию двигаем только вправо/влево или удлинняем/укорачиваем
        if (lines.last()->line().p1().x() == lines.last()->line().p2().x()){
            if ((lines.first()->line().length() < MINDELTA) || (lines.last()->line().length() < MINDELTA)){
                return false;
            }

            lines.last()->setLine(QLineF(QPointF(lines.last()->line().p1().x() + dx,
                                                 lines.last()->line().p1().y()),
                                         QPointF(lines.last()->line().p2().x() + dx,
                                                 lines.last()->line().p2().y() + dy)
                                         )
                                  );
            //перерисуем следующий кусочек тоже (если он есть)
            if (lines.count() > 1){
                lines.at(lines.count()-2)->setLine(QLineF(lines.at(lines.count()-2)->line().p1(),
                                                          QPointF(lines.at(lines.count()-2)->line().p2().x() + dx,
                                                                  lines.at(lines.count()-2)->line().p2().y())
                                                          )
                                                   );
            //если дуга соединяет вершины напрямую, то ограничим перемещение шириной начальной вершины
            } else {
                if ((lines.last()->line().p2().x() < myStartTop->sceneBoundingRect().bottomLeft().x()) ||
                    (lines.last()->line().p2().x() > myStartTop->sceneBoundingRect().bottomRight().x())){
                    lines.last()->setLine(QLineF(QPointF(lines.last()->line().p1().x() - dx,
                                                         lines.last()->line().p1().y()),
                                                 QPointF(lines.last()->line().p2().x() - dx,
                                                         lines.last()->line().p2().y() - dy)
                                                 )
                                          );
                    return false;
                }
            }
            return true;
        }
        //горизонтальную линию можем двигать только вверх/вниз или удлиннять/укорачивать
        if (lines.last()->line().p1().y() == lines.last()->line().p2().y()){
            if ((lines.first()->line().length() < MINDELTA) || (lines.last()->line().length() < MINDELTA)){
                return false;
            }

            lines.last()->setLine(QLineF(QPointF(lines.last()->line().p1().x(),
                                                 lines.last()->line().p1().y() + dy),
                                         QPointF(lines.last()->line().p2().x() + dx,
                                                 lines.last()->line().p2().y() + dy)
                                         )
                                  );
            //перерисуем следующий кусочек тоже (если он есть)
            if (lines.count() > 1){
                lines.at(lines.count()-2)->setLine(QLineF(lines.at(lines.count()-2)->line().p1(),
                                                          QPointF(lines.at(lines.count()-2)->line().p2().x(),
                                                                  lines.at(lines.count()-2)->line().p2().y() + dy)
                                                          )
                                                   );
            //если дуга соединяет вершины напрямую, то ограничим перемещение шириной начальной вершины
            } else {
                if ((lines.last()->line().p2().y() > myStartTop->sceneBoundingRect().bottomLeft().y()) ||
                    (lines.last()->line().p2().y() < myStartTop->sceneBoundingRect().topLeft().y())){
                    lines.last()->setLine(QLineF(QPointF(lines.last()->line().p1().x(),
                                                         lines.last()->line().p1().y() - dy),
                                                 QPointF(lines.last()->line().p2().x() - dx,
                                                         lines.last()->line().p2().y() - dy)
                                                 )
                                          );
                    return false;
                }
            }
            return true;
        }
    }
    //если мы попали сюда, то что то не так!!!!
    //вернем-ка false на всякий случай
    return false;
}

void TArc::setPen(const QPen &pen){
    foreach (TArcLine* arcLine, lines)
        arcLine->setPen(pen);
    QGraphicsLineItem::setPen(pen);
}

/*!
  Конструктор
  @param startItem - указатель на начальную вершину. м.б. NULL
  @param ensItem - указатель на конечную вершину. м.б. NULL
  @param parent - родительский объект. (всегда NULL)
  @param scene - указатель на контейнер (сцену)
*/
TArc::TArc(TTop *startItem, TTop *endItem, QMenu *contextMenu,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myStartTop = startItem;
    myEndTop = endItem;
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    arcTop = new TArcTop(contextMenu, this, scene);
    width = 2;
    currentLine = NULL;
    arcTop->hide();
}

TArc::~TArc()
{
    foreach (QGraphicsLineItem *line, lines){
       delete line;
    }
}

/*!
  Возвращает прямоугольник включающий все отрезки дуги. необходимо для правильной отрисовки.
*/
QRectF TArc::boundingRect() const
{
    qreal extra = width*(pen().width() + 20) / 2.0;
    QRectF rect;
    foreach (TArcLine* line, lines){
        rect = rect.united(line->boundingRect());
    }
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

/*!
  Работает не правильно.
*/
QPainterPath TArc::shape() const
{
    QPainterPath path;
    path.addPolygon(arcHead);
    foreach (TArcLine *line, lines){
       path.addPath(line->shape());
    }
    path.addPath(arcTop->shape());

    return path;
}

void TArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (lines.count() == 0) return;

    if ((myStartTop == NULL) || (myEndTop == NULL)) {
        //СЂРµР¶РёРј СЃРѕР·РґР°РЅРёСЏ РґСѓРіРё!

    } else {
        //СЂРµР¶РёРј РѕС‚СЂРёСЃРѕРІРєРё Р·Р°РІРµСЂС€РµРЅРЅРѕР№ РґСѓРіРё

        //РѕС‚РѕР±СЂР°Р¶Р°РµРј СЃРµСЂС‹Р№ РєРІР°РґСЂР°С‚РёРє
        QPointF intersectPoint;
        myStartTop->getIntersectBound(lines.first()->line()).intersect(lines.first()->line(), &intersectPoint);
        float koeff = QLineF(lines.first()->line().p1(), intersectPoint).length() + 15 > lines.first()->line().length() ? lines.first()->line().length() : QLineF(lines.first()->line().p1(), intersectPoint).length() + 15; //ЫЫЫ =)
        arcTop->setPos(lines.first()->line().p1() + QPointF(cos(lines.first()->line().angle() * Pi / 180) * koeff, -sin(lines.first()->line().angle() * Pi / 180) * koeff));
        arcTop->show();
        //СЂРёСЃСѓРµРј СЃС‚СЂРµР»РєСѓ
        QPen myPen = pen();
        painter->setPen(myPen);
        painter->setBrush(myPen.color());
        myEndTop->getIntersectBound(lines.last()->line()).intersect(lines.last()->line(), &intersectPoint);
        float koeff2 = QLineF(lines.last()->line().p2(), intersectPoint).length(); //ЫЫЫ =)
        QPointF t = lines.last()->line().p2() - QPointF(cos(lines.last()->line().angle() * Pi / 180) * koeff2, -sin(lines.last()->line().angle() * Pi / 180) * koeff2);
        double angle = ((myEndTop->getIntersectBound(lines.last()->line()).normalVector().angle()) + 180) * Pi / 180;
        QPointF arcP1 = t + QPointF(sin(angle + Pi / 3) * 3*width,
                                              cos(angle + Pi / 3) * 3*width);
        QPointF arcP2 = t + QPointF(sin(angle + Pi - Pi / 3) * 3*width,
                                              cos(angle + Pi - Pi / 3) * 3*width);

        arcHead.clear();
        arcHead << t << arcP1 << arcP2;
        painter->drawPolygon(arcHead);
   }

   return;
}

/*!
  Добавляет отрезок дуги в список отрезков
  @param line - указатель на объект типа TArcLine
  @return true если добавилось
*/
bool TArc::addLine(TArcLine *line){
    if ((prevLine() != NULL) &&
        (prevLine()->line().p2() == line->line().p1()) &&
        (prevLine() != line) &&
        ((prevLine()->line().angleTo(line->line()) == 0) ||
        (prevLine()->line().angleTo(line->line()) == 180))) {
        line->setLine(QLineF(prevLine()->line().p1(), line->line().p2()));
        delete prevLine();
        lines.removeLast();
    }
    if (!lines.contains(line))
        lines.append(line);
    return lines.contains(line);
}

/*!
  Создает новый отрезок дуги.
  @param p1 - начальная точка
  @param p2 - конечная точка
  @return указатель на объект типа TArcLine
*/
TArcLine* TArc::newLine(QPointF p1, QPointF p2){
    if (currentLine == NULL)
        currentLine = new TArcLine(QLineF(p1, p2), this, scene());
    else {
        addLine(currentLine);
        currentLine = new TArcLine(QLineF(prevLine()->line().p2(), p2), this, scene());
    }
    currentLine->setPen(pen());
    currentLine->setFlag(QGraphicsItem::ItemIsMovable, true);
    currentLine->setFlag(QGraphicsItem::ItemIsSelectable, true);
    currentLine->setZValue(-1000);
    return currentLine;
}

/*!
  Обновляет границы. Необходимо вызываеть после изменения размеров, перемещения и т.д.
*/
void TArc::updateBounds(){
    if (lines.count() > 0)
        setLine(QLineF(lines.first()->line().p1(), lines.last()->line().p2()));
}

/*!
  Установка приоритета
  @param w - приоритет
*/
void TArc::setPriority(int w){
    width = w;
    QPen old_pen = pen();
    old_pen.setWidth(w);
    setPen(old_pen);
}

void TArcTop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

TArcTop::TArcTop(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myContextMenu = contextMenu;
    QPolygonF myPolygon;
    myPolygon << QPointF(-8, 8) << QPointF(8, 8)
              << QPointF(8,-8) << QPointF(-8, -8)
              << QPointF(-8, 8);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setPolygon(myPolygon);
}

QRectF TArcTop::boundingRect() const {
    return QGraphicsPolygonItem::boundingRect().adjusted(-2, -2, 2, 2);
}

QVariant TArcTop::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        TArc* arc = qgraphicsitem_cast<TArc* >(parentItem());
        QPen pen = arc->pen();
        if (isSelected())
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::black);
        arc->setPen(pen);
    }

    return value;
}

TArcLine::TArcLine(QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene){};

QPainterPath TArcLine::shape() const {
    QPainterPath path;
    path.addRect(QGraphicsLineItem::shape().boundingRect().adjusted(-5, -5, 5, 5));
    return path;
}

#include <QtGui>
#include "qarc.h"
#include <math.h>
#include <time.h>
#include "qparallelarctop.h"
#include "qserialarctop.h"
#include "qterminatearctop.h"
#include "commonutils.h"

const qreal Pi = M_PI;
const int MINDELTA = 40;

/*!
  Упрощенный алгоритм перестройки дуги
  @param top - указатель на перетаскиваемую вершину
  @param dx - перемещение по X
  @param dy - перемещение по Y
*/
bool QArc::autoBuild(QTop* top, float dx, float dy){

    //особый случай, когда начало и конец совпадают
    if (myStartTop == top && myEndTop == top){
        foreach (QArcLine* line, lines)
            line->setLine(line->line().translated(dx, dy));
        return true;
    }

    QPointF startPoint;
    QPointF endPoint;
    if (top == startItem()){
        startPoint = lines.first()->line().p1() + QPointF(dx, dy);
        endPoint = lines.last()->line().p2();
    } else {
        startPoint = lines.first()->line().p1();
        endPoint = lines.last()->line().p2() + QPointF(dx, dy);
    }

    qDeleteAll(lines);
    lines.clear();

    /*высчитываем главный вектор дуги*/
    QLineF centerLine(startPoint, endPoint);

    QPointF startIntersectPoint;
    QLineF polyLineStart;
    polyLineStart = startItem()->getIntersectBound(centerLine);
    QPointF endIntersectPoint;
    QLineF polyLineEnd;
    polyLineEnd = endItem()->getIntersectBound(centerLine);

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
  @param aMovedTop - указатель на перетаскиваемую вершину
  @param dx - перемещение по X
  @param dy - перемещение по Y
*/
bool QArc::remake(QTop* aMovedTop, float dx, float dy){
    bool result = true;

    int mx, my, j;
    QPointF pnts[4];
    bool flag; //false - если надо все напрочь переделать
    int lgdir, lgolddir;

    if (freezed) {
        foreach (QArcLine* line, lines)
            line->setLine(line->line().translated(dx/2, dy/2));
        return true;
    }

    if ((myStartTop == aMovedTop) && (myEndTop == aMovedTop)){
        foreach (QArcLine* line, lines)
            line->setLine(line->line().translated(dx, dy));
        return true;
    }

    if (lines.count() == 1 || lines.count() == 3){
        //старый алгоритм
        //переработано, дополнено, прокоментировано
        //и как ни странно - не работает
        //заполняем структуры, необходимые для старого алгоритма
        int otr = lines.count();
        int old_otr = otr;
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

        if (startBorder.intersect(lines.first()->line(), &startIntersectPoint) != QLineF::BoundedIntersection)
            return false;
        if (endBorder.intersect(lines.last()->line(), &endIntersectPoint) != QLineF::BoundedIntersection)
            return false;

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
                if ((!(pnts[1].x() - pnts[0].x()) && !dx) ||
                    (!(pnts[1].y() - pnts[0].y()) && !dy)){
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
            /*case 2:
            lgolddir = dvec2log(pnts[2].x() - pnts[0].x(), pnts[2].y() - pnts[0].y());
            lgdir    = dvec2log(pnts[2].x() + dx - pnts[0].x(), pnts[2].y() + dy - pnts[0].y());
            if (lgolddir == lgdir){
                pnts[1] += QPointF(dx*!(!(pnts[1].x() - pnts[0].x())), dy*!(!(pnts[1].y()-pnts[0].y())));
                pnts[2] += QPointF(dx, dy);
            }
            else flag = false;*/
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
        //if (old_otr != otr) {
            qDeleteAll(lines);
            lines.clear();
            currentLine = NULL;

            newLine(pts[0] + deltaStartPoint, pts[1]);
            for (j = 1; j < otr - 1; j++){
                newLine(pts[j], pts[j+1]);
            }
            newLine(pts[otr-1], pts[otr] + deltaEndPoint);
            addLine(currentLine);
            currentLine = NULL;
        /*} else {
            if (otr == 1)
                lines.first()->setLine(QLineF(pts[0] + deltaStartPoint, pts[1] + deltaEndPoint));
            else {
                lines.first()->setLine(QLineF(pts[0] + deltaStartPoint, pts[1]));
                for (j = 1; j < otr - 1; j++){
                    lines.at(j)->setLine(QLineF(pts[j], pts[j+1]));
                }
                lines.last()->setLine(QLineF(pts[otr-1], pts[otr] + deltaEndPoint));
            }
        }*/

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

/*!
  Конструктор
  @param startItem - указатель на начальную вершину. м.б. NULL
  @param ensItem - указатель на конечную вершину. м.б. NULL
  @param parent - родительский объект. (всегда NULL)
  @param scene - указатель на контейнер (сцену)
*/
QArc::QArc(QTop *startItem, QTop *endItem, QMenu *contextMenu,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene){
    //инициализация указателей
    arcTop          = NULL;
    currentLine     = NULL;
    predicate       = NULL;
    myStartTop      = startItem;
    myEndTop        = endItem;
    myContextMenu   = contextMenu;

    freezed = false;

    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

    setArcType(QArc::SerialArc);
    myPriority = 0;
    arcTop->hide();
}

QArc::~QArc(){
    if (myStartTop != NULL)
        myStartTop->removeArc(this);
    if (myEndTop != NULL)
        myEndTop->removeArc(this);

   qDeleteAll(lines);
}

/*!
  Возвращает прямоугольник включающий все отрезки дуги. необходимо для правильной отрисовки.
*/
QRectF QArc::boundingRect() const
{
    QRectF rect;
    foreach (QArcLine* line, lines){
        rect = rect.united(line->boundingRect());
    }
    return rect.normalized();
}

/*!
  Форма дуги
*/
QPainterPath QArc::shape() const
{
    QPainterPath path;
    path.addPolygon(arcHead);
    foreach (QArcLine *line, lines)
       path.addPath(line->shape());
    path.addPath(arcTop->shape());

    return path;
}

void QArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    if (lines.count() == 0) return;

    if ((myStartTop == NULL) || (myEndTop == NULL)) {
        //пока рисуем(создаем) дугу
    } else {
        QPointF intersectPoint(-1, -1);
        myStartTop->getIntersectBound(lines.first()->line()).intersect(lines.first()->line(), &intersectPoint);
        float koeff = QLineF(lines.first()->line().p1(), intersectPoint).length() + 15 > lines.first()->line().length() ? lines.first()->line().length() : QLineF(lines.first()->line().p1(), intersectPoint).length() + 15; //ЫЫЫ =)
        if (arcTop != NULL){
            arcTop->setPos(lines.first()->line().p1() + QPointF(cos(lines.first()->line().angle() * Pi / 180) * koeff, -sin(lines.first()->line().angle() * Pi / 180) * koeff));
            arcTop->show();
        }
        QList<QArc* > arcList = myStartTop->outArcs();
        QPen old_pen = pen();
        old_pen.setWidth(arcList.count() - myPriority + 2);
        setPen(old_pen);
        painter->setPen(old_pen);
        painter->setBrush(old_pen.color());
        myEndTop->getIntersectBound(lines.last()->line()).intersect(lines.last()->line(), &intersectPoint);
        float koeff2 = QLineF(lines.last()->line().p2(), intersectPoint).length() + 2; //ЫЫЫ =)
        QPointF t = lines.last()->line().p2() - QPointF(cos(lines.last()->line().angle() * Pi / 180) * koeff2, -sin(lines.last()->line().angle() * Pi / 180) * koeff2);
        double angle = ((myEndTop->getIntersectBound(lines.last()->line()).normalVector().angle()) + 180) * Pi / 180;
        int width = old_pen.width();
        QPointF arcP1 = t - QPointF(sin(angle + Pi / 3) * 1.8*width,
                                    cos(angle + Pi / 3) * 1.8*width);
        QPointF arcP2 = t - QPointF(sin(angle + Pi - Pi / 3) * 1.8*width,
                                    cos(angle + Pi - Pi / 3) * 1.8*width);
        arcHead.clear();
        arcHead << t << arcP1 << arcP2;
        painter->setRenderHints(QPainter::TextAntialiasing |
                                QPainter::Antialiasing |
                                QPainter::HighQualityAntialiasing);
        painter->drawPolygon(arcHead);
    }
}

/*!
  Добавляет отрезок дуги в список отрезков
  @param line - указатель на объект типа QArcLine
  @return true если добавилось
*/
bool QArc::addLine(QArcLine *line){
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
  @return указатель на объект типа QArcLine
*/
QArcLine* QArc::newLine(QPointF p1, QPointF p2){
    if (currentLine == NULL)
        currentLine = new QArcLine(QLineF(p1, p2), this, scene());
    else {
        addLine(currentLine);
        currentLine = new QArcLine(QLineF(prevLine()->line().p2(), p2), this, scene());
    }
    currentLine->setPen(pen());
    currentLine->setFlag(QGraphicsItem::ItemIsMovable, true);
    currentLine->setFlag(QGraphicsItem::ItemIsSelectable, true);
    currentLine->setZValue(0);
    return currentLine;
}

/*!
  Обновляет границы. Необходимо вызывать после изменения размеров, перемещения и т.д.
*/
void QArc::updateBounds(){
    if (lines.count() > 0)
        setLine(QLineF(lines.first()->line().p1(), lines.last()->line().p2()));
}

/*!
  Установка приоритета
  @param w - приоритет
*/
void QArc::setPriority(int w){
    myPriority = w;
}

void QArc::setPen(const QPen &pen){
    foreach (QArcLine* arcLine, lines)
        arcLine->setPen(pen);
    QGraphicsLineItem::setPen(pen);
}

void QArc::setArcType(ArcType type)
{
    myArcType = type;
    if (arcTop != NULL){
        delete arcTop;
        arcTop = NULL;
    }
    switch (myArcType) {
    case QArc::SerialArc:
        arcTop = new QSerialArcTop(myContextMenu, this, scene());
        break;
    case QArc::ParallelArc:
        arcTop = new QParallelArcTop(myContextMenu, this, scene());
        break;
    case QArc::TerminateArc:
        arcTop = new QTerminateArcTop(myContextMenu, this, scene());
        break;
    }
}

Arc* QArc::toArc()
{
    QStringList nodes;
    foreach(QArcLine* line, lines){
         nodes.append(QString::number(line->line().x1()) + " " + QString::number(line->line().y1()) + " " +
                      QString::number(line->line().x2()) + " " + QString::number(line->line().y2()));
    }
    return new Arc(Arc::ArcType(arcType()),
                   priority(),
                   myStartTop->number,
                   myEndTop->number,
                   predicate == NULL ? "" : predicate->name,
                   nodes);
}

Arc::Arc(ArcType type, int priority, int startTop, int endTop, QString predicate, QStringList &lines)
{
    this->type = type;
    this->priority = priority;
    this->startTop = startTop;
    this->endTop = endTop;
    this->predicate = predicate;
    this->lines = lines;
}

/*!
   Перемещает, если можно, часть дуги
   @param line перемещаемая линия
   @param dx перемещение по оси Ох
   @param dy перемещение по оси Oy
   @return true - если переместить удалось
*/
bool QArc::moveLine(QArcLine *line, float dx, float dy)
{
    if (!lines.contains(line)) return false;
    QArcLine *selectedLine = line;
    QArcLine *prevLine = NULL;
    QArcLine *nextLine = NULL;
    if (lines.count() == 1){
        prevLine = NULL;
        nextLine = NULL;
        if (!(myEndTop->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(line->line().p2() + QPointF(dx, dy)) &&
            myStartTop->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(line->line().p1() + QPointF(dx, dy))))
            return false;
    } else {
        if (selectedLine == lines.first()){
            nextLine = lines.at(1);
            prevLine = NULL;
            if (!myStartTop->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(line->line().p1() + QPointF(dx, dy)))
                return false;
        } else if (selectedLine == lines.last()){
            prevLine = lines.at(lines.indexOf(selectedLine) - 1);
            nextLine = NULL;
            if (!myEndTop->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(line->line().p2() + QPointF(dx, dy)))
                return false;
        } else {
            prevLine = lines.at(lines.indexOf(selectedLine) - 1);
            nextLine = lines.at(lines.indexOf(selectedLine) + 1);
        }
    }

    //вправо-влево можно двигать только вертикальные линии (пока)
    if (selectedLine->line().p1().x() == selectedLine->line().p2().x()){
        if (prevLine != NULL)
            prevLine->setLine(QLineF(prevLine->line().p1(),
                                     QPointF(prevLine->line().p2().x() + dx,
                                             prevLine->line().p1().y())
                                     )
                              );
        if (nextLine != NULL)
            nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x() + dx,
                                             nextLine->line().p1().y()
                                             ),
                                     nextLine->line().p2())
                              );
        selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x() + dx,
                                             selectedLine->line().p1().y()),
                                     QPointF(selectedLine->line().p2().x() + dx,
                                             selectedLine->line().p2().y())
                                     )
                              );
    }

    //вверх-вниз можно двигать только горизонтальные линии (пока)
    if (selectedLine->line().p1().y() == selectedLine->line().p2().y()){
        if (prevLine != NULL)
            prevLine->setLine(QLineF(prevLine->line().p1(),
                                     QPointF(prevLine->line().p2().x(),
                                             prevLine->line().p2().y() + dy)
                                     )
                              );
        if (nextLine != NULL)
            nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x(),
                                             nextLine->line().p1().y() + dy),
                                     nextLine->line().p2()
                                     )
                              );
        selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x(),
                                             selectedLine->line().p1().y() + dy),
                                     QPointF(selectedLine->line().p2().x(),
                                             selectedLine->line().p2().y() + dy)
                                     )
                              );
    }
    updateBounds();
    return true;
}

void QArc::freeze()
{
    freezed = true;
}

void QArc::unfreeze()
{
    freezed = false;
}

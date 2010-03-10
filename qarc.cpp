#include <QtGui>
#include "qarc.h"
#include <math.h>
#include <time.h>

const qreal Pi = 3.14;
const int MINDELTA = 40;

/**********************************************************************
*			 Misc functions 			      *
**********************************************************************/
//from arc.cpp
int dvec2log(float dx, float dy)
{
    int res = 0;

    if (dx > 0) res |= RIGHT;
    else if (dx < 0) res |= LEFT;
    if (dy > 0) res |= DOWN;
    else if (dy < 0) res |= UP;
    return res;
}

bool TArc::autoBuild(){

    foreach(TArcLine* line, lines){
        delete line;
        line = NULL;
    }
    lines.clear();

/*  double x = double(rand())/RAND_MAX;// rand();

    QPointF startP = QPointF(x*(myStartTop->sceneBoundingRect().topRight().x() - myStartTop->sceneBoundingRect().topLeft().x()) + myStartTop->sceneBoundingRect().topLeft().x(),
                             x*(myStartTop->sceneBoundingRect().bottomRight().y() - myStartTop->sceneBoundingRect().topRight().y()) + myStartTop->sceneBoundingRect().topRight().y());

    QPointF endP = QPointF(x*(myEndTop->sceneBoundingRect().topRight().x() - myEndTop->sceneBoundingRect().topLeft().x()) + myEndTop->sceneBoundingRect().topLeft().x(),
                           x*(myEndTop->sceneBoundingRect().bottomRight().y() - myEndTop->sceneBoundingRect().topRight().y()) + myEndTop->sceneBoundingRect().topRight().y());

    QLineF startLine = QLineF(startP, QPointF(startP.x(), endP.y()));
    QLineF endLine = QLineF(QPointF(startP.x(), endP.y()), endP);
*/
    QLineF endLine = QLineF(QPointF(myStartTop->scenePos().x(), myEndTop->scenePos().y()), myEndTop->scenePos());
    QLineF startLine = QLineF(myStartTop->scenePos(), QPointF(myStartTop->scenePos().x(), myEndTop->scenePos().y()));

    newLine(startLine.p1(), startLine.p2());
    newLine(endLine.p1(), endLine.p2());
    addLine(currentLine);

    currentLine = NULL;
}

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
        for(int i = 0; i < otr; i++){
            pts[i] = lines.at(i)->line().p1();
            pts[i+1] = lines.at(i)->line().p2();
        }

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
        for (j = 0; j < otr; j++){
            newLine(pts[j], pts[j+1]);
        }
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

/********************************************************************/

TArc::TArc(TTop *startItem, TTop *endItem, QMenu *contextMenu,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myStartTop = startItem;
    myEndTop = endItem;
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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

QRectF TArc::boundingRect() const
{
    qreal extra = width*(pen().width() + 20) / 2.0;

    QRectF rect;

    foreach (TArcLine* line, lines){
        rect = rect.united(line->boundingRect());
    }

    return rect.normalized().adjusted(-extra, -extra, extra, extra);
    /*
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);*/

}

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

void TArc::updatePosition()
{
    QLineF line(mapFromItem(myStartTop, 0, 0), mapFromItem(myEndTop, 0, 0));
    setLine(line);
}

void TArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (lines.count() == 0) return;

    if ((myStartTop == NULL) || (myEndTop == NULL)) {
        //СЂРµР¶РёРј СЃРѕР·РґР°РЅРёСЏ РґСѓРіРё!

    } else {
        //СЂРµР¶РёРј РѕС‚СЂРёСЃРѕРІРєРё Р·Р°РІРµСЂС€РµРЅРЅРѕР№ РґСѓРіРё
        /*foreach (TArcLine *line, lines){
            line->setPen(QPen(QBrush(Qt::black,Qt::SolidPattern), width, Qt::SolidLine));
            //line->update();
        }*/
        //РѕС‚РѕР±СЂР°Р¶Р°РµРј СЃРµСЂС‹Р№ РєРІР°РґСЂР°С‚РёРє
        QPointF intersectPoint;
        myStartTop->getIntersectBound(lines.first()->line()).intersect(lines.first()->line(), &intersectPoint);
        float koeff = QLineF(lines.first()->line().p1(), intersectPoint).length() + 15 > lines.first()->line().length() ? lines.first()->line().length() : QLineF(lines.first()->line().p1(), intersectPoint).length() + 15; //ЫЫЫ =)
        arcTop->setPos(lines.first()->line().p1() + QPointF(cos(lines.first()->line().angle() * Pi / 180) * koeff, -sin(lines.first()->line().angle() * Pi / 180) * koeff));
        arcTop->show();
        //СЂРёСЃСѓРµРј СЃС‚СЂРµР»РєСѓ
        QPen myPen = pen();
        myPen.setColor(myColor);
        painter->setPen(myPen);
        painter->setBrush(myColor);
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

    return; //РґР°Р»СЊС€Рµ РЅРёС‡Рµ РЅРµ РґРµР»Р°РµРј!!!!!

    if (myStartTop->collidesWithItem(myEndTop))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arcSize = 8;
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QPointF p1;
    QPointF p2;
    QPointF startP;
    QPointF endP;
    QLineF polyLineStart;
    QLineF polyLineEnd;
    QPointF intersectPoint;
    QList<TArc *> arcsStart;
    QList<TArc *> arcsEnd;
    double delta;
    bool canDraw = true;

    /*РІС‹СЃС‡РёС‚С‹РІР°РµРј РіР»Р°РІРЅС‹Р№ РІРµРєС‚РѕСЂ РґСѓРіРё*/
    QLineF centerLine(myStartTop->pos(), myEndTop->pos());
    setLine(centerLine);
    /*РґРѕСЃС‚Р°С‚РѕС‡РЅРѕ Р»Рё РјРµСЃС‚Р° РґР»СЏ РѕС‚СЂРёСЃРѕРІРєРё???*/
    polyLineStart = myStartTop->getIntersectBound(line());
    polyLineEnd = myEndTop->getIntersectBound(line());
    arcsStart = myStartTop->getArcsAtBound(polyLineStart);
    arcsEnd = myEndTop->getArcsAtBound(polyLineEnd);
    polyLineStart.normalVector().intersect(polyLineEnd, &intersectPoint);
    double deltaS = QLineF(polyLineStart.normalVector().p1(), intersectPoint).length();
    if (deltaS < 80) {
        canDraw = false;
        arcTop->hide();
        painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter->setPen(QPen(Qt::red, 3, Qt::DashDotLine));
        painter->drawLine(line());
        return;
    }

    /*РІС‹СЃС‡РёС‚С‹РІР°РµРј РЅР°С‡Р°Р»СЊРЅСѓСЋ С‚РѕС‡РєСѓ*/
    //polyLineStart = myStartTop->getIntersectBound(line());
    //arcs = myStartTop->getArcsAtBound(polyLineStart);
    polyLineStart.intersect(line(), &intersectPoint);
    p2 = polyLineStart.p2();
    p1 = polyLineStart.p1();

    double dist = polyLineStart.length()/(arcsStart.count() + 1);
    //left side
    if ((p2.x() == p1.x()) &&
         intersectPoint.x() < startItem()->x()) {
            startP.setX(p1.x());
            startP.setY(p1.y() + (arcsStart.indexOf(this) + 1) * dist);
            arcTop->setPos(startP.x() - 20, startP.y());
        }
    //right side
    if ((p2.x() == p1.x()) &&
        intersectPoint.x() > startItem()->x()) {
            startP.setX(p2.x());
            startP.setY(p2.y() + (arcsStart.indexOf(this) + 1) * dist);
            arcTop->setPos(startP.x() + 20, startP.y());
        }
    //top
    if ((p2.y() == p1.y()) &&
         intersectPoint.y() < startItem()->y()) {
            startP.setX(p2.x() + (arcsStart.indexOf(this) + 1) * dist);
            startP.setY(p2.y());
            arcTop->setPos(startP.x(), startP.y() - 20);
        }
    //bottom
    if ((p2.y() == p1.y()) &&
         intersectPoint.y() > startItem()->y()) {
            startP.setX(p1.x() + (arcsStart.indexOf(this) + 1) * dist);
            startP.setY(p1.y());
            arcTop->setPos(startP.x(), startP.y() + 20);
         }

    /*РІС‹СЃС‡РёС‚С‹РІР°РµРј РєРѕРЅРµС‡РЅСѓСЋ С‚РѕС‡РєСѓ*/
    //polyLineEnd = myEndTop->getIntersectBound(line());
    //arcs = myEndTop->getArcsAtBound(polyLineEnd);
    polyLineEnd.intersect(line(), &intersectPoint);
    p2 = polyLineEnd.p2();
    p1 = polyLineEnd.p1();

    dist = polyLineEnd.length()/(arcsEnd.count() + 1);
    //left side
    if ((p2.x() == p1.x()) &&
         intersectPoint.x() < endItem()->x()) {
            endP.setX(p1.x());
            endP.setY(p1.y() + (arcsEnd.indexOf(this) + 1) * dist);
        }
    //right side
    if ((p2.x() == p1.x()) &&
        intersectPoint.x() > endItem()->x()) {
            endP.setX(p2.x());
            endP.setY(p2.y() + (arcsEnd.indexOf(this) + 1) * dist);
        }
    //top
    if ((p2.y() == p1.y()) &&
         intersectPoint.y() < endItem()->y()) {
            endP.setX(p2.x() + (arcsEnd.indexOf(this) + 1) * dist);
            endP.setY(p2.y());
        }
    //bottom
    if ((p2.y() == p1.y()) &&
         intersectPoint.y() > endItem()->y()) {
            endP.setX(p1.x() + (arcsEnd.indexOf(this) + 1) * dist);
            endP.setY(p1.y());
         }

    //line() С„Р°РєС‚РёС‡РµСЃРєРё СЃРѕРґРµСЂР¶РёС‚ РіР»Р°РІРЅС‹Р№ РІРµРєС‚РѕСЂ РґСѓРіРё
    setLine(QLineF(startP, endP));
    //painter->drawLine(line());

    /*РІС‹С‡РёСЃР»СЏРµРј С„РѕСЂРјСѓ, РјРµСЃС‚Рѕ Рё РЅР°РїСЂР°РІР»РµРЅРёРµ РёР·РіРёР±Р°*/
    /*РѕС‚СЂРёСЃРѕРІС‹РІР°РµРј СЂРµР·СѓР»СЊС‚Р°С‚ РЅР° С…РѕР»СЃС‚Рµ*/
    /*СЂРёСЃСѓРµРј РєРІР°РґСЂР°С‚РёРє*/
    delta = polyLineStart.length()/2 - (arcsEnd.indexOf(this) + 1) * dist;
    double deltaY = endP.y() - startP.y();
    double deltaX = endP.x() - startP.x();
    QPointF P1, P2;
    //vertical border start
    if ((polyLineStart.angle() == 90) ||
        (polyLineStart.angle() == 270)) {

        //parallel border end
        if ((polyLineEnd.angleTo(polyLineStart) == 0) ||
            (polyLineEnd.angleTo(polyLineStart) == 180)) {
            P1.setX(startP.x() + deltaX/2 - delta);
            P1.setY(startP.y());
            P2.setX(P1.x());
            P2.setY(endP.y());
            painter->drawLine(QLineF(startP, P1));
            painter->drawLine(QLineF(P1, P2));
            painter->drawLine(QLineF(P2, endP));
        }
        //perpendicular border end
        if ((polyLineEnd.angleTo(polyLineStart) == 90) ||
            (polyLineEnd.angleTo(polyLineStart) == 270)) {
            P1.setX(endP.x());
            P1.setY(startP.y());
            painter->drawLine(QLineF(startP, P1));
            painter->drawLine(QLineF(P1, endP));
        }
    }
    //horizontal border start
    if ((polyLineStart.angle() == 0) ||
        (polyLineStart.angle() == 180)) {
        //perpendicular border end
        if ((polyLineEnd.angleTo(polyLineStart) == 90) ||
            (polyLineEnd.angleTo(polyLineStart) == 270)) {
            P1.setX(startP.x());
            P1.setY(endP.y());
            painter->drawLine(QLineF(startP, P1));
            painter->drawLine(QLineF(P1, endP));
        }
        //parallel border end
        if ((polyLineEnd.angleTo(polyLineStart) == 0) ||
            (polyLineEnd.angleTo(polyLineStart) == 180)) {
            P1.setX(startP.x());
            P1.setY(startP.y() + deltaY/2 - delta);
            P2.setX(endP.x());
            P2.setY(P1.y());
            painter->drawLine(QLineF(startP, P1));
            painter->drawLine(QLineF(P1, P2));
            painter->drawLine(QLineF(P2, endP));
        }
    }

    /*СЂРёСЃСѓРµРј СЃС‚СЂРµР»РєСѓ*/
    double angle = ((polyLineEnd.normalVector().angle()) + 180) * Pi / 180;
    QPointF arcP1 = line().p2() + QPointF(sin(angle + Pi / 3) * arcSize,
                                          cos(angle + Pi / 3) * arcSize);
    QPointF arcP2 = line().p2() + QPointF(sin(angle + Pi - Pi / 3) * arcSize,
                                          cos(angle + Pi - Pi / 3) * arcSize);

    arcHead.clear();
    arcHead << line().p2() << arcP1 << arcP2;
    painter->drawPolygon(arcHead);
    arcTop->show();
    //painter->drawLine(line());

    /*if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }*/
}

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

TArcLine* TArc::newLine(QPointF p1, QPointF p2){
    if (currentLine == NULL)
        currentLine = new TArcLine(QLineF(p1, p2), this, scene());
    else {
        addLine(currentLine);
        currentLine = new TArcLine(QLineF(prevLine()->line().p2(), p2), this, scene());
    }
    currentLine->setPen(QPen(QBrush(Qt::black,Qt::SolidPattern),2,Qt::SolidLine));
    currentLine->setFlag(QGraphicsItem::ItemIsMovable, true);
    currentLine->setFlag(QGraphicsItem::ItemIsSelectable, true);
    currentLine->setZValue(-1000);
    return currentLine;
}

void TArc::realloc(){
    QPointF startP;
    QPointF endP;
    QLineF startTopBorder;
    QLineF endTopBorder;

    if ((myStartTop != NULL) && (myEndTop != NULL)) {
        startTopBorder = myStartTop->getIntersectBound(lines.first()->line());
        endTopBorder   = myEndTop->getIntersectBound(lines.last()->line());
        startTopBorder.intersect(lines.first()->line(), &startP);
        lines.first()->setLine(QLineF(startP, lines.first()->line().p2()));
        endTopBorder.intersect(lines.last()->line(), &endP);
        lines.last()->setLine(QLineF(lines.last()->line().p1(), endP));
    }
}

void TArc::updateBounds(){
    if (lines.count() > 0)
        setLine(QLineF(lines.first()->line().p1(), lines.last()->line().p2()));
}

void TArc::setPriority(int w){
    width = w;
    QPen old_pen;
    foreach(TArcLine *line, lines){
        old_pen = line->pen();
        old_pen.setWidth(w);
        line->setPen(old_pen);
    }
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

void TArcTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);

    /*TArc* arc;
    arc = qgraphicsitem_cast<TArc *>(parentItem());

    if (isSelected()) {
        foreach(TArcLine *arcLine, arc->lines)
        {
            painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
            QLineF myLine = arcLine->line();
            myLine.translate(4.0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(-8.0,-8.0);
            painter->drawLine(myLine);
        }
    }*/
}

void TArcTop::mousePressEvent(QGraphicsSceneMouseEvent *event){
    /*TArc* arc;
    arc = qgraphicsitem_cast<TArc *>(parentItem());
    if (event->buttons() == Qt::LeftButton){
        QGraphicsPolygonItem::mousePressEvent(event);
        event->ignore();
    }
    else*/
    QGraphicsPolygonItem::mousePressEvent(event);
}

void TArcTop::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    /*TArc* arc;
    arc = qgraphicsitem_cast<TArc *>(parentItem());
    if (event->buttons() == Qt::LeftButton)
        arc->lines.first()->mouseMoveEvent(event);
    event->ignore();*/
    QGraphicsPolygonItem::mouseMoveEvent(event);
}
void TArcTop::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    /*TArc* arc;
    arc = qgraphicsitem_cast<TArc *>(parentItem());
    if (event->buttons() == Qt::LeftButton)
        arc->lines.first()->mouseReleaseEvent(event);
    if (event->buttons() == Qt::LeftButton){
        QGraphicsPolygonItem::mousePressEvent(event);
        event->ignore();}
    else*/
    QGraphicsPolygonItem::mousePressEvent(event);
}

TArcLine::TArcLine(QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene)
{
}

QPainterPath TArcLine::shape() const {
    QPainterPath path;
    path.addRect(QGraphicsLineItem::shape().boundingRect().adjusted(-5, -5, 5, 5));
    return path;
}

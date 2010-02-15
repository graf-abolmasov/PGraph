#include <QtGui>
#include "qarc.h"
#include <math.h>

const qreal Pi = 3.14;
const int MINDELTA = 40;

/**********************************************************************
*			 Misc functions 			      *
**********************************************************************/
/*карантин************************************************************/
//from arc.cpp
int dvec2log(int dx,int dy)
{
    int res=0;

    if (dx>0) res |= RIGHT;
    else if (dx<0) res |= LEFT;
    if (dy>0) res |= DOWN;
    else if (dy<0) res |= UP;
    return res;
}

inline int Sign(float d)
{
    return (!d) ? 0 : (d>0) ? 1 : -1;
}

inline int min(int x, int y) { return (x<y) ? x : y; }
inline int max(int x, int y) { return (x>y) ? x : y; }

void TArc::rebuild(TTop* aMovedTop, int deltaX, int deltaY)
{
    /*int i;
    QPointF ptBeg, ptEnd;
    TArc* anArc;

    ptBeg.setX(pts[0].x() - Sign(pts[1].x() - pts[0].x()) * min(Trans.Prior<<3, (int)myStartTop->boundingRect().width()>>1));
    ptBeg.setY(pts[0].y() - Sign(pts[1].y() - pts[0].y()) * min(Trans.Prior<<3, (int)myStartTop->boundingRect().height()>>1));
    ptEnd.setX(pts[otr].x() - Sign(pts[otr-1].x() - pts[otr].x()) * min(Trans.Prior<<3, (int)myEndTop->boundingRect().width()>>1));
    ptEnd.setY(pts[otr].y() - Sign(pts[otr-1].y() - pts[otr].y()) * min(Trans.Prior<<3, (int)myEndTop->boundingRect().height()>>1));

    if (aMovedTop==myStartTop)
    {
        ptBeg+=QPointF(deltaX, deltaY);
    }
    else
    {
        ptEnd+=QPointF(deltaX, deltaY);
    }

    DrawWin->BeginArc(aTopFrom,ptBeg);
    while (!DrawWin->BuildArc(ptEnd,anArc));
    DrawWin->EndArc();
    otr=anArc->otr;
    for (i=0; i<=otr; i++) pts[i]=anArc->pts[i];
    delete anArc;*/
}

void TArc::remake(TTop* aMovedTop, int deltaX, int deltaY)
{
/*    int mx,my;
    QPointF pnts[4];
    int j;
    bool flag; //высший пилотаж назвать переменную flag!!!!!!!!!!!!!!!!!!!!
    int lgdir,lgolddir;

    if (myStartTop==aMovedTop && myEndTop==aMovedTop) {
        for (j =0; j<=otr; j++)
            pts[j] += QPointF(deltaX, deltaY);
    } else {
        bool fMovedTo = (aMovedTop==myEndTop);
        if (fMovedTo)
            for (j=0; j<=otr; j++)
                pnts[j] = pts[j];
        else
            for (j=0; j<=otr; j++)
                pnts[otr-j] = pts[j];
        flag = TRUE;
        switch (otr) {
            case 1:
                lgolddir = dvec2log(pnts[1].x() - pnts[0].x(), pnts[1].y() - pnts[0].y());
                lgdir    = dvec2log(pnts[1].x() + deltaX - pnts[0].x(), pnts[1].y() + deltaY - pnts[0].y());
                if (lgolddir & lgdir) {
                    if (!(pnts[1].x() - pnts[0].x()) && !deltaX ||
                        !(pnts[1].y() - pnts[0].y()) && !deltaY) {
                        pnts[1] += QPointF(deltaX, deltaY);
                    } else {
                        pnts[3].setX(pnts[1].x() + deltaX);
                        pnts[3].setY(pnts[1].y() + deltaY);
                        pnts[1].setX(pnts[0].x() + !(!(pnts[1].x() - pnts[0].x())) * (pnts[3].x() - pnts[0].x())*(4 - fMovedTo)/7);
                        pnts[1].setY(pnts[0].y() + !(!(pnts[1].y() - pnts[0].y())) * (pnts[3].y() - pnts[0].y())*(4 - fMovedTo)/7);
                        if ((lgdir^lgolddir) & (LEFT | RIGHT))
                        { mx=1; my=0; }
                        else { mx=0; my=1; }
                        pnts[2].x=pnts[1].x+deltaX*mx; pnts[2].y=pnts[1].y+deltaY*my;
                        otr=3;
                    }
                }
                else flag=FALSE;
                break;
            case 2:
                lgolddir=dvec2log(pnts[2].x-pnts[0].x,pnts[2].y-pnts[0].y);
                lgdir=dvec2log(pnts[2].x+deltaX-pnts[0].x,
                               pnts[2].y+deltaY-pnts[0].y);
                if (lgolddir==lgdir)
                {
                    pnts[1].x+=deltaX*!(!(pnts[1].x-pnts[0].x));
                    pnts[1].y+=deltaY*!(!(pnts[1].y-pnts[0].y));
                    pnts[2].x+=deltaX; pnts[2].y+=deltaY;
                }
                else flag=FALSE;
                break;
            case 3:
                lgolddir=dvec2log(pnts[3].x-pnts[2].x,pnts[3].y-pnts[2].y);
                lgdir=dvec2log(pnts[3].x+deltaX-pnts[1].x,
                               pnts[3].y+deltaY-pnts[1].y);
                if (lgolddir & lgdir)
                {
                    pnts[2].x+=deltaX*!(!(pnts[2].x-pnts[1].x));
                    pnts[2].y+=deltaY*!(!(pnts[2].y-pnts[1].y));
                    pnts[3].x+=deltaX; pnts[3].y+=deltaY;
                    if (pnts[1].x==pnts[2].x && pnts[1].y==pnts[2].y)
                    {
                        pnts[1]=pnts[3]; otr=1;
                    }
                }
                else flag=FALSE;
                break;
            }
        if (flag)
            if (fMovedTo)
                for (j=0; j<=otr; j++) pts[j]=pnts[j];
        else for (j=0; j<=otr; j++) pts[j]=pnts[otr-j];
        else Rebuild(aMovedTop,deltaX,deltaY);
    }
    DrawWin->Invalidate();*/
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
   /* QRectF rec(myStartTop->pos(), myEndTop->pos());
    rec = rec.normalized();
    foreach (TArcLine *line, lines){
    rec = rec.united(line->boundingRect()).normalized();
    }
    return rec;*/
    return scene()->sceneRect();
}

   /* qreal extra = (pen().width() + 20) / 2.0;

    if ((myStartTop != NULL) && (myEndTop != NULL))
        return QRectF(myStartTop->pos(), myEndTop->pos())
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}*/

QPainterPath TArc::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arcHead);
    foreach (TArcLine *line, lines){
       path.addRect(line->boundingRect());
    }
    path.addRect(arcTop->boundingRect());
    //path.addRect(scene()->sceneRect());
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
        //режим создания дуги!

    } else {
        //режим отрисовки завершенной дуги
        foreach (QGraphicsLineItem *line, lines){
            line->setPen(QPen(QBrush(Qt::black,Qt::SolidPattern), width, Qt::SolidLine));
            line->update();
        }
        //отображаем серый квадратик
        arcTop->setPos(lines.first()->line().p1() + QPointF(cos(lines.first()->line().angle() * Pi / 180) * 20, -sin(lines.first()->line().angle() * Pi / 180) * 20));
        arcTop->show();
        //рисуем стрелку
        QPen myPen = pen();
        myPen.setColor(myColor);
        painter->setPen(myPen);
        painter->setBrush(myColor);
        double angle = ((myEndTop->getIntersectBound(lines.last()->line()).normalVector().angle()) + 180) * Pi / 180;
        QPointF arcP1 = lines.last()->line().p2() + QPointF(sin(angle + Pi / 3) * 6*width,
                                              cos(angle + Pi / 3) * 4*width);
        QPointF arcP2 = lines.last()->line().p2() + QPointF(sin(angle + Pi - Pi / 3) * 6*width,
                                              cos(angle + Pi - Pi / 3) * 4*width);

        arcHead.clear();
        arcHead << lines.last()->line().p2() << arcP1 << arcP2;
        painter->drawPolygon(arcHead);
   }

    return; //дальше ниче не делаем!!!!!

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

    /*высчитываем главный вектор дуги*/
    QLineF centerLine(myStartTop->pos(), myEndTop->pos());
    setLine(centerLine);
    /*достаточно ли места для отрисовки???*/
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

    /*высчитываем начальную точку*/
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

    /*высчитываем конечную точку*/
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

    //line() фактически содержит главный вектор дуги
    setLine(QLineF(startP, endP));
    //painter->drawLine(line());

    /*вычисляем форму, место и направление изгиба*/
    /*отрисовываем результат на холсте*/
    /*рисуем квадратик*/
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

    /*рисуем стрелку*/
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
        currentLine = new TArcLine(this, QLineF(p1, p2), this, scene());
    else {
        addLine(currentLine);
        currentLine = new TArcLine(this, QLineF(prevLine()->line().p2(), p2), this, scene());
    }
    currentLine->setPen(QPen(QBrush(Qt::black,Qt::SolidPattern),2,Qt::SolidLine));
    currentLine->setFlag(QGraphicsItem::ItemIsMovable, true);
    currentLine->setFlag(QGraphicsItem::ItemIsSelectable, true);
    currentLine->setZValue(-1000);
    return currentLine;
}

bool TArc::realloc(){
    QPointF startP;
    QPointF endP;
    QLineF startTopBorder;
    QLineF endTopBorder;
    QPointF intersectPoint;

    //подогнать начальную и конечную точки дуги к краю вершины
    if ((myStartTop != NULL) && (myEndTop != NULL)) {
        startTopBorder = myStartTop->getIntersectBound(lines.first()->line());
        endTopBorder   = myEndTop->getIntersectBound(lines.last()->line());
        startTopBorder.intersect(lines.first()->line(), &intersectPoint);
        lines.first()->setLine(QLineF(intersectPoint, lines.first()->line().p2()));
        endTopBorder.intersect(lines.last()->line(), &intersectPoint);
        lines.last()->setLine(QLineF(lines.last()->line().p1(), intersectPoint));
    }
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

TArcLine::TArcLine(TArc *owner, QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene)
{
    myOwner = owner;
}

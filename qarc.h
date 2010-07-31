#ifndef QARC_H
#define QARC_H

#include <QtGui>
#include <QtCore>
#include "qdiagramitem.h"
#include "qarcline.h"
#include "qserialarctop.h"

#define ARC_TYPE      UserType+4
#define UP	0x80
#define LEFT	0x20
#define DOWN	0x08
#define RIGHT	0x02

class TTop;
class QArcLine;
class QSerialArcTop;

class TArc : public QGraphicsLineItem
{
    friend class QDiagramScene;
public:
    enum { Type = ARC_TYPE };
    enum ArcType { SerialArc, ParallelArc, TerminateArc };

    QList<QArcLine *> lines;
    QArcLine* currentLine;
    TArc(TTop *startItem, TTop *endItem, QMenu *contextMenu,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~TArc();
    bool addLine(QArcLine *line);
    QArcLine* newLine(QPointF p1, QPointF p2);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setPriority(int w);
    void updateBounds();
    int type() const
        { return Type; }
    int priority()
        { return width; }
    void setStartTop(TTop* startTop)
        { myStartTop = startTop; }
    void setEndTop(TTop* endTop)
        { myEndTop = endTop; }
    TTop *startItem() const
        { return myStartTop; }
    TTop *endItem() const
        { return myEndTop; }
    ArcType arcType() const
        { return myArcType; }
    void setArcType(ArcType type);
    QArcLine* prevLine(){
        if (lines.count() > 0)
            return lines.last();
         else return NULL;
    }
    void setPen(const QPen &pen);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    TTop *myStartTop;
    TTop *myEndTop;
    ArcType myArcType;
    QGraphicsItem *arcTop;
    QPolygonF arcHead;
    QMenu *myContextMenu;
    int width;  //приоритет
    bool autoBuild(TTop* top, float dx, float dy);
    bool remake(TTop *, float dx, float dy);
};

#endif

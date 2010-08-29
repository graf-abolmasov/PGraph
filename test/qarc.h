#ifndef QARC_H
#define QARC_H

#include <QtGui>
#include <QtCore>
#include "qtop.h"
#include "qarcline.h"
#include "qserialarctop.h"
#include "predicate.h"

#define ARC_TYPE      UserType+4
#define UP	0x80
#define LEFT	0x20
#define DOWN	0x08
#define RIGHT	0x02

class QTop;
class QArcLine;
class QSerialArcTop;

class Arc
{
public:
    enum ArcType { SerialArc, ParallelArc, TerminateArc };
    Arc(ArcType type, int priority, int startTop, int endTop, QString predicate, QStringList &lines);
    ArcType type;
    int priority;
    int startTop;
    int endTop;
    QStringList lines;
    QString predicate;
};

class QArc : public QGraphicsLineItem
{
public:
    enum { Type = ARC_TYPE };
    enum ArcType { SerialArc, ParallelArc, TerminateArc };

    QList<QArcLine *> lines;
    QArcLine* currentLine;
    QArc(QTop *startItem, QTop *endItem, QMenu *contextMenu,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~QArc();
    bool addLine(QArcLine *line);
    QArcLine* newLine(QPointF p1, QPointF p2);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setPriority(int w);
    void updateBounds();
    int type() const
        { return Type; }
    int priority() const
        { return myPriority; }
    void setStartTop(QTop* startTop)
        { myStartTop = startTop; }
    void setEndTop(QTop* endTop)
        { myEndTop = endTop; }
    QTop *startItem() const
        { return myStartTop; }
    QTop *endItem() const
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
    Predicate* predicate;
    Arc* toArc();
    bool autoBuild(QTop* top, float dx, float dy);
    bool remake(QTop *, float dx, float dy);
    bool moveLine(QArcLine* line, float dx, float dy);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    ArcType myArcType;
    QTop *myStartTop;
    QTop *myEndTop;
    QMenu *myContextMenu;
    QPolygonF arcHead;
    QGraphicsItem *arcTop;
    int myPriority;  //приоритет
};

#endif

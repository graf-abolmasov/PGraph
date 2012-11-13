#ifndef QARC_H
#define QARC_H

#include <QtGui/QGraphicsLineItem>
#include "../../src/common/VO/graphvo.h"

#define ARC_TYPE      UserType+4

class QTop;
class QArcLine;
class Predicate;
class QSerialArcTop;

class QArc : public QGraphicsLineItem
{
public:
    enum { Type = ARC_TYPE };

    QList<QArcLine *> lines;
    QGraphicsItem *arcTop;
    QArcLine *currentLine;
    const Predicate *predicate;
    QArc(QTop *startItem, QTop *endItem, QMenu *contextMenu,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~QArc();
    bool addLine(QArcLine *line);
    QArcLine *newLine(QPointF p1, QPointF p2);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setPriority(int w);
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
    Arc::ArcType arcType() const
        { return myArcType; }
    void setArcType(Arc::ArcType type);
    QArcLine *prevLine(){
        if (lines.count() > 0)
            return lines.last();
         else return NULL;
    }
    void setPen(const QPen &pen);

    Arc toArc() const;
    bool autoBuild(QTop* top, float dx, float dy);
    bool remake(QTop *, float dx, float dy);
    bool moveLine(QArcLine* line, float dx, float dy);
    void freeze();
    void unfreeze();
    void updateBounds();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    bool freezed;
    Arc::ArcType myArcType;
    QTop *myStartTop;
    QTop *myEndTop;
    QMenu *myContextMenu;
    QPolygonF arcHead;
    int myPriority;
};

#endif

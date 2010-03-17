#ifndef QARC_H
#define QARC_H

#include "qdiagramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

#define ARC_TOP_TYPE  UserType+5
#define ARC_TYPE      UserType+4
#define ARC_LINE_TYPE UserType+6
#define UP	0x80
#define LEFT	0x20
#define DOWN	0x08
#define RIGHT	0x02

class TArcTop : public QGraphicsPolygonItem
{
public:
    enum { Type = ARC_TOP_TYPE };
    TArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type;}
    QRectF boundingRect() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    QMenu *myContextMenu;
};

class TArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };
    TArcLine(QLineF line, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QPainterPath shape() const;
    int type() const
        { return Type;}
    TArc* owner() const
        { return qgraphicsitem_cast<TArc *>(parentItem()); }
};

class TArc : public QGraphicsLineItem
{
    friend class QDiagramScene;
public:
    enum { Type = ARC_TYPE };
    QList<TArcLine *> lines;
    TArcLine* currentLine;
    TArc(TTop *startItem, TTop *endItem, QMenu *contextMenu,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~TArc();
    bool addLine(TArcLine *line);
    TArcLine* newLine(QPointF p1, QPointF p2);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setPriority(int w);
    void updateBounds();
    int type() const
        { return Type; }
    void setColor(const QColor &color)
        { myColor = color; }
    QColor color() const
        { return myColor; }
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
    TArcLine* prevLine(){
        if (lines.count() > 0)
            return lines.last();
         else return NULL;
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
    TTop *myStartTop;
    TTop *myEndTop;
    QColor myColor;
    QPolygonF arcHead;
    TArcTop *arcTop;
    QMenu *myContextMenu;
    int width;  //приоритет
    bool autoBuild(TTop* top, float dx, float dy);
    bool remake(TTop *, float dx, float dy);
};

#endif

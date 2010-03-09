#ifndef QARC_H
#define QARC_H

#include <QGraphicsLineItem>

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

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    QMenu *myContextMenu;
};

class TArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };

    TArcLine(QLineF line, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
    { return Type;}
    TArc* owner() const
    { return qgraphicsitem_cast<TArc *>(parentItem()); }
    QPainterPath shape() const;

private:

};

class TArc : public QGraphicsLineItem
{
    //Q_OBJECT
public:
    enum { Type = ARC_TYPE };
    QList<TArcLine *> lines;

    TArc(TTop *startItem, TTop *endItem, QMenu *contextMenu,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~TArc();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    bool addLine(TArcLine *line);
    TArcLine* newLine(QPointF p1, QPointF p2);
    TArcLine* currentLine;

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
    void setPriority(int w);

    bool remake(TTop *, float dx, float dy);
    void realloc();
    bool autoBuild();
    void updateBounds();
public slots:
    void updatePosition();

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
};

#endif

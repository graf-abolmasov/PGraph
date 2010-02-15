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

/*карантин************************************************************/
#define UP	0x80
#define LEFT	0x20
#define DOWN	0x08
#define RIGHT	0x02

//from struct.h
#define	OBJNAMELEN	9
#define EXTNAMELEN	255

struct TArcTransferStruct
 {
  char ObjName[OBJNAMELEN];
  char ExtName[EXTNAMELEN];
  int Prior;
  bool CritSection;
 };
/*карантин************************************************************/

class TArcTop : public QGraphicsPolygonItem
{
public:
    enum { Type = ARC_TOP_TYPE };

    TArcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
    { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QMenu *myContextMenu;
};

class TArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };

    TArcLine(TArc *owner, QLineF line, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
    { return Type;}
    TArc* owner() const
    { return myOwner; }

private:
    TArc *myOwner;
};

class TArc : public QGraphicsLineItem
{
public:
    enum { Type = ARC_TYPE };
    int width;
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
    void setPriority(int w);
    void setStartTop(TTop* startTop){
        myStartTop = startTop;
    }
    void setEndTop(TTop* endTop){
        myEndTop = endTop;
    }
    TTop *startItem() const
        { return myStartTop; }
    TTop *endItem() const
        { return myEndTop; }
    TArcLine* prevLine(){
        if (lines.count() > 0)
            return lines.last();
         else return NULL;
    }

    void remake(TTop *, int, int);
    void rebuild(TTop *, int, int);
    bool realloc();
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
};

#endif

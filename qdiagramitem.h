#ifndef QDIAGRAMITEM_H
#define QDIAGRAMITEM_H

#include <qgraphicsitem.h>
#include "qlist.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class TArc;

class TTop : public QGraphicsPolygonItem
{
    friend class QDiagramScene;
    friend class TArc;

public:
    enum { Type = UserType + 15 };
    TTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int number;
    void removeArc(TArc *arc);
    void removeArcs();
    void setIcon(QImage icon);
    void autoArrangeArcsAtBound(QLineF bound);
    void autoArrangeArcs(QList<TArc* > brokenLines);
    void addArc(TArc *arc);
    void setAsRoot(bool flag);
    float getMinWidth();
    float getMinHeight();
    QLineF getIntersectBound(QLineF line);
    int type() const
        { return Type; }
    QList<TArc *> allArcs()
        { return arcs; }
    QList<TArc *> inArcs();
    QList<TArc *> outArcs();
    QRectF boundingRect() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    bool isRoot;
    QList<TArc *> arcs;
    QMenu *myContextMenu;
    QImage myIcon;
    QList<TArc *> getArcsAtBound(int i);
    QList<TArc *> getArcsAtBound(QLineF bound);
    static int counter;
};

#endif

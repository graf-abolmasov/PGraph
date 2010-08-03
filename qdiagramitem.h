#ifndef QDIAGRAMITEM_H
#define QDIAGRAMITEM_H

#include <QGraphicsItem>
#include <QList>
#include "qsyncarc.h"
#include "qarc.h"
#include "actor.h"

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

class QArc;
class QSyncArc;

#define TOP_TYPE UserType+15

class QTop : public QGraphicsPolygonItem{

    friend class QDiagramScene;
    friend class QArc;
    friend class QSyncArc;

public:
    enum { Type = TOP_TYPE};
    QTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int number;
    void removeArc(QArc *arc);
    void removeArcs();
    void removeSync(QSyncArc *arc);
    void removeSyncs();
    void setIcon(QImage icon);
    void addArc(QArc *arc);
    void addSync(QSyncArc *arc);
    void setAsRoot(bool flag);
    float getMinWidth();
    float getMinHeight();
    QLineF getIntersectBound(QLineF line);
    int type() const
        { return Type; }
    QList<QArc *> allArcs()
        { return arcs; }
    QList<QArc *> inArcs();
    QList<QArc *> outArcs();
    QRectF boundingRect() const;
    bool isRoot;
    Actor* actor;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QList<QArc *> arcs;
    QList<QSyncArc *> sync;
    QMenu *myContextMenu;
    QImage myIcon;
    QList<QArc *> getArcsAtBound(int i);
    QList<QArc *> getArcsAtBound(QLineF bound);
    static int counter;
};

#endif

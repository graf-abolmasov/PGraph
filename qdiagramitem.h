#ifndef QDIAGRAMITEM_H
#define QDIAGRAMITEM_H

#include <QGraphicsItem>
#include <QList>
#include "qsyncarc.h"
#include "qarc.h"

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
class QSyncArc;

class TTop : public QGraphicsPolygonItem{
    friend class QDiagramScene;
    friend class TArc;
    friend class QSyncArc;

public:
    enum { Type = UserType + 15 };
    TTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int number;
    void removeArc(TArc *arc);
    void removeArcs();
    void removeSync(QSyncArc *arc);
    void removeSyncs();
    void setIcon(QImage icon);
    void addArc(TArc *arc);
    void addSync(QSyncArc *arc);
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
    bool isRoot;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QList<TArc *> arcs;
    QList<QSyncArc *> sync;
    QMenu *myContextMenu;
    QImage myIcon;
    QList<TArc *> getArcsAtBound(int i);
    QList<TArc *> getArcsAtBound(QLineF bound);
    static int counter;
};

#endif

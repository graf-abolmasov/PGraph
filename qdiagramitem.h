#ifndef QDIAGRAMITEM_H
#define QDIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
//#include "qdiagramscene.h"

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

public:
    enum { Type = UserType + 15 };
    enum DiagramType {Top};

    TTop(DiagramType diagramType, QMenu *contextMenu,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeArc(TArc *arc);
    void removeArcs();
    void setIcon(QImage icon);
    QList<TArc *> getArcsAtBound(int i);
    QList<TArc *> getArcsAtBound(QLineF bound);
    QLineF getIntersectBound(QLineF line);
    void autoArrangeArcsAtBound(QLineF bound);
    DiagramType diagramType() const
        { return myDiagramType; }
    QPolygonF polygon() const
        { return myPolygon; }
    void addArc(TArc *arc);
    int type() const
        { return Type; }
    QList<TArc *> allArcs()
    { return arcs; }
    QList<TArc *> inArcs();
    QList<TArc *> outArcs();
    void setPolygon ( const QPolygonF & polygon ){
        QGraphicsPolygonItem::setPolygon(polygon);
    }
    void setAsRoot(bool flag);


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
    DiagramType myDiagramType;
    bool isRoot;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<TArc *> arcs;
    QImage myIcon;
};

#endif

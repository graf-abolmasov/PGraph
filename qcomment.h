#ifndef QCOMMENT_H
#define QCOMMENT_H

#include "qgraphicsitem.h"
#include "qpen.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class TComment : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    TComment(QMenu *menu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }

signals:
    void lostFocus(TComment *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QMenu* myContextMenu;
};


#endif

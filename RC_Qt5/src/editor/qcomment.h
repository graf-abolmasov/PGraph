#ifndef QCOMMENT_H
#define QCOMMENT_H

#include <QGraphicsTextItem>
#include <QtGui/QFont>
#include "../../src/common/VO/graphvo.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsItem;
class QGraphicsScene;
class QPen;
QT_END_NAMESPACE

class QComment : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };
    QComment(QMenu *menu, QGraphicsItem *parent = 0);
    int type() const
        { return Type; }
    Comment toComment();

signals:
    void lostFocus(QComment *item);

protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QMenu* myContextMenu;
};


#endif

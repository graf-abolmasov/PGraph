#include <QtGui>

#include "qcomment.h"

QComment::QComment(QMenu *menu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
    myContextMenu = menu;
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant QComment::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void QComment::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void QComment::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void QComment::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    if (textInteractionFlags() == Qt::NoTextInteraction)
        myContextMenu->exec(event->screenPos());
    else
        QGraphicsTextItem::contextMenuEvent(event);
}

Comment* QComment::toComment()
{
    return new Comment(scenePos().x(), scenePos().y(), document()->toPlainText());
}

Comment::Comment(float x, float y, QString text)
{
    this->x = x;
    this->y = y;
    this->text = text;
}

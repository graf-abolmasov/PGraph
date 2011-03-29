#ifndef QCOMMENT_H
#define QCOMMENT_H

#include <QGraphicsItem>
#include <QPen>
#include <QFont>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QFont;
QT_END_NAMESPACE

class Comment
{
public:
    Comment(float x, float y, QString text, QFont font);
    float x;
    float y;
    QString text;
    QFont font;
};

class QComment : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };
    QComment(QMenu *menu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type; }
    Comment* toComment();

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

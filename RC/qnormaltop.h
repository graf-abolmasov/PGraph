#ifndef QNORMALTOP_H
#define QNORMALTOP_H

#include "qtop.h"

#define NORMAL_TOP_TYPE UserType+15

class QNormalTop : public QTop
{
public:
    enum { Type = NORMAL_TOP_TYPE };
    QNormalTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type; }
    void setAsRoot(bool flag);
    float getMinWidth() const;
    float getMinHeight() const;
    void setIcon(QImage &icon);
    Top toTop() const;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    int procCount;
};


#endif // QNORMALTOP_H

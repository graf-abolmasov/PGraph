#ifndef QMULTIPROCTOP_H
#define QMULTIPROCTOP_H

#include "qtop.h"

#define MULTIPROC_TOP_TYPE UserType+15

class QMultiProcTop : public QTop
{
public:
    enum { Type = MULTIPROC_TOP_TYPE };
    QMultiProcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type; }
    Top *toTop() const;
    int procCount;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
};

#endif // QMULTIPROCTOP_H

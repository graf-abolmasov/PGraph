#ifndef QMULTIPROCTOP_H
#define QMULTIPROCTOP_H

#include <QtCore>
#include <QtGui>
#include <QGraphicsItem>

#define MULTIPROC_TOP_TYPE UserType+8

class QMultiProcTop : public QGraphicsRectItem
{
public:
    enum { Type = MULTIPROC_TOP_TYPE };
    QMultiProcTop(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type; }
    QRectF boundingRect() const;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
    QMenu* myContextMenu;
    int procCount;
};

#endif // QMULTIPROCTOP_H

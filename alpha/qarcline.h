#ifndef QARCLINE_H
#define QARCLINE_H

#include <QGraphicsLineItem>
#include <QtCore>
#include <QtGui>

#define ARC_LINE_TYPE UserType+6

class QArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };
    QArcLine(QLineF line, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QPainterPath shape() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const
        { return Type;}
};

#endif // QARCLINE_H

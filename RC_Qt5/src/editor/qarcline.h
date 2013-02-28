#ifndef QARCLINE_H
#define QARCLINE_H

#include <QGraphicsLineItem>

#define ARC_LINE_TYPE UserType+6

class QArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };
    QArcLine(QLineF line, QGraphicsItem *parent = 0);
    QPainterPath shape() const;
    QRectF boundingRect() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const
        { return Type;}
};

#endif // QARCLINE_H

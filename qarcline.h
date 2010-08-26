#ifndef QARCLINE_H
#define QARCLINE_H

#include <QGraphicsLineItem>

#define ARC_LINE_TYPE UserType+6

class QArcLine : public QGraphicsLineItem
{
public:
    enum { Type = ARC_LINE_TYPE };
    QArcLine(QLineF line, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QPainterPath shape() const;
    int type() const
        { return Type;}
    void setSelected(bool selected);
};

#endif // QARCLINE_H

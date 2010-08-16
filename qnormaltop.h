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
    float getMinWidth();
    float getMinHeight();
    void setIcon(QImage &icon);
    Top* toTop();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    int procCount;
    QImage myIcon;
};


#endif // QNORMALTOP_H

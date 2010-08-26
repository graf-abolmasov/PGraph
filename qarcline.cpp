#include "qarcline.h"
#include "qarc.h"

QArcLine::QArcLine(QLineF line, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(line, parent, scene)
{

};

QPainterPath QArcLine::shape() const {
    QPainterPath path;
    path.addRect(QGraphicsLineItem::shape().boundingRect().adjusted(-5, -5, 5, 5));
    return path;
}

void QArcLine::setSelected(bool selected)
{
    scene()->clearSelection();
    foreach (QGraphicsLineItem *line, qgraphicsitem_cast<QArc* >(parentItem())->lines)
        line->setSelected(selected);
}

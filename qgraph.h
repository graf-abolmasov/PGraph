#ifndef QGRAPH_H
#define QGRAPH_H

#include <QObject>
#include <QList>
#include "qdiagramitem.h"
#include "qarc.h"
#include "qcomment.h"

class QGraph : public QObject
{
Q_OBJECT
public:
    explicit QGraph(QObject *parent = 0);

    QList<QTop* >     topList;
    QList<QArc* >     arcList;
    QList<TComment* > commentList;
    QList<QSyncArc* > syncArcList;
};

#endif // QGRAPH_H

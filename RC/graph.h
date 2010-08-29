#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include "qtop.h"
#include "qarc.h"
#include "qcomment.h"
#include "qmultiproctop.h"

class Graph
{
public:
    Graph(QString name, QString extName, QList<Top* > &topList, QList<Arc* > &arcList, QList<Comment* > &commentList, QList<QSyncArc* > &syncArcList, QList<QMultiProcTop* > &multiProcTopList);

    QString name;
    QString extName;

    QList<Top* >           topList;
    QList<Arc* >           arcList;
    QList<Comment* >       commentList;
    QList<QSyncArc* >      syncArcList;
    QList<QMultiProcTop* > multiProcTopList;
};

#endif // GRAPH_H

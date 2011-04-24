#ifndef GRAPH_H
#define GRAPH_H

#include <QtCore/QList>
#include <QtCore/QString>

class Top;
class Arc;
class Comment;
class QSyncArc;
class QMultiProcTop;

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

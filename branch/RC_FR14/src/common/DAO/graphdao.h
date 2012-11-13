#ifndef GRAPHDAO_H
#define GRAPHDAO_H

#include "../../src/common/DAO/basedao.h"

class Graph;
class GraphDAO : private BaseDAO
{
public:
    GraphDAO(const QSqlDatabase &db);
    void persist(const Graph &graph);
    void persist(const Graph *graph);

    void remove(const Graph &graph);
    void remove(const QString &name);

    Graph findByName(const QString &name);
    QList<Graph> findAll(bool loadContent = false);
};

#endif // GRAPHDAO_H

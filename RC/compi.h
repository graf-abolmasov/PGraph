#ifndef COMPI_H
#define COMPI_H

#include <QtCore/QString>
#include <QDebug>
#include "compiler.h"
#include "graph.h"

class GraphCompiler : public ICompiler
{
public:
//    GraphCompiler(const QString &graphName);
    GraphCompiler(const Graph &graph);
    virtual void compile();
private:

    QString myGraphName;
    Graph myGraph;
    virtual void init(){}
    virtual void finalize(){}
};

#endif // COMPI_H

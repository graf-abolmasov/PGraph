#ifndef COMPI_H
#define COMPI_H

#include <QtCore/QString>
#include <QDebug>
#include "../../src/common/VO/graphvo.h"
#include "../../src/compiler/abstractcompiler.h"

class DefineTop {
public:
    DefineTop(){}
    DefineTop(QString actor, int firstIndex, int lastIndex);
    int firstIndex;
    int lastIndex;
    QString actor;
};

class DefineGraph {
public:
    DefineGraph(QString predicate, int topIndex);
    DefineGraph(QString predicate, int topIndex, int type);
    QString predicate;
    int topIndex;
    int type;
};

class GraphStruct {
public:
    QString namepr;
    int procsMax;
    QMap<QString, int> entries;
    QList<DefineTop> topsTable;
    QList<DefineGraph> graphsTable;
    QSet<const Predicate *> usedPredicates;
    QSet<const Actor *> usedActors;
    QSet<const BaseModule *> usedBasemodules;
};

class GraphCompiler : public AbstractCompiler
{
public:
    GraphCompiler();
    QList<GraphStruct> compile(const Graph &graph);
private:
    QSet<QString> mySkipList;
    QSet<QString> compiledGraphs;
    QList<GraphStruct> result;
    QMap<QString, int> procsMax;
    QMap<QString, int> procsCounter;

    QStringList compileRecursively(const Graph &graph);
    GraphStruct compileStruct(const Graph &graph);
};

#endif // COMPI_H

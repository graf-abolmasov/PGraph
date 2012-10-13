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
    DefineGraph(int predicateIndex, int topIndex);
    DefineGraph(int predicateIndex, int topIndex, int type);
    int predicateIndex;
    int topIndex;
    int type;
};

class DefinePredicate
{
public:
    DefinePredicate(QString namepr);
    QString namepr;
    QString toString() const { return "DefinePredicate(\"" + namepr + "\", &" + namepr + ")"; }
};

class GraphStruct {
public:
    QString namepr;
    int procsMax;
    QMap<QString, int> entries;
    QList<DefineTop> topsTable;
    QList<DefinePredicate> predicatesTable;
    QList<DefineGraph> graphsTable;
    QList<const Predicate *> usedPredicates;
    QList<const Actor *> usedActors;
    QList<const BaseModule *> usedBasemodules;
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
    void compileStruct(const Graph &graph);
};

#endif // COMPI_H

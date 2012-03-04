#ifndef COMPI_H
#define COMPI_H

#include <QtCore/QString>
#include <QDebug>
#include "../../src/common/VO/graph.h"
#include "../../src/compiler/compiler.h"

class GraphCompiler : public ICompiler
{
public:
    GraphCompiler(const Graph &graph);
    virtual bool compile();
private:
    Graph myGraph;

    GraphCompiler(const Graph &graph, QSet<QString> &skip);
    void initDirectories();

    QSet<QString> mySkipList;

    QString myOutputDirectory;
    QString myBaseDirectory;
    QString myTemplateDirectory;

    QList<const Predicate *> usedPredicateList;
    QList<const Actor *> usedActorList;
    QList<const BaseModule *> usedBaseModuleList;
    QStringList myOtherFilesList;

    void collectUsedData();
    void copyStaticTemplates();
    void copyUsedFiles();
    void unpackGraph(const Graph &graph, QSet<const Predicate *> &predicates, QSet<const Actor *> &actors, QSet<QString> &exrtractedGraphs);

    void compileMain() const;
    void compileStruct() const;
    void compileMakefile(QString buildType) const;

};

#endif // COMPI_H

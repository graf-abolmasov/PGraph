#ifndef COMPI_H
#define COMPI_H

#include <QtCore/QString>
#include <QDebug>
#include "../../src/common/VO/graphvo.h"
#include "../../src/compiler/compiler.h"

class GraphCompiler : public ICompiler
{
public:
    GraphCompiler(const Graph &graph);
    virtual bool compile();
private:
    Graph myGraph;

    GraphCompiler(const Graph &graph, QSet<QString> &skip);

    QStringList compileRecursively(const Graph &graph);
    void initDirectories();

    QSet<QString> mySkipList;

    QString myOutputDirectory;
    QString myBaseDirectory;
    QString myTemplateDirectory;

    QStringList myOtherFilesList;

    QSet<const Predicate *> predicates;
    QSet<const Actor *> actors;
    QSet<const BaseModule *> baseModules;
    QSet<QString> compiledGraphs;

    void copyStaticTemplates();
    void copyUsedFiles();
    QString buildGraph(const QString &name, const QString &extName, int root) const;

    void compileMain() const;
    void compileStruct(const Graph &graph);
    void compileMakefile(QString buildType) const;

};

#endif // COMPI_H

#ifndef SOURCECOMPILER_H
#define SOURCECOMPILER_H

#include <QtCore>
#include <QtGui>
#include "abstractcompiler.h"
#include "graphcompiler.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/datatype.h"
#include "../../src/common/VO/variable.h"
#include "../../src/common/qgraphsettings.h"

#define PODATA_FILE_NAME "tpodata"
#define USER_TYPES_FILE_NAME "utype"

class SourceCompiler : public AbstractCompiler
{
private:
    QString myOutputDirectory;
    QString myTemplateDirectory;
    QString myBaseDirectory;

    QStringList myOtherFilesList;
    QMap<QString, QString> mpiTypes;

    QStringList compileUserTypes(QList<const DataType *> dataTypes);
    QStringList compileTpoData(QList<const Variable *> variables);
    void copyStaticTemplates();
    void compileEnvironment(int procsMax) const;
    void copyUsedFiles(const QList<const Actor *> &actors, const QList<const Predicate *> &predicates, const QList<const BaseModule *> &baseModules);
    void compileMain(const QString graphName, const QStringList &actors) const;
    void compileMakefile(QString buildType, const QList<const Actor *> &actors, const QList<const Predicate *> &predicates, const QList<const BaseModule *> &baseModules) const;
    void initDirectories();
    QString buildGraph(const QString &name, const QString &extName, int root) const;
    QStringList compileStruct(const GraphStruct &graphStruct);
    QStringList compileActor(const Actor *actor) const;
    QStringList compilePredicate(const Predicate *predicate) const;
    QStringList compileBasemodule(const BaseModule *baseModule) const;
    QString buildDataType(const DataType *dataType) const;

public:
    SourceCompiler();

    QStringList compileData(QList<const Variable *> variables);
    QStringList compileCode(const QList<GraphStruct> &graphStruct);
};

#endif // SOURCECOMPILER_H

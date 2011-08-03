#ifndef DATACOMPILER_H
#define DATACOMPILER_H

#include <QtCore/QString>
#include "compiler.h"

#define USER_TYPES_FILE_NAME   "utype"
#define PODATA_FILE_NAME   "tpodata"

class DataCompiler : public ICompiler
{
public:
    enum Type {Serial, Parallel};

    DataCompiler(Type type);
    void compile();
private:
    void init();
    QString getTemplate(const QString &fileName) const;

    void compileParallel();
    void compileSerial();

    QString myOutputDirectory;
    QString myTemplateDirectory;
    const Type myType;
};

#endif // DATACOMPILER_H

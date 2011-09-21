#ifndef DATACOMPILER_H
#define DATACOMPILER_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include "compiler.h"

#define USER_TYPES_FILE_NAME   "utype"
#define PODATA_FILE_NAME   "tpodata"

class DataCompiler : public ICompiler
{
public:
    enum Type {Serial, Parallel};

    DataCompiler(Type type);
    bool compile();
private:
    void compileParallel();
    void compileSerial();

    QString myOutputDirectory;
    QString myTemplateDirectory;
    const Type myType;

    QMap<QString, QString> mpiTypes;

    void compileUserTypes();
    void compileTpoData();
    void compileSimpleTpoData();
};

#endif // DATACOMPILER_H

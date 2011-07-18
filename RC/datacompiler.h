#ifndef DATACOMPILER_H
#define DATACOMPILER_H

#include <QtCore/QString>
#include "compiler.h"

#define USER_TYPES_FILE_NAME   "utype"
#define PODATA_FILE_NAME   "podata"

class DataCompiler : public ICompiler
{
public:
    DataCompiler();
    void compile();
private:
    void init();
    QString getTemplate(const QString &fileName) const;

    QString myOutputDirectory;
};

#endif // DATACOMPILER_H

#ifndef COMPILER_H
#define COMPILER_H

#include <QtCore/QString>

//#define SYSTEM_TYPES_FILE_NAME "stype.h"
#define USER_TYPES_FILE_NAME   "utype"
#define PODATA_FILE_NAME   "podata"

class Compiler
{
public:
    Compiler();

    void compileData() const;
private:
    QString getTemplate(const QString &type) const;

    QString myOutputDirectory;
};

#endif // COMPILER_H

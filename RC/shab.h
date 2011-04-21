#ifndef SHAB_H
#define SHAB_H

#include "datatype.h"
#include <qstring.h>
#include "qvariant.h"

class Shab
{
public:
        Shab(int NLEX, QString TIPLEX, QString LEXEM);
    int NLEX;
    QString TIPLEX;
    QString LEXEM;

};

#endif // SHAB_H

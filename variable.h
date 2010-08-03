#ifndef VARIABLE_H
#define VARIABLE_H
#include "datatype.h"
#include <qstring.h>
#include "qvariant.h"

class Variable
{
public:
    Variable();
    Variable(QString name, QString type, QVariant initValue, QString description);

    QString name;
    QString type;
    QVariant initValue;
    QString description;

};

#endif // VARIABLE_H

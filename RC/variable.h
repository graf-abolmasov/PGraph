#ifndef VARIABLE_H
#define VARIABLE_H
#include "datatype.h"
#include <qstring.h>
#include "qvariant.h"

class Variable
{
public:
    Variable(QString name, QString type, QVariant initValue, QString comment);
    QString name;
    QString type;
    QVariant initValue;
    QString comment;

};

#endif // VARIABLE_H

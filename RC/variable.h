#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtCore/QVariant>
#include <QtCore/QString>
#include "datatype.h"

class Variable
{
public:
    Variable(const QString &name, const QString &initValue, const QString &comment, const DataType *type);
    QString name;
    QString initValue;
    QString comment;
    const DataType *type;
};

#endif // VARIABLE_H

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtCore/QVariant>
#include <QtCore/QString>
#include "datatype.h"

class Variable
{
private:
    DataType *myType;
public:
    Variable(QString name, QString type, QString initValue, QString comment);
    QString name;
    QString initValue;
    QString comment;
    QString type;

    //QString getType() const;
    //void setType(const QString &value);
    DataType *getDataType();
};

#endif // VARIABLE_H

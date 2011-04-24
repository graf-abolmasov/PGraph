#ifndef DATATYPE_H
#define DATATYPE_H

#include <QtCore/QString>

class DataType
{
public:
    DataType(QString name, QString typedefStr);
    QString name;
    QString typedefStr;
};

#endif // DATATYPE_H

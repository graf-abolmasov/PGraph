#ifndef DATATYPE_H
#define DATATYPE_H
#include <QString>

class DataType
{
public:
    DataType();
    DataType(QString name, QString typedefStr);
    QString name;
    QString typedefStr;
};

#endif // DATATYPE_H

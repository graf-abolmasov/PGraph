#include "datatype.h"

DataType::DataType()
{
}

DataType::DataType(QString name, QString typedefStr)
{
    this->name = name;
    this->typedefStr = typedefStr;
}

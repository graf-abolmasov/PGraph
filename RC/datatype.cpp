#include "datatype.h"

DataType::DataType(QString name, QString typedefStr)
{
    this->name = name;
    this->typedefStr = typedefStr;
}

bool DataType::operator ==(const DataType& other) const
{
    return other.name == this->name;
}

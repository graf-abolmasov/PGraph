#include "datatype.h"
#include <QDebug>

DataType::DataType(QString name, QString typedefStr)
{
    this->name = name;
    this->typedefStr = typedefStr;
}

DataType::~DataType()
{
}


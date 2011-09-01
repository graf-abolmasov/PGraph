#include "datatype.h"
#include "errors.h"

DataType::DataType(QString name, QString typedefStr)
{
    this->name = name;
    this->typedefStr = typedefStr;
}

QStringList DataType::validate() const
{
    QStringList msgs;
    if (name.isEmpty())
        msgs << QObject::tr(ERR_DATATYPE_EMPTY_NAME);
    if (typedefStr.isEmpty())
        msgs << QObject::tr(ERR_DATATYPE_EMPTY_TYPEDEF);
    return msgs;
}

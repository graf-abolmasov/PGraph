#include "datatype.h"
#include "../../src/common/errors.h"
#include "../../src/common/qgraphsettings.h"

DataType::DataType(QString name, QString typedefStr)
{
    this->name = name;
    this->typedefStr = typedefStr;
}

QStringList DataType::validate() const
{
    QStringList msgs;
    if (name.trimmed().isEmpty())
        msgs << QObject::tr(ERR_DATATYPE_EMPTY_NAME);
    if (typedefStr.trimmed().isEmpty())
        msgs << QObject::tr(ERR_DATATYPE_EMPTY_TYPEDEF);
    return msgs;
}

bool DataType::isArray() const
{
    QRegExp rx("typedef.*\\];$");
    int i = rx.indexIn(typedefStr);
    return i >= 0;
}

bool DataType::isSimple() const
{
    return (!(isStruct() || isArray()));
}

bool DataType::isStruct() const
{
    QRegExp rx("typedef struct.*\\} " + name + ";$");
    return rx.indexIn(typedefStr) > 0;
}

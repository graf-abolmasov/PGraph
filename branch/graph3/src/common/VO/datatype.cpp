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

QString DataType::build() const
{
    QString result;
    if (typedefStr.isEmpty())
        return result;

    result.append(typedefStr).append("\r\n");

    if (!QGraphSettings::isParallel())
        return result;

    if (isArray()) {
        //Парсим количество элементов
        QRegExp r("(\\[(.+)\\])");
        r.setMinimal(true);
        Q_ASSERT(r.indexIn(typedefStr) != -1);
        result.append("#define ").append(name).append("_LENGTH ").append(r.cap(2)).append("\r\n");

        //Парсим тип элемента
        QRegExp r2("(typedef\\s+(\\b.+\\b))");
        r2.setMinimal(true);
        Q_ASSERT(r2.indexIn(typedefStr) != -1);
        result.append("typedef ").append(r2.cap(2)).append("* ptr").append(name).append(";\r\n");
    }

    return result;
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

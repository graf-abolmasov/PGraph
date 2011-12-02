#ifndef DATATYPE_H
#define DATATYPE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

class DataType
{
public:
    DataType(QString name, QString typedefStr);
    QString name;
    QString typedefStr;
    QStringList validate() const;
    QString build() const;
    bool isStruct() const;
    bool isArray() const;
    bool isSimple() const;
};

#endif // DATATYPE_H

#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QtCore/QString>
#include <QtCore/QStringList>

class BaseModuleParameter
{
public:
    BaseModuleParameter(QString accessMode, QString name, QString type, QString comment);

    QString accessMode;
    QString name;
    QString type;
    QString comment;

    bool operator ==(const BaseModuleParameter &other) const {
        return name == other.name;
    }
};

class BaseModule
{
public:
    BaseModule(QString name, QString uniqName, QString comment, QList<BaseModuleParameter> parameterList);
    QString name;
    QString uniqName;
    QList<BaseModuleParameter> parameterList;
    QString comment;
};

#endif // BASEMODULE_H

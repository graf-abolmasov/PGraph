#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QtCore/QString>
#include <QtCore/QStringList>

class BaseModule
{
public:
    BaseModule(QString name, QString unicName, QString comment, QStringList parameterList);
    QString name;
    QString unicName;
    QStringList parameterList;
    QString comment;

};

#endif // BASEMODULE_H

#ifndef BASEMODULE_H
#define BASEMODULE_H
#include <QString>
#include <QStringList>

class BaseModule
{
public:
    BaseModule();
    BaseModule(QString name, QString unicName, QString comment, QStringList parameterList);
    QString name;
    QString unicName;
    QStringList parameterList;
    QString comment;

};

#endif // BASEMODULE_H

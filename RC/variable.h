#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtCore/QVariant>
#include <QtCore/QString>

class Variable
{
public:
    Variable(QString name, QString type, QVariant initValue, QString comment);
    QString name;
    QString type;
    QVariant initValue;
    QString comment;
};

#endif // VARIABLE_H

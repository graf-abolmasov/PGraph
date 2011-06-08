#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtCore/QVariant>
#include <QtCore/QString>

class Variable
{
public:
    Variable(QString name, QString type, QString initValue, QString comment);
    QString name;
    QString type;
    QString initValue;
    QString comment;

    //bool operator ==(const Variable& other) const;
};

#endif // VARIABLE_H

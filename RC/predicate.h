#ifndef PREDICATE_H
#define PREDICATE_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "variable.h"
#include "basemodule.h"

class Predicate
{
public:
    enum Type {NormalType, InlineType};
    Predicate(QString name, QString extName, Type type, const BaseModule *baseModule, QList<const Variable *> variableList);
    QString extName;
    QString name;
    Type type;
    QList<const Variable *> variableList;
    const BaseModule *baseModule;
};

#endif // PREDICATE_H

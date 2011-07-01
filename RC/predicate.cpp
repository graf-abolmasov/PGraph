#include "predicate.h"

Predicate::Predicate(QString name, QString extName, Type type, const BaseModule *baseModule, QList<const Variable *> variableList)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
}

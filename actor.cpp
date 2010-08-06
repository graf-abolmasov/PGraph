#include "actor.h"

Actor::Actor(QString name, QString extName, Type type, QString baseModule, QList<Variable*> variableList)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
}

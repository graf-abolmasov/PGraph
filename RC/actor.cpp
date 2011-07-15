#include "actor.h"

Actor::Actor(const QString &name, const QString &extName, const Type &type, const BaseModule *baseModule, const QList<const Variable *> &variableList, const QStringList &varAccModeList, const QImage &icon)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
    this->varAccModeList = varAccModeList;
    this->icon = icon;
}

Actor::Actor(const Actor &actor)
{
    this->extName = actor.extName;
    this->name = actor.name;
    this->type = actor.type;
    this->baseModule = actor.baseModule;
    this->varAccModeList = actor.varAccModeList;
    this->icon = actor.icon;
    this->variableList = actor.variableList;
}

Actor::Actor(const QString &name, const QString &extName, const Type &type)
{
    this->name = name;
    this->extName = extName;
    this->type = type;
    this->baseModule = NULL;
}

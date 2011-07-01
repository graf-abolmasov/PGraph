#include "actor.h"

Actor::Actor(QString name, QString extName, Type type, QString baseModule, QList<const Variable *> variableList, QStringList varAccModeList, QImage icon)
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
    this->variableList = variableList;
}

Actor::Actor(QString name, QString extName, Type type)
{
    this->name = name;
    this->extName = extName;
    this->type = type;
}

#include "actor.h"

Actor::Actor(QString name, QString extName, Type type, QString baseModule, QList<Variable*> variableList, QStringList varAMList, QImage icon)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
    this->varAccModeList = varAMList;
    this->icon = icon;
}

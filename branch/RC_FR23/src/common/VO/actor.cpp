#include "actor.h"
#include "basemodule.h"
#include "../../src/common/qgraphsettings.h"
#include "QtCore"
#include "variable.h"
#include "../../src/common/globalvariables.h"

Actor::Actor(const QString &name, const QString &extName, const Type &type, const BaseModule *baseModule, const QList<const Variable *> &variableList, const QStringList &varAccModeList, const QPixmap &icon)
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

QStringList Actor::validate() const
{
    QStringList msgs;   
    if (type == NormalType && baseModule == NULL)
        msgs << QObject::tr(ERR_ACTOR_NULL_BASEMODULE).arg(name);
    if (name.isEmpty())
        msgs << QObject::tr(ERR_ACTOR_EMPTY_NAME).arg(name);
    if (extName.isEmpty())
        msgs << (type == Actor::NormalType ? QObject::tr(ERR_ACTOR_EMPTY_EXTNAME).arg(name) : QObject::tr(ERR_ACTOR_EMPTY_INLINE).arg(name));
    if (baseModule != NULL && variableList.count() != baseModule->parameterList.count())
        msgs << QObject::tr(ERR_ACTOR_UNDEF_VAR).arg(name);
    if (variableList.count() != varAccModeList.count())
        msgs << QObject::tr(ERR_ACTOR_UNDEF_VAR_ACC).arg(name);
    return msgs;
}

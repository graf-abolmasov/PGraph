#include "predicate.h"
#include "../../src/common/errors.h"
#include "../../src/common/qgraphsettings.h"


Predicate::Predicate(QString name, QString extName, Type type, const BaseModule *baseModule, QList<const Variable *> variableList)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
}

QStringList Predicate::validate() const
{
    QStringList msgs;
    if (type == NormalType && baseModule == NULL)
        msgs << QObject::tr(ERR_PREDICATE_NULL_BASEMODULE).arg(name);
    if (name.isEmpty())
        msgs << QObject::tr(ERR_PREDICATE_EMPTY_NAME).arg(name);
    if (extName.isEmpty())
        msgs << (type == NormalType ? QObject::tr(ERR_PREDICATE_EMPTY_EXTNAME).arg(name) : QObject::tr(ERR_PREDICATE_EMPTY_INLINE).arg(name));
    if (baseModule != NULL && variableList.count() != baseModule->parameterList.count())
        msgs << QObject::tr(ERR_PREDICATE_UNDEF_VAR).arg(name);
    return msgs;
}

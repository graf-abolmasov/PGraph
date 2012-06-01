#include "basemodule.h"

BaseModule::BaseModule(QString name, QString uniqName, QString comment, QList<BaseModuleParameter> parameterList)
{
    this->name = name;
    this->uniqName = uniqName;
    this->parameterList = parameterList;
    this->comment = comment;
}

BaseModuleParameter::BaseModuleParameter(QString accessMode, QString name, QString type, QString comment)
{
    this->accessMode = accessMode;
    this->name = name;
    this->type = type;
    this->comment = comment;
}

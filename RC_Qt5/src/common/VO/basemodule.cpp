#include "basemodule.h"

BaseModule::BaseModule(QString name, QString uniqName, QString comment, QList<BaseModuleParameter> parameterList, QString sourceCode)
{
    this->name = name;
    this->uniqName = uniqName;
    this->parameterList = parameterList;
    this->comment = comment;
    this->sourceCode = sourceCode;
}

BaseModuleParameter::BaseModuleParameter(QString accessMode, QString name, QString type, QString comment)
{
    this->accessMode = accessMode;
    this->name = name;
    this->type = type;
    this->comment = comment;
}

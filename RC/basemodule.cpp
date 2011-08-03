#include "basemodule.h"

BaseModule::BaseModule(QString name, QString unicName, QString comment, QStringList parameterList)
{
    this->name = name;
    this->uniqName = unicName;
    this->parameterList = parameterList;
    this->comment = comment;
}

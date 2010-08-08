#include "basemodule.h"

BaseModule::BaseModule(){}

BaseModule::BaseModule(QString name, QString unicName, QString comment, QStringList parameterList)
{
    this->name = name;
    this->unicName = unicName;
    this->parameterList = parameterList;
    this->comment = comment;
}

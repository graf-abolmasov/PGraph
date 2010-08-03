#include "variable.h"

Variable::Variable()
{
}

Variable::Variable(QString name, QString type, QVariant initValue, QString description)
{
    this->name = name;
    this->type = type;
    this->initValue = initValue;
    this->description = description;
}

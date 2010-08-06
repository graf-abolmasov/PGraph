#include "variable.h"

Variable::Variable(QString name, QString type, QVariant initValue, QString comment)
{
    this->name = name;
    this->type = type;
    this->initValue = initValue;
    this->comment = comment;
}

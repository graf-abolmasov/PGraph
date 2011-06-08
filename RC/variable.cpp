#include "variable.h"

Variable::Variable(QString name, QString type, QString initValue, QString comment)
{
    this->name = name;
    this->type = type;
    this->initValue = initValue;
    this->comment = comment;
}

//bool Variable::operator ==(const Variable& other) const
//{
//    return other.name == this->name;
//}

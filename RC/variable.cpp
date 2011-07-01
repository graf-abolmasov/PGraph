#include "variable.h"
#include "globalvariables.h"

Variable::Variable(const QString &name, const QString &initValue, const QString &comment, const DataType *type)
{
    this->name = name;
    this->initValue = initValue;
    this->comment = comment;
    this->type = type;
}

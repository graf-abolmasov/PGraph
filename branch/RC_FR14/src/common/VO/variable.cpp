#include "variable.h"
#include "../../src/common/globalvariables.h"

Variable::Variable(const QString &name, const QString &initValue, const QString &comment, bool isGlobal, const DataType *type)
{
    this->name = name;
    this->type = type;
    this->comment = comment;
    this->initValue = initValue;
    this->isGlobal = isGlobal;
}

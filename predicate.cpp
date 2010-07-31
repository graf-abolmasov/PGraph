#include "predicate.h"

Predicate::Predicate(QString name, QString extName, Type type)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
}

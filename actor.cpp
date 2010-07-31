#include "actor.h"

Actor::Actor(QString name, QString extName, Type type)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
}

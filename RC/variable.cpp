#include "variable.h"
#include "globalvariables.h"

Variable::Variable(QString name, QString type, QString initValue, QString comment)
{
    this->name = name;
    this->type = type;
    this->initValue = initValue;
    this->comment = comment;

    this->myType = NULL;
}

DataType *Variable::getDataType()
{
    if (this->myType == NULL)
        myType = globalDBManager->getDataType(this->type);
    return myType;
}

//void Variable::setType(const QString &value)
//{
//    this->type = value;
//    this->myType = NULL;
//}

//QString Variable::getType() const
//{
//    return this->type;
//}

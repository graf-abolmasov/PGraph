#ifndef DATATYPEDAO_H
#define DATATYPEDAO_H

#include "DAO/basedao.h"

class DataType;
class DataTypeDAO : private BaseDAO
{
private:
    void persist(const DataType &dataType);

public:
    DataTypeDAO(const QSqlDatabase &db);
    void persistList(const QList<DataType> &dataTypeList);
    void persistList(const QList<const DataType *> &dataTypeList);
    QList<DataType> findAll();
};

#endif // DATATYPEDAO_H

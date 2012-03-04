#include "../../src/common/VO/datatype.h"
#include "../../src/common/DAO/datatypedao.h"
#include "../../src/common/globalvariables.h"

DataTypeDAO::DataTypeDAO(const QSqlDatabase &db)
    : BaseDAO(db, "typsys")
{
}

void DataTypeDAO::persist(const DataType &dataType)
{
    QMap<QString, QVariant> values;
    values["TYPE"]    = dataType.name;
    //values["SEQNUM"]  = dataType.sequenceNum;
    values["TYPEDEF"] = dataType.typedefStr;
    BaseDAO::persist(values);
}

void DataTypeDAO::persistList(const QList<DataType> &dataTypeList)
{
    removeAll();
    int i = 0;
    foreach (DataType dataType, dataTypeList) {
        dataType.sequenceNum = i++;
        persist(dataType);
    }
}

void DataTypeDAO::persistList(const QList<const DataType *> &dataTypeList)
{
    QList<DataType> dbDataTypeList;
    foreach (const DataType *dataType, dataTypeList)
        dbDataTypeList.append(DataType(*dataType));
    persistList(dbDataTypeList);
}

QList<DataType> DataTypeDAO::findAll()
{
    openDb();
    QSqlQuery query = prepareSelectAll();
    execQuery(query);
    QList<DataType> result;
    while (query.next()) {
        const QSqlRecord r = query.record();
        result.append(DataType(r.value("TYPE").toString(), r.value("TYPEDEF").toString()));
    }
    myDb.close();
    return result;
}

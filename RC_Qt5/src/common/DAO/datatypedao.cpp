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
    values["SEQNUM"]  = dataType.sequenceNum;
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
    QSqlQuery query = prepareSelectAll("SEQNUM");
    execQuery(query);
    QList<DataType> result;
    while (query.next()) {
        const QSqlRecord r = query.record();
        result.append(DataType(r.value("TYPE").toString(), r.value("TYPEDEF").toString()));
    }
    myDb.close();
    return result;
}

QStringList DataTypeDAO::findUsage(const QString &name)
{
    openDb();
    QSqlQuery query;
    query.prepare("select DISTINCT data from data d JOIN typsys t ON d.type=t.type where t.type=:type and t.project_id=:project_id;");
    query.bindValue(":type", name);
    query.bindValue(":project_id", globalDBManager->getProjectId());
    execQuery(query);
    QStringList result;
    while (query.next())
        result.append(query.record().value("data").toString());
    myDb.close();
    return result;
}

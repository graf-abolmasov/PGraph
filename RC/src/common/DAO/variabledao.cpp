#include "../../src/common/VO/variable.h"
#include "../../src/common/DAO/variabledao.h"
#include "../../src/common/globalvariables.h"

VariableDAO::VariableDAO(const QSqlDatabase &db)
    : BaseDAO(db, "data")
{
}

void VariableDAO::persist(const Variable &variable)
{
    QMap<QString, QVariant> values;
    values["DATA"] = variable.name;
    values["TYPE"] = variable.type->name;
    values["INIT"] = variable.initValue;
    values["COMMENT"] = variable.comment;
//    values["ISGLOBAL"] = variable.isGlobal;
    BaseDAO::persist(values);
}

void VariableDAO::persistList(const QList<Variable> &variableList)
{
    removeAll();
    foreach (Variable variable, variableList)
        persist(variable);
}

void VariableDAO::persistList(const QList<const Variable *> &variableList)
{
    QList<Variable> dbVariableList;
    foreach (const Variable *variable, variableList)
        dbVariableList.append(Variable(*variable));
    persistList(dbVariableList);
}

QList<Variable> VariableDAO::findAll()
{
    openDb();
    QSqlQuery query = prepareSelectAll("DATA");
    execQuery(query);
    QList<Variable> result;
    while (query.next()) {
        const QSqlRecord r = query.record();
        result.append(Variable(r.value("DATA").toString(),
                               r.value("INIT").toString(),
                               r.value("COMMENT").toString(),
                               false,
                               globalDBManager->getDataType(r.value("TYPE").toString())));
    }
    myDb.close();
    return result;
}

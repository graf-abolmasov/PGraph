#include "../../src/common/DAO/predicatedao.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/common/globalvariables.h"

PredicateDAO::PredicateDAO(const QSqlDatabase &db)
    : BaseDAO(db, "actor")
{
}

void PredicateDAO::persist(const Predicate &predicate)
{
    openDb();
    QSqlQuery query;
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, PROTOTIP)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :PROTOTIP);");
    query.bindValue(":NAMEPR",     predicate.name);
    query.bindValue(":CLASPR",     "p");
    query.bindValue(":EXTNAME",    predicate.extName);
    query.bindValue(":PROTOTIP",   predicate.baseModule == NULL ? NULL : predicate.baseModule->name);
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    execQuery(query);

    QList<const Variable *> predicateVariables = predicate.variableList;
    for (int j = 0; j < predicateVariables.count(); j++) {
        BaseDAO passportDAO = BaseDAO(myDb, "pasport");
        QMap<QString, QVariant> pasportValues;
        pasportValues["NEV"]    = j;
        pasportValues["DATA"]   = predicateVariables[j]->name;
        pasportValues["MODE"]   = "I";
        pasportValues["NAMEPR"] = predicate.name;
        passportDAO.persist(pasportValues);
    }
    myDb.close();
}

void PredicateDAO::persistList(const QList<Predicate> &predicateList)
{
    removeAll();
    foreach (Predicate predicate, predicateList)
        persist(predicate);
}

void PredicateDAO::persistList(const QList<const Predicate *> &predicateList)
{
    QList<Predicate> dbPredicateList;
    foreach (const Predicate *predicate, predicateList)
        dbPredicateList.append(Predicate(*predicate));
    persistList(dbPredicateList);
}

QList<Predicate> PredicateDAO::findAll()
{
    openDb();
    QMap<QString, QVariant> where;
    where["CLASPR"] = "p";
    QSqlQuery queryPredicate = prepareSelect(where, "EXTNAME");
    execQuery(queryPredicate);

    QList<Predicate> result;
    while (queryPredicate.next()) {
        const QSqlRecord predicateRecord = queryPredicate.record();
        const BaseDAO passportDAO = BaseDAO(myDb, "pasport");
        QMap<QString, QVariant> where;
        where["NAMEPR"] = predicateRecord.value("NAMEPR").toString();
        QSqlQuery queryVariables = passportDAO.prepareSelect(where, "NEV");
        passportDAO.execQuery(queryVariables);

        QList<const Variable *> predicateVariableList;
        while (queryVariables.next())
            predicateVariableList.append(globalDBManager->getVariable(queryVariables.record().value("DATA").toString()));
        result.append(Predicate(predicateRecord.value("NAMEPR").toString(),
                                predicateRecord.value("EXTNAME").toString(),
                                predicateRecord.value("PROTOTIP").toString().isEmpty() ? Predicate::InlineType : Predicate::NormalType,
                                globalDBManager->getBaseModule(predicateRecord.value("PROTOTIP").toString()),
                                predicateVariableList));
    }
    myDb.close();
    return result;
}

void PredicateDAO::removeAll()
{
    openDb();
    QMap<QString, QVariant> where;
    where["CLASPR"] = "p";
    QSqlQuery query = prepareDelete(where);
    execQuery(query);
    myDb.close();
}

QStringList PredicateDAO::findUsage(const QString &name)
{
    openDb();
    QSqlQuery query;
    query.prepare("select DISTINCT extname from arcpic ar JOIN actor ac ON ar.namepr=ac.namepr where ar.predicate=:predicate and ar.project_id=:project_id;");
    query.bindValue(":predicate", name);
    query.bindValue(":project_id", globalDBManager->getProjectId());
    execQuery(query);
    QStringList result;
    while (query.next())
        result.append(query.record().value("extname").toString());
    myDb.close();
    return result;
}

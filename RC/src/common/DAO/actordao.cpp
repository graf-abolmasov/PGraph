#include "../../src/common/DAO/actordao.h"
#include "../../src/common/VO/actor.h"
#include "../../src/common/globalvariables.h"

ActorDAO::ActorDAO(const QSqlDatabase &db)
    : BaseDAO(db, "actor")
{
}

void ActorDAO::persist(const Actor &actor)
{
    openDb();
    QSqlQuery query;
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, PROTOTIP)"
                   "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :PROTOTIP);");
    query.bindValue(":NAMEPR",     actor.name);
    query.bindValue(":CLASPR",     "a");
    query.bindValue(":EXTNAME",    actor.extName);
    query.bindValue(":PROTOTIP",   actor.baseModule == NULL ? NULL : actor.baseModule->name);
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    execQuery(query);

    QList<const Variable *> actorVariables = actor.variableList;
    for (int j = 0; j < actorVariables.count(); j++) {
        BaseDAO passportDAO = BaseDAO(myDb, "pasport");
        QMap<QString, QVariant> pasportValues;
        QString vaMode;
        if (actor.varAccModeList[j] == QObject::tr("Исходный"))
            vaMode = "I";
        else if (actor.varAccModeList[j] ==  QObject::tr("Модифицируемый"))
            vaMode = "M";
        else if (actor.varAccModeList[j] ==  QObject::tr("Вычисляемый"))
            vaMode = "R";
        pasportValues["NEV"] = j;
        pasportValues["DATA"] = actorVariables[j]->name;
        pasportValues["NAMEPR"] = actor.name;
        pasportValues["MODE"] = vaMode;
        passportDAO.persist(pasportValues);
    }
    myDb.close();
}

void ActorDAO::persistList(const QList<Actor> &actorList)
{
    removeAll();
    foreach (Actor actor, actorList)
        persist(actor);
}

void ActorDAO::persistList(const QList<const Actor *> &actorList)
{
    QList<Actor> dbActorList;
    foreach (const Actor *actor, actorList)
        dbActorList.append(Actor(*actor));
    persistList(dbActorList);
}

QList<Actor> ActorDAO::findAll()
{
    openDb();
    QMap<QString, QVariant> where;
    where["CLASPR"] = "a";
    QSqlQuery queryActor = prepareSelect(where, "EXTNAME");
    execQuery(queryActor);

    QList<Actor> result;
    while (queryActor.next()) {
        const QSqlRecord actorRecord = queryActor.record();
        const BaseDAO passportDAO = BaseDAO(myDb, "pasport");
        QMap<QString, QVariant> where;
        where["NAMEPR"] = actorRecord.value("NAMEPR").toString();
        QSqlQuery queryVariables = passportDAO.prepareSelect(where, "NEV");
        passportDAO.execQuery(queryVariables);

        QList<const Variable *> actorVariableList;
        QStringList actorVarAccList;
        while (queryVariables.next()) {
            const QSqlRecord variableRecord = queryVariables.record();
            actorVariableList.append(globalDBManager->getVariable(variableRecord.value("DATA").toString()));
            QString vaMode = variableRecord.value("MODE").toString();
            if (vaMode == "I")
                vaMode = QObject::tr("Исходный");
            else if (vaMode == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (vaMode == "R")
                vaMode = QObject::tr("Вычисляемый");
            actorVarAccList.append(vaMode);
        }
        QPixmap p;
        p.loadFromData(actorRecord.value("ICON").toByteArray(), "PNG");
        result.append(Actor(actorRecord.value("NAMEPR").toString(),
                            actorRecord.value("EXTNAME").toString(),
                            actorRecord.value("PROTOTIP").toString().isEmpty() ? Actor::InlineType : Actor::NormalType,
                            globalDBManager->getBaseModule(actorRecord.value("PROTOTIP").toString()),
                            actorVariableList,
                            actorVarAccList,
                            p));
    }
    myDb.close();
    return result;
}

void ActorDAO::saveIcon(const QString &actorName, const QPixmap &image)
{
    openDb();

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    buffer.close();

    QSqlQuery query;
    query.prepare("UPDATE actor SET ICON=:ICON WHERE NAMEPR=:NAMEPR AND PROJECT_ID=:PROJECT_ID");
    query.bindValue(":ICON",       ba);
    query.bindValue(":NAMEPR",     actorName);
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    execQuery(query);
    myDb.close();
}

void ActorDAO::removeAll()
{
    openDb();
    QMap<QString, QVariant> where;
    where["CLASPR"] = "a";
    QSqlQuery query = prepareDelete(where);
    execQuery(query);
    myDb.close();
}

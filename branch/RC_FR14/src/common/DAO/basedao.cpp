#include "basedao.h"
#include "../../src/common/globalvariables.h"

BaseDAO::BaseDAO(const QSqlDatabase &db, const QString &table)
    : table(table), myDb(db)
{
    Q_ASSERT(!table.isEmpty());
}

void BaseDAO::openDb()
{
    if (myDb.isOpen())
        return;
    if (!myDb.open()) {
        globalLogger->log(myDb.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.") + myDb.lastError().text();
    }
}

void BaseDAO::execQuery(QSqlQuery &query) const
{
    if (!query.exec()) {
        globalLogger->log(myDb.lastError().text(), Logger::Critical);
        throw QObject::tr("Ошибка выполнения запроса.") + myDb.lastError().text();
    }
}

QSqlQuery BaseDAO::prepareInsert(QMap<QString, QVariant> values) const
{
    QSqlQuery query;
    QString q = QString("INSERT INTO %1 (%2) VALUES (:%3);").arg(table);

    QMap<QString, QVariant>::const_iterator i = values.constBegin();
    while (i != values.constEnd()) {
        q = q.arg(i.key() + ", %2", i.key() + ", :%3");
        ++i;
    }

    q = q.arg("PROJECT_ID", "PROJECT_ID");
    query.prepare(q);

    i = values.constBegin();
    while (i != values.constEnd()) {
        query.bindValue(":" + i.key(), i.value());
        ++i;
    }

    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareSelectAll(const QString &orderBy) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE PROJECT_ID = :PROJECT_ID" + (orderBy.isEmpty() ? ";" : (" ORDER BY " + orderBy + ";"))).arg(table));
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareSelect(const QMap<QString, QVariant> &where, const QString &orderBy) const
{
    if (where.isEmpty())
        return prepareSelectAll(orderBy);

    QSqlQuery query;
    QString q = QString("SELECT * FROM %1 WHERE %2").arg(table);

    QMap<QString, QVariant>::const_iterator i = where.constBegin();
    while (i != where.constEnd()) {
        q = q.arg(i.key() + " = :" + i.key() + " and %2 ");
        ++i;
    }

    q = q.arg("PROJECT_ID = :PROJECT_ID");
    q += orderBy.isEmpty() ? ";" : (" ORDER BY " + orderBy + ";");
    query.prepare(q);

    i = where.constBegin();
    while (i != where.constEnd()) {
        query.bindValue(":" + i.key(), i.value());
        ++i;
    }

    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareSelect(const QString &where, const QString &orderBy) const
{
    if (where.isEmpty())
        return prepareSelectAll(orderBy);

    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE %2").arg(table).arg(where + " and PROJECT_ID = :PROJECT_ID")
                  + (orderBy.isEmpty() ? ";" : (" ORDER BY " + orderBy + ";")));

    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareDelete(const QMap<QString, QVariant> &where) const
{
    if (where.isEmpty())
        return prepareDeleteAll();

    QSqlQuery query;
    QString q = QString("DELETE FROM %1 WHERE %2").arg(table);

    QMap<QString, QVariant>::const_iterator i = where.constBegin();
    while (i != where.constEnd()) {
        q = q.arg(i.key() + " = :" + i.key() + " and %2 ");
        ++i;
    }

    query.prepare(q.arg("PROJECT_ID = :PROJECT_ID;"));

    i = where.constBegin();
    while (i != where.constEnd()) {
        query.bindValue(":" + i.key(), i.value());
        ++i;
    }

    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareDelete(const QString &where) const
{
    if (where.isEmpty())
        return prepareDeleteAll();

    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE %2").arg(table).arg(where + " and PROJECT_ID = :PROJECT_ID;"));
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

QSqlQuery BaseDAO::prepareDeleteAll() const
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE PROJECT_ID = :PROJECT_ID;").arg(table));
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    return query;
}

void BaseDAO::removeAll()
{
    openDb();
    QSqlQuery query = prepareDeleteAll();
    execQuery(query);
    myDb.close();
}

void BaseDAO::persist(const QMap<QString, QVariant> &values)
{
    openDb();
    QSqlQuery query = prepareInsert(values);
    execQuery(query);
    myDb.close();
}

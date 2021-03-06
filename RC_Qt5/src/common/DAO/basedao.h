#ifndef BASEDAO_H
#define BASEDAO_H

#include <QtCore>
#include <QtSql>

class BaseDAO
{
private:
    QString table;

protected:
    QSqlDatabase myDb;

public:
    BaseDAO(const QSqlDatabase &db, const QString &table);

    void openDb();
    void execQuery(QSqlQuery &query) const;
    QSqlQuery prepareInsert(QMap<QString, QVariant> values) const;
    QSqlQuery prepareSelectAll(const QString &orderBy = "") const;
    QSqlQuery prepareSelect(const QString &where = "", const QString &orderBy = "") const;
    QSqlQuery prepareSelect(const QMap<QString, QVariant> &where = QMap<QString, QVariant>(),
                            const QString &orderBy = "") const;

    QSqlQuery prepareDeleteAll() const;
    QSqlQuery prepareDelete(const QString &where = "") const;
    QSqlQuery prepareDelete(const QMap<QString, QVariant> &where = QMap<QString, QVariant>()) const;

    void persist(const QMap<QString, QVariant> &values);
    void removeAll();
};

#endif // BASEDAO_H

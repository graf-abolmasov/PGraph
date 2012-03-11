#ifndef PREDICATEDAO_H
#define PREDICATEDAO_H

#include "../../src/common/DAO/basedao.h"

class Predicate;
class Variable;
class PredicateDAO : private BaseDAO
{
private:
    void persist(const Predicate &predicate);
    void removeAll();

public:
    PredicateDAO(const QSqlDatabase &db);
    void persistList(const QList<Predicate> &predicateList);
    void persistList(const QList<const Predicate *> &predicateList);
    QList<Predicate> findAll();
    QStringList findUsage(const QString &name);
};

#endif // PREDICATEDAO_H

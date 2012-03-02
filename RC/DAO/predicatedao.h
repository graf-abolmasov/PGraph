#ifndef PREDICATEDAO_H
#define PREDICATEDAO_H

#include "DAO/basedao.h"

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
};

#endif // PREDICATEDAO_H

#ifndef VARIABLEDAO_H
#define VARIABLEDAO_H

#include "DAO/basedao.h"

class Variable;
class VariableDAO : private BaseDAO
{
private:
    void persist(const Variable &variable);
public:
    VariableDAO(const QSqlDatabase &db);
    void persistList(const QList<Variable> &variableList);
    void persistList(const QList<const Variable *> &variableList);
    QList<Variable> findAll();
};

#endif // VARIABLEDAO_H

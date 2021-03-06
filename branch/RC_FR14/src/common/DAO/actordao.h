#ifndef ACTORDAO_H
#define ACTORDAO_H

#include "../../src/common/DAO/basedao.h"

class Actor;
class Graph;
class Variable;
class ActorDAO : private BaseDAO
{
private:
    void persist(const Actor &actor);
    void removeAll();

public:
    ActorDAO(const QSqlDatabase &db);
    void persistList(const QList<Actor> &actorList);
    void persistList(const QList<const Actor *> &actorList);
    void saveIcon(const QString &actorName, const QPixmap &image);
    QList<Actor> findAll();

    QStringList findUsage(const QString &name);
};

#endif // ACTORDAO_H

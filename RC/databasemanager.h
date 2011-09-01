#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>

#include "graph.h"
#include "compi.h"

class DataBaseManager
{
private:
    QSqlDatabase db;
    int myProjectId;

    QList<const Graph *>      myGraphList;
    QList<const Actor *>      myActorList;
    QList<const DataType *>   myDataTypeList;
    QList<const Variable *>   myVariableList;
    QList<const Predicate *>  myPredicateList;
    QList<const BaseModule *> myBaseModuleList;


    void saveVariableListDB(const QList<Variable> &varList) throw (QString);
    QList<Variable> getVariableListDB() throw (QString);

    void saveDataTypeListDB(const QList<DataType> &typeList) throw (QString);
    QList<DataType> getDataTypeListDB() throw (QString);

    void saveActorListDB(const QList<Actor> &actorList) throw (QString);
    QList<Actor> getActorListDB() throw (QString);

    void savePredicateListDB(const QList<Predicate> &predList) throw (QString);
    QList<Predicate> getPredicateListDB() throw (QString);

    QList<BaseModule> getBaseModuleListDB() throw (QString);
    QList<Graph> getGraphListDB() throw (QString);
public:
    DataBaseManager();
    ~DataBaseManager();

    void setProjectId(int pid)
        { myProjectId = pid; }
    int getProjectId()
        { return myProjectId; }
    QString getProjectName()
        { return QString::number(myProjectId); }

    const Actor *getActor(const QString &name) const;
    const Graph *getGraph(const QString &name) const;
    const DataType *getDataType(const QString &type) const;
    const Variable *getVariable(const QString &name) const;
    const Predicate *getPredicate(const QString &name) const;
    const BaseModule *getBaseModule(const QString &name) const;

    QList<const DataType *> getDataTypeList() const;
    void setDatatypeList(const QList<const DataType *> &list);
    QList<const Variable *> getVariableList() const;
    void setVariableList(const QList<const Variable *> &list);
    QList<const Actor *> getActorList() const;
    void setActorList(const QList<const Actor *> &list);
    QList<const Predicate *> getPredicateList() const;
    void setPredicateList(const QList<const Predicate *> &list);
    QList<const BaseModule *> getBaseModuleList() const;
    QList<const Graph *> getGraphList();

    Graph getGraphDB(const QString &namepr) throw (QString);
    void saveGraphDB(const Graph &graph) throw (QString);
    void updateGraphDB(const Graph &graph) throw (QString);
    void openProjectDB(int projectId);
    void saveStructDB(const Graph &graph) throw (QString);
    void registerModuleDB(const BaseModule *baseModule) throw (QString);

    QSqlError lastError();

};

#endif // DATABASEMANAGER_H

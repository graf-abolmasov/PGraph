#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>

#include "../../src/common/VO/graphvo.h"
#include "../../src/compiler/graphcompiler.h"


class DataBaseManager
{
private:
    QSqlDatabase db;
    int myProjectId;
    QString myProjectName;

    QList<const Graph *>      myGraphList;
    QList<const Actor *>      myActorList;
    QList<const DataType *>   myDataTypeList;
    QList<const Variable *>   myVariableList;
    QList<const Predicate *>  myPredicateList;
    QList<const BaseModule *> myBaseModuleList;

    QList<BaseModule> getBaseModuleListDB();
    QList<Graph> getGraphListDB();

    void openDB();
    void execQuery(QSqlQuery &query);
public:
    DataBaseManager();
    ~DataBaseManager();

    void setProjectId(int pid)
        { myProjectId = pid; }
    int getProjectId()
        { return myProjectId; }
    QString getProjectName()
        { return myProjectName; }

    QStringList getOtherFilesDB();

    const Actor *getActor(const QString &name) const;
    const Graph *getGraph(const QString &name) const;
    const DataType *getDataType(const QString &type) const;
    const Variable *getVariable(const QString &name) const;
    const Predicate *getPredicate(const QString &name) const;
    const BaseModule *getBaseModule(const QString &name) const;

    QStringList findActorUsage(const QString &name);
    QStringList findPredicateUsage(const QString &name);
    QStringList findDataTypeUsage(const QString &name);

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

    Graph getGraphDB(const QString &namepr);
    void saveGraphDB(const Graph &graph);
    void updateGraphDB(const Graph &graph);

    void cloneProjectDB(int srcProjectId, QString projectName, QString author, QString description);
    QList<Project> getProjectListDB();
    void openProjectDB(int projectId);
    void removeProjectDB(int projectId);
    int createProjectDB(QString projectName, QString , QString );

    void saveStructDB(const Graph &graph);

    void registerModuleDB(const BaseModule *baseModule);

    void saveActorPictute(const QString &actorName, const QPixmap &image);

    QSqlError lastError();

};

#endif // DATABASEMANAGER_H

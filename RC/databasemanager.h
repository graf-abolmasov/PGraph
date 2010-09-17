#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlTableModel>
#include <QString>
#include "graph.h"
#include "qdiagramscene.h"
#include "datatype.h"
#include "variable.h"
#include "actor.h"
#include "predicate.h"
#include "basemodule.h"
#include <QList>

class DataBaseManager
{
private:
    QSqlDatabase db;
    QList<Variable*> varListProxy;
    QList<Actor*> actorListProxy;
    QList<Predicate*> predListProxy;
    int myProjectId;
public:
    DataBaseManager();
    ~DataBaseManager();

    void setProjectId(int pid)
        { myProjectId = pid; }
    int projectId()
        { return myProjectId; }

    bool getGraphList(QList<Graph* > &graphList);
    bool getGraph(QString extName, Graph &graph);
    bool saveGraph(Graph* graph);
    bool updateGraph(Graph* graph);

    bool saveVariableList(QList<Variable*>& varList);
    bool getVariableList(QList<Variable*>& varList);

    bool saveDataTypeList(QList<DataType*>& typeList);
    bool getDataTypeList(QList<DataType*>& typeList);

    bool saveActorList(QList<Actor*>& actorList);
    bool getActorList(QList<Actor*>& actorList);
    Actor* getActor(QString namepr);

    bool savePredicateList(QList<Predicate*>& predList);
    bool getPredicateList(QList<Predicate*>& predList);
    Predicate* getPredicate(QString namepr);

    bool registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList);
    bool getRegisteredModules(QList<BaseModule*> &moduleList);

    bool saveStruct(Graph *graph);

    QSqlError lastError();
};

#endif // DATABASEMANAGER_H

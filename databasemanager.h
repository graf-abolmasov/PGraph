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

    void setProjectId(int pid)
        { myProjectId = pid; }
    int projectId()
        { return myProjectId; }

    int getGraphList(QList<Graph* > &graphList);
    int getGraph(QString extName, Graph &graph);
    bool saveGraph(Graph* graph);
    bool updateGraph(Graph* graph);

    bool saveVariableList(QList<Variable*>& varList);
    int getVariableList(QList<Variable*>& varList);

    int saveDataTypeList(QList<DataType*>& typeList);
    int getDataTypeList(QList<DataType*>& typeList);

    int saveActorList(QList<Actor*>& actorList);
    int getActorList(QList<Actor*>& actorList);
    Actor* getActor(QString namepr);

    int savePredicateList(QList<Predicate*>& predList);
    int getPredicateList(QList<Predicate*>& predList);
    Predicate* getPredicate(QString namepr);

    int registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList);
    int getRegisteredModules(QList<BaseModule*> &moduleList);

    int saveStruct(Graph *graph);
};

extern DataBaseManager* globalDBManager;

#endif // DATABASEMANAGER_H

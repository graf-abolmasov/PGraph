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
#include "qgraph.h"
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
    int myProgectId;
public:
    DataBaseManager();

    void setProjectId(int pid)
        { myProgectId = pid; }
    int projectId()
        { return myProgectId; }

    int getGraphList(QList<Graph* > &graphList);
    int getGraph(QString extName, Graph &graph);
    int saveGraph(Graph* graph);
    int updateGraph(Graph* graph);

    int saveVariableList(QList<Variable*>& varList);
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

    int getLastInsertID();
};

extern DataBaseManager* globalDBManager;

#endif // DATABASEMANAGER_H

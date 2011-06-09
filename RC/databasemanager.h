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
#include "shab.h"
#include "compi.h"
#include <QList>

class DataBaseManager
{
private:
    QSqlDatabase db;
    QList<Variable*> varListProxy;
    QList<Actor*> actorListProxy;
    QList<Predicate*> predListProxy;
    //QList<Shab*> myShabList;
    int myProjectId;
public:
    DataBaseManager();
    ~DataBaseManager();

    void setProjectId(int pid)
        { myProjectId = pid; }
    int getProjectId()
        { return myProjectId; }

    bool getGraphList(QList<Graph* > &graphList);
    bool getGraph(Graph &graph);
    bool saveGraph(Graph* graph);
    bool updateGraph(Graph* graph);

    bool saveVariableList(QList<Variable*> &varList);
    bool getVariableList(QList<Variable*> &varList);

    bool saveDataTypeList(QList<DataType*> &typeList);
    bool getDataTypeList(QList<DataType*> &typeList);
    DataType *getDataType(const QString &value);
    //QList<DataType> getDataTypeList();

    bool saveActorList(QList<Actor*> &actorList);
    bool getActorList(QList<Actor*> &actorList);
    Actor* getActor(QString namepr);

    bool savePredicateList(QList<Predicate*>& predList);
    bool getPredicateList(QList<Predicate*>& predList);
    Predicate* getPredicate(QString namepr);

    bool registerModule(const QString &uniqName, const QString &fileName, const QString &comment, QStringList &paramList);
    bool getRegisteredModules(QList<BaseModule*> &moduleList);

    bool saveStruct(Graph *graph);

    bool Compi_get_GSP_Shab_List();
    int Compi_count_MaxGH(QString myGraphName, int* MaxGH);
    int Compi_count_MaxLT(QString myGraphName, int* MaxLT);
    int Compi_count_MaxLP(QString myGraphName, int* MaxLP);
    int Compi_get_root_top(QString myGraphName, int* root_top);
    int Compi_fill_Graph_struct(QString myGraphName, int MaxGH, COMPHs *Graph);
    int Compi_fill_ListT_struct(QString myGraphName, int MaxLT, COMPTOPs *ListTop);
    int Compi_fill_ListP_struct(QString myGraphName, int MaxLP, COMPREs *ListP);

    QSqlError lastError();

    QList<Shab*> myShabList;
};

#endif // DATABASEMANAGER_H

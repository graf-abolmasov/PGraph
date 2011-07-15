#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>

#include "graph.h"
#include "compi.h"
#include "shab.h"

class DataBaseManager
{
private:
    QSqlDatabase db;
    int myProjectId;

    QList<Graph *>      myGraphList;
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
public:
    DataBaseManager();
    ~DataBaseManager();

    void setProjectId(int pid)
        { myProjectId = pid; }
    int getProjectId()
        { return myProjectId; }

    const Actor *getActor(const QString &name) const;
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
    QList<Graph *> getGraphList();

    void openProjectDB(int projectId);
    Graph getGraphDB(const QString &namepr) throw (QString);
    QList<Graph> getGraphListDB() throw (QString);
    void saveStruct(const Graph &graph) throw (QString);
    void saveGraphDB(const Graph &graph) throw (QString);
    void updateGraphDB(const Graph &graph) throw (QString);
    void registerModuleDB(const QString &uniqName, const QString &fileName, const QString &comment, const QStringList &paramList) throw (QString);

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

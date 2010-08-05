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
#include <QList>

class DataBaseManager
{
private:
    QSqlDatabase db;
    QList<Variable*> varListProxy;
    QList<Actor*> actorListProxy;
    QList<Predicate*> predListProxy;
public:
    DataBaseManager();
    void getGraph(QString projectName, QDiagramScene* scene, QMenu* arcMenu, QMenu* topMenu, QMenu* syncArcMenu);
    void saveGraph(QString projectName, QString extName, QGraph* graph);

    int saveVariableList(QList<Variable*>& varList);
    int getVariableList(QList<Variable*>& varList);

    int saveDataTypeList(QList<DataType*>& typeList);
    int getDataTypeList(QList<DataType*>& typeList);

    int saveActorList(QList<Actor*>& actorList);
    int getActorList(QList<Actor*>& actorList);

    int savePredicateList(QList<Predicate*>& predList);
    int getPredicateList(QList<Predicate*>& predList);

    int registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList);
    int getRegisteredModules(QStringList &moduleList);

    int getLastInsertID();
};

extern DataBaseManager* globalDBManager;

#endif // DATABASEMANAGER_H

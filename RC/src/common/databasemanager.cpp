#include "../../src/common/databasemanager.h"
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStringList>
#include <QString>
#include <QList>
#include <QtGui>
#include <QSqlDriver>

#include "../../src/common/commonutils.h"
#include "../../src/common/logger.h"
#include "../../src/common/globalvariables.h"

#include "../../src/common/VO/datatype.h"
#include "../../src/common/VO/variable.h"
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/common/VO/basemodule.h"
#include "../../src/common/DAO/datatypedao.h"
#include "../../src/common/DAO/variabledao.h"
#include "../../src/common/DAO/actordao.h"
#include "../../src/common/DAO/predicatedao.h"
#include "../../src/common/DAO/graphdao.h"

DataBaseManager* globalDBManager;

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    if (db.lastError().type() != QSqlError::NoError)
        globalLogger->log(db.lastError().driverText(), Logger::Critical);
    QSettings myDBSettings("graph.ini", QSettings::IniFormat);
    db.setHostName(myDBSettings.value("DB/hostname", "localhost").toString());
    db.setPort(myDBSettings.value("DB/port", 3306).toInt());
    db.setDatabaseName(myDBSettings.value("DB/dbname", "graph4").toString());
    db.setUserName(myDBSettings.value("DB/login", "root").toString());
    db.setPassword(myDBSettings.value("DB/password", "615243").toString());
    db.open();
    db.close();
    myProjectId = -1;
}

Graph DataBaseManager::getGraphDB(const QString &name) throw (QString)
{
    return GraphDAO(db).findByName(name);
}

QList<const Graph *> DataBaseManager::getGraphList()
{
    return myGraphList;
}

QList<const DataType *> DataBaseManager::getDataTypeList() const
{
    return myDataTypeList;
}

void DataBaseManager::setDatatypeList(const QList<const DataType *> &list)
{
    myDataTypeList = list;
    DataTypeDAO(db).persistList(myDataTypeList);
}

QList<const Variable *> DataBaseManager::getVariableList() const
{
    return myVariableList;
}

void DataBaseManager::setVariableList(const QList<const Variable *> &list)
{
    myVariableList = list;
    VariableDAO(db).persistList(myVariableList);
}

void DataBaseManager::saveGraph(const Graph &graph) throw (QString)
{
    GraphDAO(db).persist(graph);

    Graph *newGraph = const_cast<Graph *>(getGraph(graph.name));
    if (newGraph == NULL) {
        newGraph = new Graph(graph.name, graph.extName, graph.topList, graph.arcList, graph.commentList, graph.syncArcList);
    } else {
        newGraph->name = graph.name;
        newGraph->extName = graph.extName;
        newGraph->topList = graph.topList;
        newGraph->arcList = graph.arcList;
        newGraph->commentList = graph.commentList;
        newGraph->syncArcList = graph.syncArcList;
    }

    if (!myGraphList.contains(newGraph))
        myGraphList.append(newGraph);
}

/*!
  Удалает и сохраняет заново агрегат
*/
void DataBaseManager::updateGraphDB(const Graph &graph) throw (QString)
{
    GraphDAO(db).remove(graph);
    return saveGraph(graph);
}

QList<Graph> DataBaseManager::getGraphListDB() throw (QString)
{
    return GraphDAO(db).findAll();
}

const DataType *DataBaseManager::getDataType(const QString &name) const
{
    const DataType *result = NULL;
    foreach (const DataType *dataType, myDataTypeList) {
        if (dataType->name == name) {
            result = dataType;
            break;
        }
    }
    return result;
}

const Variable *DataBaseManager::getVariable(const QString &name) const
{
    const Variable *result = NULL;
    foreach (const Variable *variable, myVariableList) {
        if (variable->name == name) {
            result = variable;
            break;
        }
    }
    return result;
}

QList<const Actor *> DataBaseManager::getActorList() const
{
    return myActorList;
}

void DataBaseManager::setActorList(const QList<const Actor *> &list)
{
    myActorList = list;
    ActorDAO(db).persistList(myActorList);
}

QList<const Predicate *> DataBaseManager::getPredicateList() const
{
    return myPredicateList;
}

void DataBaseManager::setPredicateList(const QList<const Predicate *> &list)
{
    myPredicateList = list;
    PredicateDAO(db).persistList(myPredicateList);
}

QList<const BaseModule *> DataBaseManager::getBaseModuleList() const
{
    return myBaseModuleList;
}

const Actor *DataBaseManager::getActor(const QString &name) const
{
    const Actor *result = NULL;
    foreach (const Actor *actor, myActorList) {
        if (actor->name == name) {
            result = actor;
            break;
        }
    }
    if (result)
        return result;
    return getGraph(name);
}

const Graph *DataBaseManager::getGraph(const QString &name) const
{
    const Graph *result = NULL;
    foreach (const Graph *Graph, myGraphList) {
        if (Graph->name == name) {
            result = Graph;
            break;
        }
    }
    return result;
}


const Predicate *DataBaseManager::getPredicate(const QString &name) const
{
    const Predicate *result = NULL;
    foreach (const Predicate *predicate, myPredicateList) {
        if (predicate->name == name) {
            result = predicate;
            break;
        }
    }
    return result;
}

void DataBaseManager::registerModuleDB(const BaseModule *baseModule) throw (QString)
{
    openDB();
    QSqlQuery query;
    query.prepare("INSERT INTO bazmod (PROJECT_ID, PROTOTIP, NAMEPR, COMMENT)"
                  "VALUES (:PROJECT_ID, :PROTOTIP, :NAMEPR, :COMMENT);");
    query.bindValue(":NAMEPR",   baseModule->name);
    query.bindValue(":COMMENT",  baseModule->comment);
    query.bindValue(":PROTOTIP", baseModule->uniqName);
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось сохранить базовый модуль.\n") + db.lastError().text();
    }

    for (int i = 0; i < baseModule->parameterList.count(); i++){
        query.clear();
        query.prepare("INSERT INTO databaz (PROJECT_ID, PROTOTIP, DATA, TYPE, MODE, NEV, COMMENT)"
                      "VALUES (:PROJECT_ID, :PROTOTIP, :DATA, :TYPE, :MODE, :NEV, :COMMENT);");
        BaseModuleParameter parameter = baseModule->parameterList[i];
        query.bindValue(":NEV",         i);
        query.bindValue(":DATA",        parameter.name);
        query.bindValue(":TYPE",        parameter.type);
        query.bindValue(":PROTOTIP",    baseModule->uniqName);
        query.bindValue(":PROJECT_ID",  myProjectId);
        QString vaMode;
        if (parameter.accessMode == QObject::tr("Исходный"))
            vaMode = "I";
        else if (parameter.accessMode == QObject::tr("Модифицируемый"))
            vaMode = "M";
        else if (parameter.accessMode == QObject::tr("Вычисляемый"))
            vaMode = "R";
        query.bindValue(":MODE",        vaMode);
        query.bindValue(":COMMENT",     parameter.comment);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить параметры базового модуля.\n") + db.lastError().text();
        }
    }
    myBaseModuleList.append(baseModule);
    db.close();
}

QList<BaseModule> DataBaseManager::getBaseModuleListDB() throw (QString)
{
    openDB();
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT PROTOTIP, NAMEPR, COMMENT FROM bazmod WHERE PROJECT_ID = :PROJECT_ID;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось список базовых модулей.\n") + db.lastError().text();
    }

    QList<BaseModule> result;
    while (query1.next()){
        query2.prepare("SELECT TYPE, DATA, MODE, COMMENT FROM databaz WHERE PROTOTIP = :PROTOTIP AND PROJECT_ID = :PROJECT_ID order by NEV;");
        query2.bindValue(":PROTOTIP", query1.value(0).toString());
        query2.bindValue(":PROJECT_ID", myProjectId);
        if (!query2.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось получить параметры модулей.\n") + db.lastError().text();
        }
        globalLogger->log(query2.executedQuery().toUtf8());
        QList<BaseModuleParameter> parameterList;
        while (query2.next()){
            QString vaMode;
            if (query2.value(2).toString() == "I")
                vaMode = QObject::tr("Исходный");
            else if (query2.value(2).toString() == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (query2.value(2).toString() == "R")
                vaMode = QObject::tr("Вычисляемый");
            parameterList.append(BaseModuleParameter(vaMode,  query2.value(1).toString(), query2.value(0).toString(), query2.value(3).toString()));
        }
        query2.clear();
        result.append(BaseModule(query1.value(1).toString(), query1.value(0).toString(), query1.value(2).toString(), parameterList));
    }
    db.close();
    return result;
}

const BaseModule *DataBaseManager::getBaseModule(const QString &name) const
{
    const BaseModule *result = NULL;
    foreach (const BaseModule *baseModule, myBaseModuleList) {
        if (baseModule->name == name) {
            result = baseModule;
            break;
        }
    }
    return result;
}

void DataBaseManager::saveStructDB(const Graph &graph) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    QList<const Predicate *> predicateList;
    int i = 0;
    query.prepare("DELETE FROM graph WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR",      graph.name);
    query.bindValue(":PROJECT_ID",  myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить структуру.\n") + db.lastError().text();
    }
    query.prepare("DELETE FROM graphtop WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR",      graph.name);
    query.bindValue(":PROJECT_ID",  myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить структуру.\n") + db.lastError().text();
    }
    query.prepare("DELETE FROM graphpre WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR",      graph.name);
    query.bindValue(":PROJECT_ID",  myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить структуру.\n") + db.lastError().text();
    }

    foreach (Arc arc, graph.arcList){
        if (!predicateList.contains(arc.predicate)){
            predicateList.append(arc.predicate);
            query.prepare("INSERT INTO graphpre (PROJECT_ID, NAMEPR, NPRED, NAME)"
                          "VALUES (:PROJECT_ID, :NAMEPR, :NPRED, :NAME)");
            query.bindValue(":PROJECT_ID", myProjectId);
            query.bindValue(":NAMEPR", graph.name);
            query.bindValue(":NPRED", i);
            query.bindValue(":NAME", arc.predicate);
            if (!query.exec()) {
                globalLogger->log(db.lastError().text(), Logger::Critical);
                db.close();
                throw QObject::tr("Не удалось записать структуру.\n") + db.lastError().text();
            }
            i++;
        }
        query.prepare("INSERT INTO graph (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", graph.name);
        query.bindValue(":NFROM", arc.startTop);
        query.bindValue(":NTO", arc.endTop);
        query.bindValue(":NPRED", predicateList.indexOf(arc.predicate));
        query.bindValue(":PRIOR", arc.priority);
        int arcType;
        switch (arc.type){
        case Arc::ParallelArc:
            arcType = 2;
            break;
        case Arc::TerminateArc:
            arcType = 3;
            break;
        case Arc::SerialArc:
        default:
            arcType = 1;
        }
        query.bindValue(":ARCTYPE", arcType);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось записать структуру.\n") + db.lastError().text();
        }
    }

    int rootTop = -1;
    foreach (Top top, graph.topList){
        query.prepare("INSERT INTO graphtop (PROJECT_ID, NAMEPR, NTOP, NAME)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NTOP, :NAME);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", graph.name);
        query.bindValue(":NTOP", top.number);
        query.bindValue(":NAME", top.actor);
        if (top.isRoot)
            rootTop = top.number;
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось записать структуру.\n") + db.lastError().text();
        }
    }
    query.prepare("INSERT INTO graph (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":NFROM",rootTop);
    query.bindValue(":NTO", 0);
    query.bindValue(":NPRED", 0);
    query.bindValue(":PRIOR", 0);
    query.bindValue(":ARCTYPE", 0);
    execQuery(query);

    db.close();
}

QSqlError DataBaseManager::lastError()
{
    return db.lastError();
}

DataBaseManager::~DataBaseManager()
{
    if (db.isOpen())
        db.close();
}

void DataBaseManager::openProjectDB(int projectId)
{
    if (myProjectId != -1)
        return;
    myProjectId = projectId;
    openDB();
    QSqlQuery query;
    query.prepare("SELECT * from project WHERE PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    execQuery(query);
    query.next();
    myProjectName = query.value(1).toString();
    db.close();

    const QList<DataType> dbDataTypeList = DataTypeDAO(db).findAll();
    foreach (DataType dataType, dbDataTypeList)
        myDataTypeList.append(new DataType(dataType));
    QList<Variable> dbVariableList = VariableDAO(db).findAll();
    foreach (Variable variable, dbVariableList)
        myVariableList.append(new Variable(variable));
    QList<BaseModule> dbModuleList = getBaseModuleListDB();
    foreach (BaseModule baseModule, dbModuleList)
        myBaseModuleList.append(new BaseModule(baseModule));
    QList<Actor> dbActorList = ActorDAO(db).findAll();
    foreach (Actor actor, dbActorList)
        myActorList.append(new Actor(actor));
    QList<Predicate> dbPredicateList = PredicateDAO(db).findAll();
    foreach (Predicate predicate, dbPredicateList)
        myPredicateList.append(new Predicate(predicate));
    QList<Graph> dbGraphList = getGraphListDB();
    foreach (Graph graph, dbGraphList)
        myGraphList.append(new Graph(graph));
}

void DataBaseManager::cloneProjectDB(int srcProjectId, QString projectName, QString author, QString description)
{
    openProjectDB(srcProjectId);
    int dstProjectId = createProjectDB(projectName, author, description);
    myProjectId = dstProjectId;
    DataTypeDAO(db).persistList(myDataTypeList);
    VariableDAO(db).persistList(myVariableList);
    ActorDAO(db).persistList(myActorList);
    PredicateDAO(db).persistList(myPredicateList);
    QList<const BaseModule *> baseModules = getBaseModuleList();
    foreach (const BaseModule *baseModule, baseModules)
        registerModuleDB(baseModule);
    QList<const Graph*> graphs = getGraphList();
    foreach (const Graph *graph, graphs) {
        myProjectId = srcProjectId;
        const Graph g = getGraphDB(graph->name);
        myProjectId = dstProjectId;
        saveGraph(g);
    }
}

int DataBaseManager::createProjectDB(QString projectName, QString, QString)
{
    openDB();
    QSqlQuery query;
    query.prepare("INSERT INTO project (PROJECT_NAME) VALUES (PROJECT_NAME=:PROJECT_NAME)");
    query.bindValue(":PROJECT_NAME", projectName);
    execQuery(query);
    int result = query.lastInsertId().toInt();
    db.close();
    return result;
}

void DataBaseManager::removeProjectDB(int projectId)
{
    Q_ASSERT(projectId != -1);

    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    query.prepare("DELETE form project WHERE PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", projectId);
    execQuery(query);
    db.close();
}

void DataBaseManager::saveActorPictute(const QString &actorName, const QPixmap &image)
{
    ActorDAO(db).saveIcon(actorName, image);
}

QList<Project> DataBaseManager::getProjectListDB()
{
    openDB();
    QSqlQuery query;
    query.prepare("SELECT * FROM project");
    execQuery(query);
    QList<Project> result;
    while (query.next())
        result.append(Project(query.value(0).toInt(), query.value(1).toString()));
    db.close();
    return result;
}

void DataBaseManager::openDB() {
    if (db.isOpen())
        return;
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
}

void DataBaseManager::execQuery(QSqlQuery &query) {
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("SQL query error.\n") + db.lastError().text();
    }
}

QStringList DataBaseManager::findActorUsage(const QString &name)
{
    return ActorDAO(db).findUsage(name);
}


QStringList DataBaseManager::findDataTypeUsage(const QString &name)
{
    return DataTypeDAO(db).findUsage(name);
}

QStringList DataBaseManager::findPredicateUsage(const QString &name)
{
    return PredicateDAO(db).findUsage(name);
}

QStringList DataBaseManager::getOtherFilesDB()
{
    BaseDAO dao(db, "othfls");
    dao.openDb();
    QSqlQuery q = dao.prepareSelectAll();
    dao.execQuery(q);
    QStringList result;
    while (q.next()) {
        QSqlRecord r = q.record();
        result.append(r.value("name").toString());
    }
    db.close();
    return result;
}

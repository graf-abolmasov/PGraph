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
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    //получаем список вершин
    query.prepare("SELECT toppic.*, toppic.SizeX AS procCount FROM toppic "
                  "WHERE toppic.NAMEPR=:NAMEPR AND toppic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список вершин.\n") + db.lastError().text();
    }
    QList<Top> topList;
    while (query.next()){
        QSqlRecord record = query.record();
        const Actor *actor = getActor(record.value("Actor").toString());
        if (record.value("Type").toString() == "T") {
            topList.append(Top(record.value("X").toFloat(),
                    record.value("Y").toFloat(),
                    record.value("SizeX").toFloat(),
                    record.value("SizeY").toFloat(),
                    record.value("ntop").toInt(),
                    -1,
                    record.value("isRoot").toBool(),
                    actor,
                    Top::NormalTop));
        } else if (record.value("Type").toString() == "M") {
            topList.append(Top(record.value("X").toFloat(),
                    record.value("Y").toFloat(),
                    -1,
                    -1,
                    record.value("ntop").toInt(),
                    record.value("procCount").toInt(),
                    false,
                    actor,
                    Top::MultiProcTop));
        }
    }

    //получаем список дуг управления
    query.prepare("SELECT arcpic.* FROM arcpic "
                  "WHERE arcpic.NAMEPR=:NAMEPR AND arcpic.PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список дуг.\n") + db.lastError().text();
    }
    QList<Arc> arcList;
    while (query.next()){
        QSqlRecord record = query.record();
        Arc::ArcType arcType;
        if (record.value("Type").toString() == QObject::tr("S"))
            arcType = Arc::SerialArc;
        else if (record.value("Type").toString() == QObject::tr("P"))
            arcType = Arc::ParallelArc;
        else if (record.value("Type").toString() == QObject::tr("T"))
            arcType = Arc::TerminateArc;
        QStringList lines = record.value("Nodes").toString().split(";;");
        arcList.append(Arc(arcType,
                record.value("Priority").toInt(),
                record.value("FromTop").toInt(),
                record.value("ToTop").toInt(),
                getPredicate(record.value("Predicate").toString()),
                lines));
    }

    //получаем список комментариев
    query.prepare("SELECT commentpic.* FROM commentpic "
                  "WHERE commentpic.NAMEPR=:NAMEPR AND commentpic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список коментариев.\n") + db.lastError().text();
    }
    QList<Comment> commentList;
    while (query.next()) {
        QSqlRecord record = query.record();
        Comment comment(record.value("X").toFloat(),
                        record.value("Y").toFloat(),
                        record.value("TEXT").toString(),
                        QFont(record.value("FONT").toString()));
        commentList.append(comment);
    }

    //Получаем полное название агрегата
    query.prepare("SELECT actor.NAMEPR, actor.EXTNAME FROM actor "
                  "WHERE actor.NAMEPR=:NAMEPR AND actor.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->log(query.lastError().driverText(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить полное название агрегата.\n") + db.lastError().text();
    }
    query.next();
    Graph result(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, QList<SyncArc>());

    db.close();
    return result;
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

void DataBaseManager::saveGraphDB(const Graph &graph) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, NULL)");

    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph.extName);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось сохранить агрегат.\n") + db.lastError().text();
    }

    foreach (Top top, graph.topList){
        query.prepare("INSERT INTO toppic (PROJECT_ID, NAMEPR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Type) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, :Type)");

        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":NAMEPR",      graph.name);
        query.bindValue(":X",           top.x);
        query.bindValue(":Y",           top.y);
        query.bindValue(":SizeX",       top.type == Top::MultiProcTop ? top.procCount : top.sizeX);
        query.bindValue(":SizeY",       top.sizeY);
        query.bindValue(":ntop",        top.number);
        query.bindValue(":isRoot",      top.isRoot ? 1 : 0 );
        query.bindValue(":Actor",       top.actor == NULL ? "" : top.actor->name );
        query.bindValue(":Type",        top.type == Top::MultiProcTop ? "M" : "T");
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить список вершин.\n") + db.lastError().text();
        }
    }

    foreach(Comment comment, graph.commentList) {
        query.prepare("INSERT INTO commentpic (PROJECT_ID, NAMEPR, TEXT, FONT, X, Y) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :TEXT, :FONT, :X, :Y)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph.name);
        query.bindValue(":X",          comment.x);
        query.bindValue(":Y",          comment.y);
        query.bindValue(":TEXT",       comment.text);
        query.bindValue(":FONT",       comment.font.toString());
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить список комментариев.\n") + db.lastError().text();
        }
    }

    foreach (Arc arc, graph.arcList){
        query.prepare("INSERT INTO arcpic (PROJECT_ID, NAMEPR, Nodes, Priority, FromTop, ToTop, Predicate, Type) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :Nodes, :Priority, :FromTop, :ToTop, :Predicate, :Type)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph.name);
        QString arcType;
        switch (arc.type){
        case Arc::SerialArc:
            arcType = "S";
            break;
        case Arc::ParallelArc:
            arcType = "P";
            break;
        case Arc::TerminateArc:
            arcType = "T";
            break;
        }
        query.bindValue(":Type",      arcType);
        query.bindValue(":Nodes",     arc.lines.join(";;"));
        query.bindValue(":Priority",  arc.priority);
        query.bindValue(":FromTop",   arc.startTop);
        query.bindValue(":ToTop",     arc.endTop);
        query.bindValue(":Predicate", arc.predicate == NULL ? "" : arc.predicate->name);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить список дуг.\n") + db.lastError().text();
        }
    }

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
    db.close();
}

/*!
  Удалает и сохраняет заново агрегат
*/
void DataBaseManager::updateGraphDB(const Graph &graph) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND NAMEPR = :NAMEPR;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить агрегат.\n") + db.lastError().text();
    }
    db.close();
    return saveGraphDB(graph);
}

QList<Graph> DataBaseManager::getGraphListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT NAMEPR, EXTNAME, ICON FROM actor WHERE CLASPR = 'g' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список агрегатов.\n") + db.lastError().text();
    }
    QList<Graph> result;
    while(query.next()){
        QList<Top> topList;
        QList<Arc> arcList;
        QList<Comment> commentList;
        QList<SyncArc> syncArcList;
        Graph newGraph(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, syncArcList);
        QPixmap p;
        p.loadFromData(query.value(2).toByteArray(), "PNG");
        newGraph.icon = p;
        result.append(newGraph);
    }
    db.close();
    return result;
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
        case Arc::SerialArc:
            arcType = 1;
            break;
        case Arc::ParallelArc:
            arcType = 2;
            break;
        case Arc::TerminateArc:
            arcType = 3;
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
        saveGraphDB(g);
    }
}

int DataBaseManager::createProjectDB(QString projectName, QString author, QString description)
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
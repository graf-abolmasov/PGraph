#include "databasemanager.h"
#include "qdiagramscene.h"
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQueryModel>
#include <QStringList>
#include <QString>
#include <QList>
#include <QtGui>
#include <QSqlDriver>

#include "commonutils.h"
#include "logger.h"
#include "globalvariables.h"
#include "qarc.h"
#include "qdiagramscene.h"
#include "datatype.h"
#include "variable.h"
#include "actor.h"
#include "predicate.h"
#include "basemodule.h"
#include "qcomment.h"

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
    QList<DataType> dbDataTypeList;
    foreach (const DataType *dataType, myDataTypeList)
        dbDataTypeList.append(DataType(*dataType));
    saveDataTypeListDB(dbDataTypeList);
}

QList<const Variable *> DataBaseManager::getVariableList() const
{
    return myVariableList;
}

void DataBaseManager::setVariableList(const QList<const Variable *> &list)
{
    myVariableList = list;
    QList<Variable> dbVariablelist;
    foreach (const Variable *variable, myVariableList)
        dbVariablelist.append(Variable(*variable));
    saveVariableListDB(dbVariablelist);
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

void DataBaseManager::saveDataTypeListDB(const QList<DataType> &typeList) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM typsys WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список типов.\n") + db.lastError().text();
    }
    foreach (DataType type, typeList) {
        query.prepare("INSERT INTO typsys (PROJECT_ID, TYPE, TYPEDEF)"
                      "VALUES (:PROJECT_ID, :TYPE, :TYPEDEF);");
        query.bindValue(":TYPE",        type.name);
        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":TYPEDEF",     type.typedefStr);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить список типов.\n") + db.lastError().text();
        }
        query.clear();
    }
    db.close();
}

QList<DataType> DataBaseManager::getDataTypeListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT PROJECT_ID, TYPE, TYPEDEF FROM typsys WHERE PROJECT_ID = :PROJECT_ID ORDER BY TYPE;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список типов.\n") + db.lastError().text();
    }
    QList<DataType> result;
    while (query.next())
        result.append(DataType(query.value(1).toString(), query.value(2).toString()));
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



void DataBaseManager::saveVariableListDB(const QList<Variable> &varList) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM data WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список переменных.\n") + db.lastError().text();
    }
    foreach (Variable var, varList) {
        Q_ASSERT(var.type != NULL);
        query.prepare("INSERT INTO data (PROJECT_ID, DATA, TYPE, INIT, COMMENT)"
                      "VALUES (:PROJECT_ID, :DATA, :TYPE, :INIT, :COMMENT);");
        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":DATA",        var.name);
        query.bindValue(":TYPE",        var.type->name);
        query.bindValue(":INIT",        var.initValue);
        query.bindValue(":COMMENT",     var.comment);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить переменную.\n") + db.lastError().text();
        }
        query.clear();
    }
    db.close();
}

QList<Variable> DataBaseManager::getVariableListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT DATA, TYPE, INIT, COMMENT FROM data d WHERE PROJECT_ID = :PROJECT_ID ORDER BY DATA;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список переменных.\n") + db.lastError().text();
    }
    QList<Variable> result;
    while (query.next())
        result.append(Variable(query.value(0).toString(),query.value(2).toString(),query.value(3).toString(), getDataType(query.value(1).toString())));

    db.close();
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
    QList<Actor> dbActorList;
    foreach (const Actor *actor, myActorList)
        dbActorList.append(Actor(*actor));
    saveActorListDB(dbActorList);
}

QList<const Predicate *> DataBaseManager::getPredicateList() const
{
    return myPredicateList;
}

void DataBaseManager::setPredicateList(const QList<const Predicate *> &list)
{
    myPredicateList = list;
    QList<Predicate> dbPredicateList;
    foreach (const Predicate *predicate, myPredicateList)
        dbPredicateList.append(Predicate(*predicate));
    savePredicateListDB(dbPredicateList);
}

QList<const BaseModule *> DataBaseManager::getBaseModuleList() const
{
    return myBaseModuleList;
}

void DataBaseManager::saveActorListDB(const QList<Actor> &actorList) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    query1.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'a'");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список акторов.\n") + db.lastError().text();
    }
    foreach (Actor actor, actorList) {
        query1.clear();
        query1.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, PROTOTIP)"
                       "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :PROTOTIP);");
        query1.bindValue(":NAMEPR",     actor.name);
        query1.bindValue(":CLASPR",     "a");
        query1.bindValue(":EXTNAME",    actor.extName);
        query1.bindValue(":PROTOTIP",   actor.baseModule == NULL ? NULL : actor.baseModule->name);
        query1.bindValue(":PROJECT_ID", myProjectId);
        if (!query1.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить актор.\n") + db.lastError().text();
        }


        QSqlQuery query2;
        QList<const Variable *> actorVariables = actor.variableList;
        for (int j = 0; j < actorVariables.count(); j++){
            query2.clear();
            query2.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query2.bindValue(":NEV",        j);
            query2.bindValue(":DATA",       actorVariables[j]->name);
            query2.bindValue(":NAMEPR",     actor.name);
            query2.bindValue(":PROJECT_ID", myProjectId);
            QString vaMode;
            if (actor.varAccModeList.at(j) == QObject::tr("Исходный"))
                vaMode = "I";
            else if (actor.varAccModeList.at(j) ==  QObject::tr("Модифицируемый"))
                vaMode = "M";
            else if (actor.varAccModeList.at(j) ==  QObject::tr("Вычисляемый"))
                vaMode = "R";

            query2.bindValue(":MODE", vaMode);
            if (!query2.exec()) {
                globalLogger->log(db.lastError().text(), Logger::Critical);
                db.close();
                throw QObject::tr("Не удалось сохранить параметры для актора.\n") + db.lastError().text();
            }
        }
    }
    db.close();
}

QList<Actor> DataBaseManager::getActorListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'a' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список акторов.\n") + db.lastError().text();
    }
    QList<Actor> result;
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        if (!query2.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось получить параметры для актора.\n") + db.lastError().text();
        }
        QList<const Variable *> actorVariableList;
        QStringList actorVarAccList;
        while (query2.next()) {
            actorVariableList.append(getVariable(query2.value(1).toString()));
            QString vaMode;
            if (query2.value(2).toString() == "I")
                vaMode = QObject::tr("Исходный");
            else if (query2.value(2).toString() == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (query2.value(2).toString() == "R")
                vaMode = QObject::tr("Вычисляемый");
            actorVarAccList.append(vaMode);
        }
        QSqlRecord r = query1.record();
        QPixmap p;
        p.loadFromData(r.value("ICON").toByteArray(), "PNG");
        result.append(Actor(query1.value(0).toString(),
                            query1.value(2).toString(),
                            query1.value(6).toString().isEmpty() ? Actor::InlineType : Actor::NormalType,
                            getBaseModule(query1.value(6).toString()),
                            actorVariableList,
                            actorVarAccList,
                            p));
        query2.clear();
    }
    db.close();
    return result;
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

void DataBaseManager::savePredicateListDB(const QList<Predicate> &predList) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'p'");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список предикатов.\n") + db.lastError().text();
    }

    foreach (Predicate predicate, predList) {
        query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP);");
        query.bindValue(":NAMEPR",      predicate.name);
        query.bindValue(":CLASPR",      "p");
        query.bindValue(":EXTNAME",     predicate.extName);
        query.bindValue(":PROTOTIP",    predicate.baseModule == NULL ? NULL : predicate.baseModule->name);
        query.bindValue(":PROJECT_ID",  myProjectId);
        if (!query.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить предикат.\n") + db.lastError().text();
        }

        int j = 0;
        foreach (const Variable *var, predicate.variableList) {
            query.clear();
            query.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                          "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query.bindValue(":NEV",         j++);
            query.bindValue(":DATA",        var->name);
            query.bindValue(":MODE",        "I");
            query.bindValue(":NAMEPR",      predicate.name);
            query.bindValue(":PROJECT_ID",  myProjectId);
            if (!query.exec()) {
                globalLogger->log(db.lastError().text(), Logger::Critical);
                db.close();
                throw QObject::tr("Не удалось получить список параметров для предиката.\n") + db.lastError().text();
            }
        }
    }
    db.close();
}

QList<Predicate> DataBaseManager::getPredicateListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'p' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список предикатов.\n") + db.lastError().text();
    }

    QList<Predicate> result;
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        if (!query2.exec()) {
            globalLogger->log(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось получить список параметров для предиката.\n") + db.lastError().text();
        }
        QList<const Variable *> variableList;
        while (query2.next())
            variableList.append(getVariable(query2.value(1).toString()));
        result.append(Predicate(query1.value(0).toString(),
                                query1.value(2).toString(),
                                query1.value(6).toString().isNull() ? Predicate::InlineType : Predicate::NormalType,
                                getBaseModule(query1.value(6).toString()),
                                variableList));
    }
    db.close();
    return result;
}

void DataBaseManager::registerModuleDB(const BaseModule *baseModule) throw (QString)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
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
        QStringList parameter = baseModule->parameterList[i].split(";;");
        query.bindValue(":NEV",         i);
        query.bindValue(":DATA",        parameter[0]);
        query.bindValue(":TYPE",        parameter[1]);
        query.bindValue(":PROTOTIP",    baseModule->uniqName);
        query.bindValue(":PROJECT_ID",  myProjectId);
        QString vaMode;
        if (parameter.at(2) == QObject::tr("Исходный"))
            vaMode = "I";
        else if (parameter.at(2) == QObject::tr("Модифицируемый"))
            vaMode = "M";
        else if (parameter.at(2) == QObject::tr("Вычисляемый"))
            vaMode = "R";
        query.bindValue(":MODE",        vaMode);
        query.bindValue(":COMMENT",     parameter[3]);
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
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
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
            throw QObject::tr("Не удалось параметры модулей.\n") + db.lastError().text();
        }
        globalLogger->log(query2.executedQuery().toUtf8());
        QStringList parameterList;
        while (query2.next()){
            QString vaMode;
            if (query2.value(2).toString() == "I")
                vaMode = QObject::tr("Исходный");
            else if (query2.value(2).toString() == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (query2.value(2).toString() == "R")
                vaMode = QObject::tr("Вычисляемый");
            parameterList.append(query2.value(0).toString() + ";;" +
                                 query2.value(1).toString() + ";;" +
                                 vaMode + ";;" +
                                 query2.value(3).toString());
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
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось записать структуру.\n") + db.lastError().text();
    }

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

    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    query.prepare("SELECT * from project WHERE PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось открыть проект.\n") + db.lastError().text();
    }
    if (!query.next()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Нет такого проекта.\n") + db.lastError().text();
    }
    myProjectName = query.value(1).toString();
    db.close();

    QList<DataType> dbDataTypeList = getDataTypeListDB();
    foreach (DataType dataType, dbDataTypeList)
        myDataTypeList.append(new DataType(dataType));
    QList<Variable> dbVariableList = getVariableListDB();
    foreach (Variable variable, dbVariableList)
        myVariableList.append(new Variable(variable));
    QList<BaseModule> dbModuleList = getBaseModuleListDB();
    foreach (BaseModule baseModule, dbModuleList)
        myBaseModuleList.append(new BaseModule(baseModule));
    QList<Actor> dbActorList = getActorListDB();
    foreach (Actor actor, dbActorList)
        myActorList.append(new Actor(actor));
    QList<Predicate> dbPredicateList = getPredicateListDB();
    foreach (Predicate predicate, dbPredicateList)
        myPredicateList.append(new Predicate(predicate));
    QList<Graph> dbGraphList = getGraphListDB();
    foreach (Graph graph, dbGraphList)
        myGraphList.append(new Graph(graph));
}

void DataBaseManager::saveActorPictute(const QString &actorName, const QPixmap &image)
{
    if (!db.open()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    buffer.close();

    QSqlQuery query;
    query.prepare("UPDATE actor SET ICON=:ICON WHERE NAMEPR=:NAMEPR AND PROJECT_ID=:PROJECT_ID");
    query.bindValue(":ICON",       ba);
    query.bindValue(":NAMEPR",     actorName);
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->log(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось сохранить актор.\n") + db.lastError().text();
    }
    db.close();
}

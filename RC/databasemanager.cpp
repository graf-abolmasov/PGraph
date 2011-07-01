#include "databasemanager.h"
#include "qdiagramscene.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
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
#include "qcomment.h"

DataBaseManager* globalDBManager;

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    if (db.lastError().type() != QSqlError::NoError)
        globalLogger->writeLog(db.lastError().driverText(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    //получаем список вершин
    query.prepare("SELECT toppic.*, toppic.SizeX AS procCount FROM toppic "
                  "WHERE toppic.NAMEPR=:NAMEPR AND toppic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список вершин.\n") + db.lastError().text();
    }
    QList<Top> topList;
    while (query.next()){
        QSqlRecord record = query.record();
        if (record.value("Type").toString() == "T"){
            Top top(record.value("X").toFloat(),
                    record.value("Y").toFloat(),
                    record.value("SizeX").toFloat(),
                    record.value("SizeY").toFloat(),
                    record.value("ntop").toInt(),
                    -1,
                    record.value("isRoot").toBool(),
                    record.value("Actor").toString(),
                    "T");
            topList.append(top);
        }

        if (record.value("Type").toString() == "M"){
            Top top(record.value("X").toFloat(),
                    record.value("Y").toFloat(),
                    -1,
                    -1,
                    record.value("ntop").toInt(),
                    record.value("procCount").toInt(),
                    false,
                    record.value("Actor").toString(),
                    "M");
            topList.append(top);
        }
    }

    //получаем список дуг управления
    query.prepare("SELECT arcpic.* FROM arcpic "
                  "WHERE arcpic.NAMEPR=:NAMEPR AND arcpic.PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список дуг.\n") + db.lastError().text();
    }
    QList<Arc> arcList;
    while (query.next()){
        QSqlRecord record = query.record();
        Arc::ArcType arcType;
        if (record.value("Type").toString() == QObject::tr("S"))
            arcType = Arc::SerialArc;
        if (record.value("Type").toString() == QObject::tr("P"))
            arcType = Arc::ParallelArc;
        if (record.value("Type").toString() == QObject::tr("T"))
            arcType = Arc::TerminateArc;
        QStringList lines = record.value("Nodes").toString().split(";;");
        Arc arc(arcType,
                record.value("Priority").toInt(),
                record.value("FromTop").toInt(),
                record.value("ToTop").toInt(),
                getPredicate(record.value("Predicate").toString()),
                lines);
        arcList.append(arc);
    }

    //получаем список комментариев
    query.prepare("SELECT commentpic.* FROM commentpic "
                  "WHERE commentpic.NAMEPR=:NAMEPR AND commentpic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
    query.prepare("SELECT NAMEPR, EXTNAME FROM Actor a"
                  "WHERE a.NAMEPR=:NAMEPR AND a.PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", name);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить полное название агрегата.\n") + db.lastError().text();
    }
    query.next();
    Graph result(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, QList<SyncArc>());

    db.close();
    return result;
}

QList<Graph *> DataBaseManager::getGraphList()
{
    myGraphList.clear();
    QList<Graph> dbGraphList = getGraphListDB();
    foreach (Graph graph, dbGraphList)
        myGraphList.append(new Graph(graph));
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, NULL, NULL)");

    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph.extName);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        query.bindValue(":SizeX",       top.type == "M" ? top.procCount : top.sizeX);
        query.bindValue(":SizeY",       top.sizeY);
        query.bindValue(":ntop",        top.number);
        query.bindValue(":isRoot",      top.isRoot ? 1 : 0 );
        query.bindValue(":Actor",       top.actor);
        query.bindValue(":Type",        top.type);
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        query.bindValue(":Predicate", arc.predicate);
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить список дуг.\n") + db.lastError().text();
        }
    }
    db.close();
}

/*!
  Удалает и сохраняет заново агрегат
*/
void DataBaseManager::updateGraphDB(const Graph &graph) throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND NAMEPR = :NAMEPR;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить агрегат.\n") + db.lastError().text();
    }
    db.close();
    return saveGraphDB(graph);
}

QList<Graph> DataBaseManager::getGraphListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT NAMEPR, EXTNAME FROM actor WHERE CLASPR = 'g' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список агрегатов.\n") + db.lastError().text();
    }
    QList<Graph> result;
    while(query.next()){
        QList<Top> topList;
        QList<Arc> arcList;
        QList<Comment> commentList;
        QList<SyncArc> syncArcList;
        result.append(Graph(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, syncArcList));
    }
    db.close();
    return result;
}

void DataBaseManager::saveDataTypeListDB(const QList<DataType> &typeList) throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM typsys WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список типов.\n") + db.lastError().text();
    }
    foreach (DataType type, typeList) {
        query.prepare("INSERT INTO typsys (PROJECT_ID, TYPE, TYPEDEF)"
                      "VALUES (:PROJECT_ID, :TYPE, :TYPEDEF);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":TYPE", type.name);
        query.bindValue(":TYPEDEF", type.typedefStr);
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT PROJECT_ID, TYPE, TYPEDEF FROM typsys WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM data WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список переменных.\n") + db.lastError().text();
    }
    foreach (Variable var, varList) {
        query.prepare("INSERT INTO data (PROJECT_ID, DATA, TYPE, INIT, COMMENT)"
                      "VALUES (:PROJECT_ID, :DATA, :TYPE, :INIT, :COMMENT);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":DATA", var.name);
        query.bindValue(":TYPE", var.type->name);
        query.bindValue(":INIT", var.initValue);
        query.bindValue(":COMMENT", var.comment);
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("SELECT DATA, TYPE, INIT, COMMENT FROM data d WHERE PROJECT_ID = :PROJECT_ID ORDER BY DATA;");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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

QList<Actor *> DataBaseManager::getActorList() const
{
    return myActorList;
}

void DataBaseManager::setActorList(const QList<Actor *> &list)
{
    myActorList = list;
    QList<Actor> dbActorList;
    foreach (Actor *actor, myActorList)
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    query1.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'a'");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список акторов.\n") + db.lastError().text();
    }
    foreach (Actor actor, actorList) {
        query1.clear();
        query1.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                       "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :ICON, :PROTOTIP);");
        query1.bindValue(":PROJECT_ID", myProjectId);
        query1.bindValue(":NAMEPR", actor.name);
        query1.bindValue(":CLASPR", "a");
        query1.bindValue(":EXTNAME", actor.extName);
        query1.bindValue(":ICON", NULL);
        query1.bindValue(":PROTOTIP", actor.baseModule);
        if (!query1.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить актор.\n") + db.lastError().text();
        }

        QSqlQuery query2;
        QList<const Variable *> actorVariables = actor.variableList;
        for (int j = 0; j < actorVariables.count(); j++){
            query2.clear();
            query2.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query2.bindValue(":PROJECT_ID", myProjectId);
            query2.bindValue(":NAMEPR", actor.name);
            query2.bindValue(":NEV", j);
            query2.bindValue(":DATA", actorVariables[j]->name);
            QString vaMode;
            if (actor.varAccModeList.at(j) == QObject::tr("Исходный"))
                vaMode = "I";
            else if (actor.varAccModeList.at(j) ==  QObject::tr("Модифицируемый"))
                vaMode = "M";
            else if (actor.varAccModeList.at(j) ==  QObject::tr("Вычисляемый"))
                vaMode = "R";

            query2.bindValue(":MODE", vaMode);
            if (!query2.exec()) {
                globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'a' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список акторов.\n") + db.lastError().text();
    }
    QList<Actor> result;
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        if (!query2.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        result.append(Actor(query1.value(0).toString(),
                            query1.value(2).toString(),
                            query1.value(6).toString().isEmpty() ? Actor::InlineType : Actor::NormalType,
                            query1.value(6).toString(),
                            actorVariableList,
                            actorVarAccList,
                            QImage()));
        query2.clear();
    }
    db.close();
    return result;
}

Actor *DataBaseManager::getActor(const QString &name)
{
    Actor *result = NULL;
    foreach (Actor *actor, myActorList) {
        if (actor->name == name) {
            result = actor;
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
            result == predicate;
            break;
        }
    }
    return result;
}

//Actor DataBaseManager::getActorDB(QString namepr) throw (QString)
//{
//    if (namepr.isEmpty())
//        throw QObject::tr("Пустое имя актора");
//    QList<Variable> varList = getVariableListDB();
//    if (!db.open()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
//    }
//    QSqlQuery query1;
//    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE NAMEPR = :NAMEPR;");
//    query1.bindValue(":NAMEPR", namepr);
//    if (!query1.exec()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        db.close();
//        throw QObject::tr("Не удалось получить актор.\n") + db.lastError().text();
//    }
//    if (!query1.first()) {
//        globalLogger->writeLog("Запрошен несуществующий актор.", Logger::Critical);
//        db.close();
//        throw QObject::tr("Запрошен несуществующий актор.\n");
//    }

//    QSqlQuery query2;
//    query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
//    query2.bindValue(":NAMEPR", namepr);
//    if (!query2.exec()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        db.close();
//        throw QObject::tr("Не удалось получить параметры для актора.\n") + db.lastError().text();
//    }
//    globalLogger->writeLog(query2.executedQuery().toUtf8());
//    QList<Variable *> actorVariableList;
//    QStringList actorVarAccList;
//    while (query2.next()){
//        for (int i = 0; i < varList.count(); i++)
//            if (query2.value(1).toString() == varList.at(i).name){
//                actorVariableList.append(new Variable(varList[i]));
//                break;
//            }
//        QString vaMode;
//        if (query2.value(2).toString() == "I")
//            vaMode = QObject::tr("Исходный");
//        else if (query2.value(2).toString() == "M")
//            vaMode = QObject::tr("Модифицируемый");
//        else if (query2.value(2).toString() == "R")
//            vaMode = QObject::tr("Вычисляемый");
//        actorVarAccList.append(vaMode);
//    }
//    Actor::Type actorType;
//    if (query1.value(0).toString() == "g") {
//        actorType = Actor::GraphType;
//    } else {
//        actorType = query1.value(5).toString().isEmpty() ? Actor::InlineType : Actor::NormalType;
//    }
//    Actor result(namepr,
//                 query1.value(1).toString(),
//                 actorType,
//                 query1.value(5).toString(),
//                 actorVariableList,
//                 actorVarAccList,
//                 QImage());
//    db.close();
//    return result;
//}

void DataBaseManager::savePredicateListDB(const QList<Predicate> &predList) throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'p'");
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить список предикатов.\n") + db.lastError().text();
    }

    foreach (Predicate predicate, predList) {
        query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", predicate.name);
        query.bindValue(":CLASPR", "p");
        query.bindValue(":EXTNAME", predicate.extName);
        query.bindValue(":PROTOTIP", predicate.baseModule);
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить предикат.\n") + db.lastError().text();
        }

        int j = 0;
        foreach (const Variable *var, predicate.variableList) {
            query.clear();
            query.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                          "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query.bindValue(":PROJECT_ID", myProjectId);
            query.bindValue(":NAMEPR", predicate.name);
            query.bindValue(":NEV", j++);
            query.bindValue(":DATA", var->name);
            query.bindValue(":MODE", "I");
            if (!query.exec()) {
                globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'p' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось получить список предикатов.\n") + db.lastError().text();
    }

    QList<Predicate> result;
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        if (!query2.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось получить список параметров для предиката.\n") + db.lastError().text();
        }
        QList<const Variable *> variableList;
        while (query2.next())
            variableList.append(getVariable(query2.value(1).toString()));
        result.append(Predicate(query1.value(0).toString(),
                                query1.value(2).toString(),
                                query1.value(6).toString().isEmpty() ? Predicate::inlineType : Predicate::NormalType,
                                getBaseModule(query1.value(6).toString()),
                                variableList));
    }
    db.close();
    return result;
}

//Predicate DataBaseManager::getPredicateDB(const QString &namepr) throw (QString)
//{
//    if (namepr.isEmpty())
//        throw QObject::tr("Пустое имя актора");
//    QList<Variable> variableList = getVariableListDB();
//    if (!db.open()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
//    }
//    QSqlQuery query1;
//    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
//    query1.bindValue(":NAMEPR", namepr);
//    query1.bindValue(":PROJECT_ID", myProjectId);
//    if (!query1.exec()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        db.close();
//        throw QObject::tr("Не удалось получить предикат.\n") + db.lastError().text();
//    }

//    if (!query1.first()) {
//        globalLogger->writeLog("Запрошен несуществующий предикат.", Logger::Critical);
//        db.close();
//        throw QObject::tr("Запрошен несуществующий предикат.\n");
//    }

//    QSqlQuery query2;
//    query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
//    query2.bindValue(":NAMEPR", namepr);
//    if (!query2.exec()) {
//        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
//        db.close();
//        throw QObject::tr("Не удалось получить параметры для предиката.\n") + db.lastError().text();
//    }
//    QList<Variable> myVariableList;
//    while (query2.next()){
//        for (int i = 0; i < variableList.count(); i++)
//            if (query2.value(1).toString() == variableList[i].name){
//                myVariableList.append(variableList.at(i));
//                break;
//            }
//    }
//    Predicate predicate(namepr,
//                        query1.value(1).toString(),
//                        query1.value(5).toString().isEmpty() ? Predicate::inlineType : Predicate::NormalType,
//                        query1.value(5).toString(),
//                        myVariableList);
//    db.close();
//    return predicate;
//}

void DataBaseManager::registerModuleDB(const QString &uniqName, const QString &fileName, const QString &comment, const QStringList &paramList) throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query;
    query.prepare("INSERT INTO bazmod (PROJECT_ID, PROTOTIP, NAMEPR, COMMENT)"
                  "VALUES (:PROJECT_ID, :PROTOTIP, :NAMEPR, :COMMENT);");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":PROTOTIP", uniqName);
    query.bindValue(":NAMEPR",   fileName);
    query.bindValue(":COMMENT",  comment);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось сохранить базовый модуль.\n") + db.lastError().text();
    }

    for (int i = 0; i < paramList.count(); i++){
        query.clear();
        query.prepare("INSERT INTO databaz (PROJECT_ID, PROTOTIP, DATA, TYPE, MODE, NEV, COMMENT)"
                      "VALUES (:PROJECT_ID, :PROTOTIP, :DATA, :TYPE, :MODE, :NEV, :COMMENT);");
        QStringList parameter = paramList.at(i).split(";;");
        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":PROTOTIP",    uniqName);
        query.bindValue(":DATA",        parameter.at(0));
        query.bindValue(":TYPE",        parameter.at(1));
        query.bindValue(":NEV",         i);
        QString vaMode;
        if (parameter.at(2) == QObject::tr("Исходный"))
            vaMode = "I";
        else if (parameter.at(2) == QObject::tr("Модифицируемый"))
            vaMode = "M";
        else if (parameter.at(2) == QObject::tr("Вычисляемый"))
            vaMode = "R";
        query.bindValue(":MODE",        vaMode);
        query.bindValue(":COMMENT",     parameter.at(3));
        if (!query.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось сохранить параметры базового модуля.\n") + db.lastError().text();
        }
    }
    db.close();
}

QList<BaseModule> DataBaseManager::getBaseModuleListDB() throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT PROTOTIP, NAMEPR, COMMENT FROM bazmod WHERE PROJECT_ID = :PROJECT_ID;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    if (!query1.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось список базовых модулей.\n") + db.lastError().text();
    }

    QList<BaseModule> result;
    while (query1.next()){
        query2.prepare("SELECT TYPE, DATA, MODE, COMMENT FROM databaz WHERE PROTOTIP = :PROTOTIP AND PROJECT_ID = :PROJECT_ID order by NEV;");
        query2.bindValue(":PROTOTIP", query1.value(0).toString());
        query2.bindValue(":PROJECT_ID", myProjectId);
        if (!query1.exec()) {
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
            db.close();
            throw QObject::tr("Не удалось параметры модулей.\n") + db.lastError().text();
        }
        globalLogger->writeLog(query2.executedQuery().toUtf8());
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

void DataBaseManager::saveStruct(const Graph &graph) throw (QString)
{
    if (!db.open()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        throw QObject::tr("Не удалось открыть базу данных.\n") + db.lastError().text();
    }

    QSqlQuery query;
    QList<const Predicate *> predicateList;
    int i = 0;
    query.prepare("DELETE FROM graph WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить структуру.\n") + db.lastError().text();
    }
    query.prepare("DELETE FROM graphtop WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        db.close();
        throw QObject::tr("Не удалось удалить структуру.\n") + db.lastError().text();
    }
    query.prepare("DELETE FROM graphpre WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":PROJECT_ID", myProjectId);
    if (!query.exec()) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
                globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
            globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
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

int DataBaseManager::Compi_count_MaxGH(QString myGraphName, int* MaxGH)
{
    if (!db.open())
        return false;

    // int *MaxGH = new(int);
    (*MaxGH) = 0;
    QSqlQuery query;
    query.prepare("SELECT * FROM graph"
                  " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    if (queryres)
    {
        while (query.next()) {
            (*MaxGH)++;
        }

        //        QMessageBox::information(NULL,
        //                                      QObject::tr("OK"),
        //                                      QObject::tr("MaxGH =")
        //                                      + QString("%1 ").arg((*MaxGH))
        //                                      + myGraphName,
        //                                      QMessageBox::Ok,
        //                                      QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("GRAPH doesn't contain ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

int DataBaseManager::Compi_count_MaxLT(QString myGraphName, int* MaxLT)
{
    if (!db.open())
        return false;

    // int *MaxLT = new(int);
    (*MaxLT) = 0;
    QSqlQuery query;
    query.prepare("SELECT * FROM graphtop"
                  " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    if (queryres)
    {
        while (query.next()) {
            (*MaxLT)++;
        }

        //        QMessageBox::information(NULL,
        //                                      QObject::tr("OK"),
        //                                      QObject::tr("MaxLT =")
        //                                      + QString("%1 ").arg((*MaxLT))
        //                                      + myGraphName,
        //                                      QMessageBox::Ok,
        //                                      QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("GRAPHTOP doesn't contain ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

int DataBaseManager::Compi_count_MaxLP(QString myGraphName, int* MaxLP)
{
    if (!db.open())
        return false;

    // int *MaxLP = new(int);
    (*MaxLP) = 0;
    QSqlQuery query;
    query.prepare("SELECT * FROM graphpre"
                  " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    if (queryres)
    {
        while (query.next()) {
            (*MaxLP)++;
        }

        //        QMessageBox::information(NULL,
        //                                      QObject::tr("OK"),
        //                                      QObject::tr("MaxLP =")
        //                                      + QString("%1 ").arg((*MaxLP))
        //                                      + myGraphName,
        //                                      QMessageBox::Ok,
        //                                      QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("GRAPHPRE doesn't contain ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

int DataBaseManager::Compi_get_root_top(QString myGraphName, int* root_top)
{
    if (!db.open())
        return false;

    // int *MaxGH = new(int);
    QSqlQuery query;
    query.prepare("SELECT NFROM FROM graph"
                  " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID AND PRIOR = 0;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    if ( (queryres) && (query.next()) )
    {
        if (query.first()) {
            // query.value(0).convert(QVariant::Int);
            (*root_top) = (query.value(0).toInt());
        }

        //        QMessageBox::information(NULL,
        //                                      QObject::tr("OK"),
        //                                      QObject::tr("root_top = ")
        //                                      % QString::number((*root_top))
        //                                      % QObject::tr("!!!") + query.lastQuery(),
        //                                      //+ myGraphName,
        //                                      QMessageBox::Ok,
        //                                      QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("GRAPH doesn't contain ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

bool DataBaseManager::Compi_get_GSP_Shab_List()
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("SELECT * FROM gsp_shab;");
    bool queryres = query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());

    while ( (queryres) && (query.next()) ){
        myShabList.append(new Shab(query.value(1).toInt(),query.value(2).toString(),query.value(3).toString()));
    }

    db.close();

    return (db.lastError().type() == QSqlError::NoError);
}


int DataBaseManager::Compi_fill_Graph_struct(QString myGraphName, int MaxGH, COMPHs *Graph)
{
    int i=0;

    if (!db.open())
        return false;

    QSqlQuery query;
    query.prepare("SELECT * FROM graph"
                  " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    // int fieldNo = query.record().indexOf("country");
    if (queryres)
    {
        i=0;
        while ( query.next()  &&   i < (MaxGH) )
        {
            (*(*Graph+i)).FromTop = (query.value(2).toInt());   //f4int(f4ref("NFROM"));
            (*(*Graph+i)).ToTop   = (query.value(3).toInt());   //f4int(f4ref("NTO"));
            (*(*Graph+i)).Pred    = (query.value(4).toInt());   //f4int(f4ref("NPRED"));
            (*(*Graph+i)).ArcType = (query.value(7).toInt());   //f4int(f4ref("ARCTYPE"));

            i++;
        }
    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("GRAPH doesn't contain ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}


int DataBaseManager::Compi_fill_ListT_struct(QString myGraphName, int MaxLT, COMPTOPs *ListTop)
{
    int i=0;
    int SPRED_exists = 0;
    int SMSG_exists = 0;

    if (!db.open())
        return false;

    //    QSqlTableModel *model = new QSqlTableModel;
    //    model->setTable("graphtop");
    //    model->select();

    //    if ( model->fieldIndex("SPRED") >= 0 )
    //    { SPRED_exists = 1; }
    //    else
    //    { SPRED_exists = 0; }

    //    if ( model->fieldIndex("SMSG") >= 0 )
    //    { SMSG_exists = 1; }
    //    else
    //    { SMSG_exists = 0; }


    QSqlQuery query;
    query.prepare("SELECT GT.PROJECT_ID, GT.NAMEPR, GT.NTOP, GT.NAME, GT.EXCL, GT.SPRED, GT.SMSG, A.PROTOTIP FROM graphtop AS GT, actor AS A"
                  " WHERE GT.NAMEPR = :EXTNAME AND GT.PROJECT_ID = :PROJECT_ID AND A.NAMEPR = GT.NAME AND A.PROJECT_ID = GT.PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    // int fieldNo = query.record().indexOf("country");
    if (queryres)
    {
        i=0;
        while ( query.next()  &&   i < (MaxLT) )
        {
            strncpy((*(*ListTop+i)).Name, (query.value(3).toString().toStdString().c_str()), 8);   //f4str(f4ref("NAME"))
            (*(*ListTop+i)).Top   = (query.value(2).toInt());   //f4int(f4ref("NTOP"));
            (*(*ListTop+i)).FirstDef =-77;
            (*(*ListTop+i)).LastDef  =-77;
            if ( strncmp( query.value(5).toString().toStdString().c_str(), "        ",8 ) ) {
                strncpy((*(*ListTop + i)).SPred,query.value(5).toString().toStdString().c_str(),8);
            }
            else {  strcpy((*(*ListTop + i)).SPred,"NULL");  }

            if ( strncmp( query.value(6).toString().toStdString().c_str(), "        ",8 ) ) {
                strncpy((*(*ListTop + i)).SMsg,query.value(6).toString().toStdString().c_str(),8);
            }
            else {  strcpy((*(*ListTop + i)).SMsg,"NULL");  }

            strncpy((*(*ListTop + i)).NameProt,query.value(7).toString().toStdString().c_str(),8);

            i++;
        }


        //  d4select(ref_AKTOR);
        //  strncpy((*(*ListTop + i)).NameProt,f4str(f4ref("PROTOTIP")),8);
        //  d4select(ref_LT);
        //  printf("Name %s Top=%d  FistDef=%d LastDef=%d NameProt=%s\n",(*(*ListTop + i)).Name,
        //             (*(*ListTop+i)).Top,(*(*ListTop+i)).FirstDef,(*(*ListTop+i)).LastDef,(*(*ListTop + i)).NameProt);
        //  x4skip(1L);
        // }

    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка in DataBaseManager::Compi_fill_ListT_struct()"),
                              QObject::tr("Таблицы: GRAPHTOP, ACTOR. Агрегат: ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

int DataBaseManager::Compi_fill_ListP_struct(QString myGraphName, int MaxLP, COMPREs *ListP)
{
    int i=0;

    if (!db.open())
        return false;

    QSqlQuery query;
    query.prepare("SELECT GP.PROJECT_ID, GP.NAMEPR, GP.NPRED, GP.NAME, A.PROTOTIP FROM graphpre AS GP, actor AS A"
                  " WHERE GP.NAMEPR = :EXTNAME AND GP.PROJECT_ID = :PROJECT_ID AND A.NAMEPR = GP.NAME AND A.PROJECT_ID = GP.PROJECT_ID;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":EXTNAME", myGraphName);
    bool queryres = query.exec();

    if (queryres)
    {
        i=0;
        while ( query.next()  &&   i < (MaxLP) )
        {
            strncpy((*(*ListP+i)).Name, (query.value(3).toString().toStdString().c_str()), 8);   //f4str(f4ref("NAME"))

            (*(*ListP+i)).Pred = (query.value(2).toInt());  // f4int(f4ref("NPRED"));

            strncpy((*(*ListP + i)).NameProt,query.value(4).toString().toStdString().c_str(),8);    // f4str(f4ref("PROTOTIP"))

            i++;
        }

    }
    else
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка in DataBaseManager::Compi_fill_ListP_struct()"),
                              QObject::tr("Таблицы: GRAPHPRE, ACTOR. Агрегат: ")
                              + myGraphName,
                              QMessageBox::Ok);
    }

    db.close();

    return 0;
}

void DataBaseManager::openProjectDB(int projectId)
{
    if (myProjectId != -1)
        return;
    myProjectId = projectId;
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
}

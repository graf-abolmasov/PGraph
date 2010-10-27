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
    db.setPassword(myDBSettings.value("DB/password", "Marina").toString());
    db.open();
    db.close();

    myProjectId = 1;
}

bool DataBaseManager::getGraph(Graph &graph)
{
    bool ok = db.open();
    if (!ok) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        return false;
    }

    QSqlQuery query;
    //получаем список вершин
    query.prepare("SELECT toppic.*, toppic.SizeX AS procCount FROM toppic "
                  "WHERE toppic.NAMEPR=:NAMEPR AND toppic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    while (query.next()){
        QSqlRecord record = query.record();
        if (record.value("Type").toString() == "T"){
            Top* top = new Top(record.value("X").toFloat(),
                               record.value("Y").toFloat(),
                               record.value("SizeX").toFloat(),
                               record.value("SizeY").toFloat(),
                               record.value("ntop").toInt(),
                               -1,
                               record.value("isRoot").toBool(),
                               record.value("Actor").toString(),
                               "T");
            graph.topList.append(top);
        }

        if (record.value("Type").toString() == "M"){
            Top* top = new Top(record.value("X").toFloat(),
                               record.value("Y").toFloat(),
                               -1,
                               -1,
                               record.value("ntop").toInt(),
                               record.value("procCount").toInt(),
                               false,
                               record.value("Actor").toString(),
                               "M");
            graph.topList.append(top);
        }
    }

    //получаем список дуг управления
    query.prepare("SELECT arcpic.* FROM arcpic "
                  "WHERE arcpic.NAMEPR=:NAMEPR AND arcpic.PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
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
        Arc* arc = new Arc(arcType,
                           record.value("Priority").toInt(),
                           record.value("FromTop").toInt(),
                           record.value("ToTop").toInt(),
                           record.value("Predicate").toString(),
                           lines);
        graph.arcList.append(arc);
    }

    //получаем список комментариев
    query.prepare("SELECT commentpic.* FROM commentpic "
                  "WHERE commentpic.NAMEPR=:NAMEPR AND commentpic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    while (query.next()){
        QSqlRecord record = query.record();
        Comment* comment = new Comment(record.value("X").toFloat(),
                                       record.value("Y").toFloat(),
                                       record.value("TEXT").toString());
        graph.commentList.append(comment);
    }

    //Получаем полное название агрегата
    query.prepare("SELECT EXTNAME FROM Actor "
                  "WHERE Actor.NAMEPR=:NAMEPR AND Actor.PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.exec();
    query.next();
    graph.extName = query.value(0).toString();

    db.close();
    ok = (db.lastError().type() == QSqlError::NoError);
    if (!ok) globalLogger->writeLog(db.lastError().text(), Logger::Critical);
    return ok;
}

bool DataBaseManager::saveGraph(Graph *graph)
{
    bool ok = db.open();
    if (!ok) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        return false;
    }
    QSqlQuery query;

    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, NULL, NULL)");

    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph->extName);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());

    foreach (Top* top, graph->topList){
        query.prepare("INSERT INTO toppic (PROJECT_ID, NAMEPR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Type) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, :Type)");

        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":NAMEPR",      graph->name);
        query.bindValue(":X",           top->x);
        query.bindValue(":Y",           top->y);
        query.bindValue(":SizeX",       top->type == "M" ? top->procCount : top->sizeX);
        query.bindValue(":SizeY",       top->sizeY);
        query.bindValue(":ntop",        top->number);
        query.bindValue(":isRoot",      top->isRoot ? 1 : 0 );
        query.bindValue(":Actor",       top->actor);
        query.bindValue(":Type",        top->type);
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }

    foreach(Comment* comment, graph->commentList){
        query.prepare("INSERT INTO commentpic (PROJECT_ID, NAMEPR, TEXT, X, Y) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :TEXT, :X, :Y)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":X",          comment->x);
        query.bindValue(":Y",          comment->y);
        query.bindValue(":TEXT",       comment->text);
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }

    foreach (Arc* arc, graph->arcList){
        query.prepare("INSERT INTO arcpic (PROJECT_ID, NAMEPR, Nodes, Priority, FromTop, ToTop, Predicate, Type) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :Nodes, :Priority, :FromTop, :ToTop, :Predicate, :Type)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph->name);
        QString arcType;
        switch (arc->type){
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
        query.bindValue(":Nodes",     arc->lines.join(";;"));
        query.bindValue(":Priority",  arc->priority);
        query.bindValue(":FromTop",   arc->startTop);
        query.bindValue(":ToTop",     arc->endTop);
        query.bindValue(":Predicate", arc->predicate);
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }
    db.close();
    ok = (db.lastError().type() == QSqlError::NoError);
    if (!ok) globalLogger->writeLog(db.lastError().text(), Logger::Critical);
    return ok;
}

bool DataBaseManager::updateGraph(Graph *graph)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND NAMEPR = :NAMEPR;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph->name);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    db.close();
    return saveGraph(graph);
}

bool DataBaseManager::getGraphList(QList<Graph* > &graphList)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("SELECT NAMEPR, EXTNAME FROM actor WHERE CLASPR = 'g' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    while(query.next()){
        QList<Top* > topList;
        QList<Arc* > arcList;
        QList<Comment* > commentList;
        QList<QSyncArc* > syncArcList;
        QList<QMultiProcTop* > multiProcTopList;
        graphList.append(new Graph(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, syncArcList, multiProcTopList));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::saveDataTypeList(QList<DataType*>& typeList){
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("DELETE FROM typsys WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    for (int i = 0; i < typeList.count(); i++){
        query.prepare("INSERT INTO typsys (PROJECT_ID, TYPE, TYPEDEF)"
                      "VALUES (:PROJECT_ID, :TYPE, :TYPEDEF);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":TYPE", typeList.at(i)->name);
        query.bindValue(":TYPEDEF", typeList.at(i)->typedefStr);
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
        query.clear();
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::getDataTypeList(QList<DataType*>& typeList){
    if (!db.open())
        return false;
    QSqlQuery query;
    typeList.clear();
    query.prepare("SELECT PROJECT_ID, TYPE, TYPEDEF FROM typsys WHERE PROJECT_ID = :PROJECT_ID ORDER BY TYPE;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    while (query.next()){
        typeList.append(new DataType(query.value(1).toString(), query.value(2).toString()));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::saveVariableList(QList<Variable*>& varList)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("DELETE FROM data WHERE PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    for (int i = 0; i < varList.count(); i++){
        query.prepare("INSERT INTO data (PROJECT_ID, DATA, TYPE, INIT, COMMENT)"
                      "VALUES (:PROJECT_ID, :DATA, :TYPE, :INIT, :COMMENT);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":DATA", varList.at(i)->name);
        query.bindValue(":TYPE", varList.at(i)->type);
        query.bindValue(":INIT", varList.at(i)->initValue.toString());
        query.bindValue(":COMMENT", varList.at(i)->comment);
        query.exec();
        query.clear();
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::getVariableList(QList<Variable* >& varList)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("SELECT DATA, TYPE, INIT, COMMENT FROM data WHERE PROJECT_ID = :PROJECT_ID ORDER BY DATA;");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    while (query.next()){
        varList.append(new Variable(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString()));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::saveActorList(QList<Actor *> &actorList)
{
    if (!db.open())
        return false;
    QSqlQuery query1;
    query1.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'a'");
    query1.bindValue(":PROJECT_ID", myProjectId);
    query1.exec();
    for (int i = 0; i < actorList.count(); i++){
        query1.clear();
        query1.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :ICON, :PROTOTIP);");
        query1.bindValue(":PROJECT_ID", myProjectId);
        query1.bindValue(":NAMEPR", actorList.at(i)->name);
        query1.bindValue(":CLASPR", "a");
        query1.bindValue(":EXTNAME", actorList.at(i)->extName);
        //query1.bindValue(":ICON", actorList.at(i)->icon.bits());
        query1.bindValue(":ICON", NULL);
        query1.bindValue(":PROTOTIP", actorList.at(i)->baseModule);
        query1.exec();
        globalLogger->writeLog(query1.executedQuery().toUtf8());

        QSqlQuery query2;
        for (int j = 0; j < actorList.at(i)->variableList.count(); j++){
            query2.clear();
            query2.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query2.bindValue(":PROJECT_ID", myProjectId);
            query2.bindValue(":NAMEPR", actorList.at(i)->name);
            query2.bindValue(":NEV", j);
            query2.bindValue(":DATA", actorList.at(i)->variableList.at(j)->name);
            QString vaMode;
            if (actorList.at(i)->varAMList.at(j) == QObject::tr("Исходный"))
                vaMode = "I";
            else if (actorList.at(i)->varAMList.at(j) ==  QObject::tr("Модифицируемый"))
                vaMode = "M";
            else if (actorList.at(i)->varAMList.at(j) ==  QObject::tr("Вычисляемый"))
                vaMode = "R";
            query2.bindValue(":MODE", vaMode);
            query2.exec();
            globalLogger->writeLog(query2.executedQuery().toUtf8());
        }
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::getActorList(QList<Actor *> &actorList)
{
    QList<Variable* > varList;
    if (!getVariableList(varList))
        return false;
    if (!db.open())
        return false;
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'a' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    query1.exec();
    globalLogger->writeLog(query1.executedQuery().toUtf8());
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        query2.exec();
        globalLogger->writeLog(query2.executedQuery().toUtf8());
        QList<Variable* > myVariableList;
        QStringList myVAList;
        while (query2.next()){
            for (int i = 0; i < varList.count(); i++)
                if (query2.value(1).toString() == varList.at(i)->name){
                    myVariableList.append(varList.at(i));
                    break;
                }
            QString vaMode;
            if (query2.value(2).toString() == "I")
                vaMode = QObject::tr("Исходный");
            else if (query2.value(2).toString() == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (query2.value(2).toString() == "R")
                vaMode = QObject::tr("Вычисляемый");
            myVAList.append(vaMode);
        }
        actorList.append(new Actor(query1.value(0).toString(),
                                   query1.value(2).toString(),
                                   query1.value(6).toString() == "" ? Actor::InlineType : Actor::NormalType,
                                   query1.value(6).toString(),
                                   myVariableList,
                                   myVAList,
                                   QImage()));
        query2.clear();
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

Actor* DataBaseManager::getActor(QString namepr)
{
    if (namepr == "") return NULL;
    QList<Variable* > varList;
    if (!getVariableList(varList))
        return false;
    if (!db.open())
        return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE NAMEPR = :NAMEPR;");
    query1.bindValue(":NAMEPR", namepr);
    query1.exec();
    globalLogger->writeLog(query1.executedQuery().toUtf8());
    if(!query1.first()){
        globalLogger->writeLog("Запрошен несуществующий актор.", Logger::Critical);
        db.close();
        return NULL;
    }

    QSqlQuery query2;
    query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
    query2.bindValue(":NAMEPR", namepr);
    query2.exec();
    globalLogger->writeLog(query2.executedQuery().toUtf8());
    QList<Variable* > myVariableList;
    QStringList myVAList;
    while (query2.next()){
        for (int i = 0; i < varList.count(); i++)
            if (query2.value(1).toString() == varList.at(i)->name){
            myVariableList.append(varList.at(i));
            break;
        }
        QString vaMode;
        if (query2.value(2).toString() == "I")
            vaMode = QObject::tr("Исходный");
        else if (query2.value(2).toString() == "M")
            vaMode = QObject::tr("Модифицируемый");
        else if (query2.value(2).toString() == "R")
            vaMode = QObject::tr("Вычисляемый");
        myVAList.append(vaMode);
    }
    Actor::Type actorType;
    if (query1.value(0).toString() == "g") {
        actorType = Actor::GraphType;
    } else {
        actorType = query1.value(5).toString() == "" ? Actor::InlineType : Actor::NormalType;
    }
    Actor* newActor = new Actor(namepr,
                               query1.value(1).toString(),
                               actorType,
                               query1.value(5).toString(),
                               myVariableList,
                               myVAList,
                               QImage());
    db.close();
    return newActor;
}

bool DataBaseManager::savePredicateList(QList<Predicate *> &predList)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("DELETE FROM actor WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = 'p'");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    for (int i = 0; i < predList.count(); i++){
        query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", predList.at(i)->name);
        query.bindValue(":CLASPR", "p");
        query.bindValue(":EXTNAME", predList.at(i)->extName);
        query.bindValue(":PROTOTIP", predList.at(i)->baseModule);
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());

        for (int j = 0; j < predList.at(i)->variableList.count(); j++){
            query.clear();
            query.prepare("INSERT INTO pasport (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query.bindValue(":PROJECT_ID", myProjectId);
            query.bindValue(":NAMEPR", predList.at(i)->name);
            query.bindValue(":NEV", j);
            query.bindValue(":DATA", predList.at(i)->variableList.at(j)->name);
            query.bindValue(":MODE", "i");
            query.exec();
            globalLogger->writeLog(query.executedQuery().toUtf8());
        }
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::getPredicateList(QList<Predicate *> &predList)
{
    QList<Variable* > varList;
    if (!getVariableList(varList))
        return false;
    if (!db.open())
        return false;
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE CLASPR = 'p' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    query1.exec();
    globalLogger->writeLog(query1.executedQuery().toUtf8());
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        query2.exec();
        globalLogger->writeLog(query2.executedQuery().toUtf8());
        QList<Variable* > myVariableList;
        while (query2.next()){
            for (int i = 0; i < varList.count(); i++)
                if (query2.value(1).toString() == varList.at(i)->name){
                    myVariableList.append(varList.at(i));
                    break;
                }
        }
        predList.append(new Predicate(query1.value(0).toString(),
                                      query1.value(2).toString(),
                                      query1.value(6).toString() == "" ? Predicate::inlineType : Predicate::normalType,
                                      query1.value(6).toString(),
                                      myVariableList));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

Predicate* DataBaseManager::getPredicate(QString namepr)
{
    if (namepr == "") return NULL;
    QList<Variable* > varList;
    if (!getVariableList(varList))
        return false;
    if (!db.open())
        return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM actor WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query1.bindValue(":NAMEPR", namepr);
    query1.bindValue(":PROJECT_ID", myProjectId);
    query1.exec();
    globalLogger->writeLog(query1.executedQuery().toUtf8());

    if (!query1.first()) {
        db.close();
        globalLogger->writeLog("Запрошен несуществующий предикат.", Logger::Critical);
        return NULL;
    }

    QSqlQuery query2;
    query2.prepare("SELECT NEV, DATA, MODE FROM pasport WHERE NAMEPR = :NAMEPR ORDER BY NEV");
    query2.bindValue(":NAMEPR", namepr);
    query2.exec();
    globalLogger->writeLog(query2.executedQuery().toUtf8());
    QList<Variable* > myVariableList;
    while (query2.next()){
        for (int i = 0; i < varList.count(); i++)
            if (query2.value(1).toString() == varList.at(i)->name){
            myVariableList.append(varList.at(i));
            break;
        }
    }
    db.close();
    return new Predicate(namepr,
                         query1.value(1).toString(),
                         query1.value(5).toString() == "" ? Predicate::inlineType : Predicate::normalType,
                         query1.value(5).toString(),
                         myVariableList);
}

bool DataBaseManager::registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList)
{
    if (!db.open())
        return false;
    QSqlQuery query;
    query.prepare("INSERT INTO bazmod (PROJECT_ID, PROTOTIP, NAMEPR, COMMENT)"
                  "VALUES (:PROJECT_ID, :PROTOTIP, :NAMEPR, :COMMENT);");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":PROTOTIP", uniqName);
    query.bindValue(":NAMEPR",   fileName);
    query.bindValue(":COMMENT",  comment);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());

    for (int i = 0; i < paramList.count(); i++){
        query.clear();
        query.prepare("INSERT INTO databaz (PROJECT_ID, PROTOTIP, DATA, TYPE, MODE, COMMENT)"
                      "VALUES (:PROJECT_ID, :PROTOTIP, :DATA, :TYPE, :MODE, :COMMENT);");
        QStringList parameter = paramList.at(i).split(";;");
        query.bindValue(":PROJECT_ID",  myProjectId);
        query.bindValue(":PROTOTIP",    uniqName);
        query.bindValue(":DATA",        parameter.at(0));
        query.bindValue(":TYPE",        parameter.at(1));
        QString vaMode;
        if (parameter.at(2) == QObject::tr("Исходный"))
            vaMode = "I";
        else if (parameter.at(2) == QObject::tr("Модифицируемый"))
            vaMode = "M";
        else if (parameter.at(2) == QObject::tr("Вычисляемый"))
            vaMode = "R";
        query.bindValue(":MODE",        vaMode);
        query.bindValue(":COMMENT",     parameter.at(3));
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }

    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::getRegisteredModules(QList<BaseModule*> &moduleList)
{
    if (!db.open())
        return false;
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT PROTOTIP, NAMEPR, COMMENT FROM bazmod WHERE PROJECT_ID = :PROJECT_ID;");
    query1.bindValue(":PROJECT_ID", myProjectId);
    query1.exec();
    globalLogger->writeLog(query1.executedQuery().toUtf8());
    while (query1.next()){
        query2.prepare("SELECT TYPE, DATA, MODE, COMMENT FROM databaz WHERE PROTOTIP = :PROTOTIP AND PROJECT_ID = :PROJECT_ID;");
        query2.bindValue(":PROTOTIP", query1.value(0).toString());
        query2.bindValue(":PROJECT_ID", myProjectId);
        query2.exec();
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
        moduleList.append(new BaseModule(query1.value(1).toString(), query1.value(0).toString(), query1.value(2).toString(), parameterList));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

bool DataBaseManager::saveStruct(Graph *graph)
{
    bool ok = db.open();
    if (!ok) {
        globalLogger->writeLog(db.lastError().text(), Logger::Critical);
        return false;
    }
    QSqlQuery query;
    QStringList predicateList;
    int i = 0;

    query.prepare("DELETE FROM graph WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    query.prepare("DELETE FROM graphtop WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());
    query.prepare("DELETE FROM graphpre WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProjectId);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());

    foreach (Arc* arc, graph->arcList){
        if (!predicateList.contains(arc->predicate)){
            predicateList.append(arc->predicate);
            query.prepare("INSERT INTO graphpre (PROJECT_ID, NAMEPR, NPRED, NAME)"
                          "VALUES (:PROJECT_ID, :NAMEPR, :NPRED, :NAME)");
            query.bindValue(":PROJECT_ID", myProjectId);
            query.bindValue(":NAMEPR", graph->name);
            query.bindValue(":NPRED", i);
            query.bindValue(":NAME", arc->predicate);
            query.exec();
            i++;
            globalLogger->writeLog(query.executedQuery().toUtf8());
        }
        query.prepare("INSERT INTO graph (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", graph->name);
        query.bindValue(":NFROM", arc->startTop);
        query.bindValue(":NTO", arc->endTop);
        query.bindValue(":NPRED", predicateList.indexOf(arc->predicate));
        query.bindValue(":PRIOR", arc->priority);
        int arcType;
        switch (arc->type){
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
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }

    int rootTop = -1;
    foreach (Top* top, graph->topList){
        query.prepare("INSERT INTO graphtop (PROJECT_ID, NAMEPR, NTOP, NAME)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NTOP, :NAME);");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR", graph->name);
        query.bindValue(":NTOP", top->number);
        query.bindValue(":NAME", top->actor);
        if (top->isRoot)
            rootTop = top->number;
        query.exec();
        globalLogger->writeLog(query.executedQuery().toUtf8());
    }
    query.prepare("INSERT INTO graph (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":NFROM",rootTop);
    query.bindValue(":NTO", 0);
    query.bindValue(":NPRED", 0);
    query.bindValue(":PRIOR", 0);
    query.bindValue(":ARCTYPE", 0);
    query.exec();
    globalLogger->writeLog(query.executedQuery().toUtf8());

    db.close();
    ok = (db.lastError().type() == QSqlError::NoError);
    if (!ok) globalLogger->writeLog(db.lastError().text(), Logger::Critical);
    return ok;
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

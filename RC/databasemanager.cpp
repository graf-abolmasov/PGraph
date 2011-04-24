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
                                       record.value("TEXT").toString(),
                                       QFont(record.value("FONT").toString()));
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
        query.prepare("INSERT INTO commentpic (PROJECT_ID, NAMEPR, TEXT, FONT, X, Y) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :TEXT, :FONT, :X, :Y)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":X",          comment->x);
        query.bindValue(":Y",          comment->y);
        query.bindValue(":TEXT",       comment->text);
        query.bindValue(":FONT",       comment->font.toString());
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
            if (actorList.at(i)->varAccModeList.at(j) == QObject::tr("Исходный"))
                vaMode = "I";
            else if (actorList.at(i)->varAccModeList.at(j) ==  QObject::tr("Модифицируемый"))
                vaMode = "M";
            else if (actorList.at(i)->varAccModeList.at(j) ==  QObject::tr("Вычисляемый"))
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
                                   query1.value(6).toString().isEmpty() ? Actor::InlineType : Actor::NormalType,
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
    if (namepr.isEmpty()) return NULL;
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
        actorType = query1.value(5).toString().isEmpty() ? Actor::InlineType : Actor::NormalType;
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
            query.bindValue(":MODE", "I");
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
                                      query1.value(6).toString().isEmpty() ? Predicate::inlineType : Predicate::normalType,
                                      query1.value(6).toString(),
                                      myVariableList));
    }
    db.close();
    return (db.lastError().type() == QSqlError::NoError);
}

Predicate* DataBaseManager::getPredicate(QString namepr)
{
    if (namepr.isEmpty()) return NULL;
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
                         query1.value(5).toString().isEmpty() ? Predicate::inlineType : Predicate::normalType,
                         query1.value(5).toString(),
                         myVariableList);
}

bool DataBaseManager::registerModule(const QString &uniqName, const QString &fileName, const QString &comment, QStringList &paramList)
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
        query2.prepare("SELECT TYPE, DATA, MODE, COMMENT FROM databaz WHERE PROTOTIP = :PROTOTIP AND PROJECT_ID = :PROJECT_ID order by NEV;");
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

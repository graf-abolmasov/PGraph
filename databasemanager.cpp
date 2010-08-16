#include "databasemanager.h"
#include "qdiagramscene.h"
#include <QSqlDatabase>
#include <QSqlQuery>
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
#include "commonutils.h"
#include "logger.h"

DataBaseManager* globalDBManager;

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    globalLogger->writeLog(db.lastError().text());
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("graph3");
    db.setUserName("root");
    db.setPassword("Marina");

    myProgectId = 1;

}

int DataBaseManager::getGraph(QString extName, Graph &graph)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("SELECT ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, "
                  " Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred, ArcType "
                  " FROM PROJECT pr JOIN ACTOR ac ON pr.PROJECT_ID = ac.PROJECT_ID "
                  " JOIN GRAPHPIC grp ON ac.NAMEPR = grp.NAMEPR "
                  " WHERE ac.EXTNAME = :EXTNAME AND ac.CLASPR = 'g' AND pr.PROJECT_ID = :PROJECT_ID"
                  " ORDER BY ELTYP DESC;");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":EXTNAME", extName);
    query.exec();

    while (query.next()){
        if (query.value(0).toString() == "T"){
            Top* top = new Top(query.value(2).toFloat(),
                               query.value(3).toFloat(),
                               query.value(4).toDouble(),
                               query.value(5).toDouble(),
                               query.value(6).toInt(),
                               -1,
                               query.value(7).toBool(),
                               query.value(8).toString(),
                               "T");
            graph.topList.append(top);
        }

        if (query.value(0).toString() == "M"){
            Top* top = new Top(query.value(2).toFloat(),
                               query.value(3).toFloat(),
                               -1,
                               -1,
                               query.value(6).toInt(),
                               10,
                               query.value(7).toBool(),
                               query.value(8).toString(),
                               "M");
            graph.topList.append(top);
        }

        if (query.value(0).toString() == "A"){
            Arc::ArcType arcType;
            if (query.value(14).toString() == QObject::tr("S"))
                arcType = Arc::SerialArc;
            if (query.value(14).toString() == QObject::tr("P"))
                arcType = Arc::ParallelArc;
            if (query.value(14).toString() == QObject::tr("T"))
                arcType = Arc::TerminateArc;
            QStringList lines = query.value(9).toString().split(";;");
            Arc* arc = new Arc(arcType,
                               query.value(10).toInt(),
                               query.value(11).toInt(),
                               query.value(12).toInt(),
                               query.value(13).toString(),
                               lines);
            graph.arcList.append(arc);
        }

        if (query.value(0).toString() == "C"){
            Comment* comment = new Comment(query.value(2).toFloat(), query.value(3).toFloat(), query.value(1).toString());
            graph.commentList.append(comment);
        }
    }
    db.close();
    return db.lastError().number();
}

bool DataBaseManager::saveGraph(Graph *graph)
{
    if (!db.open()) return false;
    QSqlQuery query;

    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP, :BAZIS)");

    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph->extName);
    query.bindValue(":PROTOTIP",  NULL);
    query.bindValue(":BAZIS",  NULL);
    query.exec();

    foreach (Top* top, graph->topList){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred, ArcType) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, NULL, NULL, NULL, NULL, NULL, NULL)");

        query.bindValue(":PROJECT_ID",  myProgectId);
        query.bindValue(":NAMEPR",      graph->name);
        query.bindValue(":ELTYP",       top->type);
        query.bindValue(":X",           top->x);
        query.bindValue(":Y",           top->y);
        query.bindValue(":ISTR",        QString::number(top->x) + " " +
                                        QString::number(top->y) + " " +
                                        QString::number(top->number));
        query.bindValue(":SizeX",       top->sizeX);
        query.bindValue(":SizeY",       top->sizeY);
        query.bindValue(":ntop",        top->number);
        query.bindValue(":isRoot",      top->isRoot ? 1 : 0 );
        query.bindValue(":Actor",       top->actor);
        query.exec();
    }

    foreach(Comment* comment, graph->commentList){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred, ArcType) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":ELTYP",      "C");
        query.bindValue(":X",          comment->x);
        query.bindValue(":Y",          comment->y);
        query.bindValue(":ISTR",       comment->text);
        query.exec();
    }

    foreach (Arc* arc, graph->arcList){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred, ArcType) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL, :Nodes, :ArcPrior, :ArcFromTop, :ArcToTop, :ArcPred, :ArcType)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":ELTYP",      "A");
        query.bindValue(":ISTR",       arc->lines.join(" "));
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
        query.bindValue(":ArcType",     arcType);
        query.bindValue(":Nodes",       arc->lines.join(";;"));
        query.bindValue(":ArcPrior",    arc->priority);
        query.bindValue(":ArcFromTop",  arc->startTop);
        query.bindValue(":ArcToTop",    arc->endTop);
        query.bindValue(":ArcPred",     arc->predicate);
        query.exec();
    }
    db.close();
    return db.lastError().number();
}

bool DataBaseManager::updateGraph(Graph *graph)
{
    if (!db.open()) return false;
    QSqlQuery query;
    query.prepare("DELETE FROM ACTOR WHERE PROJECT_ID = :PROJECT_ID AND NAMEPR = :NAMEPR;");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":NAMEPR", graph->name);
    query.exec();
    db.close();
    return saveGraph(graph);
}

int DataBaseManager::getGraphList(QList<Graph* > &graphList)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("SELECT NAMEPR, EXTNAME FROM ACTOR WHERE CLASPR = 'g' AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.exec();
    while(query.next()){
        QList<Top* > topList;
        QList<Arc* > arcList;
        QList<Comment* > commentList;
        QList<QSyncArc* > syncArcList;
        QList<QMultiProcTop* > multiProcTopList;
        graphList.append(new Graph(query.value(0).toString(), query.value(1).toString(), topList, arcList, commentList, syncArcList, multiProcTopList));
    }
    db.close();
    return db.lastError().number();
}

int DataBaseManager::saveDataTypeList(QList<DataType*>& typeList){
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("DELETE FROM TYPSYS;");
    query.exec();
    for (int i = 0; i < typeList.count(); i++){
        query.prepare("INSERT INTO TYPSYS (PROJECT_ID, TYPE, TYPEDEF)"
                      "VALUES (:PROJECT_ID, :TYPE, :TYPEDEF);");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":TYPE", typeList.at(i)->name);
        query.bindValue(":TYPEDEF", typeList.at(i)->typedefStr);
        query.exec();
        query.clear();
    }
    db.close();
    return db.lastError().number();
}

int DataBaseManager::getDataTypeList(QList<DataType*>& typeList){
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    typeList.clear();
    query.prepare("SELECT PROJECT_ID, TYPE, TYPEDEF FROM TYPSYS;");
    query.exec();
    while (query.next()){
        typeList.append(new DataType(query.value(1).toString(), query.value(2).toString()));
    }
    db.close();
    return db.lastError().number();
}

bool DataBaseManager::saveVariableList(QList<Variable*>& varList){
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("DELETE FROM DATA;");
    query.exec();
    for (int i = 0; i < varList.count(); i++){
        query.prepare("INSERT INTO DATA (PROJECT_ID, DATA, TYPE, INIT, COMMENT)"
                      "VALUES (:PROJECT_ID, :DATA, :TYPE, :INIT, :COMMENT);");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":DATA", varList.at(i)->name);
        query.bindValue(":TYPE", varList.at(i)->type);
        query.bindValue(":INIT", varList.at(i)->initValue.toString());
        query.bindValue(":COMMENT", varList.at(i)->comment);
        query.exec();
        query.clear();
    }
    db.close();
    return (db.lastError().number() == QSqlError::NoError);
}

int DataBaseManager::getVariableList(QList<Variable* >& varList){
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("SELECT DATA, TYPE, INIT, COMMENT FROM DATA;");
    query.exec();
    while (query.next()){
        varList.append(new Variable(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString()));
    }
    db.close();
    return db.lastError().number();
}

int DataBaseManager::saveActorList(QList<Actor *> &actorList)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query1;
    for (int i = 0; i < actorList.count(); i++){
        query1.clear();
        query1.prepare("INSERT INTO ACTOR (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :ICON, :PROTOTIP);");
        query1.bindValue(":PROJECT_ID", myProgectId);
        query1.bindValue(":NAMEPR", actorList.at(i)->name);
        query1.bindValue(":CLASPR", "a");
        query1.bindValue(":EXTNAME", actorList.at(i)->extName);
        query1.bindValue(":ICON", NULL);
        query1.bindValue(":PROTOTIP", actorList.at(i)->baseModule);
        query1.exec();

        QSqlQuery query2;
        for (int j = 0; j < actorList.at(i)->variableList.count(); j++){
            query2.clear();
            query2.prepare("INSERT INTO PASPORT (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query2.bindValue(":PROJECT_ID", myProgectId);
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
        }
    }
    db.close();
    return db.lastError().number();
}

int DataBaseManager::getActorList(QList<Actor *> &actorList)
{
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE CLASPR = 'a';");
    query1.exec();
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM PASPORT WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        query2.exec();
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
                                   query1.value(6).toString() == "" ? Actor::inlineType : Actor::normalType,
                                   query1.value(6).toString(),
                                   myVariableList,
                                   myVAList));
        query2.clear();
    }
    db.close();
    return db.lastError().number();
}

Actor* DataBaseManager::getActor(QString namepr)
{
    if (namepr == "") return NULL;
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE NAMEPR = :NAMEPR;");
    query1.bindValue(":NAMEPR", namepr);
    query1.exec();
    if(!query1.first()){
        db.close();
        return NULL;
    }

    QSqlQuery query2;
    query2.prepare("SELECT NEV, DATA, MODE FROM PASPORT WHERE NAMEPR = :NAMEPR ORDER BY NEV");
    query2.bindValue(":NAMEPR", namepr);
    query2.exec();
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
    Actor* newActor = new Actor(namepr,
                               query1.value(1).toString(),
                               query1.value(5).toString() == "" ? Actor::inlineType : Actor::normalType,
                               query1.value(5).toString(),
                               myVariableList,
                               myVAList);
    db.close();
    return newActor;
}

int DataBaseManager::savePredicateList(QList<Predicate *> &predList)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query1;
    for (int i = 0; i < predList.count(); i++){
        query1.clear();
        query1.prepare("INSERT INTO ACTOR (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), :ICON, :PROTOTIP);");
        query1.bindValue(":PROJECT_ID", myProgectId);
        query1.bindValue(":NAMEPR", predList.at(i)->name);
        query1.bindValue(":CLASPR", "p");
        query1.bindValue(":EXTNAME", predList.at(i)->extName);
        query1.bindValue(":ICON", NULL);
        query1.bindValue(":PROTOTIP", predList.at(i)->baseModule);
        query1.exec();

        QSqlQuery query2;
        for (int j = 0; j < predList.at(i)->variableList.count(); j++){
            query2.clear();
            query2.prepare("INSERT INTO PASPORT (PROJECT_ID, NAMEPR, NEV, DATA, MODE)"
                           "VALUES (:PROJECT_ID, :NAMEPR, :NEV, :DATA, :MODE);");
            query2.bindValue(":PROJECT_ID", myProgectId);
            query2.bindValue(":NAMEPR", predList.at(i)->name);
            query2.bindValue(":NEV", j);
            query2.bindValue(":DATA", predList.at(i)->variableList.at(j)->name);
            query2.bindValue(":MODE", "i");
            query2.exec();
        }
    }
    db.close();
    return 0;
}

int DataBaseManager::getPredicateList(QList<Predicate *> &predList)
{
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE CLASPR = 'p';");
    query1.exec();
    while (query1.next()){
        query2.prepare("SELECT NEV, DATA, MODE FROM PASPORT WHERE NAMEPR = :NAMEPR ORDER BY NEV");
        query2.bindValue(":NAMEPR", query1.value(0).toString());
        query2.exec();
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
        query2.clear();
    }
    db.close();
    return db.lastError().number();
}

Predicate* DataBaseManager::getPredicate(QString namepr)
{
    if (namepr == "") return NULL;
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query1.bindValue(":NAMEPR", namepr);
    query1.bindValue(":PROJECT_ID", myProgectId);
    query1.exec();

    if (!query1.first()) {
        db.close();
        return NULL;
    }

    QSqlQuery query2;
    query2.prepare("SELECT NEV, DATA, MODE FROM PASPORT WHERE NAMEPR = :NAMEPR ORDER BY NEV");
    query2.bindValue(":NAMEPR", namepr);
    query2.exec();
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

int DataBaseManager::registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("INSERT INTO BAZMOD (PROJECT_ID, PROTOTIP, NAMEPR, COMMENT)"
                  "VALUES (:PROJECT_ID, :PROTOTIP, :NAMEPR, :COMMENT);");
    query.bindValue(":PROJECT_ID", 1);
    query.bindValue(":PROTOTIP", uniqName);
    query.bindValue(":NAMEPR",   fileName);
    query.bindValue(":COMMENT",  comment);
    query.exec();

    for (int i = 0; i < paramList.count(); i++){
        query.clear();
        query.prepare("INSERT INTO DATABAZ (PROJECT_ID, PROTOTIP, DATA, TYPE, MODE, COMMENT)"
                      "VALUES (:PROJECT_ID, :PROTOTIP, :DATA, :TYPE, :MODE, :COMMENT);");
        QStringList parameter = paramList.at(i).split(";;");
        query.bindValue(":PROJECT_ID",  myProgectId);
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
    }

    db.close();
    return db.lastError().number();
}

int DataBaseManager::getRegisteredModules(QList<BaseModule*> &moduleList)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query1;
    QSqlQuery query2;
    query1.prepare("SELECT PROTOTIP, NAMEPR, COMMENT FROM BAZMOD;");
    query1.exec();
    while (query1.next()){
        query2.prepare("SELECT TYPE, DATA, MODE, COMMENT FROM DATABAZ WHERE PROTOTIP = :PROTOTIP;");
        query2.bindValue(":PROTOTIP", query1.value(0).toString());
        query2.exec();
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
    return db.lastError().number();
}

int DataBaseManager::saveStruct(Graph *graph)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    QStringList predicateList;
    int i = 0;

    query.prepare("DELETE FROM GRAPH WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProgectId);
    query.exec();
    query.prepare("DELETE FROM GRAPHTOP WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProgectId);
    query.exec();
    query.prepare("DELETE FROM GRAPHPRE WHERE NAMEPR = :NAMEPR AND PROJECT_ID = :PROJECT_ID;");
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":PROJECT_ID", myProgectId);
    query.exec();

    foreach (Arc* arc, graph->arcList){
        if (!predicateList.contains(arc->predicate)){
            predicateList.append(arc->predicate);
            query.prepare("INSERT INTO GRAPHPRE (PROJECT_ID, NAMEPR, NPRED, NAME)"
                          "VALUES (:PROJECT_ID, :NAMEPR, :NPRED, :NAME)");
            query.bindValue(":PROJECT_ID", myProgectId);
            query.bindValue(":NAMEPR", graph->name);
            query.bindValue(":NPRED", i);
            query.bindValue(":NAME", arc->predicate);
            query.exec();
        }
        query.prepare("INSERT INTO GRAPH (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
        query.bindValue(":PROJECT_ID", myProgectId);
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
    }

    int rootTop = -1;
    foreach (Top* top, graph->topList){
        query.prepare("INSERT INTO GRAPHTOP (PROJECT_ID, NAMEPR, NTOP, NAME)"
                      "VALUES (:PROJECT_ID, :NAMEPR, :NTOP, :NAME);");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR", graph->name);
        query.bindValue(":NTOP", top->number);
        query.bindValue(":NAME", top->actor);
        if (top->isRoot)
            rootTop = top->number;
        query.exec();
    }
    query.prepare("INSERT INTO GRAPH (PROJECT_ID, NAMEPR, NFROM, NTO, NPRED, PRIOR, ARCTYPE)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :NFROM, :NTO, :NPRED, :PRIOR, :ARCTYPE);");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":NFROM",rootTop);
    query.bindValue(":NTO", 0);
    query.bindValue(":NPRED", 0);
    query.bindValue(":PRIOR", 0);
    query.bindValue(":ARCTYPE", 0);
    query.exec();
    db.close();

    return db.lastError().number();
}

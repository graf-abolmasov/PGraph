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

DataBaseManager* globalDBManager;

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("graph3");
    db.setUserName("root");
    db.setPassword("Marina");

    myProgectId = 1;

}

int DataBaseManager::getLastInsertID()
{
    QSqlQuery query;
    query.exec("SELECT LAST_INSERT_ID()");
    QString text = query.lastError().text();
    query.next();
    return query.value(0).toInt();;
}

int DataBaseManager::getGraph(QString extName, Graph &graph)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("SELECT ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, "
                  " Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred "
                  " FROM PROJECT pr JOIN ACTOR ac ON pr.PROJECT_ID = ac.PROJECT_ID "
                  " JOIN GRAPHPIC grp ON ac.NAMEPR = grp.NAMEPR "
                  " WHERE ac.EXTNAME = :EXTNAME AND ac.CLASPR = 'g' AND pr.PROJECT_ID = :PROJECT_ID"
                  " ORDER BY ELTYP DESC;");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":EXTNAME", extName);
    query.exec();

    while (query.next()){
        if (query.value(0).toString() == "T"){
            Top* top = new Top(query.value(2).toFloat() + query.value(4).toFloat()/2,
                               query.value(3).toFloat() + query.value(5).toFloat()/2,
                               query.value(4).toDouble(),
                               query.value(5).toDouble(),
                               query.value(6).toInt(),
                               query.value(7).toBool(),
                               query.value(8).toString());
            graph.topList.append(top);
        }

        if (query.value(0).toString() == "A"){
            Arc::ArcType arcType;
            if (query.value(1).toString() == "S")
                arcType = Arc::SerialArc;
            if (query.value(1).toString() == "P")
                arcType = Arc::ParallelArc;
            if (query.value(1).toString() == "T")
                arcType = Arc::TerminateArc;
            QStringList lines = query.value(9).toString().split(";;");
            Arc* arc = new Arc(arcType,
                               query.value(10).toInt(),
                               query.value(11).toInt(),
                               query.value(12).toInt(),
                               query.value(14).toString(),
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


int DataBaseManager::saveGraph(Graph *graph)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    /*query.prepare("INSERT INTO project (project_name) "
           "VALUES (:project_name)");
    query.bindValue(":project_name", projectName);
    query.exec();

    myProgectId =  getLastInsertID();*/
    graph->name = "G" + getCRC(graph->extName.toUtf8());
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP, :BAZIS)");

    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":NAMEPR", graph->name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph->extName);
    query.bindValue(":PROTOTIP",  NULL);
    query.bindValue(":BAZIS",  NULL);
    query.exec();

    /*query.prepare("UPDATE ACTOR SET NAMEPR = CONCAT(CLASPR,AUTOINCREMENT_NAMEPR)"
           "WHERE PROJECT_ID = :PROJECT_ID");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.exec();*/

    /*query.prepare("SELECT NAMEPR FROM ACTOR WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = :CLASPR");
    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":CLASPR", "g");
    query.exec();

    query.next();

    QString namepr = query.value(0).toString();*/

    //QList<QArc*> arcList = graph->arcList;
    //QList<QComment*> commentList = graph->commentList;

    foreach (Top* top, graph->topList){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, NULL, NULL, NULL, NULL, NULL)");

        query.bindValue(":PROJECT_ID",  myProgectId);
        query.bindValue(":NAMEPR",      graph->name);
        query.bindValue(":ELTYP",       "T");
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
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":ELTYP",      "C");
        query.bindValue(":X",          comment->x);
        query.bindValue(":Y",          comment->y);
        query.bindValue(":ISTR",       comment->text);
        query.exec();
    }

    foreach (Arc* arc, graph->arcList){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL, :Nodes, :ArcPrior, :ArcFromTop, :ArcToTop, :ArcPred)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     graph->name);
        query.bindValue(":ELTYP",      "A");
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
        query.bindValue(":ISTR",        arcType);
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

int DataBaseManager::saveVariableList(QList<Variable*>& varList){
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
    return db.lastError().number();
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
            if (actorList.at(i)->varAMList.at(j) == "Исходный")
                vaMode = "I";
            else if (actorList.at(i)->varAMList.at(j) == "Модифицируемый")
                vaMode = "M";
            else if (actorList.at(i)->varAMList.at(j) == "Вычисляемый")
                vaMode = "R";
            query2.bindValue(":MODE", vaMode);
            query2.exec();
        }
    }
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
    return db.lastError().number();
}

Actor* DataBaseManager::getActor(QString namepr)
{
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE NAMEPR = :NAMEPR;");
    query1.bindValue(":NAMEPR", namepr);
    query1.exec();
    query1.first();

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
    return db.lastError().number();
}

Predicate* DataBaseManager::getPredicate(QString namepr)
{
    QList<Variable* > varList;
    getVariableList(varList);
    bool ok = db.open();
    if (!ok) return NULL;
    QSqlQuery query1;
    query1.prepare("SELECT CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP FROM ACTOR WHERE NAMEPR = :NAMEPR;");
    query1.bindValue(":NAMEPR", namepr);
    query1.exec();
    query1.first();

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
        if (parameter.at(2) == "Исходный")
            vaMode = "I";
        else if (parameter.at(2) == "Модифицируемый")
            vaMode = "M";
        else if (parameter.at(2) == "Вычисляемый")
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
            if (query2.value(3).toString() == "I")
                vaMode = QObject::tr("Исходный");
            else if (query2.value(3).toString() == "M")
                vaMode = QObject::tr("Модифицируемый");
            else if (query2.value(3).toString() == "R")
                vaMode = QObject::tr("Вычисляемый");
            parameterList.append(query2.value(0).toString() + ";;" +
                                 query2.value(1).toString() + ";;" +
                                 query2.value(2).toString() + ";;" +
                                 vaMode);
        }
        query2.clear();
        moduleList.append(new BaseModule(query1.value(1).toString(), query1.value(0).toString(), query1.value(2).toString(), parameterList));
    }
    db.close();
    return db.lastError().number();
}

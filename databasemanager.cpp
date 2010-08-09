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

int DataBaseManager::getGraph(QString projectName, QDiagramScene* scene, QMenu* arcMenu, QMenu* topMenu, QMenu* syncArcMenu)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    query.prepare("SELECT ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, "
                  " Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred "
                  " FROM PROJECT pr JOIN ACTOR ac ON pr.PROJECT_ID = ac.PROJECT_ID "
                  " JOIN GRAPHPIC grp ON ac.NAMEPR = grp.NAMEPR "
                  " WHERE pr.PROJECT_NAME = :PROJECT_NAME AND ac.CLASPR = 'g'"
                  " ORDER BY ELTYP DESC");
    query.bindValue(":PROJECT_NAME", projectName);
    query.exec();

    while (query.next()){
        QString a  = query.value(0).toString();

        if (query.value(0).toString() == "T"){
            QTop *top = new QTop(topMenu, 0, scene);
            top->setPos(QPointF(query.value(2).toFloat() + query.value(4).toFloat()/2, query.value(3).toFloat() + query.value(5).toFloat()/2));
            top->number = query.value(6).toInt();
            top->isRoot = query.value(7).toBool();

            QPolygonF myPolygon;
            double w = query.value(4).toDouble();
            double h = query.value(5).toDouble();
            myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
                    << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
                    << QPointF(-w/2, h/2);
            top->setPolygon(myPolygon);

            top->actor = getActor(query.value(8).toString());
        }

        if (query.value(0).toString() == "A"){
            QString nodes = query.value(9).toString();
            QStringList list = nodes.split(" ");
            QTop *startTop = NULL;
            QTop *endTop = NULL;
            for (int i = 0; i < scene->items().count(); i++){
                if (scene->items().at(i)->type() == QTop::Type){
                    //int checkValue = ((QTop *)qgraphicsitem_cast<QTop* >(scene->items().at(i)))->number;
                    if (((QTop *)qgraphicsitem_cast<QTop* >(scene->items().at(i)))->number == query.value(11).toInt()){
                        startTop = qgraphicsitem_cast<QTop* >(scene->items().at(i));
                    }
                    if (((QTop *)qgraphicsitem_cast<QTop* >(scene->items().at(i)))->number == query.value(12).toInt()){
                        endTop = qgraphicsitem_cast<QTop* >(scene->items().at(i));
                    }
                }
            }
            QArc *arc = new QArc(startTop, endTop, arcMenu, NULL, scene);
            int i = 0;
            while(i < list.count() - 3){
                QPointF startPoint = QPointF(list.at(i).toFloat(), list.at(i+1).toFloat());
                QPointF endPoint = QPointF(list.at(i+2).toFloat(), list.at(i+3).toFloat());
                i+=2;

                arc->newLine(startPoint, endPoint);
            }
            arc->addLine(arc->currentLine);
            arc->currentLine = NULL;
            startTop->addArc(arc);
            endTop->addArc(arc);
            arc->setPriority(query.value(10).toInt());
            arc->predicate = getPredicate(query.value(14).toString());
        }

        if (query.value(0).toString() == "C"){
            TComment *comment = new TComment(NULL, NULL, scene);
            comment->setPos(QPointF(query.value(2).toFloat(), query.value(3).toFloat()));
            comment->setPlainText(query.value(8).toString());
        }
    }
    db.close();
    return db.lastError().number();
}


int DataBaseManager::saveGraph(QString extName, QGraph *graph)
{
    bool ok = db.open();
    if (!ok) return db.lastError().number();
    QSqlQuery query;
    /*query.prepare("INSERT INTO project (project_name) "
           "VALUES (:project_name)");
    query.bindValue(":project_name", projectName);
    query.exec();

    myProgectId =  getLastInsertID();*/
    QString namepr = "G" + QString::number(getCRC(extName.toUtf8().data(), sizeof(extName.toUtf8().data())), 16).toUpper();
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP, :BAZIS)");

    query.bindValue(":PROJECT_ID", myProgectId);
    query.bindValue(":NAMEPR", namepr);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  extName);
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

    QList<QTop*> topList = graph->topList;
    QList<QArc*> arcList = graph->arcList;
    QList<TComment*> commentList = graph->commentList;

    for (int i = 0; i < topList.count(); i++){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, NULL, NULL, NULL, NULL, NULL)");

        QPointF p = topList.at(i)->polygon().at(0) + topList.at(i)->scenePos();
        query.bindValue(":PROJECT_ID",  myProgectId);
        query.bindValue(":NAMEPR",      namepr);
        query.bindValue(":ELTYP",       "T");
        query.bindValue(":X",           (p.x()));
        query.bindValue(":Y",           (p.y()));
        query.bindValue(":ISTR",        QString::number((p.x())) + " " +
                                        QString::number((p.y())) + " " +
                                        QString::number(topList.at(i)->number));
        p = (QPointF)(topList.at(i)->polygon().at(1)  - topList.at(i)->polygon().at(0));
        query.bindValue(":SizeX",       p.x());
        p = (QPointF)(topList.at(i)->polygon().at(2)  - topList.at(i)->polygon().at(1));
        query.bindValue(":SizeY",       p.y());
        query.bindValue(":ntop",        topList.at(i)->number);
        query.bindValue(":isRoot",      topList.at(i)->isRoot ? 1 : 0 );
        query.bindValue(":Actor", topList.at(i)->actor->name);
        query.exec();
    }

    for (int i = 0; i < commentList.count(); i++){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     namepr);
        query.bindValue(":ELTYP",      "C");
        query.bindValue(":X",          commentList.at(0)->scenePos().x());
        query.bindValue(":Y",          commentList.at(0)->scenePos().y());
        query.bindValue(":ISTR",       QString::number(commentList.at(0)->scenePos().x()) + " " +
                                       QString::number(commentList.at(0)->scenePos().y()));
        query.exec();
    }


    for (int i = 0; i < arcList.count(); i++){
        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL, :Nodes, :ArcPrior, :ArcFromTop, :ArcToTop, :ArcPred)");
        query.bindValue(":PROJECT_ID", myProgectId);
        query.bindValue(":NAMEPR",     namepr);
        query.bindValue(":ELTYP",      "A");
        query.bindValue(":ISTR",       QString::number(arcList.at(i)->lines.count()) + " " +
                                       " " + " " + QString::number(arcList.at(i)->lines.count() + 1) +
                                       " " + " " + QString::number(arcList.at(i)->startItem()->number) +
                                       " " + QString::number(arcList.at(i)->endItem()->number) +
                                       " " + QString::number(arcList.at(i)->priority()) + " ");


        QString nodes = QString::number(arcList.at(i)->lines.at(0)->line().x1()) + " " +
                        QString::number(arcList.at(i)->lines.at(0)->line().y1()) +
                        " " + QString::number(arcList.at(i)->lines.at(0)->line().x2()) + " " +
                        QString::number(arcList.at(i)->lines.at(0)->line().y2());

        for (int j = 1; j < arcList.at(i)->lines.count(); j++){
            nodes += " " + QString::number(arcList.at(i)->lines.at(j)->line().x1()) + " " +
                     QString::number(arcList.at(i)->lines.at(j)->line().y1()) +
                     " " + QString::number(arcList.at(i)->lines.at(j)->line().x2()) + " " +
                     QString::number(arcList.at(i)->lines.at(j)->line().y2());
        }
        query.bindValue(":Nodes",       nodes);
        query.bindValue(":ArcPrior",    arcList.at(i)->priority());
        query.bindValue(":ArcFromTop",  arcList.at(i)->startItem()->number);
        query.bindValue(":ArcToTop",    arcList.at(i)->endItem()->number);
        query.bindValue(":ArcPred",     arcList.at(i)->predicate->name);
        query.exec();
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
        query.bindValue(":MODE",        parameter.at(2)[0]);
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
            parameterList.append(query2.value(0).toString() + ";;" +
                                 query2.value(1).toString() + ";;" +
                                 query2.value(2).toString() + ";;" +
                                 query2.value(3).toString());
        }
        query2.clear();
        moduleList.append(new BaseModule(query1.value(1).toString(), query1.value(0).toString(), query1.value(2).toString(), parameterList));
    }
    db.close();
    return db.lastError().number();
}

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

DataBaseManager* globalDBManager;

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("graph3");
    db.setUserName("root");
    db.setPassword("Marina");

    //заполняем справочники
    actorListProxy.append(new Actor("aaa", "ACTOR_1", Actor::normalType));
    actorListProxy.append(new Actor("aab", "ACTOR_2", Actor::normalType));
    actorListProxy.append(new Actor("aac", "inlineACTOR_1", Actor::inlineType));
    actorListProxy.append(new Actor("aad", "inlineACTOR_2", Actor::inlineType));

    predListProxy.append(new Predicate("1", "1", Predicate::inlineType));
    predListProxy.append(new Predicate("Pred1", "Сложный предикат 1", Predicate::normalType));
    predListProxy.append(new Predicate("Pred2", "Сложный предикат 2", Predicate::normalType));
}

int DataBaseManager::getLastInsertID()
{
    QSqlQuery query;
    query.exec("SELECT LAST_INSERT_ID()");
    QString text = query.lastError().text();
    query.next();
    return query.value(0).toInt();;
}

void DataBaseManager::getGraph(QString projectName, QDiagramScene* scene, QMenu* arcMenu, QMenu* topMenu, QMenu* syncArcMenu)
{
    bool ok = db.open();
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
        }

        if (query.value(0).toString() == "A"){
            QString nodes = query.value(9).toString();
            QStringList list = nodes.split(" ");
            QTop *startTop = NULL;
            QTop *endTop = NULL;
            for (int i = 0; i < scene->items().count(); i++){
                if (scene->items().at(i)->type() == QTop::Type){
                    int checkValue = ((QTop *)qgraphicsitem_cast<QTop* >(scene->items().at(i)))->number;
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
        }

        if (query.value(0).toString() == "C"){
            TComment *comment = new TComment(NULL, NULL, scene);
            comment->setPos(QPointF(query.value(2).toFloat(), query.value(3).toFloat()));
            comment->setPlainText(query.value(8).toString());
        }
    }
    db.close();
}


void DataBaseManager::saveGraph(QString projectName, QString extName, QGraph *graph)
{
    bool ok = db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO project (project_name) "
           "VALUES (:project_name)");
    query.bindValue(":project_name", projectName);
    query.exec();

    int projectID = -1;
    projectID =  getLastInsertID();
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, ICON, PROTOTIP, BAZIS)"
                  "VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, :PROTOTIP, :BAZIS)");

    query.bindValue(":PROJECT_ID", projectID);
    query.bindValue(":NAMEPR", "temp");
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  extName);
    query.bindValue(":PROTOTIP",  NULL);
    query.bindValue(":BAZIS",  NULL);
    query.exec();

    query.prepare("UPDATE ACTOR SET NAMEPR = CONCAT(CLASPR,AUTOINCREMENT_NAMEPR)"
           "WHERE PROJECT_ID = :PROJECT_ID");
    query.bindValue(":PROJECT_ID", projectID);
    query.exec();

    query.prepare("SELECT NAMEPR FROM ACTOR WHERE PROJECT_ID = :PROJECT_ID AND CLASPR = :CLASPR");
    query.bindValue(":PROJECT_ID", projectID);
    query.bindValue(":CLASPR", "g");
    query.exec();

    query.next();

    QString namepr = query.value(0).toString();

    QList<QTop*> topList = graph->topList;
    QList<QArc*> arcList = graph->arcList;
    QList<TComment*> commentList = graph->commentList;

    for (int i = 0; i < topList.count(); i++)
    {

        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, NULL, NULL, NULL, NULL, NULL, NULL)");

        query.bindValue(":PROJECT_ID", projectID);
        query.bindValue(":NAMEPR", namepr);
        query.bindValue(":ELTYP",  "T");
        QPointF p = topList.at(i)->polygon().at(0) + topList.at(i)->scenePos();
        query.bindValue(":X",  (p.x()));
        query.bindValue(":Y",  (p.y()));
        query.bindValue(":ISTR",  QString::number((p.x())) + " " +
            QString::number((p.y())) + " " + QString::number(topList.at(i)->number));
        p = (QPointF)(topList.at(i)->polygon().at(1)  - topList.at(i)->polygon().at(0));
        query.bindValue(":SizeX",  p.x());
        p = (QPointF)(topList.at(i)->polygon().at(2)  - topList.at(i)->polygon().at(1));
        query.bindValue(":SizeY",  p.y());
        query.bindValue(":ntop",  topList.at(i)->number);
        query.bindValue(":isRoot",  topList.at(i)->isRoot ? 1 : 0 );
        query.exec();

    }

    for (int i = 0; i < commentList.count(); i++)
    {

        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, :X, :Y, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");

        query.bindValue(":PROJECT_ID", projectID);
        query.bindValue(":NAMEPR", namepr);
        query.bindValue(":ELTYP",  "C");


        query.bindValue(":X",  commentList.at(0)->scenePos().x());
        query.bindValue(":Y",  commentList.at(0)->scenePos().y());

        query.bindValue(":ISTR",  QString::number(commentList.at(0)->scenePos().x()) + " " +
            QString::number(commentList.at(0)->scenePos().y()));

        query.exec();
    }


    for (int i = 0; i < arcList.count(); i++)
    {

        query.prepare("INSERT INTO GRAPHPIC (PROJECT_ID, NAMEPR, ELTYP, ISTR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Nodes, ArcPrior, ArcFromTop, ArcToTop, ArcPred) "
                      "VALUES (:PROJECT_ID, :NAMEPR, :ELTYP, :ISTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL, :Nodes, :ArcPrior, :ArcFromTop, :ArcToTop, NULL)");

        query.bindValue(":PROJECT_ID", projectID);
        query.bindValue(":NAMEPR", namepr);
        query.bindValue(":ELTYP",  "A");


        query.bindValue(":ISTR",  QString::number(arcList.at(i)->lines.count()) + " " +
            " " + " " + QString::number(arcList.at(i)->lines.count() + 1) +
            " " + " " + QString::number(arcList.at(i)->startItem()->number) +
            " " + QString::number(arcList.at(i)->endItem()->number) +
            " " + QString::number(arcList.at(i)->priority()) + " ");


        QString nodes = QString::number(arcList.at(i)->lines.at(0)->line().x1()) + " " +
                        QString::number(arcList.at(i)->lines.at(0)->line().y1());
        nodes += " " + QString::number(arcList.at(i)->lines.at(0)->line().x2()) + " " +
                 QString::number(arcList.at(i)->lines.at(0)->line().y2());

        for (int j = 1; j < arcList.at(i)->lines.count(); j++)
        {

            nodes += " " + QString::number(arcList.at(i)->lines.at(j)->line().x1()) + " " +
                     QString::number(arcList.at(i)->lines.at(j)->line().y1());
            nodes += " " + QString::number(arcList.at(i)->lines.at(j)->line().x2()) + " " +
                     QString::number(arcList.at(i)->lines.at(j)->line().y2());

        }

        query.bindValue(":Nodes",  nodes);
        query.bindValue(":ArcPrior",  arcList.at(i)->priority());
        query.bindValue(":ArcFromTop",  arcList.at(i)->startItem()->number);
        query.bindValue(":ArcToTop",  arcList.at(i)->endItem()->number);
        query.exec();
    }
    db.close();
}

int DataBaseManager::saveDataTypeList(QList<DataType*>& typeList){
    bool ok = db.open();
    QSqlQuery query;
    query.prepare("DELETE FROM TYPSYS;");
    query.exec();
    for (int i = 0; i < typeList.count(); i++){
        query.prepare("INSERT INTO TYPSYS (PROJECT_ID, TYPE, TYPEDEF)"
                      "VALUES (:PROJECT_ID, :TYPE, :TYPEDEF);");
        query.bindValue(":PROJECT_ID", i+1);
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
    QSqlQuery query;
    query.prepare("DELETE FROM DATA;");
    query.exec();
    for (int i = 0; i < varList.count(); i++){
        query.prepare("INSERT INTO DATA (PROJECT_ID, DATA, TYPE, INIT, COMMENT)"
                      "VALUES (:PROJECT_ID, :DATA, :TYPE, :INIT, :COMMENT);");
        query.bindValue(":PROJECT_ID", 1);
        query.bindValue(":DATA", varList.at(i)->name);
        query.bindValue(":TYPE", varList.at(i)->type);
        query.bindValue(":INIT", varList.at(i)->initValue.toString());
        query.bindValue(":COMMENT", varList.at(i)->description);
        query.exec();
        query.clear();
    }
    db.close();
    return db.lastError().number();
    return 0;
}

int DataBaseManager::getVariableList(QList<Variable*>& varList){
    bool ok = db.open();
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
    actorListProxy.clear();
    actorListProxy.append(actorList);
}

int DataBaseManager::getActorList(QList<Actor *> &actorList)
{
    actorList = actorListProxy;
    return 0;
}

int DataBaseManager::savePredicateList(QList<Predicate *> &predList)
{
    predListProxy.clear();
    predListProxy.append(predList);
    return 0;
}

int DataBaseManager::getPredicateList(QList<Predicate *> &predList)
{
    predList = predListProxy;
    return 0;
}

int DataBaseManager::registerModule(QString uniqName, QString fileName, QString comment, QStringList &paramList)
{
    bool ok = db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO BAZMOD (PROJECT_ID, PROTOTIP, NAMEPR, COMMENT)"
                  "VALUES (:PROJECT_ID, :PROTOTIP, :NAMEPR, :COMMENT);");
    query.bindValue(":PROJECT_ID", 1);
    query.bindValue(":PROTOTIP", uniqName.mid(0,8));
    query.bindValue(":NAMEPR", fileName);
    query.bindValue(":COMMENT", comment);
    query.exec();

    QString error = query.lastQuery();
    int error1 = db.lastError().number();
    //if (db.lastError().number() != QSqlError::NoError)
    //    return 1; //Модуль уже зарегистрирован


    for (int i = 0; i < paramList.count(); i++){
        query.clear();
        query.prepare("INSERT INTO DATABAZ (PROJECT_ID, PROTOTIP, DATA, TYPE, MODE, COMMENT)"
                      "VALUES (:PROJECT_ID, :PROTOTIP, :DATA, :TYPE, :MODE, :COMMENT);");
        QStringList parameter = paramList.at(i).split(";;");
        query.bindValue(":PROJECT_ID", 1);
        query.bindValue(":PROTOTIP", uniqName.mid(0,8));
        query.bindValue(":DATA", parameter.at(0));
        query.bindValue(":TYPE", parameter.at(1));
        query.bindValue(":MODE", parameter.at(2)[0]);
        query.bindValue(":COMMENT", parameter.at(3));
        query.exec();
    }

    db.close();
}

int DataBaseManager::getRegisteredModules(QStringList &moduleList)
{
    bool ok = db.open();
    QSqlQuery query;
    query.prepare("SELECT NAMEPR FROM BAZMOD;");
    query.exec();
    while (query.next())
        moduleList.append(query.value(0).toString());
    db.close();
}

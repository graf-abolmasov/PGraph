#include "../../src/common/DAO/graphdao.h"
#include "../../src/common/VO/graphvo.h"
#include "../../src/common/globalvariables.h"

GraphDAO::GraphDAO(const QSqlDatabase &db)
    :BaseDAO(db, "actor")
{
}

void GraphDAO::persist(const Graph &graph)
{
    openDb();

    const int myProjectId = globalDBManager->getProjectId();
    QSqlQuery query;
    query.prepare("INSERT INTO actor (PROJECT_ID, NAMEPR, CLASPR, EXTNAME, DATE, TIME, PROTOTIP, BAZIS) VALUES (:PROJECT_ID, :NAMEPR, :CLASPR, :EXTNAME, CURDATE(), CURTIME(), NULL, NULL)");
    query.bindValue(":PROJECT_ID", myProjectId);
    query.bindValue(":NAMEPR", graph.name);
    query.bindValue(":CLASPR",  "g");
    query.bindValue(":EXTNAME",  graph.extName);
    execQuery(query);

    foreach (Top top, graph.topList) {
        query.prepare("INSERT INTO toppic (PROJECT_ID, NAMEPR, X, Y, SizeX, SizeY, ntop, isRoot, Actor, Type) VALUES (:PROJECT_ID, :NAMEPR, :X, :Y, :SizeX, :SizeY, :ntop, :isRoot, :Actor, :Type)");
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
        execQuery(query);
    }

    foreach(Comment comment, graph.commentList) {
        query.prepare("INSERT INTO commentpic (PROJECT_ID, NAMEPR, TEXT, FONT, X, Y) VALUES (:PROJECT_ID, :NAMEPR, :TEXT, :FONT, :X, :Y)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph.name);
        query.bindValue(":X",          comment.x);
        query.bindValue(":Y",          comment.y);
        query.bindValue(":TEXT",       comment.text);
        query.bindValue(":FONT",       comment.font.toString());
        execQuery(query);
    }

    foreach (Arc arc, graph.arcList){
        query.prepare("INSERT INTO arcpic (PROJECT_ID, NAMEPR, Nodes, Priority, FromTop, ToTop, Predicate, Type) VALUES (:PROJECT_ID, :NAMEPR, :Nodes, :Priority, :FromTop, :ToTop, :Predicate, :Type)");
        query.bindValue(":PROJECT_ID", myProjectId);
        query.bindValue(":NAMEPR",     graph.name);
        QString arcType;
        switch (arc.type){
        case Arc::ParallelArc:
            arcType = "P";
            break;
        case Arc::TerminateArc:
            arcType = "T";
            break;
        default:
            arcType = "S";
            break;
        }
        query.bindValue(":Type",      arcType);
        query.bindValue(":Nodes",     arc.lines.join(";;"));
        query.bindValue(":Priority",  arc.priority);
        query.bindValue(":FromTop",   arc.startTop);
        query.bindValue(":ToTop",     arc.endTop);
        query.bindValue(":Predicate", arc.predicate == NULL ? "" : arc.predicate->name);
        execQuery(query);
    }
    myDb.close();
}

void GraphDAO::persist(const Graph *graph)
{
    persist(*graph);
}

QList<Graph> GraphDAO::findAll(bool loadContent)
{
    openDb();
    QSqlQuery query;
    query.prepare("SELECT NAMEPR, EXTNAME, ICON FROM actor WHERE CLASPR = 'g' AND PROJECT_ID = :PROJECT_ID ORDER BY EXTNAME;");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    execQuery(query);
    QList<Graph> result;
    while(query.next()){
        if (loadContent) {
            result.append(findByName(query.value(0).toString()));
        } else {
            QPixmap p;
            p.loadFromData(query.value(2).toByteArray(), "PNG");
            Graph newGraph(query.value(0).toString(), query.value(1).toString(), QList<Top>(), QList<Arc>(), QList<Comment>(), QList<SyncArc>(), p);
            result.append(newGraph);
        }
    }
    myDb.close();
    return result;

}

Graph GraphDAO::findByName(const QString &name)
{
    openDb();
    QSqlQuery query;
    //получаем список вершин
    query.prepare("SELECT *, SizeX AS procCount FROM toppic WHERE toppic.NAMEPR=:NAMEPR AND toppic.PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":NAMEPR", name);
    execQuery(query);

    QList<Top> topList;
    while (query.next()) {
        QSqlRecord record = query.record();
        const Actor *actor = globalDBManager->getActor(record.value("Actor").toString());
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
    query.prepare("SELECT * FROM arcpic WHERE NAMEPR=:NAMEPR AND PROJECT_ID=:PROJECT_ID ");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":NAMEPR", name);
    execQuery(query);

    QList<Arc> arcList;
    while (query.next()) {
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
                           globalDBManager->getPredicate(record.value("Predicate").toString()),
                           lines));
    }

    //получаем список комментариев
    query.prepare("SELECT * FROM commentpic WHERE NAMEPR=:NAMEPR AND PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":NAMEPR", name);
    execQuery(query);

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
    query.prepare("SELECT NAMEPR, EXTNAME FROM actor WHERE NAMEPR=:NAMEPR AND PROJECT_ID=:PROJECT_ID");
    query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    query.bindValue(":NAMEPR", name);
    execQuery(query);
    query.next();
    QSqlRecord record = query.record();
    Graph result(record.value("NAMEPR").toString(), record.value("EXTNAME").toString(), topList, arcList, commentList, QList<SyncArc>());
    myDb.close();
    return result;
}

void GraphDAO::remove(const Graph &graph)
{
    remove(graph.name);
}

void GraphDAO::remove(const QString &name)
{
    openDb();
    QMap<QString, QVariant> where;
    where["NAMEPR"] = name;
    QSqlQuery q = prepareDelete(where);
    execQuery(q);
}

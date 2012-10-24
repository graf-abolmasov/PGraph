#include <QtCore>
#include <QtGui>

#include "undocommand.h"
#include "../../src/common/commonutils.h"
#include "qarc.h"
#include "qsyncarc.h"
#include "qcomment.h"
#include "qarcline.h"

QString itemTypeToString(const QGraphicsItem *item)
{
    switch (item->type()) {
    case QTop::Type:
        return QObject::tr("вершина");
    case QArc::Type:
    case QArcLine::Type:
        return QObject::tr("дуга");
    case QComment::Type:
        return QObject::tr("комментарий");
    case QSyncArc::Type:
        return QObject::tr("синхронизация");
    }
    return "undefined";
}

AddCommand::AddCommand(QGraphicsItem *item, QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    myItem = item;
    setText(QObject::tr("+ ") + itemTypeToString(item));
}

void AddCommand::undo()
{
    QArc* arc = NULL;
    QSyncArc* sync = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
        break;
    case QSyncArc::Type:
        sync = qgraphicsitem_cast<QSyncArc* >(myItem);
        sync->startItem()->removeSync(sync);
        sync->endItem()->removeSync(sync);
        break;
    }

    myGraphicsScene->removeItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

void AddCommand::redo()
{
    QArc* arc = NULL;
    QSyncArc* sync = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->addArc(arc);
        arc->endItem()->addArc(arc);
//        foreach(QArc *qarc, arc->startItem()->outArcs())
//            qarc->setPriority(qarc->priority() + 1);
        arc->setPriority(1);
        break;
    case QSyncArc::Type:
        sync = qgraphicsitem_cast<QSyncArc* >(myItem);
        sync->startItem()->addSync(sync);
        sync->endItem()->addSync(sync);
        break;
    }
    myGraphicsScene->addItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

AddCommand::~AddCommand()
{
    delete myItem;
}

DeleteCommand::DeleteCommand(QGraphicsItem *item, QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    myItem = item;
    setText(QObject::tr("- ") + itemTypeToString(item));
}

void DeleteCommand::undo()
{
    QArc* arc = NULL;
    QSyncArc* sync = NULL;
    QTop* top = NULL;
    switch (myItem->type()) {
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        /*foreach(QArc *qarc, arc->startItem()->outArcs())
            if (qarc->priority() >= arc->priority())
                qarc->setPriority(qarc->priority() + 1);*/
        arc->startItem()->addArc(arc);
        arc->endItem()->addArc(arc);
        break;
    case QSyncArc::Type:
        sync = qgraphicsitem_cast<QSyncArc* >(myItem);
        sync->startItem()->addSync(sync);
        sync->endItem()->addSync(sync);
        break;

    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(myItem);
        foreach (QArc* arc, arcs) {
            arc->startItem()->addArc(arc);
            arc->endItem()->addArc(arc);
            myGraphicsScene->addItem(arc);
        }
        foreach (QSyncArc* sync, syncs) {
            sync->startItem()->addSync(sync);
            sync->endItem()->addSync(sync);
            myGraphicsScene->addItem(sync);
        }
    }
    myGraphicsScene->addItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

void DeleteCommand::redo()
{
    QArc* arc = NULL;
    QSyncArc* sync = NULL;
    QTop* top = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
        break;
    case QSyncArc::Type:
        sync = qgraphicsitem_cast<QSyncArc* >(myItem);
        sync->startItem()->removeSync(sync);
        sync->endItem()->removeSync(sync);
        break;
    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(myItem);
        arcs = top->allArcs();
        syncs = top->allSync();
        top->removeArcs();
        top->removeSyncs();
        foreach (QArc* arc, arcs) {
            arc->endItem()->removeArc(arc);
            arc->startItem()->removeArc(arc);
            myGraphicsScene->removeItem(arc);
        }
        foreach (QSyncArc* sync, syncs) {
            sync->endItem()->removeSync(sync);
            sync->startItem()->removeSync(sync);
            myGraphicsScene->removeItem(sync);
        }
    }
    myGraphicsScene->removeItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

DeleteCommand::~DeleteCommand()
{

}

MoveCommand::MoveCommand(QList<QGraphicsItem *>items, QGraphicsScene *graphicsScene, QLineF vector, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = graphicsScene;
    myItems = items;
    myDisplacementVector = vector;
    setText(QObject::tr("> ") + itemTypeToString(items.first()) + " (" + QString::number(myDisplacementVector.p1().x()) + ", " + QString::number(myDisplacementVector.p1().y()) + ")->" +
            "(" + QString::number(myDisplacementVector.p2().x()) + ", " + QString::number(myDisplacementVector.p2().y()) + ")");
}

void MoveCommand::undo()
{
    //если элемент один то это либо один комментарий, либо одна вершина
    if (myItems.count() == 1) {
        switch (myItems.first()->type()) {
        case QTop::Type: {
                QTop* top = qgraphicsitem_cast<QTop* >(myItems.first());
                top->moveBy(-myDisplacementVector.dx(), -myDisplacementVector.dy());
            }
            break;
        case QComment::Type:
            myItems.first()->moveBy(-myDisplacementVector.dx(), -myDisplacementVector.dy());
            break;
        }
    }
    //несколько м.б. только вершин
    else if (myItems.count() > 1) {

        QSet<QArc* > arcs;
        //собираем все дуги
        foreach (QGraphicsItem* item, myItems) {
            QTop* top = qgraphicsitem_cast<QTop* >(item);
            arcs.unite(top->allArcs().toSet());
        }
        //замораживаем дуги у которых перемещаются обе вершины
        foreach (QArc* arc, arcs)
            if (myItems.contains(arc->startItem()) && myItems.contains(arc->endItem()))
                arc->freeze();
        //перемещаем вершины
        foreach (QGraphicsItem* item, myItems) {
            QTop* top = qgraphicsitem_cast<QTop* >(item);
            top->moveBy(-myDisplacementVector.dx(), -myDisplacementVector.dy());
        }
        //размораживаем все дуги
        foreach (QArc* arc, arcs)
            arc->unfreeze();
    }
}

void MoveCommand::redo()
{
    //если элемент один то это либо один комментарий, либо одна вершина
    if (myItems.count() == 1) {
        switch (myItems.first()->type()) {
        case QTop::Type: {
                QTop* top = qgraphicsitem_cast<QTop* >(myItems.first());
                top->moveBy(myDisplacementVector.dx(), myDisplacementVector.dy());
            }
            break;
        case QComment::Type:
            myItems.first()->moveBy(myDisplacementVector.dx(), myDisplacementVector.dy());
            break;
        }
    }
    //несколько м.б. только вершин
    else if (myItems.count() > 1) {
        QSet<QArc* > arcs;
        //собираем все дуги
        foreach (QGraphicsItem* item, myItems) {
            QTop* top = qgraphicsitem_cast<QTop* >(item);
            arcs.unite(top->allArcs().toSet());
        }
        //замораживаем дуги у которых перемещаются обе вершины
        foreach (QArc* arc, arcs)
            if (myItems.contains(arc->startItem()) && myItems.contains(arc->endItem()))
                arc->freeze();
        //перемещаем вершины
        foreach (QGraphicsItem* item, myItems) {
            QTop* top = qgraphicsitem_cast<QTop* >(item);
            top->moveBy(myDisplacementVector.dx(), myDisplacementVector.dy());
        }
        //размораживаем все дуги
        foreach (QArc* arc, arcs)
            arc->unfreeze();
    }
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QList<QGraphicsItem* > items = moveCommand->myItems;

    if (myItems.count() != items.count())
        return false;

    foreach (QGraphicsItem* myItem, myItems)
        if (!items.contains(myItem))
            return false;

    myDisplacementVector.setP2(moveCommand->myDisplacementVector.p2() - (moveCommand->myDisplacementVector.p1() - myDisplacementVector.p2()));

    setText(QObject::tr("> ") + itemTypeToString(items.first()) + " (" + QString::number(myDisplacementVector.p1().x()) + ", " + QString::number(myDisplacementVector.p1().y()) + ")->" +
            "(" + QString::number(myDisplacementVector.p2().x()) + ", " + QString::number(myDisplacementVector.p2().y()) + ")");

    return true;
}

MoveCommand::~MoveCommand()
{

}

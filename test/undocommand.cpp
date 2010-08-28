#include "undocommand.h"

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
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
    }
    myGraphicsScene->removeItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

void AddCommand::redo()
{
    QArc* arc = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->addArc(arc);
        arc->endItem()->addArc(arc);
        foreach(QArc *qarc, arc->startItem()->outArcs())
            qarc->setPriority(qarc->priority() + 1);
        arc->setPriority(1);
    }
    myGraphicsScene->addItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

AddCommand::~AddCommand()
{

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
    QTop* top = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        foreach(QArc *qarc, arc->startItem()->outArcs())
            if (qarc->priority() >= arc->priority())
                qarc->setPriority(qarc->priority() + 1);
        arc->startItem()->addArc(arc);
        arc->endItem()->addArc(arc);
        break;
    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(myItem);
        foreach (QArc* arc, arcs) {
            arc->startItem()->addArc(arc);
            arc->endItem()->addArc(arc);
            myGraphicsScene->addItem(arc);
        }
    }
    myGraphicsScene->addItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

void DeleteCommand::redo()
{
    QArc* arc = NULL;
    QTop* top = NULL;
    switch (myItem->type()){
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc* >(myItem);
        arc->startItem()->removeArc(arc);
        arc->endItem()->removeArc(arc);
        break;
    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(myItem);
        arcs = top->allArcs();
        top->removeArcs();
        foreach (QArc* arc, arcs) {
            arc->endItem()->removeArc(arc);
            arc->startItem()->removeArc(arc);
            myGraphicsScene->removeItem(arc);
        }
    }
    myGraphicsScene->removeItem(myItem);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

DeleteCommand::~DeleteCommand()
{

}

MoveCommand::MoveCommand(QGraphicsItem *item, QGraphicsScene *graphicsScene, QLineF vector, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = graphicsScene;
    myItem = item;
    myDisplacementVector = vector;
    setText(QObject::tr("> ") + itemTypeToString(item) + " (" + QString::number(myDisplacementVector.p1().x()) + ", " + QString::number(myDisplacementVector.p1().y()) + ")->" +
            "(" + QString::number(myDisplacementVector.p2().x()) + ", " + QString::number(myDisplacementVector.p2().y()) + ")");
}

void MoveCommand::undo()
{
    switch (myItem->type()){
    case QTop::Type: {
            QTop* top = qgraphicsitem_cast<QTop* >(myItem);
            top->moveBy(-myDisplacementVector.dx(), -myDisplacementVector.dy(), true);
        }
        break;
    case QArcLine::Type:{
            QArc* arc = qgraphicsitem_cast<QArc *>(myItem->parentItem());
            QArcLine *selectedLine = qgraphicsitem_cast<QArcLine *>(myItem);
            QArcLine *prevLine;
            QArcLine *nextLine;
            if (arc->lines.count() == 1){
                prevLine = NULL;
                nextLine = NULL;
            } else {
                if (selectedLine == arc->lines.first()){
                    nextLine = prevLine = arc->lines.at(1);
                    prevLine = NULL;
                } else if (selectedLine == arc->lines.last()){
                    prevLine = arc->lines.at(arc->lines.indexOf(selectedLine) - 1);
                    nextLine = NULL;
                } else {
                    prevLine = arc->lines.at(arc->lines.indexOf(selectedLine) - 1);
                    nextLine = arc->lines.at(arc->lines.indexOf(selectedLine) + 1);
                }
            }

            //вправо-влево можно двигать только вертикальные линии (пока)
            if (selectedLine->line().p1().x() == selectedLine->line().p2().x()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x() - myDisplacementVector.dx(),
                                                     prevLine->line().p1().y())
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x() - myDisplacementVector.dx(),
                                                     nextLine->line().p1().y()
                                                     ),
                                             nextLine->line().p2())
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x() - myDisplacementVector.dx(),
                                                     selectedLine->line().p1().y()),
                                             QPointF(selectedLine->line().p2().x() - myDisplacementVector.dx(),
                                                     selectedLine->line().p2().y())
                                             )
                                      );
            }

            //вверх-вниз можно двигать только горизонтальные линии (пока)
            if (selectedLine->line().p1().y() == selectedLine->line().p2().y()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x(),
                                                     prevLine->line().p2().y() - myDisplacementVector.dy())
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x(),
                                                     nextLine->line().p1().y() - myDisplacementVector.dy()),
                                             nextLine->line().p2()
                                             )
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x(),
                                                     selectedLine->line().p1().y() - myDisplacementVector.dy()),
                                             QPointF(selectedLine->line().p2().x(),
                                                     selectedLine->line().p2().y() - myDisplacementVector.dy())
                                             )
                                      );
            }

            arc->updateBounds();
        }
        break;
    }
}

void MoveCommand::redo()
{
    QTop* top = NULL;
    switch (myItem->type()){
    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(myItem);
        top->moveBy(myDisplacementVector.dx(), myDisplacementVector.dy(), true);
        break;
    case QArcLine::Type:{
            QArc* arc = qgraphicsitem_cast<QArc *>(myItem->parentItem());
            QArcLine *selectedLine = qgraphicsitem_cast<QArcLine *>(myItem);
            QArcLine *prevLine;
            QArcLine *nextLine;
            if (arc->lines.count() == 1){
                prevLine = NULL;
                nextLine = NULL;
            } else {
                if (selectedLine == arc->lines.first()){
                    nextLine = prevLine = arc->lines.at(1);
                    prevLine = NULL;
                } else if (selectedLine == arc->lines.last()){
                    prevLine = arc->lines.at(arc->lines.indexOf(selectedLine) - 1);
                    nextLine = NULL;
                } else {
                    prevLine = arc->lines.at(arc->lines.indexOf(selectedLine) - 1);
                    nextLine = arc->lines.at(arc->lines.indexOf(selectedLine) + 1);
                }
            }

            //вправо-влево можно двигать только вертикальные линии (пока)
            if (selectedLine->line().p1().x() == selectedLine->line().p2().x()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x() + myDisplacementVector.dx(),
                                                     prevLine->line().p1().y())
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x() + myDisplacementVector.dx(),
                                                     nextLine->line().p1().y()
                                                     ),
                                             nextLine->line().p2())
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x() + myDisplacementVector.dx(),
                                                     selectedLine->line().p1().y()),
                                             QPointF(selectedLine->line().p2().x() + myDisplacementVector.dx(),
                                                     selectedLine->line().p2().y())
                                             )
                                      );
            }

            //вверх-вниз можно двигать только горизонтальные линии (пока)
            if (selectedLine->line().p1().y() == selectedLine->line().p2().y()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x(),
                                                     prevLine->line().p2().y() + myDisplacementVector.dy())
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x(),
                                                     nextLine->line().p1().y() + myDisplacementVector.dy()),
                                             nextLine->line().p2()
                                             )
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x(),
                                                     selectedLine->line().p1().y() + myDisplacementVector.dy()),
                                             QPointF(selectedLine->line().p2().x(),
                                                     selectedLine->line().p2().y() + myDisplacementVector.dy())
                                             )
                                      );
            }

            arc->updateBounds();
        }
        break;
    }
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QGraphicsItem *item = moveCommand->myItem;

    if (myItem != item)
        return false;

    myDisplacementVector.setP1(myDisplacementVector.p1());
    myDisplacementVector.setP2(moveCommand->myDisplacementVector.p2());

    setText(QObject::tr("> ") + itemTypeToString(item) + " (" + QString::number(myDisplacementVector.p1().x()) + ", " + QString::number(myDisplacementVector.p1().y()) + ")->" +
                                                          "(" + QString::number(myDisplacementVector.p2().x()) + ", " + QString::number(myDisplacementVector.p2().y()) + ")");

    return true;
}

MoveCommand::~MoveCommand()
{

}

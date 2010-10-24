#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H
#include <QtCore>
#include <QtGui>
#include "commonutils.h"

#define MOVE_COMMAND_ID 1

class AddCommand : public QUndoCommand
{
public:
    AddCommand(QGraphicsItem *item, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = 0);
    ~AddCommand();

    void undo();
    void redo();

private:
    QGraphicsItem *myItem;
    QGraphicsScene *myGraphicsScene;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(QGraphicsItem *item, QGraphicsScene *graphicsScene,
                  QUndoCommand *parent = 0);
    ~DeleteCommand();

    void undo();
    void redo();

private:
    QGraphicsItem *myItem;
    QGraphicsScene *myGraphicsScene;
    QList<QArc *> arcs;
    QList<QSyncArc *> syncs;
};

class MoveCommand : public QUndoCommand
{
public:
    enum {Id = MOVE_COMMAND_ID};

    MoveCommand(QList<QGraphicsItem*> items, QGraphicsScene *graphicsScene, QLineF vector, QUndoCommand *parent = 0);
    ~MoveCommand();
    bool mergeWith(const QUndoCommand *command);
    void undo();
    void redo();
    int id() const { return Id; }

private:
    QList<QGraphicsItem*> myItems;
    QGraphicsScene *myGraphicsScene;
    QLineF myDisplacementVector;
};

#endif // UNDOCOMMAND_H

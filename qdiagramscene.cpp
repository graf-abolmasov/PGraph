#include <QtGui>
#include "qdiagramscene.h"
#include "qarc.h"

QDiagramScene::QDiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;

    line = 0;
    textItem = 0;
    newArc = 0;

    myRootTop = NULL;
}

void QDiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void QDiagramScene::editorLostFocus(TComment *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void QDiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    TTop* item;
    switch (myMode) {
    case InsertItem:
        item = new TTop(myItemMenu);
        item->setBrush(QBrush(Qt::gray,Qt::SolidPattern));
        //item->setBrush(myItemColor); //прозрачный цвет
        item->setZValue(1);
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;

    case InsertLine:
        if (newArc == NULL)
            newArc = new TArc(NULL, NULL, myArcMenu, 0, this);
        line = newArc->newLine(mouseEvent->scenePos(), mouseEvent->scenePos());
        break;

    case InsertText:
        textItem = new TComment(myCommentMenu);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(TComment *)),
                this, SLOT(editorLostFocus(TComment *)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                this, SIGNAL(itemSelected(QGraphicsItem *)));
        addItem(textItem);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);

    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void QDiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //режим рисования дуги
    if (myMode == InsertLine && line != NULL && mouseEvent->buttons() == Qt::LeftButton) {
        QLineF vector(line->line().p1(), mouseEvent->scenePos());
        float dx = vector.dx();
        float dy = vector.dy();
        int angel = int(vector.angle() / 45.0);
        QLineF newLine;
        switch (angel) {
        case 0:;
        case 7:;
        case 3:;
        case 4:
            newLine.setP1(vector.p1());
            newLine.setP2(QPointF(vector.p1().x() + dx, vector.p1().y()));
            break;
        case 1:;
        case 2:;
        case 5:;
        case 6:
            newLine.setP1(vector.p1());
            newLine.setP2(QPointF(vector.p1().x(), vector.p1().y() + dy));
            break;
        default:
            ;
        }
        line->setLine(newLine);
    }
    //режим перетаскивания дуги
    else if ((myMode == MoveItem) && (mouseEvent->buttons() == Qt::LeftButton) &&
             (selectedItems().count() == 1) && (selectedItems().first()->type() == TArcLine::Type)) {
        TArcLine *selectedLine = qgraphicsitem_cast<TArcLine *>(selectedItems().first());
        TArc *arc = selectedLine->owner();

        QLineF vector(mouseEvent->lastScenePos(), mouseEvent->scenePos());
        float dx = vector.dx();
        float dy = vector.dy();

        /*//не разрешать двигать начальную и конечный кусочек дуги (пока)
        if (!((arc->lines.first() == selectedLine) || (arc->lines.last() == selectedLine))){*/
        TArcLine *prevLine;
        TArcLine *nextLine;
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
        //проверим чтобы начальная точка не вылезла из вершины!
        if ((arc->endItem()->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(arc->lines.last()->line().p2() + QPointF(dx, dy)) &&
              arc->startItem()->sceneBoundingRect().adjusted(8, 8, -8, -8).contains(arc->lines.first()->line().p1() + QPointF(dx, dy))) &&
             (arc->contains(arc->mapFromScene(mouseEvent->lastScenePos())))){

            //вправо-влево можно двигать только вертикальные линии (пока)
            if (selectedLine->line().p1().x() == selectedLine->line().p2().x()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x() + dx,
                                                     prevLine->line().p1().y())
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x() + dx,
                                                     nextLine->line().p1().y()
                                                     ),
                                             nextLine->line().p2())
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x() + dx,
                                                     selectedLine->line().p1().y()),
                                             QPointF(selectedLine->line().p2().x() + dx,
                                                     selectedLine->line().p2().y())
                                             )
                                      );
            }

            //вверх-вниз можно двигать только горизонтальные линии (пока)
            if (selectedLine->line().p1().y() == selectedLine->line().p2().y()){
                if (prevLine != NULL)
                    prevLine->setLine(QLineF(prevLine->line().p1(),
                                             QPointF(prevLine->line().p2().x(),
                                                     prevLine->line().p2().y() + dy)
                                             )
                                      );
                if (nextLine != NULL)
                    nextLine->setLine(QLineF(QPointF(nextLine->line().p1().x(),
                                                     nextLine->line().p1().y() + dy),
                                             nextLine->line().p2()
                                             )
                                      );
                selectedLine->setLine(QLineF(QPointF(selectedLine->line().p1().x(),
                                                     selectedLine->line().p1().y() + dy),
                                             QPointF(selectedLine->line().p2().x(),
                                                     selectedLine->line().p2().y() + dy)
                                             )
                                      );
            }
        };

        arc->updateBounds();
    }
    //режим перетаскивания вершины
    else if ((myMode == MoveItem) && (mouseEvent->buttons() == Qt::LeftButton) &&
             (selectedItems().count() == 1) && (selectedItems().first()->type() == TTop::Type) &&
             (selectedItems().first()->contains(selectedItems().first()->mapFromScene(mouseEvent->lastScenePos())))) {
        TTop *top = qgraphicsitem_cast<TTop *>(selectedItems().first());
        QPointF old_pos = mouseEvent->lastScenePos();
        QPointF new_pos = mouseEvent->scenePos();

        QLineF vector(old_pos, new_pos);
        float dx = vector.dx();
        float dy = vector.dy();

        top->setX(top->x() + dx);
        top->setY(top->y() + dy);

        QList<QGraphicsItem* > itemList = collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList){
            if (item->type() == TTop::Type){
                top->setX(top->x() - dx);
                top->setY(top->y() - dy);
                mouseEvent->ignore();
                return;
            }
        }

        bool isOK = false; //false - если дугу надо полностью переделать

        QList<TArc *> brokenLines; //список содержит дуги, нуждающиеся в полной переделке
        foreach (TArc *arc, top->allArcs()){
           isOK = arc->remake(top, dx, dy);
           if (!isOK)
               brokenLines.append(arc);
        }

        foreach (TArc *arc, brokenLines)
            arc->autoBuild(top, dx, dy);


        /*QList<TTop* > brokenTopList;
        foreach (TArc *arc, brokenLines){
            if (!brokenTopList.contains(arc->startItem()))
                brokenTopList.append(arc->startItem());
            if (!brokenTopList.contains(arc->endItem()))
                brokenTopList.append(arc->endItem());
        }
        foreach (TTop* top, brokenTopList){
            top->autoArrangeArcs(brokenLines);
        }*/

        //необходимо для правильной перерисовки.
        foreach (TArc *arc, top->allArcs())
            arc->updateBounds();

    }
}

void QDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != NULL && newArc != NULL && myMode == InsertLine) {
        //дуга должна начинаться с вершины!
        if (newArc->startItem() == NULL){
            QList<QGraphicsItem *> startItems = items(line->line().p1());
            while (startItems.count() > 0 && startItems.first()->type() != TTop::Type)
                startItems.removeFirst();
            if ((startItems.count() > 0) && (startItems.first()->type() == TTop::Type)) {
                TTop *startItem = qgraphicsitem_cast<TTop *>(startItems.first());
                newArc->setStartTop(startItem);
                newArc->addLine(line);
            } else {
                delete line;
                delete newArc;
                newArc = NULL;
                line = NULL;
            }
        }

        //дуга должна заканчиваться вершиной
        if ((newArc != NULL) && (newArc->endItem() == NULL) && (newArc->prevLine() != NULL)){
            QList<QGraphicsItem *> endItems = items(line->line().p2());
            while (endItems.count() > 0 && endItems.first()->type() != TTop::Type)
                endItems.removeFirst();
            if ((endItems.count() > 0) && (endItems.first()->type() == TTop::Type)){
                TTop *endItem = qgraphicsitem_cast<TTop *>(endItems.first());
                newArc->setEndTop(endItem);              
                newArc->addLine(line);
            }
        }
        //плохая ситуация, когда начало и конец совпадают, но дуга имеет явно неправилную форму
        if ((newArc != NULL) && (newArc->endItem() != NULL) && (newArc->startItem() != NULL) &&
            newArc->startItem()->collidesWithItem(newArc->endItem()) && (newArc->lines.count() == 1)){
            delete newArc;
            newArc = NULL;
            line = NULL;
        }

        //условие выполнится когда у дуги будет начало и конец!
        if ((newArc != NULL) && (newArc->endItem() != NULL) && (newArc->startItem() != NULL)){
            foreach(TArc *arc, newArc->startItem()->outArcs()){
                arc->setPriority(arc->priority() + 1);
            }
            newArc->startItem()->addArc(newArc);
            newArc->endItem()->addArc(newArc);
            newArc->updateBounds();
            update();
            newArc->currentLine = NULL;
            newArc = NULL;
            line = NULL;
        }
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void QDiagramScene::keyReleaseEvent (QKeyEvent *keyEvent){
    if ((newArc != NULL) && (keyEvent->key() == Qt::Key_Escape)) {
        delete line;
        if (newArc->lines.count() > 1){
            foreach (TArcLine *line, newArc->lines){
                delete line;
            }
        }
        newArc->lines.clear();
        delete newArc;
        newArc = NULL;
        line = NULL;
    }
    QGraphicsScene::keyReleaseEvent(keyEvent);
}

bool QDiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void QDiagramScene::setItemMenu(QMenu *menu)
{
    myItemMenu = menu;
}

void QDiagramScene::setArcMenu(QMenu *menu)
{
    myArcMenu = menu;
}

void QDiagramScene::setCommentMenu(QMenu *menu)
{
    myCommentMenu = menu;
}

void QDiagramScene::setRootTop(TTop* top){
    if (myRootTop != NULL)
        myRootTop->setAsRoot(false);
    myRootTop = top;
    top->setAsRoot(true);
}



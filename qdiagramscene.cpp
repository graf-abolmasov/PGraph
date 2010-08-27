#include <QtGui>
#include "qdiagramscene.h"
#include "qmywindow.h"
#include "qparallelarctop.h"
#include "qterminatearctop.h"
#include "globalvariables.h"

QDiagramScene::QDiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;

    setItemIndexMethod(NoIndex);

    line = 0;
    textItem = 0;
    newArc = 0;

    myRootTop = NULL;
}

void QDiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void QDiagramScene::editorLostFocus(QComment *item)
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

    QGraphicsItem* item;
    switch (myMode) {
    case InsertItem:
        item = new QNormalTop(myTopMenu);
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;

    case InsertLine:
        if (newArc == NULL)
            newArc = new QArc(NULL, NULL, myArcMenu, 0, this);
        line = newArc->newLine(mouseEvent->scenePos(), mouseEvent->scenePos());
        break;

    case InsertText:
        textItem = new QComment(myCommentMenu);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(QComment *)),
                this, SLOT(editorLostFocus(QComment *)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                this, SIGNAL(itemSelected(QGraphicsItem *)));
        addItem(textItem);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);

    case InsertSync:
        line = new QArcLine(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        addItem(line);
        break;

    case InsertMultiProcTop:
        item = new QMultiProcTop(myMultiProcTopMenu);
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;
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
             (selectedItems().count() == 1) && (selectedItems().first()->type() == QArcLine::Type)) {
        QArcLine *selectedLine = qgraphicsitem_cast<QArcLine *>(selectedItems().first());
        QArc *arc = qgraphicsitem_cast<QArc *>(selectedLine->parentItem());

        QLineF vector(mouseEvent->lastScenePos(), mouseEvent->scenePos());
        float dx = vector.dx();
        float dy = vector.dy();

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
    else if ((myMode == MoveItem) &&
             (mouseEvent->buttons() == Qt::LeftButton) &&
             (selectedItems().count() == 1) &&
             (selectedItems().first()->type() == QTop::Type) &&
             (selectedItems().first()->contains(selectedItems().first()->mapFromScene(mouseEvent->lastScenePos())))) {
        QTop *top = qgraphicsitem_cast<QTop *>(selectedItems().first());
        QPointF old_pos = mouseEvent->lastScenePos();
        QPointF new_pos = mouseEvent->scenePos();

        QLineF vector(old_pos, new_pos);
        float dx = vector.dx();
        float dy = vector.dy();

        top->moveBy(dx, dy);

        QList<QGraphicsItem* > itemList = collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList){
            if (item->type() == QTop::Type){
                top->moveBy(-dx, -dy);
                mouseEvent->ignore();
                return;
            }
        }

        bool isOK = false; //false - если дугу надо полностью переделать

        QList<QArc *> brokenLines; //список содержит дуги, нуждающиеся в полной переделке
        foreach (QArc *arc, top->allArcs()){
           isOK = arc->remake(top, dx, dy);
           if (!isOK)
               brokenLines.append(arc);
        }

        foreach (QArc *arc, brokenLines)
            arc->autoBuild(top, dx, dy);

        //необходимо для правильной перерисовки.
        foreach (QArc *arc, top->allArcs())
            arc->updateBounds();

    }
    else if (myMode == InsertSync && line != NULL) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
}

void QDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != NULL && newArc != NULL && myMode == InsertLine) {
        //дуга должна начинаться с вершины!
        if (newArc->startItem() == NULL){
            QList<QGraphicsItem *> startItems = items(line->line().p1());
            while (startItems.count() > 0 && startItems.first()->type() != QTop::Type)
                startItems.removeFirst();
            if ((startItems.count() > 0) && (startItems.first()->type() == QTop::Type)) {
                QTop *startItem = qgraphicsitem_cast<QTop *>(startItems.first());
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
            while (endItems.count() > 0 && endItems.first()->type() != QTop::Type)
                endItems.removeFirst();
            if ((endItems.count() > 0) && (endItems.first()->type() == QTop::Type)){
                QTop *endItem = qgraphicsitem_cast<QTop *>(endItems.first());
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
            newArc->startItem()->addArc(newArc);
            newArc->endItem()->addArc(newArc);
            foreach(QArc *arc, newArc->startItem()->outArcs())
                arc->setPriority(arc->priority() + 1);
            newArc->setPriority(1);
            newArc->updateBounds();
            line->setSelected(false);
            update();
            newArc->currentLine = NULL;
            newArc = NULL;
            line = NULL;
        }
    } else if (line != NULL && myMode == InsertSync) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == QTop::Type &&
            endItems.first()->type() == QTop::Type &&
            startItems.first() != endItems.first()) {
            QTop *startItem =
                qgraphicsitem_cast<QTop *>(startItems.first());
            QTop *endItem =
                qgraphicsitem_cast<QTop *>(endItems.first());
            QSyncArc *arrow = new QSyncArc(startItem, endItem, mySyncArcMenu);
            arrow->setLine(line->line());
            startItem->addSync(arrow);
            endItem->addSync(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
        removeItem(line);
        delete line;
        line = 0;
    }

    //пишем информация в статус бар
    if (selectedItems().count() == 1) {
        QArc *arc;
        QTop *top;
        QSyncArc *syncArc;
        QString status = tr("");
        QString arcTypeStr;
        switch (selectedItems().first()->type()) {
        case (QArcLine::Type):
        case (QSerialArcTop::Type):
            arc = qgraphicsitem_cast<QArc*>(selectedItems().first()->parentItem());
            switch (arc->arcType()) {
            case QArc::SerialArc:
                arcTypeStr = tr("последовательная");
                break;
            case QArc::ParallelArc:
                arcTypeStr = tr("параллельная");
                break;
            case QArc::TerminateArc:
                arcTypeStr = tr("терминирующая");
                break;
            }
            status.append(tr("Дуга ") + arcTypeStr);
            if (arc->startItem() != NULL)
                status.append(tr(" Начальная вершина ") + QString::number(arc->startItem()->number));
            if (arc->endItem() != NULL)
                status.append(tr(" Конечная вершина ") + QString::number(arc->endItem()->number));
            status.append(tr(" Приоритет ") + QString::number(arc->priority()) +
                          tr(" Ширина пера ") + QString::number(arc->pen().width()));
            if (arc->predicate != NULL)
               status.append(tr(" Предикат ") + arc->predicate->extName);
            break;
        case QArc::Type:
            arc = qgraphicsitem_cast<QArc*>(selectedItems().first());
            switch (arc->arcType()) {
            case QArc::SerialArc:
                arcTypeStr = tr("последовательная");
                break;
            case QArc::ParallelArc:
                arcTypeStr = tr("параллельная");
                break;
            case QArc::TerminateArc:
                arcTypeStr = tr("терминирующая");
                break;
            }
            status.append(tr("Дуга ") + arcTypeStr +
                          tr(" Начальная вершина ") + QString::number(arc->startItem()->number) +
                          tr(" Конечная вершина ") + QString::number(arc->endItem()->number) +
                          tr(" Приоритет ") + QString::number(arc->priority()) +
                          tr(" Ширина пера ") + QString::number(arc->pen().width()));
            if (arc->predicate != NULL)
                status.append(tr(" Предикат ") + arc->predicate->extName);
            break;
        case QTop::Type:
            top = qgraphicsitem_cast<QTop* >(selectedItems().first());
            status.append(tr("Номер вершины ") + QString::number(top->number));
            if (top->actor != NULL)
                status.append(tr(" Актор ") + top->actor->extName);
            break;
        case QSyncArc::Type:
            syncArc = qgraphicsitem_cast<QSyncArc* >(selectedItems().first());
            break;
        }
        globalStatusBar->showMessage(status);
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void QDiagramScene::keyReleaseEvent (QKeyEvent *keyEvent){
    if ((newArc != NULL) && (keyEvent->key() == Qt::Key_Escape)) {
        delete line;
        if (newArc->lines.count() > 1){
            foreach (QArcLine *line, newArc->lines){
                delete line;
            }
        }
        newArc->lines.clear();
        delete newArc;
        newArc = NULL;
        line = NULL;
    }

    if (selectedItems().count() > 0 && (keyEvent->key() == Qt::Key_Delete)) {
        /*QList<QGraphicsItem* > deleteList;
        foreach (QGraphicsItem *item, selectedItems()){
            switch (item->type()){
            case QTop::Type:
            case QArc::Type:
            case QSyncArc::Type:
                if (!deleteList.contains(item))
                    deleteList.append(item);
                break;
            case QTerminateArcTop::Type:
            case QArcLine::Type:
                if (!deleteList.contains(item->parentItem()))
                    deleteList.append(item->parentItem());
                break;
            }
        }
        foreach (QGraphicsItem *item, deleteList)
            delete item;*/
        QGraphicsItem *item = selectedItems().first();
        clearSelection();
        switch (item->type()){
        case QTop::Type:
        case QArc::Type:
        case QSyncArc::Type:
            delete item;
            break;
        case QTerminateArcTop::Type:
        case QArcLine::Type:
            delete item->parentItem();
            break;
        }
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

void QDiagramScene::setTopMenu(QMenu *menu)
{
    myTopMenu = menu;
}

void QDiagramScene::setArcMenu(QMenu *menu)
{
    myArcMenu = menu;
}

void QDiagramScene::setCommentMenu(QMenu *menu)
{
    myCommentMenu = menu;
}

void QDiagramScene::setSyncArcMenu(QMenu *menu)
{
    mySyncArcMenu = menu;
}

void QDiagramScene::setMultiProcTopMenu(QMenu *menu)
{
    myMultiProcTopMenu = menu;
}

void QDiagramScene::setRootTop(QNormalTop* top){
    if (myRootTop != NULL)
        myRootTop->setAsRoot(false);
    myRootTop = top;
    top->setAsRoot(true);
}



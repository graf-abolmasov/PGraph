#include <QtGui>
#include "qdiagramscene.h"
#include "qmywindow.h"
#include "qparallelarctop.h"
#include "qterminatearctop.h"
#include "globalvariables.h"
#include "undocommand.h"

QDiagramScene::QDiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    setItemIndexMethod(NoIndex);
    line = NULL;
    newArc = NULL;
    selectionRect = NULL;
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
    if (item->toPlainText().isEmpty())
        emit itemDeleted(item);
}

void QDiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    bool allowAddTop = true;
    QList<QGraphicsItem *> itemsAtPos = items(mouseEvent->scenePos());
    switch (myMode) {
    case InsertNormalTop:
    case InsertMultiProcTop:
        foreach (QGraphicsItem* item, itemsAtPos){
            if (item->type() == QTop::Type) {
                allowAddTop = false;
                break;
            }
        }
        if (allowAddTop)
            addTop(mouseEvent->scenePos());
        break;
    case InsertLine:
        if (newArc == NULL)
            newArc = new QArc(NULL, NULL, myArcMenu, 0, this);
        line = newArc->newLine(mouseEvent->scenePos(), mouseEvent->scenePos());
        break;
    case InsertText:
        addComment(mouseEvent->scenePos());
    case InsertSync:
        line = new QArcLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        addItem(line);
        break;
    case MoveItem:
        if (itemAt(mouseEvent->scenePos()) == NULL) {
            clearSelection();
            selectionRect = new QGraphicsRectItem(0, 0, 0, 0, NULL, this);
            selectionRect->setPos(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
            selectionRect->setPen(Qt::DashLine);
        }
        break;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void QDiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem* > mySelectedItems = selectedItems();

    //режим выделения области
    if (myMode == MoveItem && selectionRect != NULL) {
        QRectF newRect(0, 0, mouseEvent->scenePos().x() - selectionRect->scenePos().x(), mouseEvent->scenePos().y() - selectionRect->scenePos().y());
        selectionRect->setRect(newRect.normalized());
    }
    //режим рисования дуги
    else if (myMode == InsertLine && line != NULL && mouseEvent->buttons() == Qt::LeftButton) {
        QLineF vector(line->line().p1(), mouseEvent->scenePos());
        float dx = vector.dx();
        float dy = vector.dy();
        int sector = int(vector.angle() / 45.0);
        QLineF newLine;
        switch (sector) {
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
        line->setSelected(true);
    }
    //режим перетаскивания вершины
    else if ((myMode == MoveItem) &&
             (mouseEvent->buttons() == Qt::LeftButton) &&
             (mySelectedItems.first()->type() == QTop::Type)) {
        QLineF vector(mouseEvent->lastScenePos(), mouseEvent->scenePos());
        bool allowMove = false;

        //находится ли курсор внутри какого-нибудь элемента
        foreach (QGraphicsItem* item, mySelectedItems)
            if (item->contains(item->mapFromScene(mouseEvent->lastScenePos()))) {
                allowMove = true;
                break;
            }

        //если да, то
        if (allowMove) {
            //не пересечется ли какqо-нибудь элемент с уже имеющимся на сцене?
            //передвинем все элементы временно
            foreach (QGraphicsItem* item, mySelectedItems)
                item->setPos(item->scenePos().x() + vector.dx(), item->scenePos().y() + vector.dy());

            //надем пересекающиеся элементы
            foreach (QGraphicsItem* item, mySelectedItems) {
                QList<QGraphicsItem* > itemList = collidingItems(item, Qt::IntersectsItemBoundingRect);
                foreach(QGraphicsItem* item, itemList) {
                    //если есть хотя бы одна вершина
                    if (item->type() == QTop::Type){
                        //запретим перемещение
                        allowMove = false;
                        break;
                    }
                }
                //если хотябы одна вершина пересеклась, то остальные не проверяем
                if (!allowMove)
                    break;
            }

            //вернем все на место
            foreach (QGraphicsItem* item, mySelectedItems)
                item->setPos(item->scenePos().x() - vector.dx(), item->scenePos().y() - vector.dy());
        }

        //если все хорошо, перемещаем все вершины
        if (allowMove)
            emit itemsMoved(mySelectedItems, vector);
    }
    //режим перетаскивания дуги
    else if ((myMode == MoveItem) &&
             (mouseEvent->buttons() == Qt::LeftButton) &&
             (mySelectedItems.count() == 1) &&
             (mySelectedItems.first()->type() == QArcLine::Type)) {
        QArcLine *selectedLine = qgraphicsitem_cast<QArcLine *>(mySelectedItems.first());
        QLineF vector(mouseEvent->lastScenePos(), mouseEvent->scenePos());
        QArc *arc = qgraphicsitem_cast<QArc *>(selectedLine->parentItem());

        if (selectedLine->contains(selectedLine->mapFromScene(mouseEvent->lastScenePos())))
            arc->moveLine(selectedLine, vector.dx(), vector.dy());
    }
    else if (myMode == InsertSync && line != NULL) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if ((myMode == MoveItem) &&
             (mouseEvent->buttons() == Qt::LeftButton) &&
             (mySelectedItems.count() == 1) &&
             (mySelectedItems.first()->type() == QComment::Type))
        emit itemMoved(mySelectedItems.first(), QLineF(mouseEvent->lastScenePos(), mouseEvent->scenePos()));
}

void QDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == MoveItem && selectionRect != NULL) {
        QPainterPath selectedPath;
        selectedPath.addPolygon(selectionRect->mapToScene(selectionRect->rect()));
        QList<QGraphicsItem* > itemsInRect = items(selectedPath, Qt::ContainsItemShape);
        clearSelection();
        foreach (QGraphicsItem* item, itemsInRect){
            if (item->type() == QTop::Type)
                item->setSelected(true);
        }
        delete selectionRect;
        selectionRect = NULL;
    }
    else if (line != NULL && newArc != NULL && myMode == InsertLine) {
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
            QSyncArc *newSyncArc = new QSyncArc(startItem, endItem, mySyncArcMenu);
            startItem->addSync(newSyncArc);
            endItem->addSync(newSyncArc);
            newSyncArc->setZValue(3);
            newSyncArc->setLine(line->line());
            addItem(newSyncArc);
        }
        removeItem(line);
        delete line;
        line = 0;
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
        QGraphicsItem *item = selectedItems().first();
        switch (item->type()){
        case QTop::Type:
        case QArc::Type:
        case QSyncArc::Type:
        case QComment::Type:
            emit itemDeleted(item);
            break;
        case QTerminateArcTop::Type:
        case QArcLine::Type:
            emit itemDeleted(item->parentItem());
            break;
        }
    }
    QGraphicsScene::keyReleaseEvent(keyEvent);
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

QTop* QDiagramScene::addTop(const QPointF &point)
{
    QTop* top = NULL;
    switch (myMode){
    case InsertNormalTop:
        top = new QNormalTop(myTopMenu);
        break;
    case InsertMultiProcTop:
        top = new QMultiProcTop(myMultiProcTopMenu);
        break;
    default:
        break;
    }
    top->number = getNextTopNumber();
    top->setPos(point);
    emit itemInserted(top);
    return top;
}

QComment* QDiagramScene::addComment(const QPointF &point)
{
    QComment* textItem = new QComment(myCommentMenu);
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    connect(textItem, SIGNAL(lostFocus(QComment*)), this, SLOT(editorLostFocus(QComment*)));
    textItem->setPos(point);
    emit itemInserted(textItem);
    return textItem;
}

int QDiagramScene::getNextTopNumber()
{
    int maxNumber = -1;
    for (int i = 0; i < items().count(); i++)
        if (items().at(i)->type() == QTop::Type) {
           QTop* top = qgraphicsitem_cast<QTop* >(items().at(i));
           maxNumber = top->number > maxNumber ? top->number : maxNumber;
        }

    return maxNumber+1;
}

#include <QtGui>
#include "qdiagramscene.h"
#include "qarc.h"

QDiagramScene::QDiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    myItemType = TTop::Top;

    line = 0;
    textItem = 0;
    newArc = 0;

    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void QDiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(TArc::Type)) {
        TArc *item =
            qgraphicsitem_cast<TArc *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void QDiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(TComment::Type)) {
        TComment *item =
            qgraphicsitem_cast<TComment *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void QDiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(TTop::Type)) {
        TTop *item =
            qgraphicsitem_cast<TTop *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void QDiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(TComment::Type)) {
        QGraphicsTextItem *item =
            qgraphicsitem_cast<TComment *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}

void QDiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void QDiagramScene::setItemType(TTop::DiagramType type)
{
    myItemType = type;
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

    TTop* item = new TTop(myItemType, myItemMenu);
    switch (myMode) {
        case InsertItem:
            //item->setBrush(QBrush(Qt::white,Qt::SolidPattern));
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;

        case InsertLine:
            if (newArc == NULL) 
                newArc = new TArc(NULL, NULL, myArcMenu, 0, this);
            line = newArc->newLine(mouseEvent->scenePos(), mouseEvent->scenePos());
            //futureLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()), 0, this);
            //futureLine->setPen(QPen(QBrush(Qt::black,Qt::SolidPattern), 2, Qt::DotLine));
            break;

        case InsertText:
            textItem = new TComment(myCommentMenu);
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(TComment *)),
                    this, SLOT(editorLostFocus(TComment *)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                    this, SIGNAL(itemSelected(QGraphicsItem *)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
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
        //futureLine->setLine(QLineF(newLine.p2(), mouseEvent->scenePos()));
    }
    //режим перетаскивания дуги
    else if ((myMode == MoveItem) && (mouseEvent->buttons() == Qt::LeftButton) &&
             (selectedItems().count() == 1) && (selectedItems().first()->type() == TArcLine::Type)) {
        TArcLine *selectedLine = qgraphicsitem_cast<TArcLine *>(selectedItems().first());
        TArc *arc = selectedLine->owner();

        QLineF vector(mouseEvent->lastScenePos(), mouseEvent->scenePos());
        float dx = vector.dx();
        float dy = vector.dy();

        //не разрешать двигать начальную и конечную дуги (пока)
        if (!((arc->lines.first() == selectedLine) || (arc->lines.last() == selectedLine))){
            TArcLine *prevLine = arc->lines.at(arc->lines.indexOf(selectedLine) - 1);
            TArcLine *nextLine = arc->lines.at(arc->lines.indexOf(selectedLine) + 1);

            //вправо-влево можно двигать только вертикальные линии (пока)
            if (selectedLine->line().p1().x() == selectedLine->line().p2().x()){
                prevLine->setLine(QLineF(prevLine->line().p1(),
                                         QPointF(prevLine->line().p2().x() + dx,
                                                 prevLine->line().p1().y())
                                         )
                                  );
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
                prevLine->setLine(QLineF(prevLine->line().p1(),
                                         QPointF(prevLine->line().p2().x(),
                                                 prevLine->line().p2().y() + dy)
                                         )
                                  );
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
        }

    }
    //режим перетаскивания вершины
    else if ((myMode == MoveItem) && (mouseEvent->buttons() == Qt::LeftButton) &&
             (selectedItems().count() == 1) && (selectedItems().first()->type() == TTop::Type)) {
        bool allowMove = false;
        TTop *top = qgraphicsitem_cast<TTop *>(selectedItems().first());
        QPointF old_pos = mouseEvent->lastScenePos(); //top->scenePos();
        //!!QGraphicsScene::mouseMoveEvent(mouseEvent);
        QPointF new_pos = mouseEvent->scenePos();//top->scenePos();

        QLineF vector(old_pos, new_pos);
        float dx = vector.dx();
        float dy = vector.dy();

        foreach (TArc *arc, top->allArcs()){
            //для тех у кого эта вершина - начало
            if (arc->startItem() == top){
                //вертикальную линию двигаем только вправо/влево или удлинняем/укорачиваем
                if (arc->lines.first()->line().p1().x() == arc->lines.first()->line().p2().x()){
                    //можно двигать пока остается место для серого квадратика
                    if (arc->lines.first()->line().length() < 60)
                        dy = 0;
                    {
                        arc->lines.first()->setLine(QLineF(QPointF(arc->lines.first()->line().p1().x() + dx,
                                                                   arc->lines.first()->line().p1().y() + dy),
                                                           QPointF(arc->lines.first()->line().p2().x() + dx,
                                                                   arc->lines.first()->line().p2().y())
                                                           )
                                                    );
                        //перерисуем следующий кусочек тоже (если он есть)
                        if (arc->lines.count() > 1)
                            arc->lines.at(1)->setLine(QLineF(QPointF(arc->lines.at(1)->line().p1().x() + dx,
                                                                     arc->lines.at(1)->line().p1().y()),
                                                             arc->lines.at(1)->line().p2()
                                                             )
                                                      );
                    }
                }
                //горизонтальную линию можем двигать только вверх/вниз или удлиннять/укорачивать
                if (arc->lines.first()->line().p1().y() == arc->lines.first()->line().p2().y()){
                    if (arc->lines.first()->line().length() < 60)
                        dx = 0;
                    {
                        arc->lines.first()->setLine(QLineF(QPointF(arc->lines.first()->line().p1().x() + dx,
                                                                   arc->lines.first()->line().p1().y() + dy),
                                                           QPointF(arc->lines.first()->line().p2().x(),
                                                                   arc->lines.first()->line().p2().y() + dy)
                                                           )
                                                    );
                        //перерисуем следующий кусочек тоже (если он есть)
                        if (arc->lines.count() > 1)
                            arc->lines.at(1)->setLine(QLineF(QPointF(arc->lines.at(1)->line().p1().x(),
                                                                     arc->lines.at(1)->line().p1().y() + dy),
                                                             arc->lines.at(1)->line().p2()
                                                             )
                                                      );
                    }
                }
            }
            //для тех у кого эта вершина - конец
            if (arc->endItem() == top){
                //вертикальную линию двигаем только вправо/влево или удлинняем/укорачиваем
                if (arc->lines.last()->line().p1().x() == arc->lines.last()->line().p2().x()){
                    if (arc->lines.last()->line().length() < 60)
                        dy = 0;
                    {
                        arc->lines.last()->setLine(QLineF(QPointF(arc->lines.last()->line().p1().x() + dx,
                                                                  arc->lines.last()->line().p1().y()),
                                                          QPointF(arc->lines.last()->line().p2().x() + dx,
                                                                  arc->lines.last()->line().p2().y() + dy)
                                                          )
                                                   );
                        //перерисуем следующий кусочек тоже (если он есть)
                        if (arc->lines.count() > 1)
                            arc->lines.at(arc->lines.count()-2)->setLine(QLineF(arc->lines.at(arc->lines.count()-2)->line().p1(),
                                                                                QPointF(arc->lines.at(arc->lines.count()-2)->line().p2().x() + dx,
                                                                                        arc->lines.at(arc->lines.count()-2)->line().p2().y())
                                                                                )
                                                                         );
                    }
                }
                //горизонтальную линию можем двигать только вверх/вниз или удлиннять/укорачивать
                if (arc->lines.last()->line().p1().y() == arc->lines.last()->line().p2().y()){
                    if (arc->lines.last()->line().length() < 60)
                        dx = 0;
                    {
                        allowMove = true;
                        arc->lines.last()->setLine(QLineF(QPointF(arc->lines.last()->line().p1().x(),
                                                                  arc->lines.last()->line().p1().y() + dy),
                                                          QPointF(arc->lines.last()->line().p2().x() + dx,
                                                                  arc->lines.last()->line().p2().y() + dy)
                                                          )
                                                   );
                        //перерисуем следующий кусочек тоже (если он есть)
                        if (arc->lines.count() > 1)
                            arc->lines.at(arc->lines.count()-2)->setLine(QLineF(arc->lines.at(arc->lines.count()-2)->line().p1(),
                                                                                QPointF(arc->lines.at(arc->lines.count()-2)->line().p2().x(),
                                                                                        arc->lines.at(arc->lines.count()-2)->line().p2().y() + dy)
                                                                                )
                                                                         );
                    }
                }
            }
        }
        //!!old_pos.setX(old_pos.x() + dx);
        //!!old_pos.setY(old_pos.y() + dy);

        top->setX(top->x() + dx);
        top->setY(top->y() + dy);

        foreach (TArc *arc, top->allArcs()){
            arc->realloc();
        }

        /*QGraphicsSceneMouseEvent *newEvent =
                new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
        newEvent->setAccepted(true);
        newEvent->setPos(mouseEvent->pos());
        newEvent->setScenePos(mouseEvent->scenePos());
        newEvent->setScreenPos(mouseEvent->screenPos());
        newEvent->setButtonDownPos(Qt::LeftButton,
                                     mouseEvent->buttonDownPos(Qt::LeftButton));
        newEvent->setButtonDownScreenPos(Qt::LeftButton,
                                           mouseEvent->buttonDownScreenPos(Qt::LeftButton));
        newEvent->setButtonDownScenePos(Qt::LeftButton,
                                          mouseEvent->buttonDownScenePos(Qt::LeftButton));
        newEvent->setWidget(mouseEvent->widget());


        QGraphicsScene::mouseMoveEvent(newEvent);
        delete newEvent;*/
    }
}

void QDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != NULL && newArc != NULL && myMode == InsertLine) {
        //delete futureLine;
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
                arc->setPriority(arc->width+1);
            }
            newArc->startItem()->addArc(newArc);
            newArc->endItem()->addArc(newArc);
            newArc->realloc();
            update();
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



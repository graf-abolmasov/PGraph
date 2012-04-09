#include <QtGui>
#include <QtCore>
#include "../../src/editor/qdrawwindow.h"
#include "../../src/editor/qtop.h"
#include "../../src/editor/qsyncarc.h"
#include "../../src/editor/qserialarctop.h"
#include "../../src/editor/qparallelarctop.h"
#include "../../src/editor/qterminatearctop.h"
#include "../../src/editor/qnormaltop.h"
#include "../../src/editor/undocommand.h"
#include "../../src/editor/qarcline.h"
#include "../../src/editor/qmultiproctop.h"
#include "../../src/editor/qdiagramscene.h"
#include "../../src/editor/qtop.h"
#include "../../src/editor/qcomment.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/VO/graphvo.h"
#include "../../src/editor/qarc.h"
#include "../../src/editor/dialogs/arcpropertydialog.h"
#include "../../src/editor/dialogs/multiproctoppropertydialog.h"
#include "../../src/editor/dialogs/toppropertydialog.h"

QStringList globalPredicateList;

TDrawWindow::TDrawWindow(ShowRole role, QWidget *parent)
    : QMainWindow(parent)
{
    myRole = role;

    createActions();
    createMenus();
    undoStack = new QUndoStack();

    view = new QGraphicsView(this);
    scene = new QDiagramScene();
    scene->setTopMenu(topMenu);
    scene->setArcMenu(arcMenu);
    scene->setCommentMenu(commentMenu);
    scene->setSyncArcMenu(syncArcMenu);
    scene->setMultiProcTopMenu(multiProcMenu);
    scene->setBackgroundBrush(QBrush(Qt::white));
    scene->setSceneRect(-800, -600, 1600, 1200);
    //обработчик добавления объекта
    connect(scene, SIGNAL(itemInserted(QGraphicsItem*)),
            this, SLOT(itemInserted(QGraphicsItem*)));
    //обработчик перемещения объекта
    connect(scene, SIGNAL(itemMoved(QGraphicsItem*,QLineF)),
            this, SLOT(itemMoved(QGraphicsItem*,QLineF)));
    connect(scene, SIGNAL(itemsMoved(QList<QGraphicsItem*>,QLineF)),
            this, SLOT(itemsMoved(QList<QGraphicsItem*>,QLineF)));
    //выбор объекта - тоже изменение сцены
    connect(scene, SIGNAL(selectionChanged()),
            this, SIGNAL(sceneChanged()));
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    //обработчик удаления объекта
    connect(scene, SIGNAL(itemDeleted(QGraphicsItem*)),
            this, SLOT(itemDeleted(QGraphicsItem*)));
    connect(scene, SIGNAL(mouseScrollScaleChanged(float)), this, SIGNAL(mouseScrollScaleChanged(float)));

    view->setScene(scene);
    view->setAlignment(Qt::AlignCenter);
    //Рисуем сетку
    QBrush myBackgroundBrush = view->backgroundBrush();
    myBackgroundBrush.setColor(Qt::lightGray);
    myBackgroundBrush.setStyle(Qt::Dense7Pattern);
    view->setBackgroundBrush(myBackgroundBrush);
    setCentralWidget(view);
    setWindowTitle("");
    if (myRole == NormalEditor)
        setMode(QDiagramScene::MoveItem);
    else
        setMode(QDiagramScene::ReadOnly);
    
}

void TDrawWindow::createMenus()
{
    //Режим только чтения
    if (myRole == ReadOnly) {
        topMenu = NULL;
        arcMenu = NULL;
        syncArcMenu = NULL;
        commentMenu = NULL;
        multiProcMenu = NULL;
    }

    //Обычный режим
    if (myRole == NormalEditor) {
        topMenu = new QMenu();
        connect(topMenu, SIGNAL(aboutToShow()), this, SLOT(topMenuAboutToShow()));
        topMenu->addAction(deleteTopAction);
        topMenu->addSeparator();
        topMenu->addAction(setIconAction);
        topMenu->addAction(setTopPropertyAction);
        topMenu->addAction(makeAsRootAction);
        topMenu->addSeparator();
        topMenu->addAction(viewSubGraphAct);
        topMenu->addAction(editSubGraphAct);

        arcMenu = new QMenu();
        arcMenu->addAction(deleteArcAction);
        arcMenu->addSeparator();
        arcMenu->addAction(setArcPropertyAction);
        arcMenu->addAction(rebuildArcAction);

        syncArcMenu = new QMenu();
        syncArcMenu->addAction(deleteSyncAction);

        commentMenu = new QMenu();
        commentMenu->addAction(deleteCommentAction);
        commentMenu->addSeparator();
        commentMenu->addAction(setFontAction);

        multiProcMenu = new QMenu();
        multiProcMenu->addAction(deleteMultiProcTopAction);
        multiProcMenu->addSeparator();
        multiProcMenu->addAction(setMultiProcTopAction);
    }
}

void TDrawWindow::createActions()
{
    if (myRole == NormalEditor) {
        setIconAction = new QAction(tr("Загрузить иконку"), this);
        setIconAction->setStatusTip(tr("Отображает иконку на вершине вместо текста"));
        connect(setIconAction, SIGNAL(triggered()), this, SLOT(setItemIcon()));

        setTopPropertyAction = new QAction(tr("Свойства"), this);
        setTopPropertyAction->setStatusTip(tr("Изменить свойства вершины"));
        connect(setTopPropertyAction, SIGNAL(triggered()), this, SLOT(showTopPropDialog()));

        makeAsRootAction = new QAction(tr("Сделать корневой"), this);
        makeAsRootAction->setStatusTip(tr("Сделать корневой"));
        connect(makeAsRootAction, SIGNAL(triggered()), this, SLOT(makeAsRoot()));

        deleteTopAction = new QAction(QIcon(":/images/delete.png"), tr("Удалить"), this);
        deleteTopAction->setStatusTip(tr("Удаляет вершину"));
        connect(deleteTopAction, SIGNAL(triggered()), this, SLOT(deleteTop()));

        viewSubGraphAct = new QAction(tr("Просмотреть подграф"), this);
        viewSubGraphAct->setStatusTip(tr("Открывает подграф для просмотра"));
        connect(viewSubGraphAct, SIGNAL(triggered()), this, SLOT(viewSubGraph()));

        editSubGraphAct = new QAction(tr("Редактировать подграф"), this);
        editSubGraphAct->setStatusTip(tr("Открывает подграф для редактирования"));
        connect(editSubGraphAct, SIGNAL(triggered()), this, SLOT(editSubGraph()));

        setArcPropertyAction = new QAction(tr("Свойства"), this);
        setArcPropertyAction->setStatusTip(tr("Изменить свойства дуги"));
        connect(setArcPropertyAction, SIGNAL(triggered()), this, SLOT(showArcPropDialog()));

        deleteArcAction = new QAction(QIcon(":/images/delete.png"), tr("Удалить"), this);
        deleteArcAction->setStatusTip(tr("Удаляет дугу"));
        connect(deleteArcAction, SIGNAL(triggered()), this, SLOT(deleteArc()));

        rebuildArcAction = new QAction(tr("Перестроить"), this);
        rebuildArcAction->setStatusTip(tr("Перестраивает дугу по внутреннему алгоритму"));
        connect(rebuildArcAction, SIGNAL(triggered()), this, SLOT(rebuildArc()));

        deleteSyncAction = new QAction(QIcon(":/images/delete.png"), tr("Удалить"), this);
        deleteSyncAction->setStatusTip(tr("Удалить дугу синхронизации"));
        connect(deleteSyncAction, SIGNAL(triggered()), this, SLOT(deleteSync()));

        deleteCommentAction = new QAction(QIcon(":/images/delete.png"), tr("Удалить"), this);
        deleteCommentAction->setStatusTip(tr("Удаляет комментарий"));
        connect(deleteCommentAction, SIGNAL(triggered()), this, SLOT(deleteComment()));

        setFontAction = new QAction(tr("Шрифт"), this);
        connect(setFontAction, SIGNAL(triggered()), this, SLOT(showFontDialog()));

        deleteMultiProcTopAction = new QAction(QIcon(":/images/delete.png"), tr("Удалить"), this);
        deleteMultiProcTopAction->setStatusTip(tr("Удалить многопоточную вершину"));
        connect(deleteMultiProcTopAction, SIGNAL(triggered()), this, SLOT(deleteTop()));

        setMultiProcTopAction = new QAction(tr("Свойства"), this);
        setMultiProcTopAction->setStatusTip(tr("Изменить совйства многопоточной вершины"));
        connect(setMultiProcTopAction, SIGNAL(triggered()), this, SLOT(showMultiProcTopDialog()));
    }
}

void TDrawWindow::deleteTop()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QTop::Type)
        itemDeleted(item);
}

void TDrawWindow::deleteArc()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QSerialArcTop::Type)
        itemDeleted(item->parentItem());
}

void TDrawWindow::deleteSync()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() ==  QSyncArc::Type)
        itemDeleted(item);
}

void TDrawWindow::deleteComment()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QComment::Type)
        itemDeleted(item);
}

/*!
  Установить режим сцены
*/
void TDrawWindow::setMode(QDiagramScene::Mode mode)
{
    if (scene->mode() != mode) {
        scene->setMode(mode);
        emit sceneChanged();
    }
}

/*!
  Реакция на нажатие пункта меню: Загрузить иконку
*/
void TDrawWindow::setItemIcon()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Открыть картинку..."),
                                                    "",
                                                    tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    if (fileName.isEmpty())
        return;
    QNormalTop *top = qgraphicsitem_cast<QNormalTop *>(scene->selectedItems().first());
    QPixmap img(fileName);
    if (!img.isNull() && QFileInfo(fileName).size() < 10000) {
        top->setIcon(img);
        scene->invalidate(top->mapRectToScene(top->boundingRect()));
    } else QMessageBox::critical(NULL, tr("Ошибка"), tr("Размер картинки не должен превышать 10 кб"));

    emit sceneChanged();
}

/*!
  Реакция на нажатие пункта меню: Свойства вершины
*/
void TDrawWindow::showTopPropDialog(){
    TopPropertyDialog dlg;
    QNormalTop* top = qgraphicsitem_cast<QNormalTop *>(scene->selectedItems().first());
    dlg.prepareForm(top);
    if (dlg.exec()) {
        top = dlg.getResult();
        emit itemChanged(top);
        emit sceneChanged();
    }
}


/*!
  Реакция на нажатие пункта меню: Сохранить как картинку
*/
void TDrawWindow::saveAsImage(QString filename)
{
    if (scene == NULL)
        return;
    QSizeF size = scene->sceneRect().size();
    QImage Image(size.toSize(), QImage::Format_ARGB32_Premultiplied);
    Image.fill(0);
    QPainter painter(&Image);
    view->render(&painter, scene->sceneRect(), scene->sceneRect().toRect(), Qt::KeepAspectRatio);
    Image.save(filename);
}

/*!
  Реакция на нажатие пункта меню: Свойства дуги
*/
void TDrawWindow::showArcPropDialog()
{
    QArc* arc = qgraphicsitem_cast<QArc *>(scene->selectedItems().first()->parentItem());
    ArcPropertyDialog *dlg = ArcPropertyDialog::getDialog(arc);
    if (dlg->exec()) {
        arc = dlg->getResult();
        emit itemChanged(arc);
        emit sceneChanged();
    }
    delete dlg;
}

/*!
  Реакция на нажатие пункта меню: Сделать корневой
*/
void TDrawWindow::makeAsRoot()
{
    scene->setRootTop(qgraphicsitem_cast<QNormalTop* >(scene->selectedItems().first()));
    emit sceneChanged();
    emit itemChanged(scene->selectedItems().first());
}

/*!
  Возвращает список всех вершиен лежащих на сцене
*/
QList<QTop* > TDrawWindow::allTops() const
{
    QList<QTop* > topList;
    QList<QGraphicsItem *> items = scene->items();
    foreach (QGraphicsItem *item, items)
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));

    return topList;
}

/*!
  Возвращает список всех дуг лежащих на сцене
*/
QList<QArc* > TDrawWindow::allArcs() const
{
    QList<QArc* > arcList;
    QList<QGraphicsItem *> items = scene->items();
    foreach (QGraphicsItem *item, items)
        if (item->type() == QArc::Type)
            arcList.append(qgraphicsitem_cast<QArc* >(item));
    return arcList;
}

/*!
  Возвращает список всех многопоточных вершин лежащих на сцене
*/
QList<QMultiProcTop* > TDrawWindow::allMultiProcTop() const
{
    QList<QMultiProcTop* > topList;
    QList<QGraphicsItem *> items = scene->items();
    foreach (QGraphicsItem *item, items)
        if (item->type() == QMultiProcTop::Type)
            topList.append(qgraphicsitem_cast<QMultiProcTop* >(item));
    return topList;
}

/*!
  Возвращает список всех комментариев лежащих на сцене
*/
QList<QComment* > TDrawWindow::allComments() const
{
    QList<QComment* > commentList;
    QList<QGraphicsItem *> items = scene->items();
    foreach (QGraphicsItem *item, items)
        if (item->type() == QComment::Type)
            commentList.append(qgraphicsitem_cast<QComment* >(item));
    return commentList;
}

/*!
  Возвращает список всех дуг синхронизации лежащих на сцене
*/
QList<QSyncArc* > TDrawWindow::allSyncArcs() const
{
    QList<QSyncArc* > arcList;
    QList<QGraphicsItem *> items = scene->items();
    foreach (QGraphicsItem *item, items)
        if (item->type() == QSyncArc::Type)
            arcList.append(qgraphicsitem_cast<QSyncArc* >(item));
    return arcList;
}

/*!
  Возвращает граф
*/
Graph TDrawWindow::getGraph() const
{
    QList<Top> topList;
    foreach (QTop* top, allTops())
        topList.append(top->toTop());
    QList<Arc> arcList;
    foreach (QArc* arc, allArcs())
        arcList.append(arc->toArc());
    QList<Comment> commentList;
    foreach (QComment* comment, allComments())
        commentList.append(comment->toComment());

    QList<SyncArc> syncArcList;// = allSyncArcs();
    return Graph(myGraphName, myGraphExtName, topList, arcList, commentList, syncArcList);
}

/*!
  Загружает граф в редактор
*/

void TDrawWindow::loadGraph(const QString &name)
{
    globalLogger->log("TDrawWindow::loadGraph start", Logger::All);
    scene->clear();
    try {
        Graph graph = globalDBManager->getGraphDB(name);

        foreach (Top top, graph.topList) {
            QTop *qtop = NULL;
            if (top.type == Top::NormalTop) {
                qtop = new QNormalTop(topMenu, NULL, scene);
                if (top.isRoot)
                    scene->setRootTop(qgraphicsitem_cast<QNormalTop *>(qtop));
                double w = top.sizeX;
                double h = top.sizeY;
                qtop->setRect(-w/2, -h/2, w, h);
            } else if (top.type == Top::MultiProcTop) {
                QMultiProcTop *qtop1 = new QMultiProcTop(multiProcMenu, NULL, scene);
                qtop1->procCount = top.procCount;
                qtop = qtop1;
            }
            qtop->number = top.number;
            qtop->setPos(top.x, top.y);
            qtop->actor = top.actor;
        }

        QList<QPair<QArc *, int> > qarcList;
        foreach (Arc arc, graph.arcList) {
            QTop *startTop = NULL;
            QTop *endTop = NULL;
            QList<QTop* > topList = allTops();
            for (int i = 0; i < topList.count(); i++){
                if (topList[i]->number == arc.startTop)
                    startTop = topList[i];
                if (topList[i]->number == arc.endTop)
                    endTop = topList[i];
            }

            QArc *qarc = new QArc(startTop, endTop, arcMenu, NULL, scene);
            for (int i = 0; i < arc.lines.count(); i++){
                QStringList nodes = arc.lines.at(i).split(" ");
                QPointF startPoint = QPointF(nodes.at(0).toFloat(), nodes.at(1).toFloat());
                QPointF endPoint = QPointF(nodes.at(2).toFloat(), nodes.at(3).toFloat());
                qarc->newLine(startPoint, endPoint);
            }
            qarc->addLine(qarc->currentLine);
            qarc->currentLine = NULL;
            qarc->setArcType(QArc::ArcType(arc.type));
            qarc->predicate = arc.predicate;

            if (qarc->predicate != NULL && !globalPredicateList.contains(qarc->predicate->name))
                globalPredicateList.append(qarc->predicate->name);
            startTop->addArc(qarc);
            endTop->addArc(qarc);
            qarcList.append(qMakePair(qarc, arc.priority));
        }

        //расставляем приоритеты дуг
        for (int i = 0; i < qarcList.count(); i++) {
            qarcList.at(i).first->setPriority(qarcList.at(i).second);
        }

        foreach (Comment comment, graph.commentList) {
            QComment *qcomment = new QComment(commentMenu, NULL, scene);
            qcomment->setPos(comment.x, comment.y);
            qcomment->setPlainText(comment.text);
        }

        myGraphName = graph.name;
        myGraphExtName = graph.extName;
    } catch (QString e) {
        QMessageBox::critical(NULL, tr("Ошибка"), e);
    }

    if (myRole == ReadOnly)
        setWindowTitle(myGraphExtName + tr(" - Read-only"));
    else
        setWindowTitle(myGraphExtName);
    emit graphLoaded(myGraphName, myGraphExtName);
}

bool TDrawWindow::saveGraph(QString name, QString extName)
{
    if (name.isEmpty() || extName.isEmpty()) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Нельзя сохранить граф без имени."), QMessageBox::Ok);
        return false;
    }
    myGraphExtName = extName;
    myGraphName = name;
    Graph graph = getGraph();
    try {
        globalDBManager->saveGraph(graph);
        emit graphLoaded(graph.name, graph.extName);
    } catch (const QString s) {
        QMessageBox::critical(this, tr("Ошибка"), s, QMessageBox::Ok);
        return false;
    }
    return true;
}

bool TDrawWindow::updateGraph()
{
    Graph graph = getGraph();
    if (graph.extName != "") {
        try {
            globalDBManager->updateGraphDB(graph);
        } catch (const QString s) {
            QMessageBox::critical(this, tr("Ошибка"), s, QMessageBox::Ok);
            return false;
        }
        return true;
    } else return false;
}

bool TDrawWindow::saveStruct()
{
    Graph graph = getGraph();
    if (graph.extName != "") {
        try {
            globalDBManager->saveStructDB(graph);
        } catch (const QString s) {
            QMessageBox::critical(this, tr("Ошибка"), s, QMessageBox::Ok);
            return false;
        }
        return true;
    } else return false;
}

void TDrawWindow::showMultiProcTopDialog()
{
    QMultiProcTop* top = qgraphicsitem_cast<QMultiProcTop *>(scene->selectedItems().first());
    MultiProcTopPropertyDialog *dlg = MultiProcTopPropertyDialog::getDialog(top);
    if (dlg->exec()) {
        top = dlg->getResult();
        emit sceneChanged();
    }
}

void TDrawWindow::alignHLeft()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float left = topList.first()->mapRectToScene(topList.first()->rect()).left();
    foreach (QTop* top, topList) {
        if (left > top->mapRectToScene(top->rect()).left())
            left = top->mapRectToScene(top->rect()).left();
    }

    foreach (QTop* top, topList) {
        QLineF vector(top->mapRectToScene(top->rect()).left() + top->rect().width()/2, top->scenePos().y(), left + top->rect().width()/2, top->scenePos().y());

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type){
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }
    emit sceneChanged();
}

void TDrawWindow::alignHRight()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float right = topList.first()->mapRectToScene(topList.first()->rect()).right();
    foreach (QTop* top, topList) {
        if (right < top->mapRectToScene(top->rect()).right())
            right = top->mapRectToScene(top->rect()).right();
    }

    foreach (QTop* top, topList) {
        QLineF vector(top->mapRectToScene(top->rect()).right() - top->rect().width()/2, top->scenePos().y(), right - top->rect().width()/2, top->scenePos().y());

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type) {
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }
    emit sceneChanged();
}

void TDrawWindow::alignHCenter()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float center = 0;
    foreach (QTop* top, topList)
        center += top->scenePos().x();
    center /= topList.count();
    foreach (QTop* top, topList) {
        QLineF vector(top->scenePos().x(), top->scenePos().y(), center, top->scenePos().y());

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type){
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }

    emit sceneChanged();
}

void TDrawWindow::alignVTop()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float topEdge = topList.first()->mapRectToScene(topList.first()->rect()).top();
    foreach (QTop* top, topList) {
        if (topEdge > top->mapRectToScene(top->rect()).top())
            topEdge = top->mapRectToScene(top->rect()).top();
    }

    foreach (QTop* top, topList) {
        QLineF vector(top->scenePos().x(), top->mapRectToScene(top->rect()).top() + top->rect().height()/2, top->scenePos().x(), topEdge + top->rect().height()/2);

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type){
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }
    emit sceneChanged();
}

void TDrawWindow::alignVBottom()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float bottom = topList.first()->mapRectToScene(topList.first()->rect()).bottom();
    foreach (QTop* top, topList) {
        if (bottom < top->mapRectToScene(top->rect()).bottom())
            bottom = top->mapRectToScene(top->rect()).bottom();
    }
    if (topList.count() == 0) return;
    foreach (QTop* top, topList) {
        QLineF vector(top->scenePos().x(), top->mapRectToScene(top->rect()).bottom() - top->rect().height()/2, top->scenePos().x(), bottom - top->rect().height()/2);

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type){
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }
    emit sceneChanged();
}

void TDrawWindow::alignVCenter()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() == 0) return;
    float center = 0;
    foreach (QTop* top, topList)
        center += top->scenePos().y();
    center /= topList.count();
    foreach (QTop* top, topList) {
        QLineF vector(top->scenePos().x(), top->scenePos().y(), top->scenePos().x(), center);

        bool allowMove = true;
        top->setPos(top->scenePos().x() + vector.dx(), top->scenePos().y() + vector.dy());

        //надем пересекающиеся элементы
        QList<QGraphicsItem* > itemList = scene->collidingItems(top, Qt::IntersectsItemBoundingRect);
        foreach(QGraphicsItem* item, itemList)
            //если есть хотя бы одна вершина
            if (item->type() == QTop::Type){
                //запретим перемещение
                allowMove = false;
                break;
            }

        //вернем все на место
        top->setPos(top->scenePos().x() - vector.dx(), top->scenePos().y() - vector.dy());
        if (allowMove)
            itemMoved(top, vector);
    }

    emit sceneChanged();
}

void TDrawWindow::showFontDialog()
{
    QFontDialog dlg;
    QComment* comment = qgraphicsitem_cast<QComment *>(scene->selectedItems().first());
    dlg.setCurrentFont(comment->font());
    if (dlg.exec())
        comment->setFont(dlg.selectedFont());
}

void TDrawWindow::showDataLayer(bool show)
{

}

bool arcPriorLessThan(const QArc* arc1, const QArc* arc2)
{
    return (arc1->priority() < arc2->priority());
}

void TDrawWindow::distribHorizontally()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    //else if (item->type() == QArcLine::Type || item->type() == QSerialArcTop::Type)
    //    arcList.append(qgraphicsitem_cast<QArc* >(item->parentItem()));

    //Если выделено несколько вершин - распределяем вершины
    if (topList.count() > 1) {
        //хитрый ход!
        //Сортируем вершины в списке. Самая верхняя вершина - первая. Нижняя - последняя.
        qSort(topList.begin(), topList.end(), topLeftThan);

        float left = topList.first()->mapRectToScene(topList.first()->rect()).left();
        float right = topList.last()->mapRectToScene(topList.last()->rect()).right();

        float totalWidth = 0;
        foreach (QTop* top, topList) {
            totalWidth += top->rect().width();
        }
        float dist = (fabs(left - right) - totalWidth) / (topList.count() - 1);

        //не получается распределить. мало места.
        if (dist <= 0) return;

        foreach (QTop* top, topList){
            itemMoved(top, QLineF(0, 0, left - top->mapRectToScene(top->rect()).left(), 0));
            left += top->rect().width() + dist;
        }
    }
    //Если одна вершина - то дуги у этой вершины
    else if (topList.count() == 1) {
        QTop* top = qgraphicsitem_cast<QTop* >(scene->selectedItems().first());
        QList<QArc *> arcs = top->allArcs();
        //распределяем дуги сверху
        QPolygonF myPolygon(top->rect());
        QPointF intersectPoint;
        QLineF topEdge = QLineF(myPolygon.at(0) + top->scenePos(), myPolygon.at(1) + top->scenePos());
        QList<QArc* > inArcs;
        QList<QArc* > outArcs;
        foreach (QArc *arc, arcs) {
            QLineF arcLine;
            if (arc->lines.count() == 0)
                continue;
            if (top == arc->startItem()) {
                arcLine = arc->lines.first()->line();
                QLineF::IntersectType intersectType = topEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    outArcs.append(arc);
            }
            if (top == arc->endItem()) {
                arcLine = arc->lines.last()->line();
                QLineF::IntersectType intersectType = topEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    inArcs.append(arc);
            }
        }
        qSort(inArcs.begin(), inArcs.end(), arcPriorLessThan);
        qSort(outArcs.begin(), outArcs.end(), arcPriorLessThan);

        //Вычисляем промежуток между дугами
        double dist = top->rect().width()/(inArcs.count() + outArcs.count() + 1);
        foreach (QArc* arc, inArcs) {
            QArcLine *line = arc->lines.last();
            QPointF new_p1(top->mapRectToScene(top->rect()).topLeft().x() + (inArcs.indexOf(arc) + 1) * dist, line->line().y1());
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        int delta = inArcs.count();
        foreach (QArc* arc, outArcs) {
            QArcLine *line = arc->lines.first();
            QPointF new_p1(top->mapRectToScene(top->rect()).topLeft().x() + (outArcs.indexOf(arc) + 1 + delta) * dist, line->line().y1());
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        //распределяем дуги снизу
        inArcs.clear();
        outArcs.clear();
        QLineF rightEdge = QLineF(myPolygon.at(2) + top->scenePos(), myPolygon.at(3) + top->scenePos());
        foreach (QArc *arc, arcs) {
            QLineF arcLine;
            if (arc->lines.count() == 0)
                continue;
            if (top == arc->startItem()) {
                arcLine = arc->lines.first()->line();
                QLineF::IntersectType intersectType = rightEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    outArcs.append(arc);
            }
            if (top == arc->endItem()) {
                arcLine = arc->lines.last()->line();
                QLineF::IntersectType intersectType = rightEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    inArcs.append(arc);
            }
        }

        //Вычисляем промежуток между дугами
        dist = top->rect().width()/(inArcs.count() + outArcs.count() + 1);
        foreach (QArc* arc, inArcs) {
            QArcLine *line = arc->lines.last();
            QPointF new_p1(top->mapRectToScene(top->rect()).topLeft().x() + (inArcs.indexOf(arc) + 1) * dist, line->line().y1());
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        delta = inArcs.count();
        foreach (QArc* arc, outArcs) {
            QArcLine *line = arc->lines.first();
            QPointF new_p1(top->mapRectToScene(top->rect()).topLeft().x() + (outArcs.indexOf(arc) + 1 + delta) * dist, line->line().y1());
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }
    }
    emit sceneChanged();
}

void TDrawWindow::distribVertically()
{
    QList<QTop *> topList;
    foreach (QGraphicsItem* item, scene->selectedItems())
        if (item->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(item));
    if (topList.count() > 1) {

        qSort(topList.begin(), topList.end(), topUpperThan);

        float topEdge = topList.first()->mapRectToScene(topList.first()->rect()).top();
        float bottom = topList.last()->mapRectToScene(topList.last()->rect()).bottom();

        float totalHeight = 0;
        foreach (QTop* top, topList)
            totalHeight += top->rect().height();

        float dist = (fabs(topEdge - bottom) - totalHeight) / (topList.count() - 1);
        //не получается распределить. мало места.
        if (dist <= 0) return;

        foreach (QTop* top, topList){
            itemMoved(top, QLineF(0, 0, 0, topEdge - top->mapRectToScene(top->rect()).top()));
            topEdge += top->rect().height() + dist;
        }
    }
    //Если одна вершина - то дуги у этой вершины
    else if (topList.count() == 1) {
        QTop* top = qgraphicsitem_cast<QTop* >(scene->selectedItems().first());
        QList<QArc *> arcs = top->allArcs();
        //распределяем дуги справа
        QPolygonF myPolygon(top->rect());
        QPointF intersectPoint;
        QLineF leftEdge = QLineF(myPolygon.at(1) + top->scenePos(), myPolygon.at(2) + top->scenePos());
        QList<QArc* > inArcs;
        QList<QArc* > outArcs;
        foreach (QArc *arc, arcs) {
            QLineF arcLine;
            if (arc->lines.count() == 0)
                continue;
            if (top == arc->startItem()) {
                arcLine = arc->lines.first()->line();
                QLineF::IntersectType intersectType = leftEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    outArcs.append(arc);
            }
            if (top == arc->endItem()) {
                arcLine = arc->lines.last()->line();
                QLineF::IntersectType intersectType = leftEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    inArcs.append(arc);
            }
        }
        qSort(inArcs.begin(), inArcs.end(), arcPriorLessThan);
        qSort(outArcs.begin(), outArcs.end(), arcPriorLessThan);

        //Вычисляем промежуток между дугами
        double dist = top->rect().height()/(inArcs.count() + outArcs.count() + 1);
        foreach (QArc* arc, inArcs) {
            QArcLine* line = arc->lines.last();
            QPointF new_p1(line->line().x1(), top->mapRectToScene(top->rect()).bottomLeft().y() - (inArcs.indexOf(arc) + 1) * dist);
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        int delta = inArcs.count();
        foreach (QArc* arc, outArcs) {
            QArcLine* line = arc->lines.first();
            QPointF new_p1(line->line().x1(), top->mapRectToScene(top->rect()).bottomLeft().y() - (outArcs.indexOf(arc) + 1 + delta) * dist);
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        //распределяем дуги слева
        inArcs.clear();
        outArcs.clear();
        QLineF bottomEdge = QLineF(myPolygon.at(3) + top->scenePos(), myPolygon.at(4) + top->scenePos());
        foreach (QArc *arc, arcs) {
            QLineF arcLine;
            if (arc->lines.count() == 0)
                continue;
            if (top == arc->startItem()) {
                arcLine = arc->lines.first()->line();
                QLineF::IntersectType intersectType = bottomEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    outArcs.append(arc);
            }
            if (top == arc->endItem()) {
                arcLine = arc->lines.last()->line();
                QLineF::IntersectType intersectType = bottomEdge.intersect(arcLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    inArcs.append(arc);
            }
        }

        //Вычисляем промежуток между дугами
        dist = top->rect().height()/(inArcs.count() + outArcs.count() + 1);
        foreach (QArc* arc, inArcs) {
            QArcLine* line = arc->lines.last();
            QPointF new_p1(line->line().x1(), top->mapRectToScene(top->rect()).bottomLeft().y() - (inArcs.indexOf(arc) + 1) * dist);
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }

        delta = inArcs.count();
        foreach (QArc* arc, outArcs) {
            QArcLine* line = arc->lines.first();
            QPointF new_p1(line->line().x1(), top->mapRectToScene(top->rect()).bottomLeft().y() - (outArcs.indexOf(arc) + 1 + delta) * dist);
            QLineF vector(line->line().p1(), new_p1);
            arc->moveLine(line, vector.dx(), vector.dy());
        }
    }
    emit sceneChanged();
}

void TDrawWindow::itemInserted(QGraphicsItem *item)
{
    QUndoCommand *addCommnad = new AddCommand(item, scene);
    undoStack->push(addCommnad);
    emit sceneChanged();
    emit itemChanged(item);
}

void TDrawWindow::itemDeleted(QGraphicsItem *item)
{
    QUndoCommand *deleteCommand = new DeleteCommand(item, scene);
    undoStack->push(deleteCommand);
    emit sceneChanged();
}

void TDrawWindow::itemMoved(QGraphicsItem *item, QLineF vector)
{
    QList<QGraphicsItem*> items;
    items.append(item);
    QUndoCommand *moveCommnad = new MoveCommand(items, scene,  vector);
    undoStack->push(moveCommnad);
    emit sceneChanged();
    emit itemChanged(item);
}

void TDrawWindow::itemsMoved(QList<QGraphicsItem *>items, QLineF vector)
{
    QUndoCommand *moveCommnad = new MoveCommand(items, scene,  vector);
    undoStack->push(moveCommnad);
    emit sceneChanged();
    emit itemChanged(items.first());
}

void TDrawWindow::rebuildArc()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QSerialArcTop::Type) {
        QArc* arc = qgraphicsitem_cast<QArc* >(item->parentItem());
        arc->autoBuild(arc->startItem(), 0, 0);
        emit itemChanged(arc);
        emit sceneChanged();
    }
}

void TDrawWindow::selectionChanged()
{
    emit selectionChanged(scene->selectedItems());
    if (scene->selectedItems().count() > 0)
        emit itemChanged(scene->selectedItems().first());
}

void TDrawWindow::scale(float s)
{
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(s, s);
}

void TDrawWindow::topMenuAboutToShow()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QTop::Type) {
        QTop *top = qgraphicsitem_cast<QTop *>(item);
        if (top->actor != NULL && top->actor->type == Actor::GraphType) {
            viewSubGraphAct->setEnabled(true);
            editSubGraphAct->setEnabled(true);
        } else {
            viewSubGraphAct->setEnabled(false);
            editSubGraphAct->setEnabled(false);
        }
    }
}

void TDrawWindow::viewSubGraph()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QTop::Type) {
        QTop *top = qgraphicsitem_cast<QTop *>(item);
        if (top->actor != NULL && top->actor->type == Actor::GraphType) {
            TDrawWindow *drawWindow = new TDrawWindow(TDrawWindow::ReadOnly, this);
            drawWindow->loadGraph(top->actor->name);
            connect(drawWindow, SIGNAL(itemChanged(QGraphicsItem*)), this, SIGNAL(itemChanged(QGraphicsItem*)));
            drawWindow->show();
        }
    }
}

void TDrawWindow::editSubGraph()
{
    QGraphicsItem *item = scene->selectedItems().first();
    if (item->type() == QTop::Type) {
        QTop *top = qgraphicsitem_cast<QTop *>(item);
        if (top->actor != NULL && top->actor->type == Actor::GraphType) {
            QString name = top->actor->name;
            loadGraph(name);
        }
    }
}

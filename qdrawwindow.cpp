#include <QtGui>
#include <QCoreApplication>
#include <QPointF>
#include "qdrawwindow.h"
#include "qtop.h"
#include "qsyncarc.h"
#include "arcpropertydialog.h"
#include "qserialarctop.h"
#include "qparallelarctop.h"
#include "qterminatearctop.h"
#include "qnormaltop.h"
#include "multiproctoppropertydialog.h"

TDrawWindow::TDrawWindow()
{
    createActions();
    createMenus();

    view = new QGraphicsView();
    scene = new QDiagramScene();
    scene->setTopMenu(topMenu);
    scene->setArcMenu(arcMenu);
    scene->setCommentMenu(commentMenu);
    scene->setSyncArcMenu(syncArcMenu);
    scene->setMultiProcTopMenu(multiProcMenu);
    scene->setBackgroundBrush(QBrush(Qt::white));
    scene->setSceneRect(-800, -600, 1600, 1200);
    /*connect(scene, SIGNAL(itemInserted(QGraphicsItem*)),
            this, SLOT(itemInserted(QTop *)));*/
    connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
        this, SLOT(itemSelected(QGraphicsItem *)));  
    connect(scene, SIGNAL(textInserted(QComment *)),
        this, SLOT(textInserted(QComment *)));

    view->setScene(scene);
    view->setAlignment(Qt::AlignCenter);
    /*QBrush myBackgroundBrush = view->backgroundBrush();
    myBackgroundBrush.setColor(Qt::lightGray);
    myBackgroundBrush.setStyle(Qt::CrossPattern);
    view->setBackgroundBrush(myBackgroundBrush);*/
    this->setCentralWidget(view);
    setMode(QDiagramScene::MoveItem);
}

void TDrawWindow::createMenus()
{
    topMenu = new QMenu();
    topMenu->addAction(deleteAction);
    topMenu->addSeparator();
    topMenu->addAction(setIconAction);
    topMenu->addAction(setTopPropertyAction);
    topMenu->addAction(makeAsRootAction);

    arcMenu = new QMenu();
    arcMenu->addAction(deleteArcAction);
    arcMenu->addSeparator();
    arcMenu->addAction(setArcPropertyAction);

    syncArcMenu = new QMenu();
    syncArcMenu->addAction(deleteSyncAction);

    commentMenu = new QMenu();
    commentMenu->addAction(deleteCommentAction);

    multiProcMenu = new QMenu();
    multiProcMenu->addAction(deleteMultiProcTopAction);
    multiProcMenu->addSeparator();
    multiProcMenu->addAction(setMultiProcTopAction);
}

void TDrawWindow::createActions()
{
    setIconAction = new QAction(tr("Загрузить иконку"), this);
    setIconAction->setStatusTip(tr("Отображает иконку на вершине вместо текста"));
    connect(setIconAction, SIGNAL(triggered()), this, SLOT(setItemIcon()));

    setTopPropertyAction = new QAction(tr("Свойства"), this);
    setTopPropertyAction->setStatusTip(tr("Изменить свойства вершины"));
    connect(setTopPropertyAction, SIGNAL(triggered()), this, SLOT(showTopPropDialog()));

    makeAsRootAction = new QAction(tr("Сделать корневой"), this);
    makeAsRootAction->setStatusTip(tr("Сделать корневой"));
    connect(makeAsRootAction, SIGNAL(triggered()), this, SLOT(makeAsRoot()));

    deleteAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteAction->setStatusTip(tr("Удаляет вершину"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteTop()));

    setArcPropertyAction = new QAction(tr("Свойства"), this);
    setArcPropertyAction->setStatusTip(tr("Изменить свойства дуги"));
    connect(setArcPropertyAction, SIGNAL(triggered()), this, SLOT(showArcPropDialog()));

    deleteArcAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteArcAction->setStatusTip(tr("Удаляет дугу"));
    connect(deleteArcAction, SIGNAL(triggered()), this, SLOT(deleteArc()));

    deleteSyncAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteSyncAction->setStatusTip(tr("Удалить дугу синхронизации"));
    connect(deleteSyncAction, SIGNAL(triggered()), this, SLOT(deleteSync()));

    deleteCommentAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteCommentAction->setStatusTip(tr("Удаляет комментарий"));
    connect(deleteCommentAction, SIGNAL(triggered()), this, SLOT(deleteComment()));

    deleteMultiProcTopAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteMultiProcTopAction->setStatusTip(tr("Удалить многопоточную вершину"));
    connect(deleteMultiProcTopAction, SIGNAL(triggered()), this, SLOT(deleteTop()));

    setMultiProcTopAction = new QAction(tr("Свойства"), this);
    setMultiProcTopAction->setStatusTip(tr("Изменить совйства многопоточной вершины"));
    connect(setMultiProcTopAction, SIGNAL(triggered()), this, SLOT(showMultiProcTopDialog()));
}

void TDrawWindow::deleteTop()
{
    foreach (QGraphicsItem *item, scene->selectedItems())
        if (item->type() == QTop::Type)
            delete item;
    emit sceneChanged();
}

void TDrawWindow::deleteArc()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  QSerialArcTop::Type) {
            QArc *arc = qgraphicsitem_cast<QArc *>(item->parentItem());
            delete arc;
        }        
    }
    emit sceneChanged();
}

void TDrawWindow::deleteSync()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  QSyncArc::Type) {
            delete item;
        }
    }
    emit sceneChanged();
}

void TDrawWindow::deleteComment()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  QComment::Type) {
            scene->removeItem(item);
        }
    }
    emit sceneChanged();
}

void TDrawWindow::itemSelected(QGraphicsItem *item)
{

}

void TDrawWindow::textInserted(QComment *)
{
    emit sceneChanged();
}

/*!
  Установить режим сцены
*/
void TDrawWindow::setMode(QDiagramScene::Mode mode)
{
    scene->setMode(mode);
    myMode = mode;
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
    foreach (QGraphicsItem *item, scene->selectedItems())
        if (item->type() == QTop::Type)
            if (!fileName.isEmpty()) {
                QImage img(fileName);
                if (!img.isNull()) {
                    qgraphicsitem_cast<QNormalTop *>(item)->setIcon(img);
                    scene->invalidate(item->mapRectToScene(item->boundingRect()));
                }
            }

    emit sceneChanged();
}

/*!
  Реакция на нажатие пункта меню: Свойства вершины
*/
void TDrawWindow::showTopPropDialog(){
    TopPropertyDialog dlg;
    QNormalTop* top = qgraphicsitem_cast<QNormalTop *>(scene->selectedItems().first());
    dlg.prepareForm(top);
    if (dlg.exec())
        top = dlg.getResult();
}


/*!
  Реакция на нажатие пункта меню: Сохранить как картинку
*/
void TDrawWindow::saveAsImage(QString filename)
{
    if (scene != NULL) {
        QSizeF size = scene->sceneRect().size();
        QImage Image(size.toSize(), QImage::Format_ARGB32_Premultiplied);
        Image.fill(0);
        QPainter painter(&Image);
        view->render(&painter, scene->sceneRect(), scene->sceneRect().toRect(), Qt::KeepAspectRatio);
        Image.save(filename);
    }
}

/*!
  Реакция на нажатие пункта меню: Свойства дуги
*/
void TDrawWindow::showArcPropDialog()
{
    ArcPropertyDialog dlg;
    QArc* arc = qgraphicsitem_cast<QArc *>(scene->selectedItems().first()->parentItem());
    dlg.prepareForm(arc);
    if (dlg.exec()){
        dlg.getResult();
    }
}

/*!
  Реакция на нажатие пункта меню: Сделать корневой
*/
void TDrawWindow::makeAsRoot(){
    scene->setRootTop(qgraphicsitem_cast<QNormalTop* >(scene->selectedItems().first()));
}

/*!
  Возвращает список всех вершиен лежащих на сцене
*/
QList<QTop* > TDrawWindow::allTops(){
    QList<QTop* > topList;
    for (int i = 0; i < scene->items().count(); i++)
        if (scene->items().at(i)->type() == QTop::Type)
            topList.append(qgraphicsitem_cast<QTop* >(scene->items().at(i)));
    return topList;
}

/*!
  Возвращает список всех дуг лежащих на сцене
*/
QList<QArc* > TDrawWindow::allArcs(){
    QList<QArc* > arcList;
    for (int i = 0; i < scene->items().count(); i++)
        if (scene->items().at(i)->type() == QArc::Type)
            arcList.append(qgraphicsitem_cast<QArc* >(scene->items().at(i)));
    return arcList;
}

/*!
  Возвращает список всех многопоточных вершин лежащих на сцене
*/
QList<QMultiProcTop* > TDrawWindow::allMultiProcTop(){
    QList<QMultiProcTop* > topList;
    for (int i = 0; i < scene->items().count(); i++)
        if (scene->items().at(i)->type() == QMultiProcTop::Type)
            topList.append(qgraphicsitem_cast<QMultiProcTop* >(scene->items().at(i)));
    return topList;
}

/*!
  Возвращает список всех комментариев лежащих на сцене
*/
QList<QComment* > TDrawWindow::allComments(){
    QList<QComment* > commentList;
    for (int i = 0; i < scene->items().count(); i++)
        if (scene->items().at(i)->type() == QComment::Type)
            commentList.append(qgraphicsitem_cast<QComment* >(scene->items().at(i)));
    return commentList;
}

/*!
  Возвращает список всех дуг синхронизации лежащих на сцене
*/
QList<QSyncArc* > TDrawWindow::allSyncArcs(){
    QList<QSyncArc* > arcList;
    for (int i = 0; i < scene->items().count(); i++)
        if (scene->items().at(i)->type() == QSyncArc::Type)
            arcList.append(qgraphicsitem_cast<QSyncArc* >(scene->items().at(i)));
    return arcList;
}

/*!
  Возвращает граф
*/
Graph* TDrawWindow::getGraph()
{
    QList<Top* > topList;
    foreach (QTop* top, allTops())
        topList.append(top->toTop());
    QList<Arc* > arcList;
    foreach (QArc* arc, allArcs())
        arcList.append(arc->toArc());
    QList<Comment* > commentList;
    foreach (QComment* comment, allComments())
        commentList.append(comment->toComment());

    QList<QSyncArc* > syncArcList = allSyncArcs();
    QList<QMultiProcTop* > multiProcTopList = allMultiProcTop();
    Graph* result = new Graph("", "", topList, arcList, commentList, syncArcList, multiProcTopList);
    return result;
}

/*!
  Загружает граф в редактор
*/

void TDrawWindow::loadGraph(QString extName, DataBaseManager* dbManager)
{
    QList<Top* > topList;
    QList<Arc* > arcList;
    QList<Comment* > commentList;
    QList<QSyncArc* > syncArcList;
    QList<QMultiProcTop* > multiProcTopList;
    Graph graph("", "", topList, arcList, commentList, syncArcList, multiProcTopList);
    dbManager->getGraph(extName, graph);
    foreach (Top* top, graph.topList) {
        if (top->type == "T") {
            QNormalTop *qtop = new QNormalTop(topMenu, NULL, scene);
            qtop->number = top->number;
            if (top->isRoot)
                scene->setRootTop(qtop);
            double w = top->sizeX;
            double h = top->sizeY;
            qtop->setRect(-w/2, -h/2, w, h);
            qtop->setPos(top->x, top->y);
            qtop->actor = dbManager->getActor(top->actor);
        } else if (top->type == "M") {
            QMultiProcTop *qtop = new QMultiProcTop(multiProcMenu, NULL, scene);
            qtop->number = top->number;
            qtop->setPos(top->x, top->y);
            qtop->procCount = top->procCount;
            qtop->actor = dbManager->getActor(top->actor);
        }
    }

    foreach (Arc* arc, graph.arcList) {
        QTop *startTop = NULL;
        QTop *endTop = NULL;
        QList<QTop* > topList = allTops();
        for (int i = 0; i < topList.count(); i++){
            if (topList.at(i)->number == arc->startTop)
                startTop = topList.at(i);
            if (topList.at(i)->number == arc->endTop)
                endTop = topList.at(i);
        }

        QArc *qarc = new QArc(startTop, endTop, arcMenu, NULL, scene);
        for (int i = 0; i < arc->lines.count(); i++){
            QStringList nodes = arc->lines.at(i).split(" ");
            QPointF startPoint = QPointF(nodes.at(0).toFloat(), nodes.at(1).toFloat());
            QPointF endPoint = QPointF(nodes.at(2).toFloat(), nodes.at(3).toFloat());
            qarc->newLine(startPoint, endPoint);
        }
        qarc->addLine(qarc->currentLine);
        qarc->currentLine = NULL;
        qarc->setPriority(arc->priority);
        qarc->setArcType(QArc::ArcType(arc->type));
        qarc->predicate = dbManager->getPredicate(arc->predicate);
        startTop->addArc(qarc);
        endTop->addArc(qarc);
    }

    foreach (Comment* comment, graph.commentList) {
        QComment *qcomment = new QComment(NULL, NULL, scene);
        qcomment->setPos(comment->x, comment->y);
        qcomment->setPlainText(comment->text);
    }
}

void TDrawWindow::saveGraph(QString name, QString extName, DataBaseManager *dbManager, bool update)
{
    Graph* graph = getGraph();
    graph->extName = extName;
    graph->name = name;
    if (update)
        dbManager->updateGraph(graph);
    else
        dbManager->saveGraph(graph);
}

void TDrawWindow::saveStruct(QString name, DataBaseManager *dbManager)
{
    Graph* graph = getGraph();
    graph->name = name;
    dbManager->saveStruct(graph);
}

void TDrawWindow::showMultiProcTopDialog()
{
    MultiProcTopPropertyDialog dlg;
    QMultiProcTop* top = qgraphicsitem_cast<QMultiProcTop *>(scene->selectedItems().first());
    dlg.prepareForm(top);
    if (dlg.exec())
        top = dlg.getResult();
}

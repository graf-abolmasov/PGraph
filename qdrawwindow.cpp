#include <QtGui>
#include <QCoreApplication>
#include <QPointF>
#include "qdrawwindow.h"
#include "qdiagramitem.h"
#include "qsyncarc.h"
#include "arcpropertydialog.h"
#include "qserialarctop.h"
#include "qparallelarctop.h"
#include "qterminatearctop.h"

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
    scene->setSceneRect(-800, -600, 800, 600);
    connect(scene, SIGNAL(itemInserted(QTop *)),
            this, SLOT(itemInserted(QTop *)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
        this, SLOT(itemSelected(QGraphicsItem *)));  
    connect(scene, SIGNAL(textInserted(TComment *)),
        this, SLOT(textInserted(TComment *)));

    view->setScene(scene);
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
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

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
}

void TDrawWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == QTop::Type) {
            qgraphicsitem_cast<QTop *>(item)->removeArcs();
            qgraphicsitem_cast<QTop *>(item)->removeSyncs();
        }
        scene->removeItem(item);
    }
    emit sceneChanged();
}

void TDrawWindow::deleteArc()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  QSerialArcTop::Type) {
            QArc *arc = qgraphicsitem_cast<QArc *>(item->parentItem());
            arc->startItem()->removeArc(arc);
            arc->endItem()->removeArc(arc);
            scene->removeItem(item);
            foreach (QGraphicsLineItem *line, arc->lines){
                scene->removeItem(line);
            }
            scene->removeItem(arc);
            delete arc;
            arc = NULL;
        }        
    }
    emit sceneChanged();
}

void TDrawWindow::deleteSync()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  QSyncArc::Type) {
            QSyncArc *arc = qgraphicsitem_cast<QSyncArc *>(item);
            arc->startItem()->removeSync(arc);
            arc->endItem()->removeSync(arc);
            scene->removeItem(item);
            delete arc;
            arc = NULL;
        }
    }
    emit sceneChanged();
}

void TDrawWindow::deleteComment()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  TComment::Type) {
            scene->removeItem(item);
        }
    }
    emit sceneChanged();
}

void TDrawWindow::itemInserted(QTop *item)
{
    //setMode(QDiagramScene::MoveItem);
    emit sceneChanged();
}

void TDrawWindow::itemSelected(QGraphicsItem *item)
{
    //setWindowTitle("sdfdasf");
}

void TDrawWindow::textInserted(TComment *)
{
    //setMode(QDiagramScene::MoveItem);
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
                                                    tr("All Files (*)"));
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == QTop::Type) {
            if (!fileName.isEmpty()){
                QImage img(fileName);
                if (!img.isNull()){
                    qgraphicsitem_cast<QTop *>(item)->setIcon(img);
                    scene->invalidate(item->mapRectToScene(item->boundingRect()));
                }
            }
        }
    }

    emit sceneChanged();
}

/*!
  Реакция на нажатие пункта меню: Свойства
*/
void TDrawWindow::showTopPropDialog(){
    TopPropertyDialog dlg;
    QTop* top = qgraphicsitem_cast<QTop *>(scene->selectedItems().first());
    dlg.prepareForm(top);
    if (dlg.exec()){
        top = dlg.getResult();
    }
}



/*!
  Реакция на нажатие пункта меню: Сохранить как картинку
*/
void TDrawWindow::saveAsImage(QString filename)
{
    if (scene != NULL){
        QSizeF size = scene->sceneRect().size();
        QImage Image(size.toSize(), QImage::Format_ARGB32_Premultiplied);
        Image.fill(0);
        QPainter painter(&Image);
        scene->render(&painter);
        Image.save(filename);
    }
}

/*!
  Реакция на нажатие пункта меню: Свойства
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
    scene->setRootTop(qgraphicsitem_cast<QTop* >(scene->selectedItems().first()));
}

/*!
  Возвращает список всех вершиен лежащих на сцене
*/
QList<QTop* > TDrawWindow::allTops(){
    QList<QTop* > topList;
    for (int i = 0; i < scene->items().count(); i++){
        if (scene->items().at(i)->type() == QTop::Type){
            topList.append(qgraphicsitem_cast<QTop* >(scene->items().at(i)));
        }
    }
    return topList;
}

/*!
  Возвращает список всех дуг лежащих на сцене
*/
QList<QArc* > TDrawWindow::allArcs(){
    QList<QArc* > arcList;
    for (int i = 0; i < scene->items().count(); i++){
        if (scene->items().at(i)->type() == QArc::Type){
            arcList.append(qgraphicsitem_cast<QArc* >(scene->items().at(i)));
        }
    }
    return arcList;
}

/*!
  Возвращает список всех комментариев лежащих на сцене
*/
QList<TComment* > TDrawWindow::allComments(){
    QList<TComment* > commentList;
    for (int i = 0; i < scene->items().count(); i++){
        if (scene->items().at(i)->type() == TComment::Type){
            commentList.append(qgraphicsitem_cast<TComment* >(scene->items().at(i)));
        }
    }
    return commentList;
}

/*!
  Возвращает список всех дуг синхронизации лежащих на сцене
*/
QList<QSyncArc* > TDrawWindow::allSyncArcs(){
    QList<QSyncArc* > arcList;
    for (int i = 0; i < scene->items().count(); i++){
        if (scene->items().at(i)->type() == QSyncArc::Type){
            arcList.append(qgraphicsitem_cast<QSyncArc* >(scene->items().at(i)));
        }
    }
    return arcList;
}

/*!
  Возвращает граф
*/
QGraph* TDrawWindow::graph()
{
    QGraph* result = new QGraph();
    result->arcList     = allArcs();
    result->commentList = allComments();
    result->syncArcList = allSyncArcs();
    result->topList     = allTops();
    return result;
}

/*!
  Загружает граф в редактор
*/

void TDrawWindow::loadGraph(DataBaseManager* dbManager)
{
    dbManager->getGraph("Project1", scene, arcMenu, topMenu, syncArcMenu);
}

void TDrawWindow::saveGraph(DataBaseManager *dbManager)
{
    dbManager->saveGraph("Project1", "aaaaaaag", graph());
}

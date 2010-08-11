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
    /*connect(scene, SIGNAL(itemInserted(QGraphicsItem*)),
            this, SLOT(itemInserted(QTop *)));*/
    connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
        this, SLOT(itemSelected(QGraphicsItem *)));  
    connect(scene, SIGNAL(textInserted(QComment *)),
        this, SLOT(textInserted(QComment *)));

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
                    qgraphicsitem_cast<QTop *>(item)->setIcon(img);
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
    QTop* top = qgraphicsitem_cast<QTop *>(scene->selectedItems().first());
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
        scene->render(&painter, scene->sceneRect(), scene->sceneRect(), Qt::KeepAspectRatio);
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
    scene->setRootTop(qgraphicsitem_cast<QTop* >(scene->selectedItems().first()));
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
        QTop *qtop = new QTop(topMenu, NULL, scene);
        qtop->setPos(top->x, top->y);
        qtop->number = top->number;
        if (top->isRoot) scene->setRootTop(qtop);

        QPolygonF myPolygon;
        double w = top->sizeX;
        double h = top->sizeY;
        myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
                << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
                << QPointF(-w/2, h/2);
        qtop->setPolygon(myPolygon);
        qtop->actor = dbManager->getActor(top->actor);
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

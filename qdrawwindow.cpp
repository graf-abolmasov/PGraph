#include <QtGui>
#include "qdrawwindow.h"
#include "qarc.h"
#include "toppropertydialog.h"
#include <QCoreApplication>


TDrawWindow::TDrawWindow()
{
    createActions();
    createMenus();

    view = new QGraphicsView();
    scene = new QDiagramScene();
    scene->setItemMenu(itemMenu);
    scene->setBackgroundBrush(QBrush(Qt::white));
    scene->setArcMenu(arcMenu);
    scene->setCommentMenu(commentMenu);
    scene->setSceneRect(-800, -600, 800, 600);
    connect(scene, SIGNAL(itemInserted(TTop *)),
            this, SLOT(itemInserted(TTop *)));
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
    itemMenu = new QMenu();
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(Action1);
    itemMenu->addAction(Action2);
    itemMenu->addAction(makeAsRootAction);

    arcMenu = new QMenu();
    arcMenu->addAction(deleteArcAction);

    commentMenu = new QMenu();
    commentMenu->addAction(deleteCommentAction);
}

void TDrawWindow::createActions()
{
    Action1 = new QAction(tr("Загрузить иконку"), this);
    Action1->setStatusTip(tr("Устанавливает иконку"));
    connect(Action1, SIGNAL(triggered()), this, SLOT(setItemIcon()));

    Action2 = new QAction(tr("Свойства"), this);
    Action2->setStatusTip(tr("Свойства веришны"));
    connect(Action2, SIGNAL(triggered()), this, SLOT(showTopPropDialog()));

    makeAsRootAction = new QAction(tr("Сделать корневой"), this);
    makeAsRootAction->setStatusTip(tr("Свойства веришны"));
    connect(makeAsRootAction, SIGNAL(triggered()), this, SLOT(makeAsRoot()));

    deleteAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteAction->setStatusTip(tr("Удаляет объект"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    deleteArcAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteArcAction->setStatusTip(tr("Удаляет объект"));
    connect(deleteArcAction, SIGNAL(triggered()), this, SLOT(deleteArc()));

    deleteCommentAction = new QAction(QIcon(";/images/delete.png"), tr("Удалить"), this);
    deleteCommentAction->setStatusTip(tr("Удаляет объект"));
    connect(deleteCommentAction, SIGNAL(triggered()), this, SLOT(deleteComment()));
}

void TDrawWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == TTop::Type) {
            qgraphicsitem_cast<TTop *>(item)->removeArcs();
        }
        scene->removeItem(item);
    }
    emit sceneChanged();
}

void TDrawWindow::deleteArc()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  TArcTop::Type) {
            TArc *arc = qgraphicsitem_cast<TArc *>(item->parentItem());
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

void TDrawWindow::deleteComment()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() ==  TComment::Type) {
            scene->removeItem(item);
        }
    }
    emit sceneChanged();
}

void TDrawWindow::addItem(TTop::DiagramType type)
{
    scene->setItemType(type);
    scene->setMode(QDiagramScene::InsertItem);
}

void TDrawWindow::itemInserted(TTop *item)
{
    //setMode(QDiagramScene::MoveItem);
    emit sceneChanged();
}

void TDrawWindow::itemSelected(QGraphicsItem *item)
{
    if (item->type() == TArcTop::Type){
        TArcTop* arcTop = qgraphicsitem_cast<TArcTop* >(item);
        TArc* arc = qgraphicsitem_cast<TArc* >(arcTop->parentItem());
        foreach (TArcLine* line, arc->lines){
            QPen pen = line->pen();
            pen.setColor(Qt::green);
            line->setPen(pen);
        }
    }
    //setWindowTitle("sdfdasf");
}

void TDrawWindow::textInserted(TComment *)
{
    //setMode(QDiagramScene::MoveItem);
    emit sceneChanged();
}

void TDrawWindow::setMode(QDiagramScene::Mode mode)
{
    scene->setMode(mode);
    myMode = mode;
}

void TDrawWindow::setItemIcon()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Открыть файл..."),
                                                    "",
                                                    tr("All Files (*)"));
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == TTop::Type) {
            if (!fileName.isEmpty()){
                QImage img(fileName);
                if (!img.isNull()){
                    qgraphicsitem_cast<TTop *>(item)->setIcon(img);
                    scene->invalidate(item->mapRectToScene(item->boundingRect()));
                }
            }
        }
    }
    emit sceneChanged();
}

void TDrawWindow::showTopPropDialog(){
    TopPropertyDialog dlg;
    TTop* top = qgraphicsitem_cast<TTop *>(scene->selectedItems().first());
    dlg.prepareForm(top);
    /*dlg.show();
    dlg.setWindowModality(Qt::ApplicationModal);
    while (dlg.isVisible()){
        QCoreApplication::processEvents();
    }
    if (dlg.result() == dlg.Accepted) {*/
    if (dlg.exec()){
        QPolygonF myPolygon;
        int w = dlg.getWidth();
        int h = dlg.getHeight();
        myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
                << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
                << QPointF(-w/2, h/2);
        top->setPolygon(myPolygon);
    }
}

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

void TDrawWindow::makeAsRoot(){
    scene->setRootTop(qgraphicsitem_cast<TTop* >(scene->selectedItems().first()));
}

QList<TTop* > TDrawWindow::allTops(){
    QList<TTop* > topList;
    for (int i = 0; i < scene->items().count(); i++){
        if (scene->items().at(i)->type() == TTop::Type){
            topList.append(qgraphicsitem_cast<TTop* >(scene->items().at(i)));
        }
    }
    return topList;
}

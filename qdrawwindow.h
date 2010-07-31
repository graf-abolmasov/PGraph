#ifndef QDRAWWINDOW_H
#define QDRAWWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include "qdiagramscene.h"
#include "qdiagramitem.h"
#include "qcomment.h"
#include "ui_toppropertydialog.h"
#include "toppropertydialog.h"
#include "qgraph.h"
#include "databasemanager.h"

class TDrawWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createMenus();
    void createActions();

    QDiagramScene *scene;
    QGraphicsView *view;

    QMenu *itemMenu;
    QMenu *arcMenu;
    QMenu *commentMenu;
    QMenu *syncArcMenu;

    QAction *deleteAction;
    QAction *setIconAction;
    QAction *setTopPropertyAction;
    QAction *makeAsRootAction;
    QAction *setArcPropertyAction;
    QAction *deleteArcAction;
    QAction *deleteSyncAction;
    QAction *deleteCommentAction;

    QDiagramScene::Mode myMode;

    QList<TTop* > allTops();
    QList<TArc* > allArcs();
    QList<TComment* > allComments();
    QList<QSyncArc* > allSyncArcs();

public:
    TDrawWindow();
    void saveAsImage(QString filename);
    void setMode(QDiagramScene::Mode mode);
    QDiagramScene::Mode mode(){return myMode;}
    QGraph* graph();
    void loadGraph(DataBaseManager* dbManager);
    void saveGraph(DataBaseManager* dbManager);
signals:
    void sceneChanged();

private slots:
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteItem();
    void showArcPropDialog();
    void deleteArc();
    void deleteSync();
    void deleteComment();
    void itemInserted(TTop *item);
    void itemSelected(QGraphicsItem *item);
    void textInserted(TComment *);
};

#endif // QDRAWWINDOW_H

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
    QAction *Action1;
    QAction *Action2;
    QAction *makeAsRootAction;

    QAction *deleteArcAction;

    QAction *deleteSyncAction;

    QAction *deleteCommentAction;

    QDiagramScene::Mode myMode;

public:
    TDrawWindow();
    void saveAsImage(QString filename);
    void setMode(QDiagramScene::Mode mode);
    QDiagramScene::Mode mode(){return myMode;}
    QList<TTop* > allTops();
    QDiagramScene* getScene(){
        return this->scene;
    }
signals:
    void sceneChanged();

private slots:
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteItem();
    void deleteArc();
    void deleteSync();
    void deleteComment();
    void itemInserted(TTop *item);
    void itemSelected(QGraphicsItem *item);
    void textInserted(TComment *);
};

#endif // QDRAWWINDOW_H

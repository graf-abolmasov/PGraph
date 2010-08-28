#ifndef QDRAWWINDOW_H
#define QDRAWWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include "qdiagramscene.h"
#include "qtop.h"
#include "qcomment.h"
#include "toppropertydialog.h"
#include "graph.h"
#include "databasemanager.h"

class TDrawWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createMenus();
    void createActions();

    QDiagramScene *scene;
    QGraphicsView *view;

    QMenu *topMenu;
    QMenu *arcMenu;
    QMenu *commentMenu;
    QMenu *syncArcMenu;
    QMenu *multiProcMenu;

    QAction *deleteAction;
    QAction *setIconAction;
    QAction *setTopPropertyAction;
    QAction *makeAsRootAction;
    QAction *setArcPropertyAction;
    QAction *deleteArcAction;
    QAction *deleteSyncAction;
    QAction *deleteCommentAction;
    QAction *deleteMultiProcTopAction;
    QAction *setMultiProcTopAction;
    QAction *setFontAction;

    QDiagramScene::Mode myMode;

    QList<QTop* > allTops();
    QList<QArc* > allArcs();
    QList<QComment* > allComments();
    QList<QSyncArc* > allSyncArcs();
    QList<QMultiProcTop* > allMultiProcTop();

public:
    QUndoStack *undoStack;
    TDrawWindow();
    void saveAsImage(QString filename);
    void setMode(QDiagramScene::Mode mode);
    QDiagramScene::Mode mode(){return myMode;}
    Graph* getGraph();
    void loadGraph(QString extName, DataBaseManager* dbManager);
    bool saveGraph(QString name, QString extName, DataBaseManager* dbManager, bool update = false);
    bool saveStruct(QString name, DataBaseManager* dbManager);

    void alignHLeft();
    void alignHCenter();
    void alignHRight();
    void alignVTop();
    void alignVCenter();
    void alignVBottom();
    void distribVertically();
    void distribHorizontally();
signals:
    void sceneChanged();
    void itemChanged(QGraphicsItem *item);

private slots:
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteTop();
    void showArcPropDialog();
    void deleteArc();
    void deleteSync();
    void deleteComment();
    void showMultiProcTopDialog();
    void showFontDialog();
    void itemInserted(QGraphicsItem *item);
    void itemMoved(QGraphicsItem *item, QLineF vector);
    void itemDeleted(QGraphicsItem *item);
    void textInserted(QComment *);
};

#endif // QDRAWWINDOW_H

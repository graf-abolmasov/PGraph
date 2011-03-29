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

    QAction *deleteTopAction;
    QAction *setIconAction;
    QAction *setTopPropertyAction;
    QAction *makeAsRootAction;
    QAction *viewSubGraphAct;
    QAction *editSubGraphAct;
    QAction *setArcPropertyAction;
    QAction *deleteArcAction;
    QAction *rebuildArcAction;
    QAction *deleteSyncAction;
    QAction *deleteCommentAction;
    QAction *deleteMultiProcTopAction;
    QAction *setMultiProcTopAction;
    QAction *setFontAction;

    QList<QTop* > allTops();
    QList<QArc* > allArcs();
    QList<QComment* > allComments();
    QList<QSyncArc* > allSyncArcs();
    QList<QMultiProcTop* > allMultiProcTop();

public:
    enum ShowRole {NormalEditor, ReadOnly, SelectTop};

    ShowRole myRole;

    QUndoStack *undoStack;
    QString myGraphName;
    QString myGraphExtName;

    TDrawWindow(ShowRole role = NormalEditor, QWidget *parent = 0);
    void saveAsImage(QString filename);
    void setMode(QDiagramScene::Mode mode);
    void showDataLayer(bool show);
    QDiagramScene::Mode mode() const
        { return scene->mode(); }
    Graph* getGraph();
    void loadGraph(const QString &name, DataBaseManager* dbManager);
    bool saveGraph(QString name, QString extName, DataBaseManager* dbManager);
    bool updateGraph(DataBaseManager *dbManager);
    bool saveStruct(DataBaseManager* dbManager);

    void alignHLeft();
    void alignHCenter();
    void alignHRight();
    void alignVTop();
    void alignVCenter();
    void alignVBottom();
    void distribVertically();
    void distribHorizontally();
    void scale(float s);
signals:
    void sceneChanged();
    void itemChanged(QGraphicsItem *item);
    void selectionChanged(QList<QGraphicsItem *>);
    void graphLoaded(QString, QString);

private slots:
    void topMenuAboutToShow();
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteTop();
    void viewSubGraph();
    void editSubGraph();
    void showArcPropDialog();
    void deleteArc();
    void rebuildArc();
    void deleteSync();
    void deleteComment();
    void showMultiProcTopDialog();
    void showFontDialog();
    void itemInserted(QGraphicsItem *item);
    void itemMoved(QGraphicsItem *item, QLineF vector);
    void itemDeleted(QGraphicsItem *item);
    void itemsMoved(QList<QGraphicsItem*> items, QLineF vector);

    void selectionChanged();
};

#endif // QDRAWWINDOW_H

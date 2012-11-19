#ifndef QDRAWWINDOW_H
#define QDRAWWINDOW_H

#include <QtGui/QMainWindow>

#include "qdiagramscene.h"

QT_BEGIN_NAMESPACE
class QGraphicsView;
class QUndoStack;
QT_END_NAMESPACE

class QTop;
class QArc;
class QSyncArc;
class QNormalTop;
class QComment;
class QMultiProcTop;
class Graph;

class TDrawWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createMenus();
    void createActions();
    float myScale;

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
    QAction *setArcPropertyAction;
    QAction *deleteArcAction;
    QAction *rebuildArcAction;
    QAction *deleteSyncAction;
    QAction *deleteCommentAction;
    QAction *deleteMultiProcTopAction;
    QAction *setMultiProcTopAction;
    QAction *setFontAction;

    QList<QTop* > allTops() const;
    QList<QArc* > allArcs() const;
    QList<QComment* > allComments() const;
    QList<QSyncArc* > allSyncArcs() const;
    QList<QMultiProcTop* > allMultiProcTop() const;

    bool maybeSave();

protected:
    void closeEvent(QCloseEvent *event);

public:
    QString myGraphName;
    QString myGraphExtName;
    QUndoStack *undoStack;

    TDrawWindow(QWidget *parent = 0);
    void saveAsImage(QString filename);
    void setMode(QDiagramScene::Mode mode);
    void showDataLayer(bool );
    QDiagramScene::Mode mode() const
        { return scene->mode(); }
    Graph getGraph() const;

    void loadGraph(const QString &name);
    bool saveGraphAs();
    bool saveGraph();
    bool saveStruct();

    void alignHLeft();
    void alignHCenter();
    void alignHRight();
    void alignVTop();
    void alignVCenter();
    void alignVBottom();
    void distribVertically();
    void distribHorizontally();
    void scale(float s);
    float getScale()
        { return myScale; }

signals:
    void documentModified();
    void mouseScrollScaleChanged(float);
    void itemChanged(QGraphicsItem *item);
    void selectionChanged(QList<QGraphicsItem *>);
    void openSubGraph(const QString &name);

private slots:
    void documentWasModified();
    void topMenuAboutToShow();
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteTop();
    void viewSubGraph();
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
    void itemsMoved(QList<QGraphicsItem *> items, QLineF vector);

    void selectionChanged();
};

#endif // QDRAWWINDOW_H

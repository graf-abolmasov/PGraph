#ifndef QDIAGRAMSCENE_H
#define QDIAGRAMSCENE_H

#include <QGraphicsScene>
#include "qtop.h"
#include "qcomment.h"
#include "qarc.h"
#include "qsyncarc.h"
#include "qmultiproctop.h"
#include "qnormaltop.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class QTop;
class QArc;
class QSyncArc;

class QDiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertNormalTop, InsertLine, InsertText, InsertSync, InsertMultiProcTop, MoveItem};

    QDiagramScene(QObject *parent = 0);
    void setArcMenu(QMenu *menu);
    void setTopMenu(QMenu *menu);
    void setCommentMenu(QMenu *menu);
    void setSyncArcMenu(QMenu *menu);
    void setMultiProcTopMenu(QMenu *menu);
    void setRootTop(QNormalTop* top);

    //!Указатель на корневую вершину
    QNormalTop* rootTop()
        { return myRootTop; }

public slots:
    void setMode(Mode mode);
    void editorLostFocus(QComment *item);

signals:
    void itemInserted(QGraphicsItem *item);
    void itemSelected(QGraphicsItem *item);
    void itemMoved(QGraphicsItem *item, QLineF vector);
    void itemDeleted(QGraphicsItem *item);
    void textInserted(QComment *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyReleaseEvent (QKeyEvent *keyEvent);

private:
    Mode myMode;

    QNormalTop* myRootTop;

    QComment *textItem;
    QArc *newArc;
    QArcLine *line;

    bool isItemChange(int type);

    QMenu *myTopMenu;
    QMenu *myArcMenu;
    QMenu *myCommentMenu;
    QMenu *mySyncArcMenu;
    QMenu *myMultiProcTopMenu;

    QTop* addTop(const QPointF &point);
    QArc* addArc(const QPointF &point);
};

#endif

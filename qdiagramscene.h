#ifndef QDIAGRAMSCENE_H
#define QDIAGRAMSCENE_H

#include <QGraphicsScene>
#include "qdiagramitem.h"
#include "qcomment.h"
#include "qarc.h"
#include "qsyncarc.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class TTop;
class TArc;
class QSyncArc;

class QDiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, InsertSync, MoveItem};

    QDiagramScene(QObject *parent = 0);
    void setArcMenu(QMenu *menu);
    void setItemMenu(QMenu *menu);
    void setCommentMenu(QMenu *menu);
    void setSyncArcMenu(QMenu *menu);
    void setRootTop(TTop* top);

    //!”казатель на корневую вершину
    TTop* rootTop()
        { return myRootTop; }

public slots:
    void setMode(Mode mode);
    void editorLostFocus(TComment *item);

signals:
    void itemInserted(TTop *item);
    void textInserted(TComment *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyReleaseEvent (QKeyEvent *keyEvent);

private:
    Mode myMode;

    TTop* myRootTop;

    TComment *textItem;
    TArc *newArc;
    QArcLine *line;

    bool isItemChange(int type);

    QMenu *myItemMenu;
    QMenu *myArcMenu;
    QMenu *myCommentMenu;
    QMenu *mySyncArcMenu;
};

#endif

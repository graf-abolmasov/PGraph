#ifndef QDIAGRAMSCENE_H
#define QDIAGRAMSCENE_H

#include <QtGui/QGraphicsScene>

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
class QNormalTop;
class QComment;

class QDiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertNormalTop, InsertLine, InsertText, InsertSync, InsertMultiProcTop, InsertDataItem, MoveItem, ReadOnly};

    QDiagramScene(QObject *parent = 0);
    void setArcMenu(QMenu *menu);
    void setTopMenu(QMenu *menu);
    void setCommentMenu(QMenu *menu);
    void setSyncArcMenu(QMenu *menu);
    void setMultiProcTopMenu(QMenu *menu);
    void setRootTop(QNormalTop* top);

    //!Указатель на корневую вершину
    QNormalTop* rootTop() const
        { return myRootTop; }
    void setMode(Mode mode);
    Mode mode() const
        { return myMode; }

public slots:
    void editorLostFocus(QComment *item);

public Q_SLOTS:
    void clear();

signals:
    void itemInserted(QGraphicsItem *item);
    void itemMoved(QGraphicsItem *item, QLineF vector);
    void itemDeleted(QGraphicsItem *item);
    void itemsMoved(QList<QGraphicsItem*> items, QLineF vector);
    void mouseScrollScaleChanged(float delta);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) ;
    void keyReleaseEvent(QKeyEvent *keyEvent);

private:
    Mode myMode;

    QNormalTop* myRootTop;

    QArc *newArc;
    QGraphicsLineItem *line;
    QGraphicsRectItem *selectionRect;
    QLineF displacementVector;

    QMenu *myTopMenu;
    QMenu *myArcMenu;
    QMenu *myCommentMenu;
    QMenu *mySyncArcMenu;
    QMenu *myMultiProcTopMenu;

    QTop* addTop(const QPointF &point);
    QArc* addArc(const QPointF &point);
    QComment* addComment(const QPointF &point);

    int getNextTopNumber();
};

#endif

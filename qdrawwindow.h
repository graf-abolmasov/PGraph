#ifndef QDRAWWINDOW_H
#define QDRAWWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include "qdiagramscene.h"
#include "qdiagramitem.h"

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

    QAction *deleteAction;
    QAction *Action1;
    QAction *Action2;
    QAction *makeAsRootAction;

    QAction *deleteArcAction;

    QAction *deleteCommentAction;

    QDiagramScene::Mode myMode;
    /*TRegisterStruct reg;                        // Структ. с EXTNAME графа и наим. файла с его иконкой
    int DropIdx;                                // Номер дуги, на которую бросили предикат (Drag&Drop)*/

public:
    TDrawWindow();
    void newFile(){}
    void saveAsImage(QString filename);
    void addItem(TTop::DiagramType type);
    void setMode(QDiagramScene::Mode mode);
    QDiagramScene::Mode mode(){return myMode;}
    /*virtual bool CanClose();                    // Возвращает TRUE, если можно закрыть окно
    virtual void SetupWindow();                 // Инициализация окна и таймера
    virtual void CleanupWindow();               // Очистка окна, удаление таймера
    LRESULT WMDropObject(WPARAM, LPARAM);       // Обработчик "бросания" объекта изменяет предикат дуги
    LRESULT WMQueryDropObject(WPARAM, LPARAM);  // Проверка перед "бросанием" объекта
    virtual void Paint(TDC&,bool,TRect&);*/

    /*void Open();
    bool Read(LPSTR);
    bool Save();
    bool SaveAs();
    void DoSaveBMP();
    void ChangeIcon();
    void _SetIcon();
    bool CanWriteStruct();
    bool SaveStruct();
    bool Compile();
    bool Run();
    void SetFileName(LPSTR);
    bool Write(pstate);
    bool CanClear();	// Возвращает TRUE, если можно очистить окно
    void Clear();
    void CMProperties();			// Обработка выбора меню «Свойства»
    bool IsDrawing;*/
signals:
    void sceneChanged();

private slots:
    void setItemIcon();
    void showTopPropDialog();
    void makeAsRoot();
    void deleteItem();
    void deleteArc();
    void deleteComment();
    void itemInserted(TTop *item);
    void itemSelected(QGraphicsItem *item);
    void textInserted(TComment *);
};

#endif // QDRAWWINDOW_H

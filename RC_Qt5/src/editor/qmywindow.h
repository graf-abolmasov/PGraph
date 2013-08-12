#ifndef QMYWINDOW_H
#define QMYWINDOW_H

#include <QMainWindow>
#include <QtCore/QMap>
#include "../../src/editor/qdrawwindow.h"
#include "../../src/compiler/nativecompiler.h"
#include "../../src/compiler/graphdebugger.h"

QT_BEGIN_NAMESPACE
class QToolButton;
class QComboBox;
class QUndoView;
class QButtonGroup;
class QSlider;
class QGraphicsItem;
class QTextEdit;
class QLabel;
class QListWidget;
class QMdiArea;
class QMdiSubWindow;
QT_END_NAMESPACE

class TDrawWindow;

class TMyWindow : public QMainWindow
{
    Q_OBJECT

public:
    TMyWindow();
    ~TMyWindow();

private:

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    TDrawWindow *myCurrentDrawWindow;

    bool isParallel;

    QMenu *grafMenu;
    QMenu *objectMenu;
    QMenu *dataMenu;
    QMenu *buildMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    //Файл
    QAction *newGraphAct;
    QAction *openGraphAct;
    QAction *saveGraphAct;
    QAction *saveAsGraphAct;
    QAction *saveAsImageGraphAct;
    QAction *exitAction;

    //Объект
    QAction *viewContentAct;
    QAction *viewGarbageAct;
    QAction *registerUnitAct;
    QAction *openObjectEditorAct;
    QAction *viewOtherFilesAct;

    //Запуск
    QAction *buildAct;
    QAction *compileAct;
    QAction *runAct;

    //Данные
    QAction *variablesAct;
    QAction *dataTypeAct;
    QAction *compileDataAct;

    //Окно
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;

    //О программе
    QAction *aboutEditorAct;

    //LeftToolBar
    QToolButton *pointerButton;
    QToolButton *addArcButton;
    QToolButton *addCommentButton;
    QToolButton *addTopButton;
    QToolButton *addSyncArcButton;
    QToolButton *addMultiProcTopButton;
    QButtonGroup *pointerTypeGroup;

    //LayoutToolBar
    QAction* alignHLeftAct;
    QAction* alignHCenterAct;
    QAction* alignHRightAct;
    QAction* alignVTopAct;
    QAction* alignVCenterAct;
    QAction* alignVBottomAct;
    QAction* distribVerticallyAct;
    QAction* distribHorizontallyAct;

    //Масштаб
    QComboBox *scaleCombo;
    QSlider *scaleSlider;

    //Слои
    //QToolButton *showDataLayer;

    //EditToolBar
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *undoAct;
    QAction *redoAct;

    QToolBar *mainToolBar;
    QToolBar *leftToolBar;
    QToolBar *layoutToolBar;
    QToolBar *scaleToolBar;

    QUndoView *undoView;

    void createMenus();
    void createActions();
    void createToolBars();
    void createUndoView();
    void createDockWindows();
    void createOutputWindow();
    TDrawWindow *createDrawWindow();

    void CMHelp(){}		// Обработчик вызова помощи
    void readSettings();	//+ Загрузка конфигурации программы
    void writeSettings();	//+ Сохранение конфигурации программы

    NativeCompiler *nativeCompiler;
    GraphDebugger *graphDebugger;

    QListWidget *outputPanelList;
    QLabel *infoPanelLabel;

    QMdiSubWindow *findMdiChild(const QString &graphName);

protected:
    void closeEvent(QCloseEvent *); // реакция на закрытие окна

private slots:
    void CMGNew();              //+ Граф->Создать
    void CMGOpen();             //+ Граф->Открыть
    void CMGSave();     	//+ Граф->Сохранить
    void CMGSaveAs();           //+ Граф->SaveAs
    void CMGSaveAsImage();      //+ Граф->SaveAsPNG
    void CMGChangeIcon(){}      // Граф->Сменить иконку
    void CMReporter(){}		// Граф->Создать отчет
    void CMIncap(){}		// Граф->Инкапсулировать
    void CMExit();              //+ Граф->Выход
    void CMContent(){}		//+ Объект->Дерево объектов
    void CMOtherFiles();		//+ Объект->Другие файлы объектов
    void CMShowGarbage(){}	//+ Объект->Неиспользуемые объекты
    void CMNewModule();   	//+ Объект->Зарегистрировать модуль
    void CMObjList();		//+ Объект->Редактор объектов
    void CMEdtVar();		//+ Данные->Словарь данных
    void CMEdtType();		//+ Данные->Список типов
    void CMCompileData();       //+ Данные->Компиляция данных
    void CMGspUser(){}		// Данные->Gspuser.h
    void CMLibraries(){}	// Данные->Библиотеки
    void CMThickArcs(){}	// Настройки->Приоритеты дуг
    void CMConfirm(){}		// Настройки->Подтверждение удаления
    void CMSetSound(){}		// Настройки->Звук
    void CMAnimate(){}		// Настройки->Анимация иконок
    void CMPredTabl(){}		// Настройки->Таблица предикатов
    void CMFSaveBMP(){}		// Настройки->Сохранять BMP
    void CMDoCountMar(){}	// Настройки->Подсчет числа маршрутов
    void CMDirectories(){}	// Настройки->Размещение...
    void CMBuildOpt(){}		// Настройки->Построение...
    void CMFont(){}		// Настройки->Шрифт
    void CMSaveSetup(){}  	// Настройки->Сохранить
    void CMDoUserDialog(){}	//+ Запуск->Режим ручного ввода данных
    void CMCompile();           //+ Запуск->Компиляция
    void CMBuild();              //+ Запуск->Построение и запуск
    void CMRun();              //Запуск
    void CMHelpContents(){}	// Помощь->Содержание
    void CMHelpAbout(); 	//+ Помощь->О программе

    void CMECut();              // Правка->Вырезать
    void CMECopy();             // Правка->Скопировать
    void CMEPaste();            // Правка->Вставить

    void CMEUndo();             // Правка->Отменить
    void CMERedo();             // Правка->Вернуть

    void alignHLeft();          // Выравниевание к самому левому объекту из группы
    void alignHCenter();        // Выравниевание к центральному объекту из группы
    void alignHRight();         // Выравниевание к самому правому объекту из группы
    void alignVTop();           // Выравниевание к самому верхнему объекту из группы
    void alignVCenter();        // Выравниевание к центральному объекту из группы
    void alignVBottom();        // Выравниевание к самому нижнему объекту из группы
    void distribVertically();   // Распределяет по вертикали через одинаковые расстояния
    void distribHorizontally(); // Распределяет по горизонтили через одинаковые расстояния

    void setFixedScale(const QString &scale);
    void setFloatScale(const int scale);
    void updateScaleSlider(const float scaleIncrement);

    void updateToolBar(QList<QGraphicsItem *> items);
    void getInfo(QGraphicsItem *item);

    void pointerGroupClicked(int id);  //Действие при выборе инструмента

    void write2globalOutput(const QString &msg);

    void updateMenues();
    void setActiveSubWindow(QWidget *window);
    void activeSubWindowChanged(QMdiSubWindow *window);
    void updateWindowMenu();

    void openGraph(const QString &name);
};

#endif // QMYWINDOW_H

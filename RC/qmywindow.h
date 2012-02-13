#ifndef QMYWINDOW_H
#define QMYWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE
class QToolButton;
class QComboBox;
class QUndoView;
class QButtonGroup;
class QSlider;
class QGraphicsItem;
class QTextEdit;
QT_END_NAMESPACE

class TDrawWindow;

class TMyWindow : public QMainWindow
{
    Q_OBJECT

public:
    TMyWindow();
    ~TMyWindow();

private:

    bool isParallel;

    QMenu *grafMenu;
    QMenu *objectMenu;
    QMenu *dataMenu;
    QMenu *buildMenu;
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

    //Запуск
    QAction *runAct;
    QAction *compileAct;
    QAction *saveStructAct;
    QAction *manualInputAct;

    //Данные
    QAction *variablesAct;
    QAction *dataTypeAct;
    QAction *compileDataAct;

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

    QToolBar *mainToolBar;
    QToolBar *leftToolBar;
    QToolBar *layoutToolBar;
    QToolBar *scaleToolBar;
    QToolBar *layerToolBar;

    QUndoView *undoView;

    void createMenus();
    void createActions();
    void createToolBar();
    void createUndoView();
    void createDockWindows();
    void createOutputWindow();
    TDrawWindow *activeDrawWindow();

    TDrawWindow *createDrawWindow();
    void CheckOption(){} 	// Установка/сброс галочки в пункте меню
    void CMHelp(){}		// Обработчик вызова помощи
    void readSettings();	//+ Загрузка конфигурации программы
    void writeSettings();	//+ Сохранение конфигурации программы

    QMap<QString, QVariant> recentGraphs;

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
    void CMSaveStruct();        //+ Запуск->Запись структуры
    void CMCompile();           //+ Запуск->Компиляция
    void CMRun(){}              //+ Запуск->Построение и запуск
    void CMHelpContents(){}	// Помощь->Содержание
    void CMHelpAbout(); 	//+ Помощь->О программе

    void CMECut();              // Правка->Вырезать
    void CMECopy();             // Правка->Скопировать
    void CMEPaste();            // Правка->Вставить

    void grafMenuAboutToShow(); // Вызывается перед показом меню Граф

    void sceneChanged();        // Действие при изменении сцены

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

    void updateToolBar(QList<QGraphicsItem *> items);
    void getInfo(QGraphicsItem *item);

    void pointerGroupClicked(int id);  //Действие при выборе инструмента

    void showDataLayerClicked(bool clicked);

    void graphLoaded(QString name, QString extName);
};

#endif // QMYWINDOW_H

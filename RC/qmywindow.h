#ifndef QMYWINDOW_H
#define QMYWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QtGui>
#include "qdrawwindow.h"
#include "databasemanager.h"

class TMyWindow : public QMainWindow
{
    Q_OBJECT

public:
    TMyWindow();
    ~TMyWindow();

private:
    QMenu *grafMenu;
    QMenu *objectMenu;
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

    //scaleToolbar
    QComboBox *sceneScaleCombo;
    QSlider *scaleSlider;

    QToolBar *mainToolBar;
    QToolBar *leftToolBar;
    QToolBar *layoutToolBar;
    QToolBar *scaleToolBar;

    QUndoView *undoView;

    void createMenus();
    void createActions();
    void createToolBar();
    void createUndoView();
    void createDockWindows();
    TDrawWindow *activeDrawWindow();

    TDrawWindow *createDrawWindow();
    void CheckOption(){} 	// Установка/сброс галочки в пункте меню
    void CMHelp(){}		// Обработчик вызова помощи
    void loadSetup();		// Загрузка конфигурации программы
    void saveSetup();		// Сохранение конфигурации программы

    QString myGraphName;
    void setMyGraphName(QString name);
    QString myGraphExtName;
    void setMyGraphExtName(QString extName);

    QMap<QString, QVariant> recentGraphs;
private slots:
    void CMGNew();              //+ Граф->Создать
    void CMGOpen();             //+ Граф->Открыть
    void CMGSave();     	//+ Граф->Сохранить
    void CMGSaveAs();           //+ Граф->SaveAs
    void CMGSaveAsImage();      //+ Граф->SaveAsPNG
    void CMGChangeIcon(){}      // Граф->Сменить иконку
    void CMReporter(){}		// Граф->Создать отчет
    void CMIncap(){}		// Граф->Инкапсулировать
    void CMExit();
    void CMContent(){}		// +Объект->Дерево объектов
    void CMShowGarbage(){}	// +Объект->Неиспользуемые объекты
    void CMNewModule();   	// +Объект->Зарегистрировать модуль
    void CMObjList();		// +Объект->Редактор объектов
    void CMEdtVar();		// +Данные->Словарь данных
    void CMEdtType();		// +Данные->Список типов
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
    void CMDoUserDialog(){}	// +Запуск->Режим ручного ввода данных
    void CMSaveStruct();        // +Запуск->Запись структуры
    void CMCompile(){}          // +Запуск->Компиляция
    void CMRun(){}              // +Запуск->Построение и запуск
    void CMHelpContents(){}	// Помощь->Содержание
    void CMHelpAbout(); 	// +Помощь->О программе
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

    void updateAlignToolBar(QList<QGraphicsItem *> items);

    void getInfo(QGraphicsItem *item);

    void pointerGroupClicked(int id);  //Действие при выборе инструмента
};

#endif // QMYWINDOW_H

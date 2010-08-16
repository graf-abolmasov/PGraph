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

    //LeftToolBar
    QToolButton *pointerButton;
    QToolButton *addArcButton;
    QToolButton *addCommentButton;
    QToolButton *addTopButton;
    QToolButton *addSyncArcButton;
    QToolButton *addMultiProcTopButton;
    QButtonGroup *pointerTypeGroup;

    QToolBar *mainToolBar;
    QToolBar *leftToolBar;

    void createMenus();
    void createActions();
    void createToolBar();
    TDrawWindow *activeDrawWindow();

    TDrawWindow *createDrawWindow();
    void CheckOption(){} 	// Установка/сброс галочки в пункте меню
    void CMHelp(){}		// Обработчик вызова помощи
    void LoadSetup(){}		// Загрузка конфигурации программы
    void SaveSetup(){}		// Сохранение конфигурации программы

    //DataBaseManager* dbManager;
    QString myGraphName;
    void setMyGraphName(QString name);
    QString myGraphExtName;
    void setMyGraphExtName(QString extName);
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
    void CMHelpAbout(){}	// Помощь->О программе
    void updateMenus();         // Действие при активации дочернего окна
    void switchLayoutDirection();

    void addComment();
    void pointerGroupClicked(int id);  //Действие при выборе стрелки или линии
};

    extern QStatusBar *globalStatusBar;

#endif // QMYWINDOW_H

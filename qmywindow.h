#ifndef QMYWINDOW_H
#define QMYWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QtGui>
#include "qdrawwindow.h"

class TMyWindow : public QMainWindow
{
    Q_OBJECT

public:
    TMyWindow();
    ~TMyWindow();

private:
    QMenu *fileMenu;
    QMenu *objectMenu;
    QMenu *buildMenu;

    QAction *newGraphAct;
    QAction *openGraphAct;
    QAction *saveGraphAct;
    QAction *saveAsGraphAct;
    QAction *saveAsImageGraphAct;
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
    //LeftToolBar
    //!!QAction *addTopAct;
    //!!QAction *addCommentAct;
    QToolButton *pointerButton;
    QToolButton *linePointerButton;
    QToolButton *addCommentButton;
    QToolButton *addTopButton;
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

private slots:
    void CMGNew();              //+ Граф->Создать
    void CMGOpen(){}            //+ Граф->Открыть
    void CMGSave(){}     	//+ Граф->Сохранить
    void CMGSaveAs();            //+ Граф->SaveAs
    void CMGSaveAsImage();      //+ Граф->SaveAsPNG
    void CMGChangeIcon(){}      // Граф->Сменить иконку
    void CMReporter(){}		// Граф->Создать отчет
    void CMIncap(){}		// Граф->Инкапсулировать
    void CMContent(){}		// +Объект->Дерево объектов
    void CMShowGarbage(){}	// +Объект->Неиспользуемые объекты
    void CMNewModule(){}	// +Объект->Зарегистрировать модуль
    void CMObjList(){}		// +Объект->� едактор объектов
    void CMEdtVar(){}		// Данные->Словарь данных
    void CMEdtType(){}		// Данные->Список типов
    void CMGspUser(){}		// Данные->Gspuser.h
    void CMLibraries(){}	// Данные->Библиотеки
    void CMThickArcs(){}	// Настройки->Приоритеты дуг
    void CMConfirm(){}		// Настройки->Подтверждение удаления
    void CMSetSound(){}		// Настройки->Звук
    void CMAnimate(){}		// Настройки->Анимация иконок
    void CMPredTabl(){}		// Настройки->Таблица предикатов
    void CMFSaveBMP(){}		// Настройки->Сохранять BMP
    void CMDoCountMar(){}	// Настройки->Подсчет числа маршрутов
    void CMDirectories(){}	// Настройки->� азмещение...
    void CMBuildOpt(){}		// Настройки->Построение...
    void CMFont(){}		// Настройки->Шрифт
    void CMSaveSetup(){}  	// Настройки->Сохранить
    void CMDoUserDialog(){}	// +Запуск->� ежим ручного ввода данных
    void CMSaveStruct(){}       // +Запуск->Запись структуры
    void CMCompile(){}          // +Запуск->Компиляция
    void CMRun(){}              // +Запуск->Построение и запуск
    void CMHelpContents(){}	// Помощь->Содержание
    void CMHelpAbout(){}	// Помощь->О программе
    void updateMenus();         // Действие при активации дочернего окна
    void switchLayoutDirection();

    void addComment();
    void pointerGroupClicked(int id);  //Действие при выборе стрелки или линии
};

#endif // QMYWINDOW_H

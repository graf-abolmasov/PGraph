#include "qmywindow.h"
#include "qobjecteditor.h"
#include "qvariabledialog.h"
#include "qdatatypedialog.h"
#include "qmoduleregister.h"
#include "qsavegraphdialog.h"

QStatusBar *globalStatusBar;

TMyWindow::TMyWindow()
{
    globalStatusBar = new QStatusBar(this);
    setStatusBar(globalStatusBar);

    createDrawWindow();
    createActions();
    createMenus();
    createToolBar();

    setWindowIcon(QIcon(":/images/G.png"));
    setWindowTitle(tr("Граф-редактор"));
}

TMyWindow::~TMyWindow()
{

}

void TMyWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(newGraphAct);
    fileMenu->addAction(openGraphAct);
    fileMenu->addAction(saveGraphAct);
    fileMenu->addAction(saveAsGraphAct);
    fileMenu->addAction(saveAsImageGraphAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    objectMenu = menuBar()->addMenu(tr("Объект"));
    objectMenu->addAction(openObjectEditorAct);
    objectMenu->addAction(registerUnitAct);
    objectMenu->addAction(viewContentAct);
    objectMenu->addAction(viewGarbageAct);

    objectMenu = menuBar()->addMenu(tr("Данные"));
    objectMenu->addAction(variablesAct);
    objectMenu->addAction(dataTypeAct);

    buildMenu = menuBar()->addMenu(tr("Запуск"));
    buildMenu->addAction(runAct);
    buildMenu->addAction(compileAct);
    buildMenu->addAction(saveStructAct);
    buildMenu->addAction(manualInputAct);
}

void TMyWindow::createActions()
{

    newGraphAct = new QAction(tr("Создать"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Создать пустой граф"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(tr("Открыть"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    openGraphAct->setStatusTip(tr("Открыть из базы"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(tr("Сохранить"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    saveGraphAct->setStatusTip(tr("Сохранить в базу"));
    connect(saveGraphAct, SIGNAL(triggered()), this, SLOT(CMGSave()));

    saveAsGraphAct = new QAction(tr("Cохранить как"), this);
    saveAsGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsGraphAct->setStatusTip(tr("Cохраняет в базу"));
    connect(saveAsGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAs()));

    saveAsImageGraphAct = new QAction(tr("Cохранить как картинку"), this);
    saveAsImageGraphAct->setStatusTip(tr("Cхраняет как картинку"));
    connect(saveAsImageGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAsImage()));

    exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcuts(QKeySequence::Close);
    exitAction->setStatusTip(tr("Выйти из приложения"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(CMExit()));

    viewContentAct = new QAction(tr("Дерево объектов"), this);
    viewContentAct->setStatusTip(tr("Дерево объектов"));
    connect(viewContentAct, SIGNAL(triggered()), this, SLOT(CMContent()));

    viewGarbageAct = new QAction(tr("Неиспользуемые объекты"), this);
    viewGarbageAct->setStatusTip(tr("Неиспользуемые объекты"));
    connect(viewGarbageAct, SIGNAL(triggered()), this, SLOT(CMShowGarbage()));

    registerUnitAct = new QAction(tr("Зарегистрировать модуль"), this);
    registerUnitAct->setStatusTip(tr("Зарегистрировать модуль"));
    connect(registerUnitAct, SIGNAL(triggered()), this, SLOT(CMNewModule()));

    openObjectEditorAct = new QAction(tr("Редактор объектов"), this);
    openObjectEditorAct->setStatusTip(tr("Редактор объектов"));
    connect(openObjectEditorAct, SIGNAL(triggered()), this, SLOT(CMObjList()));

    variablesAct = new QAction(tr("Словарь данных"), this);
    connect(variablesAct, SIGNAL(triggered()), this, SLOT(CMEdtVar()));

    dataTypeAct = new QAction(tr("Список типов"), this);
    connect(dataTypeAct, SIGNAL(triggered()), this, SLOT(CMEdtType()));

    runAct = new QAction(QIcon(":/images/build.png"), tr("Запуск"), this);
    connect(runAct, SIGNAL(triggered()), this, SLOT(CMRun()));

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("Компилировать"), this);
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));

    saveStructAct = new QAction(tr("Записать структуру"), this);
    connect(saveStructAct, SIGNAL(triggered()), this, SLOT(CMSaveStruct()));

    manualInputAct = new QAction(tr("Ручной ввод данных"), this);
    connect(manualInputAct, SIGNAL(triggered()), this, SLOT(CMDoUserDialog()));

    //LeftToolBar
    pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    addTopButton = new QToolButton;
    addTopButton->setCheckable(true);
    addTopButton->setIcon(QIcon(":/images/top.png"));

    addCommentButton = new QToolButton;
    addCommentButton->setCheckable(true);
    addCommentButton->setIcon(QIcon(":/images/textpointer.png"));

    addArcButton = new QToolButton;
    addArcButton->setCheckable(true);
    addArcButton->setIcon(QIcon(":/images/linepointer.png"));

    addSyncArcButton = new QToolButton;
    addSyncArcButton->setCheckable(true);
    addSyncArcButton->setIcon(QIcon(":/images/syncarc.png"));

    addMultiProcTopButton = new QToolButton;
    addMultiProcTopButton->setCheckable(true);
    addMultiProcTopButton->setIcon(QIcon(":/images/MultiProcTop.png"));
}

TDrawWindow* TMyWindow::createDrawWindow()
{
    TDrawWindow *newDrawWindow = new TDrawWindow;
    setCentralWidget(newDrawWindow);
    connect(newDrawWindow, SIGNAL(sceneChanged()),
            this, SLOT(updateMenus()));
    return newDrawWindow;
}

void TMyWindow::createToolBar()
{
    mainToolBar = addToolBar(tr("Инструменты"));
    mainToolBar->addAction(runAct);
    mainToolBar->addAction(compileAct);

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(addTopButton, int(QDiagramScene::InsertItem));
    pointerTypeGroup->addButton(addCommentButton, int(QDiagramScene::InsertText));
    pointerTypeGroup->addButton(pointerButton, int(QDiagramScene::MoveItem));
    pointerTypeGroup->addButton(addArcButton, int(QDiagramScene::InsertLine));
    pointerTypeGroup->addButton(addSyncArcButton, int(QDiagramScene::InsertSync));
    pointerTypeGroup->addButton(addMultiProcTopButton, int(QDiagramScene::InsertMultiProcTop));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    leftToolBar = new QToolBar(tr("Палитра инструментов"), this);
    addToolBar(Qt::LeftToolBarArea, leftToolBar);
    leftToolBar->addWidget(pointerButton);
    leftToolBar->addWidget(addTopButton);
    leftToolBar->addWidget(addCommentButton);
    leftToolBar->addWidget(addArcButton);
    leftToolBar->addWidget(addSyncArcButton);
    leftToolBar->addWidget(addMultiProcTopButton);
}

void TMyWindow::CMGNew()
{
    TDrawWindow *newDrawWindow = createDrawWindow();
    newDrawWindow->showMaximized();
}

TDrawWindow *TMyWindow::activeDrawWindow()
{
   return qobject_cast<TDrawWindow *>(centralWidget());
}

void TMyWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void TMyWindow::addComment()
{
    activeDrawWindow()->setMode(QDiagramScene::InsertText);
}

void TMyWindow::pointerGroupClicked(int)
{
    activeDrawWindow()->setMode(QDiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void TMyWindow::updateMenus()
{
    if (activeDrawWindow())
        pointerTypeGroup->button(int(activeDrawWindow()->mode()))->setChecked(true);
}

void TMyWindow::CMGSaveAsImage()
{
    QString fileName = QFileDialog::getSaveFileName();
    if (!fileName.isEmpty())
        activeDrawWindow()->saveAsImage(fileName);
}

void TMyWindow::CMGSaveAs()
{
    QSaveGraphDialog dialog;
    if (dialog.exec()){
        activeDrawWindow()->saveGraph(dialog.getResult(), globalDBManager);
    }
}

void TMyWindow::CMObjList()
{
    QObjectEditor editor;
    if (editor.exec()){

    }
}

void TMyWindow::CMEdtVar()
{
    QVariableDialog editor;
    editor.prepareForm();
    if (editor.exec()){

    }
}

void TMyWindow::CMEdtType()
{
    QDataTypeDialog editor;
    editor.prepareForm();
    if (editor.exec()){

    }
}

void TMyWindow::CMGOpen()
{
    activeDrawWindow()->loadGraph(globalDBManager);
}

void TMyWindow::CMNewModule()
{
    QModuleRegister editor;
    editor.prepareForm();
    if (editor.exec()){

    }
}

void TMyWindow::CMExit()
{
    QApplication::closeAllWindows();
}

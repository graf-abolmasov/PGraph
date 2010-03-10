#include "qmywindow.h"

TMyWindow::TMyWindow()
{
    createDrawWindow();
    createActions();
    createMenus();
    createToolBar();
}

TMyWindow::~TMyWindow()
{

}

void TMyWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Граф"));
    fileMenu->addAction(newGraphAct);
    fileMenu->addAction(openGraphAct);
    fileMenu->addAction(saveGraphAct);
    fileMenu->addAction(saveAsGraphAct);
    fileMenu->addAction(saveAsImageGraphAct);

    objectMenu = menuBar()->addMenu(tr("Объект"));
    objectMenu->addAction(viewContentAct);
    objectMenu->addAction(viewGarbageAct);
    objectMenu->addAction(openObjectEditorAct);
    objectMenu->addAction(registerUnitAct);

    buildMenu = menuBar()->addMenu(tr("Запуск"));
    buildMenu->addAction(runAct);
    buildMenu->addAction(compileAct);
    buildMenu->addAction(saveStructAct);
    buildMenu->addAction(manualInputAct);
}

void TMyWindow::createActions()
{
    //Граф

    newGraphAct = new QAction(tr("Создать"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Создать файл"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(tr("Открыть"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    openGraphAct->setStatusTip(tr("Открыть файл"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(tr("Сохранить"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    saveGraphAct->setStatusTip(tr("Сохранить в файл"));
    connect(saveGraphAct, SIGNAL(triggered()), this, SLOT(CMGSave()));

    saveAsGraphAct = new QAction(tr("Сохранить как"), this);
    saveAsGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsGraphAct->setStatusTip(tr("Сохранить в новый файл"));
    connect(saveAsGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAs()));

    saveAsImageGraphAct = new QAction(tr("Сохранить как картинку"), this);
    saveAsImageGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsImageGraphAct->setStatusTip(tr("Сохранить как png файл"));
    connect(saveAsImageGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAsImage()));

    //Объект

    viewContentAct = new QAction(tr("Дерево объектов"), this);
    viewContentAct->setStatusTip(tr("Просмотр дерева объектов"));
    connect(viewContentAct, SIGNAL(triggered()), this, SLOT(CMContent()));

    viewGarbageAct = new QAction(tr("Неиспользуемые объекты"), this);
    viewGarbageAct->setStatusTip(tr("Просмотр неиспользуемых объектов"));
    connect(viewGarbageAct, SIGNAL(triggered()), this, SLOT(CMShowGarbage()));

    registerUnitAct = new QAction(tr("Зарегистрировать модуль"), this);
    registerUnitAct->setStatusTip(tr("Зарегистрировать модуль"));
    connect(registerUnitAct, SIGNAL(triggered()), this, SLOT(CMNewModule()));

    openObjectEditorAct = new QAction(tr("Зарегистрировать модуль"), this);
    openObjectEditorAct->setStatusTip(tr("Зарегистрировать модуль"));
    connect(openObjectEditorAct, SIGNAL(triggered()), this, SLOT(CMObjList()));

    //Запуск

    runAct = new QAction(QIcon(":/images/build.png"), tr("Построение и запуск"), this);
    runAct->setStatusTip(tr("Выполнить программу"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(CMRun()));

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("Компилировать"), this);
    compileAct->setStatusTip(tr("Скомпилировать программу"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));

    saveStructAct = new QAction(tr("Запись структуры"), this);
    saveStructAct->setStatusTip(tr("Запись структуры"));
    connect(saveStructAct, SIGNAL(triggered()), this, SLOT(CMSaveStruct()));

    manualInputAct = new QAction(tr("� ежим ручного ввода данных"), this);
    manualInputAct->setStatusTip(tr("� ежим ручного ввода данных"));
    connect(manualInputAct, SIGNAL(triggered()), this, SLOT(CMDoUserDialog()));

    //LeftToolBar

    /*addTopAct = new QAction(QIcon(":/images/top.png"), tr("&Добавить вершину"), this);
    addTopAct->setStatusTip(tr("Добавляет вершину"));
    connect(addTopAct, SIGNAL(triggered()), this, SLOT(addTop()));

    addCommentAct = new QAction(QIcon(":/images/textpointer.png"), tr("&Добавить вершину"), this);
    addCommentAct->setStatusTip(tr("Добавляет вершину"));
    connect(addCommentAct, SIGNAL(triggered()), this, SLOT(addComment()));*/

    addTopButton = new QToolButton;
    addTopButton->setCheckable(true);
    addTopButton->setIcon(QIcon(":/images/top.png"));

    addCommentButton = new QToolButton;
    addCommentButton->setCheckable(true);
    addCommentButton->setIcon(QIcon(":/images/textpointer.png"));

    pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));
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
    mainToolBar = addToolBar(tr("Запуск"));
    mainToolBar->addAction(runAct);
    mainToolBar->addAction(compileAct);

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(addTopButton, int(QDiagramScene::InsertItem));
    pointerTypeGroup->addButton(addCommentButton, int(QDiagramScene::InsertText));
    pointerTypeGroup->addButton(pointerButton, int(QDiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(QDiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    leftToolBar = new QToolBar(tr("Инструменты"), this);
    addToolBar(Qt::LeftToolBarArea, leftToolBar);
    //!!leftToolBar->addAction(addTopAct);
    //!!leftToolBar->addAction(addCommentAct);
    leftToolBar->addWidget(addTopButton);
    leftToolBar->addWidget(addCommentButton);
    leftToolBar->addWidget(pointerButton);
    leftToolBar->addWidget(linePointerButton);
}

void TMyWindow::CMGNew()
{
    TDrawWindow *newDrawWindow = createDrawWindow();
    newDrawWindow->newFile();
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

void TMyWindow::addTop()
{
    activeDrawWindow()->addItem(TTop::Top);
}

void TMyWindow::addComment()
{
    activeDrawWindow()->setMode(QDiagramScene::InsertText);
}

void TMyWindow::pointerGroupClicked(int)
{
    //!!if (pointerTypeGroup->checkedId() == int(QDiagramScene::InsertItem)) activeDrawWindow()->addItem(TTop::Top);
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
}

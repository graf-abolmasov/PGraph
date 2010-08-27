#include "qmywindow.h"
#include "qobjecteditor.h"
#include "qvariabledialog.h"
#include "qdatatypedialog.h"
#include "qmoduleregister.h"
#include "qsavegraphdialog.h"
#include "qopengraphdialog.h"
#include "commonutils.h"
#include "globalvariables.h"

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
    setMyGraphName("");
    setMyGraphExtName("");
}

TMyWindow::~TMyWindow()
{

}

void TMyWindow::createMenus()
{
    grafMenu = menuBar()->addMenu(tr("Граф"));
    grafMenu->addAction(newGraphAct);
    grafMenu->addAction(openGraphAct);
    grafMenu->addAction(saveGraphAct);
    grafMenu->addAction(saveAsGraphAct);
    grafMenu->addAction(saveAsImageGraphAct);
    grafMenu->addSeparator();
    grafMenu->addAction(exitAction);

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

    helpMenu = menuBar()->addMenu(tr("Помощь"));
    helpMenu->addAction(aboutEditorAct);
}

void TMyWindow::createActions()
{

    newGraphAct = new QAction(QIcon(":images/new.png"), tr("Создать"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Создать пустой граф"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(QIcon(":/images/open.png"), tr("Открыть"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    openGraphAct->setStatusTip(tr("Открыть из базы"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(QIcon(":/images/save.png"), tr("Сохранить"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    saveGraphAct->setStatusTip(tr("Сохранить в базу"));
    connect(saveGraphAct, SIGNAL(triggered()), this, SLOT(CMGSave()));

    saveAsGraphAct = new QAction(tr("Cохранить как"), this);
    saveAsGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsGraphAct->setStatusTip(tr("Cохраняет в базу"));
    connect(saveAsGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAs()));

    saveAsImageGraphAct = new QAction(tr("Cохранить как картинку"), this);
    saveAsImageGraphAct->setStatusTip(tr("Cохраняет как картинку"));
    connect(saveAsImageGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAsImage()));

    exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcuts(QKeySequence::Close);
    exitAction->setStatusTip(tr("Выйти из приложения"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(CMExit()));

    viewContentAct = new QAction(tr("Дерево объектов"), this);
    viewContentAct->setStatusTip(tr("Дерево объектов"));
    connect(viewContentAct, SIGNAL(triggered()), this, SLOT(CMContent()));
    viewContentAct->setEnabled(false);

    viewGarbageAct = new QAction(tr("Неиспользуемые объекты"), this);
    viewGarbageAct->setStatusTip(tr("Неиспользуемые объекты"));
    connect(viewGarbageAct, SIGNAL(triggered()), this, SLOT(CMShowGarbage()));
    viewGarbageAct->setEnabled(false);

    registerUnitAct = new QAction(tr("Зарегистрировать модуль"), this);
    registerUnitAct->setStatusTip(tr("Зарегистрировать модуль"));
    connect(registerUnitAct, SIGNAL(triggered()), this, SLOT(CMNewModule()));

    openObjectEditorAct = new QAction(QIcon(":/images/objectEditor.png"), tr("Редактор объектов"), this);
    openObjectEditorAct->setStatusTip(tr("Редактор объектов"));
    connect(openObjectEditorAct, SIGNAL(triggered()), this, SLOT(CMObjList()));

    variablesAct = new QAction(tr("Словарь данных"), this);
    connect(variablesAct, SIGNAL(triggered()), this, SLOT(CMEdtVar()));

    dataTypeAct = new QAction(tr("Список типов"), this);
    connect(dataTypeAct, SIGNAL(triggered()), this, SLOT(CMEdtType()));

    runAct = new QAction(QIcon(":/images/build.png"), tr("Запуск"), this);
    runAct->setStatusTip(tr("Компиляция и запуск"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(CMRun()));
    runAct->setEnabled(false);

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("Компилировать"), this);
    compileAct->setStatusTip(tr("Компилировать в exe"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));
    compileAct->setEnabled(false);

    saveStructAct = new QAction(tr("Записать структуру"), this);
    saveStructAct->setStatusTip(tr("Записать структуру графа в базу"));
    connect(saveStructAct, SIGNAL(triggered()), this, SLOT(CMSaveStruct()));

    manualInputAct = new QAction(tr("Ручной ввод данных"), this);
    connect(manualInputAct, SIGNAL(triggered()), this, SLOT(CMDoUserDialog()));
    manualInputAct->setEnabled(false);

    aboutEditorAct = new QAction(tr("О редакторе"), this);
    connect(aboutEditorAct, SIGNAL(triggered()), this, SLOT(CMHelpAbout()));

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
    addSyncArcButton->setEnabled(false);

    addMultiProcTopButton = new QToolButton;
    addMultiProcTopButton->setCheckable(true);
    addMultiProcTopButton->setIcon(QIcon(":/images/multiproctop.png"));

    //AlignToolBar
    alignHLeftAct = new QAction(QIcon(":/images/shape_align_left.png"), tr("Выровнять к левому краю"), this);
    alignHLeftAct->setStatusTip(tr("Выровнять вершины к левому краю"));
    connect(alignHLeftAct, SIGNAL(triggered()), this, SLOT(alignHLeft()));

    alignHRightAct = new QAction(QIcon(":/images/shape_align_right.png"), tr("Выровнять к правому краю"), this);
    alignHRightAct->setStatusTip(tr("Выровнять вершины к правому краю"));
    connect(alignHRightAct, SIGNAL(triggered()), this, SLOT(alignHRight()));

    alignHCenterAct = new QAction(QIcon(":/images/shape_align_center.png"), tr("Выровнять к центру по горизонтали"), this);
    alignHCenterAct->setStatusTip(tr("Выровнять вершины к центру по горизонтали"));
    connect(alignHCenterAct, SIGNAL(triggered()), this, SLOT(alignHCenter()));

    alignVTopAct = new QAction(QIcon(":/images/shape_align_top.png"), tr("Выровнять к верхнему краю"), this);
    alignVTopAct->setStatusTip(tr("Выровнять вершины к верхнему краю"));
    connect(alignVTopAct, SIGNAL(triggered()), this, SLOT(alignVTop()));

    alignVBottomAct = new QAction(QIcon(":/images/shape_align_bottom.png"), tr("Выровнять к нижнему краю"), this);
    alignVBottomAct->setStatusTip(tr("Выровнять вершины к нижнему краю"));
    connect(alignVBottomAct, SIGNAL(triggered()), this, SLOT(alignVBottom()));

    alignVCenterAct = new QAction(QIcon(":/images/shape_align_middle.png"), tr("Выровнять к центру по вертикали"), this);
    alignVCenterAct->setStatusTip(tr("Выровнять вершины к центру по вертикали"));
    connect(alignVCenterAct, SIGNAL(triggered()), this, SLOT(alignVCenter()));
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
    mainToolBar->addAction(newGraphAct);
    mainToolBar->addAction(openGraphAct);
    mainToolBar->addAction(saveGraphAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(openObjectEditorAct);
    mainToolBar->addSeparator();
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
    leftToolBar->addSeparator();
    leftToolBar->addWidget(addTopButton);
    leftToolBar->addWidget(addMultiProcTopButton);
    leftToolBar->addSeparator();
    leftToolBar->addWidget(addArcButton);
    leftToolBar->addWidget(addSyncArcButton);
    leftToolBar->addSeparator();
    leftToolBar->addWidget(addCommentButton);

    layoutToolBar = addToolBar(tr("Размещение"));
    layoutToolBar->addAction(alignHLeftAct);
    layoutToolBar->addAction(alignHCenterAct);
    layoutToolBar->addAction(alignHRightAct);
    layoutToolBar->addSeparator();
    layoutToolBar->addAction(alignVTopAct);
    layoutToolBar->addAction(alignVCenterAct);
    layoutToolBar->addAction(alignVBottomAct);

}

void TMyWindow::CMGNew()
{
    TDrawWindow *newDrawWindow = createDrawWindow();
    newDrawWindow->showMaximized();
    setMyGraphExtName("");
    setMyGraphName("");
}

TDrawWindow *TMyWindow::activeDrawWindow()
{
   return qobject_cast<TDrawWindow *>(centralWidget());
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
        if (dialog.getResult() != ""){
            setMyGraphExtName(dialog.getResult());
            setMyGraphName("G" + getCRC(myGraphExtName.toUtf8()));
            if (activeDrawWindow()->saveGraph(myGraphName, myGraphExtName, globalDBManager))
                globalStatusBar->showMessage(tr("Сохранено как ") + myGraphName, 3000);
        }
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
    QOpenGraphDialog dialog;
    dialog.prepareForm();
    if (dialog.exec()){
        CMGNew();
        setMyGraphExtName(dialog.getResult()->extName);
        setMyGraphName(dialog.getResult()->name);
        activeDrawWindow()->loadGraph(dialog.getResult()->extName, globalDBManager);
    }
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

void TMyWindow::CMSaveStruct()
{
    if (myGraphName == "") {
        QMessageBox::warning(this, "Ошибка", "Сохраните граф", QMessageBox::Ok);
        return;
    }
    if (activeDrawWindow()->saveStruct(myGraphName, globalDBManager))
        globalStatusBar->showMessage(tr("Структура записана"), 2000);
}

void TMyWindow::CMGSave()
{
    if (myGraphName != "")
        if (activeDrawWindow()->saveGraph(myGraphName, myGraphExtName, globalDBManager, true))
            globalStatusBar->showMessage(tr("Сохранено"), 2000);
}

void TMyWindow::setMyGraphName(QString name)
{
    myGraphName = name;
    if (myGraphName == ""){
        buildMenu->setEnabled(false);
        saveGraphAct->setEnabled(false);
    } else {
        buildMenu->setEnabled(true);
        saveGraphAct->setEnabled(true);
    }
}

void TMyWindow::setMyGraphExtName(QString extName)
{
    myGraphExtName = extName;
    if (myGraphExtName == "")
        setWindowTitle(tr("Граф-редактор"));
    else
        setWindowTitle(myGraphExtName + tr(" - Граф-редактор"));
}

void TMyWindow::CMHelpAbout()
{
    QMessageBox::about(this, tr("O Граф-редакторе"),
                       tr("<h2>Граф-редатор 2.0</h2>"
                          "<p>Внутренняя версия ") + QApplication::applicationVersion() +
                          tr("<p>Copyright &copy; 2010 FuzzyLogic Team. All rights reserved.</p>"
                          "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>"));

}

void TMyWindow::alignHLeft()
{
    activeDrawWindow()->alignHLeft();
}

void TMyWindow::alignHRight()
{
    activeDrawWindow()->alignHRight();
}
void TMyWindow::alignHCenter()
{
    activeDrawWindow()->alignHCenter();
}

void TMyWindow::alignVTop()
{
    activeDrawWindow()->alignVTop();
}

void TMyWindow::alignVCenter()
{
    activeDrawWindow()->alignVCenter();
}
void TMyWindow::alignVBottom()
{
    activeDrawWindow()->alignVBottom();
}

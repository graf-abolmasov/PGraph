#include <QtGui>

#include "../../src/editor/qmywindow.h"
#include "../../src/editor/qdrawwindow.h"
#include "../../src/editor/dialogs/qobjecteditor.h"
#include "../../src/editor/dialogs/qvariabledialog.h"
#include "../../src/editor/dialogs/qdatatypedialog.h"
#include "../../src/editor/dialogs/qmoduleregister.h"
#include "../../src/editor/dialogs/qsavegraphdialog.h"
#include "../../src/editor/dialogs/qopengraphdialog.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/globalvariables.h"
#include "../../src/compiler/graphcompiler.h"
#include "../../src/compiler/compiler.h"
#include "../../src/editor/qarcline.h"
#include "../../src/editor/qserialarctop.h"
#include "../../src/editor/qsyncarc.h"
#include "../../src/editor/qarc.h"
#include "../../src/editor/qcomment.h"
#include "../../src/editor/dialogs/projectdialog.h"
#include "../../src/editor/dialogs/otherfilesdialog.h"
#include "../../src/common/qgraphsettings.h"


TMyWindow::TMyWindow()
{
    myCurrentDrawWindow = NULL;
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(activeSubWindowChanged(QMdiSubWindow*)));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    setWindowIcon(QIcon(":/images/G.png"));
    setUnifiedTitleAndToolBarOnMac(true);

    nativeCompiler = new NativeCompiler(this);
    isParallel = globalSettings->isParallel();
    outputPanelList = new QListWidget();
    connect(globalLogger, SIGNAL(newMessage(QString)), this, SLOT(write2globalOutput(QString)));

    setWindowTitle(tr("PGraph"));
    setStatusBar(new QStatusBar(this));
    createActions();
    createMenus();
    createToolBars();
    createUndoView();
    createDockWindows();
    createOutputWindow();
    readSettings();
    CMGNew();
}

void TMyWindow::write2globalOutput(const QString &msg)
{
    outputPanelList->addItem(msg);
    outputPanelList->scrollToBottom();
}

TMyWindow::~TMyWindow()
{
    delete nativeCompiler;
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
    objectMenu->addAction(viewOtherFilesAct);
    objectMenu->addAction(viewContentAct);
    objectMenu->addAction(viewGarbageAct);

    dataMenu = menuBar()->addMenu(tr("Данные"));
    dataMenu->addAction(variablesAct);
    dataMenu->addAction(dataTypeAct);
    dataMenu->addSeparator();
    dataMenu->addAction(compileDataAct);

    buildMenu = menuBar()->addMenu(tr("Запуск"));
    buildMenu->addAction(buildAct);
    buildMenu->addAction(compileAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    helpMenu = menuBar()->addMenu(tr("Помощь"));
    helpMenu->addAction(aboutEditorAct);
}

void TMyWindow::createActions()
{

    newGraphAct = new QAction(QIcon(":images/new.png"), tr("Создать"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    const QString newShortCut = QKeySequence::keyBindings(QKeySequence::New).first().toString();
    newGraphAct->setStatusTip(tr("Создать граф") + (newShortCut.isEmpty() ? "" : " (" + newShortCut + ")"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(QIcon(":/images/open.png"), tr("Открыть"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    const QString openShortCut = QKeySequence::keyBindings(QKeySequence::Open).first().toString();
    openGraphAct->setStatusTip(tr("Открыть граф") + (openShortCut.isEmpty() ? "" : " (" + openShortCut + ")"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(QIcon(":/images/save.png"), tr("Сохранить"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    const QString saveShortCut = QKeySequence::keyBindings(QKeySequence::Save).first().toString();
    saveGraphAct->setStatusTip(tr("Сохранить граф") + (saveShortCut.isEmpty() ? "" : " (" + saveShortCut + ")"));
    connect(saveGraphAct, SIGNAL(triggered()), this, SLOT(CMGSave()));

    saveAsGraphAct = new QAction(tr("Cохранить как"), this);
    saveAsGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsGraphAct->setStatusTip(tr("Сохранить граф с новым именем"));
    connect(saveAsGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAs()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Вырезать"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Скопировать в буфер и очистить выделенную область"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(CMECut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("Копировать"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Скопировать в буфер выделенную область"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(CMECopy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("Вставить"), this);
    pasteAct->setShortcut(QKeySequence::Copy);
    pasteAct->setStatusTip(tr("Вставить из буфера"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(CMEPaste()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("Отменить"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    const QString undoShortCut = QKeySequence::keyBindings(QKeySequence::Undo).first().toString();
    undoAct->setStatusTip(tr("Отменить последнее действие") + (undoShortCut.isEmpty() ? "" : " (" + undoShortCut + ")"));
    undoAct->setEnabled(false);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(CMEUndo()));


    redoAct = new QAction(QIcon(":/images/redo.png"), tr("Вернуть"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    const QString redoShortCut = QKeySequence::keyBindings(QKeySequence::Redo).first().toString();
    redoAct->setStatusTip(tr("Вернуть отмененное действие") + (redoShortCut.isEmpty() ? "" : " (" + redoShortCut + ")"));
    redoAct->setEnabled(false);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(CMERedo()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    pasteAct->setEnabled(false);

    saveAsImageGraphAct = new QAction(tr("Cохранить как картинку"), this);
    saveAsImageGraphAct->setStatusTip(tr("Cохраняет как картинку"));
    connect(saveAsImageGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAsImage()));

    exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcuts(QKeySequence::Close);
    exitAction->setStatusTip(tr("Выход из редактора"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(CMExit()));

    viewContentAct = new QAction(tr("Дерево объектов"), this);
    viewContentAct->setStatusTip(tr("Дерево объектов"));
    connect(viewContentAct, SIGNAL(triggered()), this, SLOT(CMContent()));
    viewContentAct->setEnabled(false);

    viewOtherFilesAct = new QAction(tr("Другие файлы"), this);
    viewOtherFilesAct->setStatusTip(tr("Подключить другие файлы к проекту"));
    connect(viewOtherFilesAct, SIGNAL(triggered()), this, SLOT(CMOtherFiles()));
    viewOtherFilesAct->setEnabled(true);

    viewGarbageAct = new QAction(tr("Неиспользуемые объекты"), this);
    viewGarbageAct->setStatusTip(tr("Неиспользуемые объекты"));
    connect(viewGarbageAct, SIGNAL(triggered()), this, SLOT(CMShowGarbage()));
    viewGarbageAct->setEnabled(false);

    registerUnitAct = new QAction(tr("Зарегистрировать модуль"), this);
    registerUnitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_R));
    registerUnitAct->setStatusTip(tr("Зарегистрировать модуль (Ctrl+Alt+R)"));
    connect(registerUnitAct, SIGNAL(triggered()), this, SLOT(CMNewModule()));

    openObjectEditorAct = new QAction(QIcon(":/images/objectEditor.png"), tr("Редактор объектов"), this);
    openObjectEditorAct->setShortcut(QKeySequence(Qt::Key_F4));
    openObjectEditorAct->setStatusTip(tr("Открыть справочник объектов (F4)"));
    connect(openObjectEditorAct, SIGNAL(triggered()), this, SLOT(CMObjList()));

    variablesAct = new QAction(tr("Словарь данных"), this);
    variablesAct->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_V));
    variablesAct->setStatusTip(tr("Открыть справочник данных (Ctrl+Alt+V)"));
    connect(variablesAct, SIGNAL(triggered()), this, SLOT(CMEdtVar()));

    dataTypeAct = new QAction(tr("Список типов"), this);
    dataTypeAct->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_T));
    dataTypeAct->setStatusTip(tr("Открыть справочник типов данных (Ctrl+Alt+T)"));
    connect(dataTypeAct, SIGNAL(triggered()), this, SLOT(CMEdtType()));

    compileDataAct = new QAction(QIcon(":/images/data.png"), tr("Компиляция данных"), this);
    compileDataAct->setStatusTip(tr("Компилировать данные (F5)"));
    compileDataAct->setShortcut(QKeySequence(Qt::Key_F5));
    connect(compileDataAct, SIGNAL(triggered()), this, SLOT(CMCompileData()));

    buildAct = new QAction(QIcon(":/images/build.png"), tr("Компилировать с++"), this);
    buildAct->setShortcut(QKeySequence(Qt::Key_F8));
    buildAct->setStatusTip(tr("Компилировать в exe (F8)"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(CMBuild()));

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("Компилировать граф-модель"), this);
    compileAct->setStatusTip(tr("Компилировать граф-модель (F6)"));
    compileAct->setShortcut(QKeySequence(Qt::Key_F6));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));

    aboutEditorAct = new QAction(tr("О редакторе"), this);
    aboutEditorAct->setStatusTip(tr("Об авторе (F1)"));
    aboutEditorAct->setShortcut(QKeySequence(Qt::Key_F1));
    connect(aboutEditorAct, SIGNAL(triggered()), this, SLOT(CMHelpAbout()));

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    //LeftToolBar
    pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    addTopButton = new QToolButton;
    addTopButton->setCheckable(true);
    addTopButton->setIcon(QIcon(":/images/top.png"));
    addTopButton->setStatusTip(tr("Добавить вершину (Ctrl+Shift+T)"));
    addTopButton->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

    addCommentButton = new QToolButton;
    addCommentButton->setCheckable(true);
    addCommentButton->setIcon(QIcon(":/images/textpointer.png"));
    addCommentButton->setStatusTip(tr("Добавить комментарий (Ctrl+Shift+C)"));
    addCommentButton->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));

    addArcButton = new QToolButton;
    addArcButton->setCheckable(true);
    addArcButton->setIcon(QIcon(":/images/linepointer.png"));
    addArcButton->setStatusTip(tr("Рисовать дугу (Ctrl+Shift+A)"));
    addArcButton->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));

    addSyncArcButton = NULL;
    addMultiProcTopButton = NULL;
    if (isParallel) {
        addSyncArcButton = new QToolButton;
        addSyncArcButton->setCheckable(true);
        addSyncArcButton->setIcon(QIcon(":/images/syncarc.png"));
        addSyncArcButton->setEnabled(true);

        addMultiProcTopButton = new QToolButton;
        addMultiProcTopButton->setCheckable(true);
        addMultiProcTopButton->setIcon(QIcon(":/images/multiproctop.png"));
    }

    //AlignToolBar
    alignHLeftAct = new QAction(QIcon(":/images/shape_align_left.png"), tr("Выровнить к левому краю"), this);
    alignHLeftAct->setStatusTip(tr("Выровнять вершины по левому краю"));
    connect(alignHLeftAct, SIGNAL(triggered()), this, SLOT(alignHLeft()));

    alignHRightAct = new QAction(QIcon(":/images/shape_align_right.png"), tr("Выровнить к правому краю"), this);
    alignHRightAct->setStatusTip(tr("Выровнять вершины по правому краю"));
    connect(alignHRightAct, SIGNAL(triggered()), this, SLOT(alignHRight()));

    alignHCenterAct = new QAction(QIcon(":/images/shape_align_center.png"), tr("Выровнить к центру по горизонтали"), this);
    alignHCenterAct->setStatusTip(tr("Выровнять вершины к центру по горизонтали"));
    connect(alignHCenterAct, SIGNAL(triggered()), this, SLOT(alignHCenter()));

    alignVTopAct = new QAction(QIcon(":/images/shape_align_top.png"), tr("Выровнить к верхнему краю"), this);
    alignVTopAct->setStatusTip(tr("Выровнять вершины по верхнему краю"));
    connect(alignVTopAct, SIGNAL(triggered()), this, SLOT(alignVTop()));

    alignVBottomAct = new QAction(QIcon(":/images/shape_align_bottom.png"), tr("Выровнить к нижнему краю"), this);
    alignVBottomAct->setStatusTip(tr("Выровнять вершины по нижнему краю"));
    connect(alignVBottomAct, SIGNAL(triggered()), this, SLOT(alignVBottom()));

    alignVCenterAct = new QAction(QIcon(":/images/shape_align_middle.png"), tr("Выровнить к центру по вертикали"), this);
    alignVCenterAct->setStatusTip(tr("Выровнять вершины к центру по вертикали"));
    connect(alignVCenterAct, SIGNAL(triggered()), this, SLOT(alignVCenter()));

    distribVerticallyAct = new QAction(QIcon(":/images/shape_distrib_vertically.png"), tr("Распределить по вертикали"), this);
    distribVerticallyAct->setStatusTip(tr("Распределить вершиные по вертикали на одинаковом расстоянии"));
    connect(distribVerticallyAct, SIGNAL(triggered()), this, SLOT(distribVertically()));

    distribHorizontallyAct = new QAction(QIcon(":/images/shape_distrib_horizontally.png"), tr("Распределить по горизонтали"), this);
    distribHorizontallyAct->setStatusTip(tr("Распределить вершиные по горизонтали на одинаковом расстоянии"));
    connect(distribHorizontallyAct, SIGNAL(triggered()), this, SLOT(distribHorizontally()));

    scaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("--") << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    scaleCombo->addItems(scales);
    connect(scaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setFixedScale(QString)));

    scaleSlider = new QSlider;
    scaleSlider->setRange(50, 200);
    scaleSlider->setValue(100);
    scaleSlider->setOrientation(Qt::Horizontal);
    scaleSlider->setMaximumWidth(100);
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(setFloatScale(int)));
}

TDrawWindow* TMyWindow::createDrawWindow()
{
    TDrawWindow *newDrawWindow = new TDrawWindow();
    mdiArea->setActiveSubWindow(mdiArea->addSubWindow(newDrawWindow));
    return newDrawWindow;
}

void TMyWindow::createToolBars()
{
    mainToolBar = addToolBar(tr("Инструменты"));
    mainToolBar->addAction(newGraphAct);
    mainToolBar->addAction(openGraphAct);
    mainToolBar->addAction(saveGraphAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(undoAct);
    mainToolBar->addAction(redoAct);
    mainToolBar->addAction(cutAct);
    mainToolBar->addAction(copyAct);
    mainToolBar->addAction(pasteAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(openObjectEditorAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(compileDataAct);
    mainToolBar->addAction(compileAct);
    mainToolBar->addAction(buildAct);

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(addTopButton, int(QDiagramScene::InsertNormalTop));
    pointerTypeGroup->addButton(addCommentButton, int(QDiagramScene::InsertText));
    pointerTypeGroup->addButton(pointerButton, int(QDiagramScene::MoveItem));
    pointerTypeGroup->addButton(addArcButton, int(QDiagramScene::InsertLine));
    if (isParallel) {
        pointerTypeGroup->addButton(addSyncArcButton, int(QDiagramScene::InsertSync));
        pointerTypeGroup->addButton(addMultiProcTopButton, int(QDiagramScene::InsertMultiProcTop));
    }
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    leftToolBar = new QToolBar(tr("Палитра инструментов"), this);
    addToolBar(Qt::LeftToolBarArea, leftToolBar);
    leftToolBar->addWidget(pointerButton);
    leftToolBar->addSeparator();
    leftToolBar->addWidget(addTopButton);
    if (isParallel)
        leftToolBar->addWidget(addMultiProcTopButton);
    leftToolBar->addSeparator();
    leftToolBar->addWidget(addArcButton);
    if (isParallel)
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
    layoutToolBar->addSeparator();
    layoutToolBar->addAction(distribHorizontallyAct);
    layoutToolBar->addAction(distribVerticallyAct);

    scaleToolBar = addToolBar(tr("Масштаб"));
    scaleToolBar->addWidget(new QLabel(tr("Масштаб: ")));
    scaleToolBar->addWidget(scaleCombo);
    scaleToolBar->addWidget(scaleSlider);
}

void TMyWindow::pointerGroupClicked(int)
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->setMode(QDiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void TMyWindow::CMGNew()
{
    myCurrentDrawWindow = createDrawWindow();
    myCurrentDrawWindow->showMaximized();
}

void TMyWindow::CMGSaveAsImage()
{
    Q_ASSERT(myCurrentDrawWindow);
    QString fileName = QFileDialog::getSaveFileName();
    if (fileName.isEmpty())
        return;
    myCurrentDrawWindow->saveAsImage(fileName);
}

void TMyWindow::CMGSaveAs()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->saveGraphAs();
}

void TMyWindow::CMObjList()
{
    QObjectEditor().exec();
}

void TMyWindow::CMEdtVar()
{
    QVariableDialog(true).exec();
}

void TMyWindow::CMEdtType()
{
    QDataTypeDialog *editor = QDataTypeDialog::getDialog();
    editor->exec();
    delete editor;
}

void TMyWindow::CMGOpen()
{
    QOpenGraphDialog dialog;
    if (dialog.exec()) {
        const QString graphName = dialog.getResult();
        QMdiSubWindow *existing = findMdiChild(graphName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }
        openGraph(dialog.getResult());
        updateMenues();
    }
}

void TMyWindow::CMNewModule()
{
    QModuleRegister().exec();
}

void TMyWindow::CMExit()
{
    QApplication::closeAllWindows();
}

void TMyWindow::CMGSave()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->saveGraph();
    updateMenues();
}

void TMyWindow::CMHelpAbout()
{
    QMessageBox::about(this, tr("O Граф-редакторе"),
                       tr("<h2>Граф-редактор 2.0</h2>"
                          "<p>Internal version ") + QApplication::applicationVersion() +
                       tr("<p>Copyright &copy; 2000-2012 FuzzyLogic Team. All rights reserved.</p>"
                          "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>"));

}

void TMyWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Инфо"), this);
    infoPanelLabel = new QLabel(dock);
    infoPanelLabel->setWordWrap(true);
    infoPanelLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setWidget(infoPanelLabel);
    dock->setMinimumWidth(250);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Список команд"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setWidget(undoView);
    dock->setMinimumWidth(250);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void TMyWindow::createOutputWindow()
{
    QDockWidget *outputDock = new QDockWidget(tr("Сообщения"), this);
    outputPanelList->setParent(outputDock);
    outputDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    outputDock->setWidget(outputPanelList);
    addDockWidget(Qt::BottomDockWidgetArea, outputDock);
}

void TMyWindow::alignHLeft()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignHLeft();
}

void TMyWindow::alignHRight()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignHRight();
}

void TMyWindow::alignHCenter()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignHCenter();
}

void TMyWindow::alignVTop()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignVTop();
}

void TMyWindow::alignVCenter()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignVCenter();
}

void TMyWindow::alignVBottom()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->alignVBottom();
}

void TMyWindow::distribHorizontally()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->distribHorizontally();
}

void TMyWindow::distribVertically()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->distribVertically();
}

void TMyWindow::createUndoView()
{
    undoView = new QUndoView();
    undoView->setWindowTitle(tr("Список команд"));
}

void TMyWindow::getInfo(QGraphicsItem *item)
{
    Q_ASSERT(myCurrentDrawWindow);
    QArc *arc = NULL;
    QTop *top = NULL;
    QSyncArc *syncArc = NULL;
    if (item == NULL) {
        infoPanelLabel->setText("");
        return;
    }

    QString info;
    switch (item->type()) {
    case QArc::Type:
        arc = qgraphicsitem_cast<QArc*>(item);
    case QArcLine::Type:
    case QSerialArcTop::Type:
        if (!arc)
            arc = qgraphicsitem_cast<QArc*>(item->parentItem());
        info.append(tr("Дуга\nТип: ") + arcTypeToString(arc) + "\n");
        if (arc->startItem() != NULL)
            info.append(tr("Начальная вершина: ") + QString::number(arc->startItem()->number) + "\n");
        if (arc->endItem() != NULL)
            info.append(tr("Конечная вершина: ") + QString::number(arc->endItem()->number) + "\n");
        info.append(tr("Приоритет: ") + QString::number(arc->priority()) + "\n" +
                    tr("Ширина пера ") + QString::number(arc->pen().width()) + "\n");
        if (arc->predicate != NULL) {
            info.append(tr("\nО предикате\n"));
            info.append(tr("Тип: ") + predicateTypeToString(arc->predicate) + "\n");
            info.append(tr("Название: ") + arc->predicate->extName + "\n");
            info.append(tr("Внутреннее имя: ") + arc->predicate->name + "\n");
            if (arc->predicate->type == Predicate::NormalType)
                info.append(tr("Базовый модуль: ") + arc->predicate->baseModule->name + "\n");
        }
        break;
    case QTop::Type: {
        top = qgraphicsitem_cast<QTop* >(item);
        info.append(tr("Номер вершины ") + QString::number(top->number) + "\n");
        const QRectF topRect = top->mapRectToScene(top->rect());
        info.append(tr("Left: ") + QString::number(topRect.left()) + "\n");
        info.append(tr("Top: ") + QString::number(topRect.top()) + "\n");
        info.append(tr("Right: ") + QString::number(topRect.right()) + "\n");
        info.append(tr("Bottom: ") + QString::number(topRect.bottom()) + "\n");
        if (top->actor != NULL) {
            info.append(tr("\nОб акторе\n"));
            info.append(tr("Тип: ") + actorTypeToString(top->actor) + "\n");
            info.append(tr("Название: ") + top->actor->extName + "\n");
            info.append(tr("Внутреннее имя: ") + top->actor->name+ "\n");
            if (top->actor->type == Actor::NormalType)
                info.append(tr("Базовый модуль: ") + top->actor->baseModule->name + "\n");
        }
        break;
    }
    case QSyncArc::Type:
        syncArc = qgraphicsitem_cast<QSyncArc* >(item);
        break;
    }

    infoPanelLabel->setText(info);
}

void TMyWindow::updateToolBar(QList<QGraphicsItem *> items)
{
    Q_ASSERT(myCurrentDrawWindow);

    alignHLeftAct->setEnabled(false);
    alignHCenterAct->setEnabled(false);
    alignHRightAct->setEnabled(false);
    alignVTopAct->setEnabled(false);
    alignVCenterAct->setEnabled(false);
    alignVBottomAct->setEnabled(false);
    distribVerticallyAct->setEnabled(false);
    distribHorizontallyAct->setEnabled(false);

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    pasteAct->setEnabled(false);

    QList<QGraphicsItem* > arcList;
    QList<QGraphicsItem* > topList;
    foreach (QGraphicsItem *item, items)
        switch (item->type()){
        case QTop::Type:
            topList.append(item);
            break;
        case QArcLine::Type:
        case QSerialArcTop::Type:
        case QArc::Type:
        case QComment::Type:
        case QSyncArc::Type:
        default:
            arcList.append(item);
            break;
        }
    if (arcList.count() > 0) return;
    if (topList.count() == 1 ) {
        cutAct->setEnabled(true);
        copyAct->setEnabled(true);
        pasteAct->setEnabled(true);
        QTop* top = qgraphicsitem_cast<QTop* >(topList.first());
        if (top->allArcs().count() > 0) {
            distribVerticallyAct->setEnabled(true);
            distribHorizontallyAct->setEnabled(true);
        }
    } else if (topList.count() > 1 ) {
        alignHLeftAct->setEnabled(true);
        alignHCenterAct->setEnabled(true);
        alignHRightAct->setEnabled(true);
        alignVTopAct->setEnabled(true);
        alignVCenterAct->setEnabled(true);
        alignVBottomAct->setEnabled(true);
        distribVerticallyAct->setEnabled(true);
        distribHorizontallyAct->setEnabled(true);
        cutAct->setEnabled(true);
        copyAct->setEnabled(true);
        pasteAct->setEnabled(true);
    }

    /*NOT IMPLEMENTED YET*/
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    pasteAct->setEnabled(false);
}

void TMyWindow::setFixedScale(const QString &scale)
{
    Q_ASSERT(myCurrentDrawWindow);
    int newScale = scale.left(scale.indexOf(tr("%"))).toInt();
    myCurrentDrawWindow->scale(newScale/100.0);
    scaleSlider->setValue(newScale);
}

void TMyWindow::setFloatScale(const int scale)
{
    Q_ASSERT(myCurrentDrawWindow);
    QString scaleText = QString::number(scale) + "%";
    int idx = scaleCombo->findText(scaleText);
    if (idx > 0)
        scaleCombo->setCurrentIndex(idx);
    else {
        scaleCombo->blockSignals(true);
        scaleCombo->setItemText(0, scaleText);
        scaleCombo->setCurrentIndex(0);
        scaleCombo->blockSignals(false);
    }
    myCurrentDrawWindow->scale(scale/100.0);
}

void TMyWindow::readSettings()
{
}

void TMyWindow::writeSettings()
{
}

void TMyWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void TMyWindow::CMCompile()
{
    Q_ASSERT(myCurrentDrawWindow);
    if (myCurrentDrawWindow->saveGraph()) {
        Compiler *c = Compiler::getCompiler();
        c->compile(myCurrentDrawWindow->myGraphName);
        delete c;
    }
}

void TMyWindow::CMECopy()
{
    //TODO: реализовать копирование
}

void TMyWindow::CMECut()
{
    //TODO: реализовать вырезание
}

void TMyWindow::CMEPaste()
{
    //TODO: реализовать вставку
}

void TMyWindow::CMCompileData()
{
    SourceCompiler *c = new SourceCompiler();
    c->compileData(globalDBManager->getVariableList());
    delete c;
}

void TMyWindow::updateScaleSlider(const float scaleIncrement)
{
    scaleSlider->setValue(scaleSlider->value() + scaleIncrement);
}

void TMyWindow::CMBuild()
{
    globalLogger->skipLine();
    nativeCompiler->compile();
}

void TMyWindow::CMOtherFiles()
{
    OtherFilesDialog dlg;
    dlg.exec();
}

void TMyWindow::CMERedo()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->undoStack->redo();
}

void TMyWindow::CMEUndo()
{
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->undoStack->undo();
}

void TMyWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void TMyWindow::updateMenues()
{
    bool hasMdiChild = myCurrentDrawWindow != NULL;
    bool graphNameIsNotEmpty = hasMdiChild && !myCurrentDrawWindow->myGraphName.isEmpty();

    saveGraphAct->setEnabled(hasMdiChild && myCurrentDrawWindow->isWindowModified());
    buildMenu->setEnabled(graphNameIsNotEmpty);
    compileDataAct->setEnabled(graphNameIsNotEmpty);
    compileAct->setEnabled(graphNameIsNotEmpty);
    buildAct->setEnabled(graphNameIsNotEmpty);

    saveAsGraphAct->setEnabled(hasMdiChild);
    saveAsImageGraphAct->setEnabled(hasMdiChild);

    leftToolBar->setEnabled(hasMdiChild);
    layoutToolBar->setEnabled(hasMdiChild);
    scaleToolBar->setEnabled(hasMdiChild);
    if (hasMdiChild) {
        int scale = myCurrentDrawWindow->getScale()*100;
        scaleSlider->blockSignals(true);
        scaleCombo->blockSignals(true);
        scaleSlider->setValue(scale);
        QString scaleText = QString::number(scale) + "%";
        int idx = scaleCombo->findText(scaleText);
        if (idx > 0)
            scaleCombo->setCurrentIndex(idx);
        else {
            scaleCombo->setItemText(0, scaleText);
            scaleCombo->setCurrentIndex(0);
        }
        scaleSlider->blockSignals(false);
        scaleCombo->blockSignals(false);
    }

    undoAct->setEnabled(hasMdiChild && myCurrentDrawWindow->undoStack->canUndo());
    redoAct->setEnabled(hasMdiChild && myCurrentDrawWindow->undoStack->canRedo());
    undoView->setEnabled(hasMdiChild);
    outputPanelList->setEnabled(hasMdiChild);
    infoPanelLabel->setEnabled(hasMdiChild);

    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
}

void TMyWindow::activeSubWindowChanged(QMdiSubWindow *window)
{
    if (myCurrentDrawWindow) {
        myCurrentDrawWindow->disconnect(SIGNAL(documentModified()), this, SLOT(updateMenues()));
        myCurrentDrawWindow->disconnect(SIGNAL(itemChanged(QGraphicsItem*)));
        myCurrentDrawWindow->disconnect(SIGNAL(selectionChanged(QList<QGraphicsItem*>)));
        myCurrentDrawWindow->disconnect(SIGNAL(mouseScrollScaleChanged(float)));
        myCurrentDrawWindow->undoStack->disconnect(SIGNAL(canRedoChanged(bool)), redoAct, SLOT(setEnabled(bool)));
        myCurrentDrawWindow->undoStack->disconnect(SIGNAL(canUndoChanged(bool)), undoAct, SLOT(setEnabled(bool)));
    }
    if (window == NULL) {
        undoView->setStack(NULL);
        myCurrentDrawWindow = NULL;
    } else {
        myCurrentDrawWindow = qobject_cast<TDrawWindow *>(window->widget());
        connect(myCurrentDrawWindow, SIGNAL(openSubGraph(QString)), this, SLOT(openGraph(QString)));
        connect(myCurrentDrawWindow, SIGNAL(documentModified()), this, SLOT(updateMenues()));
        connect(myCurrentDrawWindow, SIGNAL(itemChanged(QGraphicsItem*)), this, SLOT(getInfo(QGraphicsItem*)));
        connect(myCurrentDrawWindow, SIGNAL(selectionChanged(QList<QGraphicsItem*>)), this, SLOT(updateToolBar(QList<QGraphicsItem*>)));
        connect(myCurrentDrawWindow, SIGNAL(mouseScrollScaleChanged(float)), this, SLOT(updateScaleSlider(float)));
        connect(myCurrentDrawWindow->undoStack, SIGNAL(canRedoChanged(bool)), redoAct, SLOT(setEnabled(bool)));
        connect(myCurrentDrawWindow->undoStack, SIGNAL(canUndoChanged(bool)), undoAct, SLOT(setEnabled(bool)));
        undoView->setStack(myCurrentDrawWindow->undoStack);
    }
    updateMenues();
}

void TMyWindow::openGraph(const QString &name)
{
    Q_ASSERT(!name.isEmpty());
    QMdiSubWindow *child = findMdiChild("");
    if (child != NULL) {
        TDrawWindow *mdiChild = qobject_cast<TDrawWindow *>(child->widget());
        if (!mdiChild->isWindowModified() && mdiChild->myGraphName.isEmpty()) {
            mdiArea->setActiveSubWindow(child);
            myCurrentDrawWindow = mdiChild;
        }
    }
    if (myCurrentDrawWindow == NULL)
        CMGNew();
    Q_ASSERT(myCurrentDrawWindow);
    myCurrentDrawWindow->loadGraph(name);
}

QMdiSubWindow *TMyWindow::findMdiChild(const QString &graphName)
{
    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        TDrawWindow *mdiChild = qobject_cast<TDrawWindow *>(window->widget());
        if (mdiChild->myGraphName == graphName)
            return window;
    }
    return NULL;
}

void TMyWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        TDrawWindow *child = qobject_cast<TDrawWindow *>(windows.at(i)->widget());

        QString text = i < 9 ? tr("&%1 %2") : tr("%1 %2");
        QAction *action  = windowMenu->addAction(text.arg(i + 1).arg(child->myGraphExtName));
        action->setCheckable(true);
        action->setChecked(child == myCurrentDrawWindow);
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows[i]);
    }
}

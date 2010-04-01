#include "qmywindow.h"
#include "qobjecteditor.h"

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
    fileMenu = menuBar()->addMenu(tr("Ôàéë"));
    fileMenu->addAction(newGraphAct);
    fileMenu->addAction(openGraphAct);
    fileMenu->addAction(saveGraphAct);
    fileMenu->addAction(saveAsGraphAct);
    fileMenu->addAction(saveAsImageGraphAct);

    objectMenu = menuBar()->addMenu(tr("Îáúåêò"));
    objectMenu->addAction(openObjectEditorAct);
    objectMenu->addAction(registerUnitAct);
    objectMenu->addAction(viewContentAct);
    objectMenu->addAction(viewGarbageAct);

    buildMenu = menuBar()->addMenu(tr("Çàïóñê"));
    buildMenu->addAction(runAct);
    buildMenu->addAction(compileAct);
    buildMenu->addAction(saveStructAct);
    buildMenu->addAction(manualInputAct);
}

void TMyWindow::createActions()
{
    //Ð“Ñ€Ð°Ñ„

    newGraphAct = new QAction(tr("Ñîçäàòü"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Ñîçäàòü ïóñòîé ãðàô"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(tr("Îòêðûòü"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    openGraphAct->setStatusTip(tr("ÎÒêðûòü èç áàçû"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(tr("Ñîõðàíèòü"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    saveGraphAct->setStatusTip(tr("Ñîõðàíèòü â áàçó"));
    connect(saveGraphAct, SIGNAL(triggered()), this, SLOT(CMGSave()));

    saveAsGraphAct = new QAction(tr("Ñîõðàíèòü êàê"), this);
    saveAsGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsGraphAct->setStatusTip(tr("Ñîõðàíÿåò â áàçó"));
    connect(saveAsGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAs()));

    saveAsImageGraphAct = new QAction(tr("Ñîõðàíèòü êàê êàðòèíêó"), this);
    saveAsImageGraphAct->setShortcuts(QKeySequence::SaveAs);
    saveAsImageGraphAct->setStatusTip(tr("Ñõðàíÿåò êàê êàðòèíêó"));
    connect(saveAsImageGraphAct, SIGNAL(triggered()), this, SLOT(CMGSaveAsImage()));

    //ÐžÐ±ÑŠÐµÐºÑ‚

    viewContentAct = new QAction(tr("Äåðåâî îáúåêòîâ"), this);
    viewContentAct->setStatusTip(tr("Äåðåâî îáúåêòîâ"));
    connect(viewContentAct, SIGNAL(triggered()), this, SLOT(CMContent()));

    viewGarbageAct = new QAction(tr("Íåèñïîëüçóåìûå îáúåêòû"), this);
    viewGarbageAct->setStatusTip(tr("Íåèñïîëüçóåìûå îáúåêòû"));
    connect(viewGarbageAct, SIGNAL(triggered()), this, SLOT(CMShowGarbage()));

    registerUnitAct = new QAction(tr("Çàðåãèñòðèðîâàòü ìîäóëü"), this);
    registerUnitAct->setStatusTip(tr("Çàðåãèñòðèðîâàòü ìîäóëü"));
    connect(registerUnitAct, SIGNAL(triggered()), this, SLOT(CMNewModule()));

    openObjectEditorAct = new QAction(tr("Ðåäàêòîð îáúåêòîâ"), this);
    openObjectEditorAct->setStatusTip(tr("Ðåäàêòîð îáúåêòîâ"));
    connect(openObjectEditorAct, SIGNAL(triggered()), this, SLOT(CMObjList()));

    //Ð—Ð°Ð¿ÑƒÑÐº

    runAct = new QAction(QIcon(":/images/build.png"), tr("ÐŸÐ¾ÑÑ‚Ñ€Ð¾ÐµÐ½Ð¸Ðµ Ð¸ Ð·Ð°Ð¿ÑƒÑÐº"), this);
    runAct->setStatusTip(tr("Ð’Ñ‹Ð¿Ð¾Ð»Ð½Ð¸Ñ‚ÑŒ Ð¿Ñ€Ð¾Ð³Ñ€Ð°Ð¼Ð¼Ñƒ"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(CMRun()));

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("ÐšÐ¾Ð¼Ð¿Ð¸Ð»Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ"), this);
    compileAct->setStatusTip(tr("Ð¡ÐºÐ¾Ð¼Ð¿Ð¸Ð»Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ Ð¿Ñ€Ð¾Ð³Ñ€Ð°Ð¼Ð¼Ñƒ"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));

    saveStructAct = new QAction(tr("Ð—Ð°Ð¿Ð¸ÑÑŒ ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ñ‹"), this);
    saveStructAct->setStatusTip(tr("Ð—Ð°Ð¿Ð¸ÑÑŒ ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ñ‹"));
    connect(saveStructAct, SIGNAL(triggered()), this, SLOT(CMSaveStruct()));

    manualInputAct = new QAction(tr("Ð ÐµÐ¶Ð¸Ð¼ Ñ€ÑƒÑ‡Ð½Ð¾Ð³Ð¾ Ð²Ð²Ð¾Ð´Ð° Ð´Ð°Ð½Ð½Ñ‹Ñ…"), this);
    manualInputAct->setStatusTip(tr("Ð ÐµÐ¶Ð¸Ð¼ Ñ€ÑƒÑ‡Ð½Ð¾Ð³Ð¾ Ð²Ð²Ð¾Ð´Ð° Ð´Ð°Ð½Ð½Ñ‹Ñ…"));
    connect(manualInputAct, SIGNAL(triggered()), this, SLOT(CMDoUserDialog()));

    //LeftToolBar

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

    addArcButton = new QToolButton;
    addArcButton->setCheckable(true);
    addArcButton->setIcon(QIcon(":/images/linepointer.png"));

    addSyncArcButton = new QToolButton;
    addSyncArcButton->setCheckable(true);
    addSyncArcButton->setIcon(QIcon(":/images/syncarc.png"));
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
    mainToolBar = addToolBar(tr("Ð—Ð°Ð¿ÑƒÑÐº"));
    mainToolBar->addAction(runAct);
    mainToolBar->addAction(compileAct);

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(addTopButton, int(QDiagramScene::InsertItem));
    pointerTypeGroup->addButton(addCommentButton, int(QDiagramScene::InsertText));
    pointerTypeGroup->addButton(pointerButton, int(QDiagramScene::MoveItem));
    pointerTypeGroup->addButton(addArcButton, int(QDiagramScene::InsertLine));
    pointerTypeGroup->addButton(addSyncArcButton, int(QDiagramScene::InsertSync));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    leftToolBar = new QToolBar(tr("Ð˜Ð½ÑÑ‚Ñ€ÑƒÐ¼ÐµÐ½Ñ‚Ñ‹"), this);
    addToolBar(Qt::LeftToolBarArea, leftToolBar);
    leftToolBar->addWidget(addTopButton);
    leftToolBar->addWidget(addCommentButton);
    leftToolBar->addWidget(pointerButton);
    leftToolBar->addWidget(addArcButton);
    leftToolBar->addWidget(addSyncArcButton);
}

void TMyWindow::CMGNew()
{
    TDrawWindow *newDrawWindow = createDrawWindow();
    //newDrawWindow->newFile();
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

}

void TMyWindow::CMObjList()
{
    QObjectEditor editor;
    if (editor.exec()){

    }
}

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
QLabel *globalInfoLabel;

TMyWindow::TMyWindow()
{
    globalStatusBar = new QStatusBar(this);
    setStatusBar(globalStatusBar);

    createActions();
    createMenus();
    createToolBar();
    createUndoView();
    createDockWindows();
    createDrawWindow();

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
    alignHLeftAct = new QAction(QIcon(":/images/shape_align_left.png"), tr("Выровнить к левому краю"), this);
    alignHLeftAct->setStatusTip(tr("Выровнить вершины к левому краю"));
    connect(alignHLeftAct, SIGNAL(triggered()), this, SLOT(alignHLeft()));

    alignHRightAct = new QAction(QIcon(":/images/shape_align_right.png"), tr("Выровнить к правому краю"), this);
    alignHRightAct->setStatusTip(tr("Выровнить вершины к правому краю"));
    connect(alignHRightAct, SIGNAL(triggered()), this, SLOT(alignHRight()));

    alignHCenterAct = new QAction(QIcon(":/images/shape_align_center.png"), tr("Выровнить к центру по горизонтали"), this);
    alignHCenterAct->setStatusTip(tr("Выровнить вершины к центру по горизонтали"));
    connect(alignHCenterAct, SIGNAL(triggered()), this, SLOT(alignHCenter()));

    alignVTopAct = new QAction(QIcon(":/images/shape_align_top.png"), tr("Выровнить к верхнему краю"), this);
    alignVTopAct->setStatusTip(tr("Выровнить вершины к верхнему краю"));
    connect(alignVTopAct, SIGNAL(triggered()), this, SLOT(alignVTop()));

    alignVBottomAct = new QAction(QIcon(":/images/shape_align_bottom.png"), tr("Выровнить к нижнему краю"), this);
    alignVBottomAct->setStatusTip(tr("Выровнить вершины к нижнему краю"));
    connect(alignVBottomAct, SIGNAL(triggered()), this, SLOT(alignVBottom()));

    alignVCenterAct = new QAction(QIcon(":/images/shape_align_middle.png"), tr("Выровнить к центру по вертикали"), this);
    alignVCenterAct->setStatusTip(tr("Выровнить вершины к центру по вертикали"));
    connect(alignVCenterAct, SIGNAL(triggered()), this, SLOT(alignVCenter()));

    distribVerticallyAct = new QAction(QIcon(":/images/shape_distrib_vertically.png"), tr("Распределить по вертикали"), this);
    distribVerticallyAct->setStatusTip(tr("Распределить вершиные по вертикали на одинаковом расстоянии"));
    connect(distribVerticallyAct, SIGNAL(triggered()), this, SLOT(distribVertically()));

    distribHorizontallyAct = new QAction(QIcon(":/images/shape_distrib_horizontally.png"), tr("Распределить по горизонтали"), this);
    distribHorizontallyAct->setStatusTip(tr("Распределить вершиные по горизонтали на одинаковом расстоянии"));
    connect(distribHorizontallyAct, SIGNAL(triggered()), this, SLOT(distribHorizontally()));
}

TDrawWindow* TMyWindow::createDrawWindow()
{
    TDrawWindow *newDrawWindow = new TDrawWindow;
    setCentralWidget(newDrawWindow);
    connect(newDrawWindow, SIGNAL(sceneChanged()),
            this, SLOT(sceneChanged()));
    connect(newDrawWindow, SIGNAL(itemChanged(QGraphicsItem*)),
            this, SLOT(getInfo(QGraphicsItem*)));
    connect(newDrawWindow, SIGNAL(selectionChanged(QList<QGraphicsItem*>)),
            this, SLOT(updateAlignToolBar(QList<QGraphicsItem*>)));
    undoView->setStack(activeDrawWindow()->undoStack);

    alignHLeftAct->setEnabled(false);
    alignHCenterAct->setEnabled(false);
    alignHRightAct->setEnabled(false);
    alignVTopAct->setEnabled(false);
    alignVCenterAct->setEnabled(false);
    alignVBottomAct->setEnabled(false);
    distribVerticallyAct->setEnabled(false);
    distribHorizontallyAct->setEnabled(false);

    globalInfoLabel->setText("\n\n\n\n\n\n\n\n\n\n");
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
    pointerTypeGroup->addButton(addTopButton, int(QDiagramScene::InsertNormalTop));
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
    layoutToolBar->addSeparator();
    layoutToolBar->addAction(distribHorizontallyAct);
    layoutToolBar->addAction(distribVerticallyAct);

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

void TMyWindow::sceneChanged()
{
    if (activeDrawWindow())
        pointerTypeGroup->button(int(activeDrawWindow()->mode()))->setChecked(true);
    setWindowTitle(myGraphExtName == "" ?  tr("Untitled* - Граф-редактор") : myGraphExtName + tr("* - Граф-редактор"));
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
        QMessageBox::warning(this, tr("Ошибка"), tr("Сохраните граф"), QMessageBox::Ok);
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
        setWindowTitle(tr("Untitled - Граф-редактор"));
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

void TMyWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Инфо"), this);
    globalInfoLabel = new QLabel(dock);
    globalInfoLabel->setWordWrap(true);
    globalInfoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setWidget(globalInfoLabel);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Список команд"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setWidget(undoView);
    addDockWidget(Qt::RightDockWidgetArea, dock);

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

void TMyWindow::distribHorizontally()
{
    activeDrawWindow()->distribHorizontally();
}

void TMyWindow::distribVertically()
{
    activeDrawWindow()->distribVertically();
}

void TMyWindow::createUndoView()
{
    undoView = new QUndoView();
    undoView->setWindowTitle(tr("Список команд"));
}

void TMyWindow::getInfo(QGraphicsItem *item)
{
    QArc *arc = NULL;
    QTop *top = NULL;
    QSyncArc *syncArc = NULL;
    QString status = tr("");
    QString info = tr("");
    QString arcTypeStr;
    if (item != NULL) {
        switch (item->type()) {
        case QArc::Type:
            arc = qgraphicsitem_cast<QArc*>(item);
        case QArcLine::Type:
        case QSerialArcTop::Type:
            if (!arc)
                arc = qgraphicsitem_cast<QArc*>(item->parentItem());
            switch (arc->arcType()) {
            case QArc::SerialArc:
                arcTypeStr = tr("последовательная");
                break;
            case QArc::ParallelArc:
                arcTypeStr = tr("параллельная");
                break;
            case QArc::TerminateArc:
                arcTypeStr = tr("терминирующая");
                break;
            }
            status.append(tr("Дуга ") + arcTypeStr);
            info.append(tr("Дуга\nТип: ") + arcTypeStr + "\n");
            if (arc->startItem() != NULL) {
                status.append(tr(" Начальная вершина ") + QString::number(arc->startItem()->number));
                info.append(tr("Начальная вершина: ") + QString::number(arc->startItem()->number) + "\n");
            }
            if (arc->endItem() != NULL) {
                status.append(tr(" Конечная вершина ") + QString::number(arc->endItem()->number));
                info.append(tr("Конечная вершина: ") + QString::number(arc->endItem()->number) + "\n");
            }
            status.append(tr(" Приоритет ") + QString::number(arc->priority()) +
                          tr(" Ширина пера ") + QString::number(arc->pen().width()));
            info.append(tr("Приоритет: ") + QString::number(arc->priority()) + "\n" +
                        tr("Ширина пера ") + QString::number(arc->pen().width()) + "\n");
            if (arc->predicate != NULL) {
                status.append(tr(" Предикат ") + arc->predicate->extName);
                info.append(tr("Предикат: ") + arc->predicate->extName + "\n");
            }
            info.append(tr("Число изломов: ") + QString::number(arc->lines.count()) + "\n");
            break;

    case QTop::Type:
            top = qgraphicsitem_cast<QTop* >(item);
            status.append(tr("Номер вершины ") + QString::number(top->number));
            info.append(tr("Номер вершины ") + QString::number(top->number) + "\n");
            if (top->actor != NULL) {
                status.append(tr(" Актор ") + top->actor->extName);
                info.append(tr("Актор: ") + top->actor->extName + "\n");
            }
            info.append(tr("Число дуг: ") + QString::number(top->allArcs().count()) + "\n");
            info.append(tr("Число исход. дуг: ") + QString::number(top->outArcs().count()) + "\n");
            info.append(tr("Число вход. дуг: ") + QString::number(top->inArcs().count()) + "\n");
            info.append(tr("Left: ") + QString::number(top->mapRectToScene(top->rect()).left()) + "\n");
            info.append(tr("Top: ") + QString::number(top->mapRectToScene(top->rect()).top()) + "\n");
            info.append(tr("Right: ") + QString::number(top->mapRectToScene(top->rect()).right()) + "\n");
            info.append(tr("Bottom: ") + QString::number(top->mapRectToScene(top->rect()).bottom()) + "\n");
            break;
    case QSyncArc::Type:
            syncArc = qgraphicsitem_cast<QSyncArc* >(item);
            break;
        }
    }

    globalStatusBar->showMessage(status);
    globalInfoLabel->setText(info);
}

void TMyWindow::updateAlignToolBar(QList<QGraphicsItem *> items)
{
    alignHLeftAct->setEnabled(false);
    alignHCenterAct->setEnabled(false);
    alignHRightAct->setEnabled(false);
    alignVTopAct->setEnabled(false);
    alignVCenterAct->setEnabled(false);
    alignVBottomAct->setEnabled(false);
    distribVerticallyAct->setEnabled(false);
    distribHorizontallyAct->setEnabled(false);

    items.count() > 0 ? getInfo(items.first()) : getInfo(NULL);

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
    }
}
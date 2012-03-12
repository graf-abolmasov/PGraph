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
#include "../../src/compiler/compi.h"
#include "../../src/editor/qarcline.h"
#include "../../src/editor/qserialarctop.h"
#include "../../src/editor/qsyncarc.h"
#include "../../src/editor/qarc.h"
#include "../../src/editor/qcomment.h"
#include "../../src/compiler/datacompiler.h"
#include "../../src/editor/dialogs/projectdialog.h"
#include "../../src/common/qgraphsettings.h"


TMyWindow::TMyWindow()
{
    setWindowIcon(QIcon(":/images/G.png"));

    saveGraphAct = NULL;
    nativeCompiler = new NativeCompiler(this);
    isParallel = QGraphSettings::isParallel();
    globalOutput = new QListWidget();
    connect(globalLogger, SIGNAL(newMessage(QString)), this, SLOT(write2globalOutput(QString)));

    if (globalDBManager->getProjectId() != -1) {
        setStatusBar(new QStatusBar(this));
        createActions();
        createMenus();
        createToolBar();
        createUndoView();
        createDockWindows();
        createDrawWindow();
        createOutputWindow();
        readSettings();
    }
}

void TMyWindow::write2globalOutput(const QString &msg)
{
    globalOutput->addItem(msg);
    globalOutput->scrollToBottom();
}

TMyWindow::~TMyWindow()
{
    delete nativeCompiler;
}

void TMyWindow::createMenus()
{
    grafMenu = menuBar()->addMenu(tr("Граф"));
    connect(grafMenu, SIGNAL(aboutToShow()), this, SLOT(grafMenuAboutToShow()));
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

    dataMenu = menuBar()->addMenu(tr("Данные"));
    dataMenu->addAction(variablesAct);
    dataMenu->addAction(dataTypeAct);
    dataMenu->addSeparator();
    dataMenu->addAction(compileDataAct);

    buildMenu = menuBar()->addMenu(tr("Запуск"));
    buildMenu->addAction(buildAct);
    buildMenu->addAction(saveStructAct);
    buildMenu->addAction(compileAct);
    buildMenu->addAction(manualInputAct);

    helpMenu = menuBar()->addMenu(tr("Помощь"));
    helpMenu->addAction(aboutEditorAct);
}

void TMyWindow::createActions()
{

    newGraphAct = new QAction(QIcon(":images/new.png"), tr("Создать"), this);
    newGraphAct->setShortcuts(QKeySequence::New);
    newGraphAct->setStatusTip(tr("Создать граф"));
    connect(newGraphAct, SIGNAL(triggered()), this, SLOT(CMGNew()));

    openGraphAct = new QAction(QIcon(":/images/open.png"), tr("Открыть"), this);
    openGraphAct->setShortcuts(QKeySequence::Open);
    openGraphAct->setStatusTip(tr("Открыть граф"));
    connect(openGraphAct, SIGNAL(triggered()), this, SLOT(CMGOpen()));

    saveGraphAct = new QAction(QIcon(":/images/save.png"), tr("Сохранить"), this);
    saveGraphAct->setShortcuts(QKeySequence::Save);
    saveGraphAct->setStatusTip(tr("Сохранить граф"));
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

    compileDataAct = new QAction(QIcon(":/images/data.png"), tr("Компиляция данных"), this);
    connect(compileDataAct, SIGNAL(triggered()), this, SLOT(CMCompileData()));

    buildAct = new QAction(QIcon(":/images/build.png"), tr("Компилировать с++"), this);
    buildAct->setStatusTip(tr("Компиляция"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(CMBuild()));

    compileAct = new QAction(QIcon(":/images/compile.png"), tr("Компилировать Граф-модель"), this);
    compileAct->setStatusTip(tr("Компилировать"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(CMCompile()));
    compileAct->setEnabled(false);
    compileAct->setShortcut(QKeySequence(tr("F5")));

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

    addSyncArcButton = NULL;
    addMultiProcTopButton = NULL;
    if (isParallel) {
        addSyncArcButton = new QToolButton;
        addSyncArcButton->setCheckable(true);
        addSyncArcButton->setIcon(QIcon(":/images/syncarc.png"));
        //        addSyncArcButton->setEnabled(false);

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
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    scaleCombo->addItems(scales);
    connect(scaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setFixedScale(QString)));

    scaleSlider = new QSlider;
    scaleSlider->setRange(50, 200);
    scaleSlider->setValue(100);
    scaleSlider->setOrientation(Qt::Horizontal);
    scaleSlider->setMaximumWidth(100);
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(setFloatScale(int)));

    //    showDataLayer = new QToolButton();
    //    showDataLayer->setText(tr("Данные"));
    //    showDataLayer ->setCheckable(true);
    //    connect(showDataLayer, SIGNAL(clicked(bool)), this, SLOT(showDataLayerClicked(bool)));
}

TDrawWindow* TMyWindow::createDrawWindow()
{
    TDrawWindow *newDrawWindow = new TDrawWindow;
    setCentralWidget(newDrawWindow);
    graphLoaded("", "");

    connect(newDrawWindow, SIGNAL(sceneChanged()),
            this, SLOT(sceneChanged()));
    connect(newDrawWindow, SIGNAL(itemChanged(QGraphicsItem*)),
            this, SLOT(getInfo(QGraphicsItem*)));
    connect(newDrawWindow, SIGNAL(selectionChanged(QList<QGraphicsItem*>)),
            this, SLOT(updateToolBar(QList<QGraphicsItem*>)));
    connect(newDrawWindow, SIGNAL(graphLoaded(QString,QString)), this,
            SLOT(graphLoaded(QString,QString)));
    connect(newDrawWindow, SIGNAL(mouseScrollScaleChanged(float)), this, SLOT(updateScaleSlider(float)));

    undoView->setStack(activeDrawWindow()->undoStack);

    alignHLeftAct->setEnabled(false);
    alignHCenterAct->setEnabled(false);
    alignHRightAct->setEnabled(false);
    alignVTopAct->setEnabled(false);
    alignVCenterAct->setEnabled(false);
    alignVBottomAct->setEnabled(false);
    distribVerticallyAct->setEnabled(false);
    distribHorizontallyAct->setEnabled(false);

    globalInfoLabel->setText("\n\n\n\n\n\n\n");
    scaleCombo->setCurrentIndex(2);
    return newDrawWindow;
}

void TMyWindow::createToolBar()
{
    mainToolBar = addToolBar(tr("Инструменты"));
    mainToolBar->addAction(newGraphAct);
    mainToolBar->addAction(openGraphAct);
    mainToolBar->addAction(saveGraphAct);
    mainToolBar->addSeparator();
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

    //    layerToolBar = addToolBar(tr("Слои"));
    //    layerToolBar->addWidget(showDataLayer);
    //    Если нужен ползунковый регулятор масштаба
    //    scaleToolBar->addSeparator();
    scaleToolBar->addWidget(scaleSlider);
}


TDrawWindow *TMyWindow::activeDrawWindow()
{
    return qobject_cast<TDrawWindow *>(centralWidget());
}

void TMyWindow::pointerGroupClicked(int)
{
    TDrawWindow *drawWindow  = activeDrawWindow();
    if (drawWindow != NULL)
        drawWindow->setMode(QDiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void TMyWindow::sceneChanged()
{
    if (activeDrawWindow())
        pointerTypeGroup->button(int(activeDrawWindow()->mode()))->setChecked(true);
    setWindowTitle(activeDrawWindow()->myGraphName.isEmpty() ?  tr("Untitled* - Граф-редактор") : activeDrawWindow()->myGraphExtName + tr("* - Граф-редактор"));
    saveGraphAct->setEnabled(true);
}

void TMyWindow::CMGNew()
{
    TDrawWindow *newDrawWindow = createDrawWindow();
    newDrawWindow->showMaximized();
    graphLoaded("", "");
    globalPredicateList.clear();
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
        const QString extName = dialog.getResult();
        if (extName.isEmpty())
            return;
        QString name = "G" + getCRC(extName.toUtf8());
        if (activeDrawWindow()->saveGraph(name, extName)) {
            setWindowTitle(activeDrawWindow()->myGraphExtName + tr(" - Граф-редактор"));
            saveGraphAct->setEnabled(false);
            statusBar()->showMessage(tr("Сохранено как ") + activeDrawWindow()->myGraphName, 3000);
        }
    }
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
    if (dialog.exec()){
        CMGNew();
        activeDrawWindow()->loadGraph(dialog.getResult());
        scaleSlider->setValue(100);
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

void TMyWindow::CMSaveStruct()
{
    if (activeDrawWindow()->saveStruct())
        statusBar()->showMessage(tr("Структура записана"), 2000);
}

void TMyWindow::CMGSave()
{
    const TDrawWindow *adw = activeDrawWindow();
    if (adw->myGraphName.isEmpty()) {
        CMGSaveAs();
        return;
    }
    if (activeDrawWindow()->updateGraph()) {
        setWindowTitle(activeDrawWindow()->myGraphExtName + tr(" - Граф-редактор"));
        saveGraphAct->setEnabled(false);
        statusBar()->showMessage(tr("Сохранено как ") + activeDrawWindow()->myGraphName, 3000);
    }
}

void TMyWindow::CMHelpAbout()
{
    QMessageBox::about(this, tr("O Граф-редакторе"),
                       tr("<h2>Граф-редактор 2.0</h2>"
                          "<p>Внутренняя версия ") + QApplication::applicationVersion() +
                       tr("<p>Copyright &copy; 2000-2012 FuzzyLogic Team. All rights reserved.</p>"
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
    globalOutput->setParent(outputDock);
    outputDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    outputDock->setWidget(globalOutput);
    addDockWidget(Qt::BottomDockWidgetArea, outputDock);
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
    if (item == NULL) {
        globalInfoLabel->setText("");
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
        info.append(tr("Число изломов: ") + QString::number(arc->lines.count()) + "\n");
        if (arc->predicate != NULL) {
            info.append(tr("\nО предикате\n"));
            info.append(tr("Тип: ") + predicateTypeToString(arc->predicate) + "\n");
            info.append(tr("Название: ") + arc->predicate->extName + "\n");
            info.append(tr("Внутреннее имя: ") + arc->predicate->name + "\n");
            if (arc->predicate->type == Predicate::NormalType)
                info.append(tr("Базовый модуль: ") + arc->predicate->baseModule->name + "\n");
        }
        break;
    case QTop::Type:
        top = qgraphicsitem_cast<QTop* >(item);
        info.append(tr("Номер вершины ") + QString::number(top->number) + "\n");
        info.append(tr("Число дуг: ") + QString::number(top->allArcs().count()) + "\n");
        info.append(tr("Число исход. дуг: ") + QString::number(top->outArcs().count()) + "\n");
        info.append(tr("Число вход. дуг: ") + QString::number(top->inArcs().count()) + "\n");
        info.append(tr("Left: ") + QString::number(top->mapRectToScene(top->rect()).left()) + "\n");
        info.append(tr("Top: ") + QString::number(top->mapRectToScene(top->rect()).top()) + "\n");
        info.append(tr("Right: ") + QString::number(top->mapRectToScene(top->rect()).right()) + "\n");
        info.append(tr("Bottom: ") + QString::number(top->mapRectToScene(top->rect()).bottom()) + "\n");
        if (top->actor != NULL) {
            info.append(tr("\nОб акторе\n"));
            info.append(tr("Тип: ") + actorTypeToString(top->actor) + "\n");
            info.append(tr("Название: ") + top->actor->extName + "\n");
            info.append(tr("Внутреннее имя: ") + top->actor->name+ "\n");
            if (top->actor->type == Actor::NormalType)
                info.append(tr("Базовый модуль: ") + top->actor->baseModule->name + "\n");
        }
        break;
    case QSyncArc::Type:
        syncArc = qgraphicsitem_cast<QSyncArc* >(item);
        break;
    }

    globalInfoLabel->setText(info);
}

void TMyWindow::updateToolBar(QList<QGraphicsItem *> items)
{
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
    int newScale = scale.left(scale.indexOf(tr("%"))).toInt();
    activeDrawWindow()->scale(newScale/100.0);
    scaleSlider->setValue(newScale);
}

void TMyWindow::setFloatScale(const int scale)
{
    int idx = scaleCombo->findText(QString::number(scale) + "%");
    if (idx > 0)
        scaleCombo->setCurrentIndex(idx);
    activeDrawWindow()->scale(scale/100.0);
}

void TMyWindow::readSettings()
{

}

void TMyWindow::writeSettings()
{
    //Сохраняем список недавних файлов
}

void TMyWindow::graphLoaded(QString name, QString extName)
{
    if (name.isEmpty()){
        buildMenu->setEnabled(false);
        compileAct->setEnabled(false);
    } else {
        buildMenu->setEnabled(true);
        compileAct->setEnabled(true);
    }
    saveGraphAct->setEnabled(true);
    setWindowTitle((extName.isEmpty() ? tr("Untitled") : extName) + tr(" - Граф-редактор"));
}

void TMyWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    if (saveGraphAct != NULL && saveGraphAct->isEnabled())
        if (QMessageBox::question(this, tr("Сохранить"), tr("Сохранить изменения в ") + activeDrawWindow()->myGraphExtName + tr("?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes)
            CMGSave();
    event->accept();
}

void TMyWindow::grafMenuAboutToShow()
{

}

void TMyWindow::CMCompile()
{
    CMGSave();
    GraphCompiler gc(activeDrawWindow()->getGraph());
    globalLogger->skipLine();
    gc.compile();
}

void TMyWindow::showDataLayerClicked(bool )
{
    //    activeDrawWindow()->showDataLayer(clicked);
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
    DataCompiler c;
    globalLogger->skipLine();
    c.compile();
}

void TMyWindow::updateScaleSlider(const float scale)
{
    scaleSlider->setValue(scaleSlider->value() + scale);
}

void TMyWindow::CMBuild()
{
    globalLogger->skipLine();
    nativeCompiler->compile();
}

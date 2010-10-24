#include "multiproctoppropertydialog.h"
#include "ui_multiproctoppropertydialog.h"
#include "qmultiproctop.h"
#include "databasemanager.h"
#include "graph.h"
#include "predicate.h"
#include "actor.h"
#include "qarc.h"
#include "qnormaltop.h"
#include "commonutils.h"
#include "globalvariables.h"

MultiProcTopPropertyDialog::MultiProcTopPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiProcTopPropertyDialog)
{
    ui->setupUi(this);
}

MultiProcTopPropertyDialog::~MultiProcTopPropertyDialog()
{
    delete ui;
}

void MultiProcTopPropertyDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MultiProcTopPropertyDialog::prepareForm(QMultiProcTop *top)
{
    myTop = top;
    ui->procCountSpnBox->setValue(myTop->procCount);

    //Добавляем пустой актор
    myActorList.insert(0, NULL);
    ui->actorsListWidget->addItem(tr("Нет"));

    //Добавляем агрегаты
    QList<Graph* > myGraphList;
    if (globalDBManager->getGraphList(myGraphList))
        foreach(Graph* graph, myGraphList){
            QList<Variable* > varList;
            QStringList varAMList;
            myActorList.append(new Actor(graph->name, graph->extName, Actor::graphType, "", varList, varAMList, QImage()));
        }
    else QMessageBox::critical(NULL,
                               QObject::tr("Ошибка"),
                               QObject::tr("Не удалось получить список агрегатов.\n") + globalDBManager->lastError().databaseText(),
                               QMessageBox::Ok);

    //Добавляем акторы
    if (globalDBManager->getActorList(myActorList))
        for (int i = 1 ; i < myActorList.count(); i++)
            ui->actorsListWidget->addItem(myActorList.at(i)->extName);
    else QMessageBox::critical(NULL,
                               QObject::tr("Ошибка"),
                               QObject::tr("Не удалось получить список акторов.\n") + globalDBManager->lastError().databaseText(),
                               QMessageBox::Ok);

    //Выделяем актор в списке
    if (top->actor != NULL){
        int idx = -1;
        for (int i = 1; i < myActorList.count(); i++)
            if (top->actor->name == myActorList.at(i)->name &&
                top->actor->extName == myActorList.at(i)->extName) {
                idx = i;
                break;
        }
        if (idx != -1) ui->actorsListWidget->setCurrentRow(idx);
        else {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Вершина использует несуществующий объект"), QMessageBox::Ok).exec();
            top->actor = NULL;
        }
    } else ui->actorsListWidget->setCurrentRow(0);
}

QMultiProcTop* MultiProcTopPropertyDialog::getResult()
{
    return myTop;
}

void MultiProcTopPropertyDialog::on_actorsListWidget_currentRowChanged(int currentRow)
{
    QString info("");
    if (currentRow > 0){
        Actor* actor = myActorList.at(currentRow);
        info.append(tr("Name: ") + actor->name + "\r\n");
        QString type;
        switch (actor->type){
        case Actor::inlineType:
            type = tr("inline");
            break;
        case Actor::normalType:
            type = tr("normal");
            break;
        case Actor::graphType:
            type = tr("graph");
            break;
        }
        info.append(tr("Type: ") + type + "\r\n");
        info.append(tr("Base module: ") + actor->baseModule + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

void MultiProcTopPropertyDialog::on_actorsListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    accept();
}

void MultiProcTopPropertyDialog::on_buttonBox_accepted()
{
    QList<Top* > topList;
    QList<Arc* > arcList;
    QList<Comment* > commentList;
    QList<QSyncArc* > syncArcList;
    QList<QMultiProcTop* > multiProcTopList;
    myTop->actor = myActorList.at(ui->actorsListWidget->currentRow());
    myTop->procCount = ui->procCountSpnBox->value();
    if (myTop->actor == NULL) return;
    QString extName = myTop->actor->extName + " " + QString::number(myTop->procCount) + tr(" процессов");
    QString name = getCRC(extName.toUtf8());
    Graph* newGraph = new Graph(name, extName, topList, arcList, commentList, syncArcList, multiProcTopList);
    Top* headTop = new Top(0, -85, myTop->procCount*50 - 10, 30, 0, -1, true, "", "T");
    newGraph->topList.append(headTop);
    for (int i = 0; i < myTop->procCount; i++) {
        Top* newTop = new Top(-(myTop->procCount-1)*25+i*50, 0, 40, 30, i+1, -1, false, myTop->actor->name, "T");
        newGraph->topList.append(newTop);
        QStringList nodes;
        nodes.append(QString::number(-(myTop->procCount-1)*25+i*50) + " " +
                     QString::number(-85) + " " +
                     QString::number(-(myTop->procCount-1)*25+i*50) + " " +
                     QString::number(-1));
        Arc* newArc = new Arc(Arc::ParallelArc, i+1, 0, i+1, "", nodes);
        newGraph->arcList.append(newArc);
        nodes.clear();
        nodes.append(QString::number(-(myTop->procCount-1)*25+i*50) + " " +
                     QString::number(1) + " " +
                     QString::number(-(myTop->procCount-1)*25+i*50) + " " +
                     QString::number(85));
        newArc = new Arc(Arc::TerminateArc, 1, i+1, myTop->procCount + 1, "", nodes);
        newGraph->arcList.append(newArc);
    }
    Top* endTop = new Top(0, 85, myTop->procCount*50 - 10, 30, myTop->procCount + 1, -1, true, "", "T");
    newGraph->topList.append(endTop);
    if (globalDBManager->saveGraph(newGraph))
        myTop->actor = globalDBManager->getActor(name);
}

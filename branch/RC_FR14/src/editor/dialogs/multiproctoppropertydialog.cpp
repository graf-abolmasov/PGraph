#include <QtGui>

#include "multiproctoppropertydialog.h"
#include "ui_multiproctoppropertydialog.h"
#include "../../src/editor/qmultiproctop.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/VO/graphvo.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/common/VO/actor.h"
#include "../../src/editor/qarc.h"
#include "../../src/editor/qnormaltop.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/globalvariables.h"

MultiProcTopPropertyDialog::MultiProcTopPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiProcTopPropertyDialog)
{
    ui->setupUi(this);
}

MultiProcTopPropertyDialog *MultiProcTopPropertyDialog::getDialog(QMultiProcTop *top)
{
    MultiProcTopPropertyDialog *result = new MultiProcTopPropertyDialog();
    result->prepareForm(top);
    return result;
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
    ui->procCountSpnBox->setValue(top->procCount);

    //Добавляем пустой актор
    ui->actorsListWidget->addItem(tr("Нет"));
    myActorList.append(NULL);

    //Добавляем агрегаты
    QList<const Graph *> myGraphList;
    myGraphList = globalDBManager->getGraphList();
    foreach(const Graph *graph, myGraphList)
        myActorList.append(graph);

    //Добавляем акторы
    //Наверно это не нужно, т.к. в параллельных ветвях можно использовать только агрегаты
    myActorList.append(globalDBManager->getActorList());
    for (int i = 1 ; i < myActorList.count(); i++)
        ui->actorsListWidget->addItem(myActorList[i]->extName);

    //Выделяем актор в списке
    if (top->actor != NULL) {
        int idx = myActorList.indexOf(top->actor);
        if (idx != -1) ui->actorsListWidget->setCurrentRow(idx);
        else {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Вершина использует несуществующий объект"), QMessageBox::Ok).exec();
            top->actor = NULL;
            //TODO: Сзедать констрэйт на удаление актора у который используется в вершине
        }
    } else ui->actorsListWidget->setCurrentRow(0);

    theirTop = top;
}

QMultiProcTop* MultiProcTopPropertyDialog::getResult()
{
    return theirTop;
}

void MultiProcTopPropertyDialog::on_actorsListWidget_currentRowChanged(int currentRow)
{
    QString info("");
    if (currentRow > 0){
        const Actor *actor = myActorList[currentRow];
        info.append(tr("Name: ") + actor->name + "\r\n");
        QString type;
        switch (actor->type){
        case Actor::InlineType:
            type = tr("inline");
            break;
        case Actor::NormalType:
            type = tr("Обычный");
            break;
        case Actor::GraphType:
            type = tr("Агрегат");
            break;
        }
        info.append(tr("Type: ") + type + "\r\n");
        if (Actor::NormalType == actor->type)
            info.append(tr("Base module: ") + actor->baseModule->name + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

void MultiProcTopPropertyDialog::on_actorsListWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

void MultiProcTopPropertyDialog::on_buttonBox_accepted()
{
    theirTop->actor = myActorList[ui->actorsListWidget->currentRow()];
    theirTop->procCount = ui->procCountSpnBox->value();
    if (theirTop->actor == NULL) return;
    QString extName = theirTop->actor->extName + " " + QString::number(theirTop->procCount) + tr(" процессов");
    QString name = "G" + getCRC(extName.toUtf8());
    Graph newGraph(name, extName, QList<Top>(),  QList<Arc>(), QList<Comment>(), QList<SyncArc>());
    Top headTop(0, -85, theirTop->procCount*50 - 10, 30, 0, -1, true, makeInlineActor(ui->prepareDataEdt->document()->toPlainText()), Top::NormalTop);
    newGraph.topList.append(headTop);
    for (int i = 0; i < theirTop->procCount; i++) {
        Top newTop(-(theirTop->procCount-1)*25+i*50, 0, 40, 30, i+1, -1, false, theirTop->actor, Top::NormalTop);
        newGraph.topList.append(newTop);
        QStringList nodes;
        nodes.append(QString::number(-(theirTop->procCount-1)*25+i*50) + " " +
                     QString::number(-85) + " " +
                     QString::number(-(theirTop->procCount-1)*25+i*50) + " " +
                     QString::number(-1));
        Arc newArc(Arc::ParallelArc, i+1, 0, i+1, NULL, nodes);
        newArc.predicate = globalDBManager->getPredicate("P" + getCRC("1"));
        newGraph.arcList.append(newArc);
        nodes.clear();
        nodes.append(QString::number(-(theirTop->procCount-1)*25+i*50) + " " +
                     QString::number(1) + " " +
                     QString::number(-(theirTop->procCount-1)*25+i*50) + " " +
                     QString::number(85));
        newArc = Arc(Arc::TerminateArc, 1, i+1, theirTop->procCount + 1, NULL, nodes);
        newArc.predicate = globalDBManager->getPredicate("P" + getCRC("1"));
        newGraph.arcList.append(newArc);
    }
    Top endTop(0, 85, theirTop->procCount*50 - 10, 30, theirTop->procCount + 1, -1, false, makeInlineActor(ui->collectDataEdt->document()->toPlainText()), Top::NormalTop);
    newGraph.topList.append(endTop);
    globalDBManager->saveGraph(newGraph);
    theirTop->actor = globalDBManager->getActor(name);
}

const Actor *MultiProcTopPropertyDialog::makeInlineActor(const QString &code) const
{
    const QString name = "A" + getCRC(code.toUtf8());
    const Actor *newActor = globalDBManager->getActor(name);
    if (newActor == NULL) {
        newActor = new Actor(name, code, Actor::InlineType);
        QList<const Actor *> actorList = globalDBManager->getActorList();
        actorList.append(newActor);
        globalDBManager->setActorList(actorList);
    }
    return newActor;
}

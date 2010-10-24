#include "toppropertydialog.h"
#include "ui_toppropertydialog.h"
#include "databasemanager.h"
#include "globalvariables.h"

TopPropertyDialog::TopPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopPropertyDialog)
{
    ui->setupUi(this);
}

TopPropertyDialog::~TopPropertyDialog()
{
    delete ui;
}

void TopPropertyDialog::changeEvent(QEvent *e)
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


void TopPropertyDialog::prepareForm(QNormalTop* top){
    myTop = top;

    if (!globalDBManager->getActorList(myActorList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список акторов.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);

    myActorList.insert(0, NULL);
    QList<Graph* > myGraphList;
    if (!globalDBManager->getGraphList(myGraphList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список агрегатов.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);;
    foreach(Graph* graph, myGraphList){
        QList<Variable* > varList;
        QStringList varAMList;
        myActorList.append(new Actor(graph->name, graph->extName, Actor::graphType, "", varList, varAMList, QImage()));
    }
    ui->actorsListWidget->addItem(tr("Нет"));
    for (int i = 1 ; i < myActorList.count(); i++){
        ui->actorsListWidget->addItem(myActorList.at(i)->extName);
    }

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

    ui->spnBoxHeight->setProperty("value", top->rect().height());
    ui->spnBoxWidth->setProperty("value", top->rect().width());

    float minHeight = top->getMinHeight() > 10 ? top->getMinHeight() : 10;
    float minWidth = top->getMinWidth() > 10 ? top->getMinWidth() : 10;

    ui->spnBoxHeight->setProperty("minimum", minHeight);
    ui->spnBoxWidth->setProperty("minimum", minWidth);
}

QNormalTop* TopPropertyDialog::getResult(){
    int w = ui->spnBoxWidth->value();
    int h = ui->spnBoxHeight->value();
    myTop->setRect(-w/2, -h/2, w, h);
    myTop->actor = myActorList.at(ui->actorsListWidget->currentRow());
    return myTop;
}

void TopPropertyDialog::on_actorsListWidget_currentRowChanged(int currentRow)
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
        }        info.append(tr("Type: ") + type + "\r\n");
        info.append(tr("Base module: ") + actor->baseModule + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

void TopPropertyDialog::on_actorsListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    accept();
}

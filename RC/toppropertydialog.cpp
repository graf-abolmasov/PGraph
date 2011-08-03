#include <QtCore>
#include <QtGui>

#include "toppropertydialog.h"
#include "ui_toppropertydialog.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "qnormaltop.h"

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

    myActorList.append(NULL);
    myActorList.append(globalDBManager->getActorList());
    QList<const Graph*> graphList = globalDBManager->getGraphList();
    foreach (const Graph *graph, graphList)
        myActorList.append(graph);
    ui->actorsListWidget->addItem(tr("Нет"));
    for (int i = 1; i < myActorList.length(); i++)
        ui->actorsListWidget->addItem(QString(myActorList[i]->extName).replace(QRegExp("(\r+|\n+)"), " "));

    const Actor *actor = top->actor;
    int idx = myActorList.indexOf(actor);
    if (idx != -1)
        ui->actorsListWidget->setCurrentRow(idx);
    else {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Вершина использует несуществующий объект"), QMessageBox::Ok).exec();
        top->actor = NULL;
    }

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
    myTop->actor = myActorList[ui->actorsListWidget->currentRow()];
    return myTop;
}

void TopPropertyDialog::on_actorsListWidget_currentRowChanged(int currentRow)
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
            type = tr("normal");
            break;
        case Actor::GraphType:
            type = tr("graph");
            break;
        }        info.append(tr("Type: ") + type + "\r\n");
        if (actor->baseModule != NULL)
            info.append(tr("Base module: ") + actor->baseModule->name + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

void TopPropertyDialog::on_actorsListWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

void TopPropertyDialog::on_buttonBox_accepted()
{
    accept();
}

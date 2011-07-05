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

    try {
        myActorList = globalDBManager->getActorList();
        //myActorList.insert(0, Actor("Нет", "", Actor::InlineType, "", QList<Variable>(),QStringList(), QImage()));
        QList<Graph*> graphList = globalDBManager->getGraphList();
        foreach (Graph *graph, graphList)
            myActorList.append(graph);
        foreach (const Actor *actor, myActorList)
            ui->actorsListWidget->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));

        if (top->actor != NULL) {
            const Actor *actor = top->actor;
            int idx = myActorList.indexOf(actor);
            if (idx != -1)
                ui->actorsListWidget->setCurrentRow(idx + 1);
            else {
                QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Вершина использует несуществующий объект"), QMessageBox::Ok).exec();
                top->actor = NULL;
            }
        } else ui->actorsListWidget->setCurrentRow(0);
    } catch (QString s) {
        QMessageBox::critical(this, tr("Ошибка"), s, QMessageBox::Ok);
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
    myTop->actor = myActorList[ui->actorsListWidget->currentRow()-1];
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
        info.append(tr("Base module: ") + actor->baseModule->name + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

void TopPropertyDialog::on_actorsListWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

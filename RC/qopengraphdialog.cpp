#include <QtGui>

#include "qopengraphdialog.h"
#include "ui_qopengraphdialog.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "graph.h"

QOpenGraphDialog::QOpenGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QOpenGraphDialog)
{
    ui->setupUi(this);
}

QOpenGraphDialog::~QOpenGraphDialog()
{
    delete ui;
}

void QOpenGraphDialog::changeEvent(QEvent *e)
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

Graph* QOpenGraphDialog::getResult()
{
    return myGraphList.at(ui->listWidget->currentRow());
}

void QOpenGraphDialog::prepareForm()
{
    myGraphList = globalDBManager->getGraphList();
    for (int i = 0; i < myGraphList.count(); i++)
        ui->listWidget->addItem(new QListWidgetItem(myGraphList[i]->extName));
}

void QOpenGraphDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

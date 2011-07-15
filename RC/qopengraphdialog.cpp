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

QString QOpenGraphDialog::getResult()
{
    return myGraphList[ui->listWidget->currentRow()].name;
}

void QOpenGraphDialog::prepareForm()
{
    myGraphList = globalDBManager->getGraphListDB();
    foreach (Graph graph, myGraphList)
        ui->listWidget->addItem(new QListWidgetItem(graph.extName));
}

void QOpenGraphDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

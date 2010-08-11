#include "qopengraphdialog.h"
#include "ui_qopengraphdialog.h"
#include "databasemanager.h"

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
    globalDBManager->getGraphList(myGraphList);
    for (int i = 0; i < myGraphList.count(); i++)
        ui->listWidget->addItem(new QListWidgetItem(myGraphList.at(i)->extName));
}

void QOpenGraphDialog::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    accept();
}

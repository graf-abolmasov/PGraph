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

QString QOpenGraphDialog::getResult()
{
    return ui->listWidget->currentItem()->text();
}

void QOpenGraphDialog::prepareForm()
{
    QList<Graph* > graphList;
    globalDBManager->getGraphList(graphList);
    for (int i = 0; i < graphList.count(); i++)
        ui->listWidget->addItem(new QListWidgetItem(graphList.at(i)->extName));
}

#include <QtGui>

#include "qopengraphdialog.h"
#include "ui_qopengraphdialog.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/graphvo.h"

QOpenGraphDialog::QOpenGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QOpenGraphDialog)
{
    ui->setupUi(this);
    prepareForm();
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
    return myGraphList[ui->listWidget->currentRow()]->name;
}

void QOpenGraphDialog::prepareForm()
{
    myGraphList = globalDBManager->getGraphList();
    foreach (const Graph *graph, myGraphList)
        ui->listWidget->addItem(new QListWidgetItem(graph->extName));
    enableOkButton(-1);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(enableOkButton(int)));
}

void QOpenGraphDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

void QOpenGraphDialog::enableOkButton(int currentRow)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(currentRow != -1);
}

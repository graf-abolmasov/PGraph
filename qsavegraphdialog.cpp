#include "qsavegraphdialog.h"
#include "ui_qsavegraphdialog.h"

QSaveGraphDialog::QSaveGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSaveGraphDialog)
{
    ui->setupUi(this);
}

QSaveGraphDialog::~QSaveGraphDialog()
{
    delete ui;
}

void QSaveGraphDialog::changeEvent(QEvent *e)
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

QString QSaveGraphDialog::getResult()
{
    return ui->lineEdit->text();
}

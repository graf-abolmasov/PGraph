#include "arcpropertydialog.h"
#include "ui_arcpropertydialog.h"
#include "qarc.h"
#include "databasemanager.h"

ArcPropertyDialog::ArcPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArcPropertyDialog)
{
    ui->setupUi(this);
}

ArcPropertyDialog::~ArcPropertyDialog()
{
    delete ui;
}

void ArcPropertyDialog::changeEvent(QEvent *e)
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

void ArcPropertyDialog::prepareForm(QArc *arc)
{
    if (arc == NULL) return;
    myArc = arc;
    switch (arc->arcType()) {
    case QArc::SerialArc:
        ui->serialRadioBtn->setChecked(true);
        break;
    case QArc::ParallelArc:
        ui->parallelRadioBtn->setChecked(true);
        break;
    case QArc::TerminateArc:
        ui->terminateRadioBtn->setChecked(true);
        break;
    }
    ui->prioritySpnBox->setValue(arc->priority());

    //Загружаем предикаты из базы данных
    globalDBManager->getPredicateList(predicateList);

    for (int i = 0; i < predicateList.count(); i++){
        ui->predicateList->addItem(new QListWidgetItem(predicateList.at(i)->extName));
    }
}

QArc* ArcPropertyDialog::getResult()
{
    return myArc;
}

void ArcPropertyDialog::on_buttonBox_accepted()
{
    if (ui->serialRadioBtn->isChecked()) myArc->setArcType(QArc::SerialArc);
    if (ui->parallelRadioBtn->isChecked()) myArc->setArcType(QArc::ParallelArc);
    if (ui->terminateRadioBtn->isChecked()) myArc->setArcType(QArc::TerminateArc);
    myArc->setPriority(ui->prioritySpnBox->value());
    if (ui->predicateList->currentRow() > -1)
        myArc->predicate = predicateList.at(ui->predicateList->currentRow());
    else
        myArc->predicate = NULL;

}

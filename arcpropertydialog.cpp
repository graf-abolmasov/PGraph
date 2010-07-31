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

void ArcPropertyDialog::prepareForm(TArc *arc)
{
    if (arc == NULL) return;
    myArc = arc;
    switch (arc->arcType()) {
    case TArc::SerialArc:
        ui->serialRadioBtn->setChecked(true);
        break;
    case TArc::ParallelArc:
        ui->parallelRadioBtn->setChecked(true);
        break;
    case TArc::TerminateArc:
        ui->terminateRadioBtn->setChecked(true);
        break;
    }
    ui->prioritySpnBox->setValue(arc->priority());

    //Загружаем предикаты из базы данных
    globalDBManager->getPredicateList(predicateList);

}

TArc* ArcPropertyDialog::getResult()
{
    if (ui->serialRadioBtn->isChecked()) myArc->setArcType(TArc::SerialArc);
    if (ui->parallelRadioBtn->isChecked()) myArc->setArcType(TArc::ParallelArc);
    if (ui->terminateRadioBtn->isChecked()) myArc->setArcType(TArc::TerminateArc);
    myArc->setPriority(ui->prioritySpnBox->value());
}

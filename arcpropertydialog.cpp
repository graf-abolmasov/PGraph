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
    ui->prioritySpnBox->setMaximum(myArc->startItem()->outArcs().count());

    //Загружаем предикаты из базы данных
    globalDBManager->getPredicateList(myPredicateList);
    myPredicateList.insert(0, NULL);;

    ui->predicateList->addItem(tr("Нет"));
    for (int i = 1; i < myPredicateList.count(); i++){
        ui->predicateList->addItem(myPredicateList.at(i)->extName);
    }

    if (arc->predicate != NULL) {
        int idx = -1;
        for (int i = 1; i < myPredicateList.count(); i++)
            if (arc->predicate->name == myPredicateList.at(i)->name &&
                arc->predicate->extName == myPredicateList.at(i)->extName) {
                idx = i;
                break;
        }
        if (idx != -1) ui->predicateList->setCurrentRow(idx);
        else {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Дуга использует несуществующий предикат"), QMessageBox::Ok).exec();
            arc->predicate = NULL;
        }
    } else ui->predicateList->setCurrentRow(0);
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
    QList<QArc* > arcList = myArc->startItem()->outArcs();
    for (int i = 0; i < arcList.count(); i++)
        if (ui->prioritySpnBox->value() <= arcList.at(i)->priority() && arcList.at(i)!=myArc)
            arcList.at(i)->setPriority(arcList.at(i)->priority() + 1);
    if (ui->predicateList->currentRow() > -1)
        myArc->predicate = myPredicateList.at(ui->predicateList->currentRow());
    else
        myArc->predicate = NULL;
}

void ArcPropertyDialog::on_predicateList_itemDoubleClicked(QListWidgetItem* item)
{
    accept();
}

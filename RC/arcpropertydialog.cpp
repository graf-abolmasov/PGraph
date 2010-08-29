#include "arcpropertydialog.h"
#include "ui_arcpropertydialog.h"
#include "qarc.h"
#include "databasemanager.h"
#include "globalvariables.h"

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
    ui->predicateList->setFocus(Qt::MouseFocusReason);
}

QArc* ArcPropertyDialog::getResult()
{
    if (ui->serialRadioBtn->isChecked()) myArc->setArcType(QArc::SerialArc);
    if (ui->parallelRadioBtn->isChecked()) myArc->setArcType(QArc::ParallelArc);
    if (ui->terminateRadioBtn->isChecked()) myArc->setArcType(QArc::TerminateArc);
    QList<QArc* > arcList = myArc->startItem()->outArcs();
    if (myArc->priority() < ui->prioritySpnBox->value()) {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList.at(i)->priority()>myArc->priority() && arcList.at(i)->priority() <= ui->prioritySpnBox->value())
                arcList.at(i)->setPriority(arcList.at(i)->priority() - 1);
    } else {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList.at(i)->priority() < myArc->priority() && arcList.at(i)->priority() >= ui->prioritySpnBox->value())
                arcList.at(i)->setPriority(arcList.at(i)->priority() + 1);
    }
    myArc->setPriority(ui->prioritySpnBox->value());
    if (ui->predicateList->currentRow() > -1) {
        myArc->predicate = myPredicateList.at(ui->predicateList->currentRow());
        if (myArc->predicate != NULL && !globalPredicateList.contains(myArc->predicate->extName))
            globalPredicateList.append(myArc->predicate->extName);

    } else
        myArc->predicate = NULL;
    return myArc;
}

void ArcPropertyDialog::on_buttonBox_accepted()
{

}

void ArcPropertyDialog::on_predicateList_itemDoubleClicked(QListWidgetItem* item)
{
    accept();
}

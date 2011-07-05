#include <QtGui>

#include "arcpropertydialog.h"
#include "ui_arcpropertydialog.h"
#include "qarc.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "predicate.h"
#include "qtop.h"

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

ArcPropertyDialog *ArcPropertyDialog::getDialog(QArc *arc)
{
    ArcPropertyDialog *result = new ArcPropertyDialog();
    result->prepareForm(arc);
    return result;
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
    ui->prioritySpnBox->setMaximum(arc->startItem()->outArcs().count());

    //Загружаем предикаты из базы данных
    myPredicateList  = globalDBManager->getPredicateList();
    ui->predicateList->addItem(tr("Нет"));
    for (int i = 0; i < myPredicateList.count(); i++)
        ui->predicateList->addItem(QString(myPredicateList[i]->extName).replace(QRegExp("(\r+|\n+)"), " "));

    if (arc->getPredicate() != NULL) {
        const Predicate *arcPredicate = arc->getPredicate();
        int idx = myPredicateList.indexOf(arcPredicate);
        if (idx != -1)
            ui->predicateList->setCurrentRow(idx);
        else {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Дуга использует несуществующий предикат"), QMessageBox::Ok).exec();
            arc->setPredicate(NULL);
            //TODO: Зделать констрэйт в базе на удаление предикатов к которым есть привязка.
        }
    } else ui->predicateList->setCurrentRow(0);
    ui->predicateList->setFocus(Qt::MouseFocusReason);

    theirArc = arc;
}

QArc *ArcPropertyDialog::getResult()
{
    return theirArc;
}

void ArcPropertyDialog::on_predicateList_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}
void ArcPropertyDialog::on_buttonBox_accepted()
{
    if (ui->serialRadioBtn->isChecked()) theirArc->setArcType(QArc::SerialArc);
    else if (ui->parallelRadioBtn->isChecked()) theirArc->setArcType(QArc::ParallelArc);
    else if (ui->terminateRadioBtn->isChecked()) theirArc->setArcType(QArc::TerminateArc);

    QList<QArc* > arcList = theirArc->startItem()->outArcs();
    if (theirArc->priority() < ui->prioritySpnBox->value()) {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority()>theirArc->priority() && arcList[i]->priority() <= ui->prioritySpnBox->value())
                arcList[i]->setPriority(arcList[i]->priority() - 1);
    } else {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority() < theirArc->priority() && arcList[i]->priority() >= ui->prioritySpnBox->value())
                arcList[i]->setPriority(arcList[i]->priority() + 1);
    }
    theirArc->setPriority(ui->prioritySpnBox->value());
    if (ui->predicateList->currentRow() > 0) {
        theirArc->setPredicate(myPredicateList[ui->predicateList->currentRow()]);
        if (!globalPredicateList.contains(theirArc->getPredicate()->name))
            globalPredicateList.append(theirArc->getPredicate()->name);
        //TODO: сигнал о добавлении нового предиката
    } else
        theirArc->setPredicate(NULL);
}

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
    myPredicateList.append(NULL);
    myPredicateList.append(globalDBManager->getPredicateList());
    ui->predicateList->addItem(tr("Нет"));
    for (int i = 1; i < myPredicateList.count(); i++)
        ui->predicateList->addItem(QString(myPredicateList[i]->extName).replace(QRegExp("(\r+|\n+)"), " "));

    const Predicate *arcPredicate = arc->getPredicate();
    int idx = myPredicateList.indexOf(arcPredicate);
    if (idx != -1)
        ui->predicateList->setCurrentRow(idx);
    else {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Дуга использует несуществующий предикат"), QMessageBox::Ok).exec();
        arc->setPredicate(NULL);
        //TODO: Cделать констрэйт в базе на удаление предикатов к которым есть привязка.
    }
    ui->predicateList->setFocus(Qt::MouseFocusReason);

    myArc = arc;
}

QArc *ArcPropertyDialog::getResult()
{
    return myArc;
}

void ArcPropertyDialog::on_predicateList_itemDoubleClicked(QListWidgetItem *)
{
    if (makeResult())
        accept();
}
void ArcPropertyDialog::on_buttonBox_accepted()
{
    if (makeResult())
        accept();
}

bool ArcPropertyDialog::makeResult()
{
    if (ui->serialRadioBtn->isChecked()) myArc->setArcType(QArc::SerialArc);
    else if (ui->parallelRadioBtn->isChecked()) myArc->setArcType(QArc::ParallelArc);
    else if (ui->terminateRadioBtn->isChecked()) myArc->setArcType(QArc::TerminateArc);

    QList<QArc* > arcList = myArc->startItem()->outArcs();
    if (myArc->priority() < ui->prioritySpnBox->value()) {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority()>myArc->priority() && arcList[i]->priority() <= ui->prioritySpnBox->value())
                arcList[i]->setPriority(arcList[i]->priority() - 1);
    } else {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority() < myArc->priority() && arcList[i]->priority() >= ui->prioritySpnBox->value())
                arcList[i]->setPriority(arcList[i]->priority() + 1);
    }
    myArc->setPriority(ui->prioritySpnBox->value());
    if (ui->predicateList->currentRow() > 0) {
        myArc->setPredicate(myPredicateList[ui->predicateList->currentRow()]);
        if (!globalPredicateList.contains(myArc->getPredicate()->name))
            globalPredicateList.append(myArc->getPredicate()->name);
        //TODO: сигнал о добавлении нового предиката
    } else
        myArc->setPredicate(NULL);

    return true;
}

void ArcPropertyDialog::on_predicateList_currentRowChanged(int currentRow)
{
    QString info("");
    if (currentRow > 0){
        const Predicate *predicate = myPredicateList[currentRow];
        info.append(tr("Name: ") + predicate->name + "\r\n");
        QString type;
        switch (predicate->type){
        case Predicate::InlineType:
            type = tr("inline");
            break;
        case Predicate::NormalType:
            type = tr("normal");
            break;
        }        info.append(tr("Type: ") + type + "\r\n");
        if (predicate->baseModule != NULL)
            info.append(tr("Base module: ") + predicate->baseModule->name + "\r\n");
    }
    ui->descriptionLbl->setText(info);
}

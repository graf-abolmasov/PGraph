#include <QtGui>

#include "../../src/editor/dialogs/arcpropertydialog.h"
#include "ui_arcpropertydialog.h"
#include "../../src/editor/qarc.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/editor/qtop.h"
#include "../../src/common/qgraphsettings.h"

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
    case Arc::SerialArc:
        ui->serialRadioBtn->setChecked(true);
        break;
    case Arc::ParallelArc:
        ui->parallelRadioBtn->setChecked(true);
        break;
    case Arc::TerminateArc:
        ui->terminateRadioBtn->setChecked(true);
        break;
    }
    bool isParallel = globalSettings->isParallel();
    ui->terminateRadioBtn->setEnabled(isParallel);
    ui->parallelRadioBtn->setEnabled(isParallel);

    ui->prioritySpnBox->setValue(arc->priority());
    ui->prioritySpnBox->setMaximum(arc->startItem()->outArcs().count());

    //Добавляем пустой предикат
    myPredicateList.append(NULL);
    ui->predicateList->addItem(tr("Нет"));
    //Загружаем предикаты из базы данных
    myPredicateList.append(globalDBManager->getPredicateList());
    for (int i = 1; i < myPredicateList.count(); i++)
        ui->predicateList->addItem(QString(myPredicateList[i]->extName).replace(QRegExp("(\r+|\n+)"), " "));

    const Predicate *arcPredicate = arc->predicate;
    int idx = myPredicateList.indexOf(arcPredicate);
    if (idx == -1) {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Дуга использует несуществующий предикат"), QMessageBox::Ok).exec();
        idx = 0;
        arc->predicate = NULL;
        //TODO: Cделать констрэйт в базе на удаление предикатов к которым есть привязка.
    }
    ui->predicateList->setCurrentRow(idx);
    ui->predicateList->setFocus(Qt::MouseFocusReason);

    myArc = arc;
}

QArc *ArcPropertyDialog::getResult() const
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
    if (ui->serialRadioBtn->isChecked()) myArc->setArcType(Arc::SerialArc);
    else if (ui->parallelRadioBtn->isChecked()) myArc->setArcType(Arc::ParallelArc);
    else if (ui->terminateRadioBtn->isChecked()) myArc->setArcType(Arc::TerminateArc);

    int newPriority = ui->prioritySpnBox->value();

    QList<QArc* > arcList = myArc->startItem()->outArcs();
    if (myArc->priority() < newPriority) {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority()>myArc->priority() && arcList[i]->priority() <= newPriority)
                arcList[i]->setPriority(arcList[i]->priority() - 1);
    } else {
        for (int i = 0; i < arcList.count(); i++)
            if (arcList[i]->priority() < myArc->priority() && arcList[i]->priority() >= newPriority)
                arcList[i]->setPriority(arcList[i]->priority() + 1);
    }
    myArc->setPriority(newPriority);
    const Predicate *newPredicate = NULL;
    if (ui->predicateList->currentRow() > 0) {
        newPredicate = myPredicateList[ui->predicateList->currentRow()];
        if (!globalPredicateList.contains(newPredicate->name))
            globalPredicateList.append(newPredicate->name);
    }

    myArc->predicate = newPredicate;
    return true;
}

void ArcPropertyDialog::on_predicateList_currentRowChanged(int currentRow)
{
    QString info("");
    if (currentRow > 0) {
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

#include "qactoreditor.h"
#include "ui_qactoreditor.h"
#include "databasemanager.h"
#include "qvariabledialog.h"

QActorEditor::QActorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
}

QActorEditor::~QActorEditor()
{
    delete ui;
}

void QActorEditor::changeEvent(QEvent *e)
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

QActorEditor::QActorEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QActorEditor)
{
    ui->setupUi(this);

    switch(mode){
    case Normal:
        ui->inlineWidget->setVisible(false);
        ui->normalWidget->setVisible(true);
        break;
    case Inline:
        ui->inlineWidget->setVisible(true);
        ui->normalWidget->setVisible(false);
        break;
    }
    myMode = mode;

}

void QActorEditor::prepareForm(Actor *actor)
{
    QList<Variable*> newVariableList;
    if (actor == NULL){
        myActor = new Actor("", "", myMode == Normal ? Actor::normalType : Actor::inlineType, "", newVariableList);
    } else {
        myActor = actor;
        ui->baseModuleList->setEnabled(false);
    }
    //заполняем форму
    switch(myMode){
    case Normal:
        ui->actorNameEdt->setText(myActor->extName);
        break;
    case Inline:
        ui->iactorNameEdt->setText(myActor->extName);
        break;
    }

    globalDBManager->getRegisteredModules(myModuleList);
    foreach(BaseModule* baseModule, myModuleList){
        ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
        if (myActor->baseModule == baseModule->name){
            ui->baseModuleList->blockSignals(true);
            ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
            for (int i = 0; i < myModuleList.at(ui->baseModuleList->currentRow())->parameterList.count(); i++){
                QStringList parameter = myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(i).split(";;");
                ui->paramsNormalTable->insertRow(i);
                ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
                ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
                ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myActor->variableList.at(i) == NULL ? "N/A" : myActor->variableList.at(i)->name));
            }
            ui->baseModuleList->blockSignals(false);
         }
    }
}

Actor* QActorEditor::getResult()
{
    switch(myMode){
    case Normal:
        myActor->extName = ui->actorNameEdt->text();
        myActor->baseModule = ui->baseModuleList->currentItem()->text();
        break;
    case Inline:
        myActor->extName = ui->iactorNameEdt->text();
        break;
    }
    return myActor;
}

void QActorEditor::updateInterface()
{

}

void QActorEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList.at(currentRow)->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);
    myActor->variableList.clear();
    for (int i = 0; i < myModuleList.at(currentRow)->parameterList.count(); i++){
        myActor->variableList.append(NULL);
        QStringList parameter = myModuleList.at(currentRow)->parameterList.at(i).split(";;");
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem("N/A"));
    }
}

void QActorEditor::on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1) {
        ui->paramsNormalTable->setCellWidget(previousRow, 2, NULL);
        delete varWidget;
    }
    if (currentRow == -1) return;
    varWidget = new QWidget(ui->paramsNormalTable);
    varLayout = new QHBoxLayout(varWidget);
    varEditBtn = new QToolButton(varWidget);
    varLayout->setContentsMargins(0, 0, 0, 0);
    varLayout->addStretch();
    varLayout->addWidget(varEditBtn);
    varEditBtn->setText("...");

    connect(varEditBtn, SIGNAL(clicked()), this, SLOT(on_varEditBtn_clicked()));
    ui->paramsNormalTable->setCellWidget(currentRow, 2, varWidget);

    ui->descriptionPOLbl_2->setText("");
    if (myActor->variableList.at(currentRow) != NULL) {
        ui->descriptionPOLbl_2->setText(myActor->variableList.at(currentRow)->comment);
    }
    ui->descriptionBMLbl_2->setText(myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(currentRow).split(";;").at(3));
}

void QActorEditor::on_varEditBtn_clicked()
{
    QVariableDialog dialog;
    dialog.prepareForm();
    if (dialog.exec()){
        myActor->variableList.replace(ui->paramsNormalTable->currentRow(), dialog.getVariable());
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(dialog.getVariable()->name);
        ui->descriptionPOLbl_2->setText(dialog.getVariable()->comment);
    }
}

void QActorEditor::on_buttonBox_accepted()
{

}

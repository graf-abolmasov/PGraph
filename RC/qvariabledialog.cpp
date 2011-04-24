#include <QtCore>
#include <QtGui>

#include "qvariabledialog.h"
#include "ui_qvariabledialog.h"

#include "qvariableeditor.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "variable.h"
#include "datatype.h"

QVariableDialog::QVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableDialog)
{
    ui->setupUi(this);
    ui->variablesTable->setColumnWidth(0, 80);
    ui->variablesTable->setColumnWidth(1, 70);
    ui->variablesTable->setColumnWidth(2, 145);
    ui->variablesTable->setColumnWidth(3, 145);
}

QVariableDialog::~QVariableDialog()
{
    delete ui;
}

void QVariableDialog::changeEvent(QEvent *e)
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

void QVariableDialog::prepareForm(QString filter)
{
    /*тут мы получаем данные из базы*/
    if (!globalDBManager->getVariableList(myVariableList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список переменных.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);
    if (!globalDBManager->getDataTypeList(myTypeList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список типов данных.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);;
    /*заполняем форму*/
    if (filter != "")
        foreach (Variable* var, myVariableList)
            if (var->name != filter)
                myVariableList.removeAt(myVariableList.indexOf(var));
    ui->variablesTable->blockSignals(true);
    foreach (Variable* var, myVariableList){
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(var->name));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(var->type));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,2,new QTableWidgetItem(var->initValue.toString()));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,3,new QTableWidgetItem(var->comment));
    }
    ui->variablesTable->blockSignals(false);
}

void QVariableDialog::on_newButton_clicked()
{
    QVariableEditor editor;
    editor.prepareForm(NULL);
    if (editor.exec()){
        Variable* newVar = editor.getResult();
        if (newVar != NULL) {
            myVariableList.append(newVar);
            ui->variablesTable->blockSignals(true);
            ui->variablesTable->insertRow(ui->variablesTable->rowCount());
            ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(newVar->name));
            ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(newVar->type));
            ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,2,new QTableWidgetItem(newVar->initValue.toString()));
            ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,3,new QTableWidgetItem(newVar->comment));
            ui->variablesTable->blockSignals(false);
        }
    }
}

void QVariableDialog::on_editButton_clicked()
{
    QVariableEditor editor;
    Variable* var;
    if (ui->variablesTable->selectedRanges().count() > 0) {
        int idx = ui->variablesTable->selectedRanges().first().topRow();
        if (idx == -1) return;
        var = myVariableList.at(idx);
        editor.prepareForm(var);
        if (editor.exec()){
            Variable* newVar = editor.getResult();
            if (newVar != NULL) {
                myVariableList.replace(idx, newVar);
                ui->variablesTable->blockSignals(true);
                ui->variablesTable->setItem(idx,0,new QTableWidgetItem(newVar->name));
                ui->variablesTable->setItem(idx,1,new QTableWidgetItem(newVar->type));
                ui->variablesTable->setItem(idx,2,new QTableWidgetItem(newVar->initValue.toString()));
                ui->variablesTable->setItem(idx,3,new QTableWidgetItem(newVar->comment));
                ui->variablesTable->blockSignals(false);
            }
        }
    }
}

void QVariableDialog::on_deleteButton_clicked()
{
    if (ui->variablesTable->selectedRanges().count() > 0) {
        int idx = ui->variablesTable->selectedRanges().first().topRow();
        if (idx == -1) return;
        delete myVariableList.at(idx);
        myVariableList.removeAt(idx);
        ui->variablesTable->blockSignals(true);
        ui->variablesTable->removeRow(idx);
        ui->variablesTable->blockSignals(false);
    }
}

void QVariableDialog::on_buttonBox_accepted()
{
    int idx = ui->variablesTable->currentRow();
    ui->variablesTable->setCurrentCell(-1, -1);
    if  (idx != -1)
        myVar = myVariableList.at(idx);
    if (!globalDBManager->saveVariableList(myVariableList))
        QMessageBox::warning(this, tr("Ошибка"), tr("Произошла ошибка при сохранении в информационный фонд."), QMessageBox::Ok);
}

Variable* QVariableDialog::getVariable()
{
    return myVar;
}

void QVariableDialog::on_variablesTable_cellChanged(int row, int column)
{
    if (row == -1) return;
    switch (column) {
    case 0:
        myVariableList.at(row)->name = ui->variablesTable->item(row, column)->text();
        break;
    case 1:
        myVariableList.at(row)->type = ui->variablesTable->item(row, column)->text();
        break;
    case 2:
        myVariableList.at(row)->initValue = ui->variablesTable->item(row, column)->text();
        break;
    case 3:
        myVariableList.at(row)->comment = ui->variablesTable->item(row, column)->text();
        break;
    }
}

void QVariableDialog::on_variablesTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1){
        if (previousColumn == 1){
            ui->variablesTable->setCellWidget(previousRow, 1, NULL);
            ui->variablesTable->item(previousRow, 1)->setText(varTypeCmbBox->currentText());
            delete varTypeCmbBox;
        }
    }

    if (currentColumn == 1){
        varTypeCmbBox = new QComboBox(ui->variablesTable);
        foreach (DataType* type, myTypeList)
            varTypeCmbBox->addItem(type->name);

        int idx = varTypeCmbBox->findText(ui->variablesTable->item(currentRow, 1)->text());
        varTypeCmbBox->setCurrentIndex(idx);

        ui->variablesTable->setCellWidget(currentRow, 1, varTypeCmbBox);
    }
}

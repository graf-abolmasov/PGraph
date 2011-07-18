#include <QtCore>
#include <QtGui>

#include "qvariabledialog.h"
#include "ui_qvariabledialog.h"

#include "qvariableeditor.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "variable.h"
#include "datatype.h"

QVariableDialog::QVariableDialog(const bool &editOnDblClick, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableDialog)
{
    ui->setupUi(this);
    ui->variablesTable->setColumnWidth(0, 80);
    ui->variablesTable->setColumnWidth(1, 70);
    ui->variablesTable->setColumnWidth(2, 145);
    ui->variablesTable->setColumnWidth(3, 145);

    myVariable = NULL;

    ui->variablesTable->setEditTriggers(editOnDblClick ? QAbstractItemView::DoubleClicked : QAbstractItemView::NoEditTriggers);
    if (editOnDblClick)
        connect(ui->variablesTable, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(variablesTableCurrentCellChanged(int,int,int,int)));
    else
        connect(ui->variablesTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDblClick(int,int)));

    /*тут мы получаем данные из базы*/
    myVariableList = globalDBManager->getVariableList();
    myTypeList = globalDBManager->getDataTypeList();

    /*заполняем форму*/
    ui->variablesTable->blockSignals(true);
    foreach (const Variable *var, myVariableList){
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(var->name));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(var->type->name));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,2,new QTableWidgetItem(var->initValue));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,3,new QTableWidgetItem(var->comment));
    }
    ui->variablesTable->blockSignals(false);
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

void QVariableDialog::on_newButton_clicked()
{
    QVariableEditor *editor = QVariableEditor::getCreator();
    if (editor->exec()){
        const Variable *newVar = editor->getResult();
        myVariableList.append(newVar);
        ui->variablesTable->blockSignals(true);
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(newVar->name));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(newVar->type->name));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,2,new QTableWidgetItem(newVar->initValue));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,3,new QTableWidgetItem(newVar->comment));
        ui->variablesTable->blockSignals(false);
    }
    delete editor;
}

void QVariableDialog::on_editButton_clicked()
{
    if (ui->variablesTable->selectedRanges().count() == 0)
        return;
    const int idx = ui->variablesTable->selectedRanges().first().topRow();
    if (idx == -1)
        return;
    const Variable *var = myVariableList[idx];
    QVariableEditor *editor = QVariableEditor::getEditor(var);
    if (editor->exec()){
        const Variable *newVar = editor->getResult();
        myVariableList.replace(idx, newVar);
        ui->variablesTable->blockSignals(true);
        ui->variablesTable->setItem(idx,0,new QTableWidgetItem(newVar->name));
        ui->variablesTable->setItem(idx,1,new QTableWidgetItem(newVar->type->name));
        ui->variablesTable->setItem(idx,2,new QTableWidgetItem(newVar->initValue));
        ui->variablesTable->setItem(idx,3,new QTableWidgetItem(newVar->comment));
        ui->variablesTable->blockSignals(false);
    }
    delete editor;
}

void QVariableDialog::on_deleteButton_clicked()
{
    if (ui->variablesTable->selectedRanges().count() == 0)
        return;
    const int idx = ui->variablesTable->selectedRanges().first().topRow();
    if (idx == -1)
        return;
    delete myVariableList[idx];
    myVariableList.removeAt(idx);
    ui->variablesTable->blockSignals(true);
    ui->variablesTable->removeRow(idx);
    ui->variablesTable->blockSignals(false);
}

void QVariableDialog::on_buttonBox_accepted()
{
    const int idx = ui->variablesTable->currentRow();
    ui->variablesTable->setCurrentCell(-1, -1);
    if  (idx != -1)
        myVariable = myVariableList[idx];
    globalDBManager->setVariableList(myVariableList);
    accept();
}

const Variable *QVariableDialog::getVariable() const
{
    return myVariable;
}

void QVariableDialog::on_variablesTable_cellChanged(int row, int column)
{
    if (row == -1)
        return;
    Variable *result = const_cast<Variable *>(myVariableList[row]);
    switch (column) {
    case 0:
        result->name = ui->variablesTable->item(row, column)->text();
        break;
    case 1:
        result->type = globalDBManager->getDataType(ui->variablesTable->item(row, column)->text());
        break;
    case 2:
        result->initValue = ui->variablesTable->item(row, column)->text();
        break;
    case 3:
        result->comment = ui->variablesTable->item(row, column)->text();
        break;
    }
    myVariableList.replace(row, result);
}

void QVariableDialog::variablesTableCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1) {
        if (previousColumn == 1){
            ui->variablesTable->setCellWidget(previousRow, 1, NULL);
            ui->variablesTable->item(previousRow, 1)->setText(varTypeCmbBox->currentText());
            delete varTypeCmbBox;
        }
    }

    if (currentColumn == 1) {
        varTypeCmbBox = new QComboBox(ui->variablesTable);
        foreach (const DataType *type, myTypeList)
            varTypeCmbBox->addItem(type->name);

        int idx = varTypeCmbBox->findText(ui->variablesTable->item(currentRow, 1)->text());
        varTypeCmbBox->setCurrentIndex(idx);

        ui->variablesTable->setCellWidget(currentRow, 1, varTypeCmbBox);
    }
}

void QVariableDialog::tableDblClick(int row, int)
{
    myVariable = myVariableList[row];
    on_buttonBox_accepted();
}

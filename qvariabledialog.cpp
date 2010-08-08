#include "qvariabledialog.h"
#include "ui_qvariabledialog.h"
#include "qvariableeditor.h"
#include "databasemanager.h"

QVariableDialog::QVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableDialog)
{
    ui->setupUi(this);
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
    globalDBManager->getVariableList(myVariableList);
    /*заполняем форму*/
    if (filter != "")
        foreach (Variable* var, myVariableList)
            if (var->name != filter)
                myVariableList.removeAt(myVariableList.indexOf(var));
        
    foreach (Variable* var, myVariableList){
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(var->name,QTableWidgetItem::Type));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(var->type,0));
    }
}

void QVariableDialog::on_newButton_clicked()
{
    QVariableEditor *editor = new QVariableEditor();
    editor->prepareForm(NULL);
    if (editor->exec()){
        Variable* newVar = editor->getResult();
        myVariableList.append(newVar);
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(newVar->name, QTableWidgetItem::Type));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(newVar->type,0));
    }
    delete editor;
}

void QVariableDialog::on_editButton_clicked()
{
    QVariableEditor *editor = new QVariableEditor();
    Variable* var;
    if (ui->variablesTable->selectedRanges().count() > 0) {
        int idx = ui->variablesTable->selectedRanges().first().topRow();
        if (idx == -1) return;
        var = myVariableList.at(idx);
        editor->prepareForm(var);
        if (editor->exec()){
            Variable* newVar = editor->getResult();
            myVariableList.replace(idx, newVar);
            ui->variablesTable->setItem(idx,0,new QTableWidgetItem(newVar->name, QTableWidgetItem::Type));
            ui->variablesTable->setItem(idx,1,new QTableWidgetItem(newVar->type,0));
        }
    }
    delete editor;
}

void QVariableDialog::on_deleteButton_clicked()
{
    if (ui->variablesTable->selectedRanges().count() > 0) {
        int idx = ui->variablesTable->selectedRanges().first().topRow();
        if (idx == -1) return;
        delete myVariableList.at(idx);
        myVariableList.removeAt(idx);
        ui->variablesTable->removeRow(idx);
    }
}

void QVariableDialog::on_variablesTable_doubleClicked(QModelIndex index)
{
    on_editButton_clicked();
}

void QVariableDialog::on_buttonBox_accepted()
{
    globalDBManager->saveVariableList(myVariableList);
}

Variable* QVariableDialog::getVariable()
{
    Variable* var = myVariableList.at(ui->variablesTable->currentRow());
    return var;
}

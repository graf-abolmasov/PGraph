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

void QVariableDialog::prepareForm()
{
    /*тут мы получаем данные из базы*/
    globalDBManager->getVariableList(varList);
    /*заполняем форму*/
    foreach (Variable* var, varList){
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(var->name,QTableWidgetItem::Type));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(var->type->name,0));
    }
}

void QVariableDialog::on_newButton_clicked()
{
    QVariableEditor *editor = new QVariableEditor();
    editor->prepareForm(NULL);
    if (editor->exec()){
        Variable* newVar = editor->getResult();
        varList.append(newVar);
        ui->variablesTable->insertRow(ui->variablesTable->rowCount());
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,0,new QTableWidgetItem(newVar->name, QTableWidgetItem::Type));
        ui->variablesTable->setItem(ui->variablesTable->rowCount()-1,1,new QTableWidgetItem(newVar->type->name,0));
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
        var = varList.at(idx);
        editor->prepareForm(var);
        if (editor->exec()){
            Variable* newVar = editor->getResult();
            varList.replace(idx, newVar);
            ui->variablesTable->setItem(idx,0,new QTableWidgetItem(newVar->name, QTableWidgetItem::Type));
            ui->variablesTable->setItem(idx,1,new QTableWidgetItem(newVar->type->name,0));
        }
    }
    delete editor;
}

void QVariableDialog::on_deleteButton_clicked()
{
    if (ui->variablesTable->selectedRanges().count() > 0) {
        int idx = ui->variablesTable->selectedRanges().first().topRow();
        if (idx == -1) return;
        delete varList.at(idx);
        varList.removeAt(idx);
        ui->variablesTable->removeRow(idx);
    }
}

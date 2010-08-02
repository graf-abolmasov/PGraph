#include "qdatatypedialog.h"
#include "ui_qdatatypedialog.h"
#include "qdatatypeeditor.h"
#include "databasemanager.h"
#include <QtCore>

QDataTypeDialog::QDataTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeDialog)
{
    ui->setupUi(this);
}

QDataTypeDialog::~QDataTypeDialog()
{
    delete ui;
}

void QDataTypeDialog::changeEvent(QEvent *e)
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

/*!
        Обработка нажатия на кнопку Создать
*/
void QDataTypeDialog::on_pushButton_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor();
    editor->prepareForm(NULL);
    if (editor->exec()){
        DataType* newDataType = editor->getResult();
        typeList.append(newDataType);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(newDataType->name,QTableWidgetItem::Type));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(newDataType->typedefStr,0));
    }
    delete editor;
}

/*!
        Обработка нажатия на кнопку Редактировать
*/
void QDataTypeDialog::on_pushButton_2_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor();
    DataType* dataType;
    if (ui->tableWidget->selectedRanges().count() > 0) {
        int idx = ui->tableWidget->selectedRanges().first().topRow();
        if (idx == -1) return;
        dataType = typeList.at(idx);
        editor->prepareForm(dataType);
        if (editor->exec()){
            DataType* newDataType = editor->getResult();
            typeList.replace(idx, newDataType);
            ui->tableWidget->setItem(idx,0,new QTableWidgetItem(newDataType->name, QTableWidgetItem::Type));
            ui->tableWidget->setItem(idx,1,new QTableWidgetItem(newDataType->typedefStr,0));
        }
    }
    delete editor;
}

void QDataTypeDialog::prepareForm()
{
    //тут писать получение из базы
    globalDBManager->getDataTypeList(typeList);
    //тут писать заполнение формы
    foreach (DataType* type, typeList){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(type->name,QTableWidgetItem::Type));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(type->typedefStr,0));
    }
}

/*!
        Обработка нажатия на кнопку Удалить
*/
void QDataTypeDialog::on_pushButton_3_clicked()
{
    if (ui->tableWidget->selectedRanges().count() > 0) {
        int idx = ui->tableWidget->selectedRanges().first().topRow();
        if (idx == -1) return;
        delete typeList.at(idx);
        typeList.removeAt(idx);
        ui->tableWidget->removeRow(idx);
    }
}

void QDataTypeDialog::on_buttonBox_accepted()
{
    //тут писать сохранение в базу
    globalDBManager->saveDataTypeList(typeList);
}

void QDataTypeDialog::on_tableWidget_doubleClicked(QModelIndex index)
{
    on_pushButton_2_clicked();
}

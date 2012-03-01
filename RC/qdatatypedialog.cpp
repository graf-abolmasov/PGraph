#include <QtCore>
#include <QtGui>

#include "qdatatypedialog.h"
#include "ui_qdatatypedialog.h"
#include "qdatatypeeditor.h"
#include "databasemanager.h"
#include "globalvariables.h"
#include "datatype.h"

QDataTypeDialog::QDataTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeDialog)
{
    ui->setupUi(this);
    prepareForm();
}

QDataTypeDialog::~QDataTypeDialog()
{
    delete ui;
}

QDataTypeDialog *QDataTypeDialog::getDialog()
{
    return new QDataTypeDialog();
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
    QDataTypeEditor *editor = QDataTypeEditor::getCreator(QDataTypeEditor::Simple);
    if (editor->exec()){
        const DataType *newDataType = editor->getResult();
        myTypeList.append(newDataType);
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
    if (ui->tableWidget->selectedRanges().count() > 0) {
        int idx = ui->tableWidget->selectedRanges().first().topRow();
        if (idx == -1) return;
        const DataType *dataType = myTypeList[idx];
        QDataTypeEditor *editor = QDataTypeEditor::getEditor(dataType);
        if (editor->exec()){
            const DataType *newDataType = editor->getResult();
            myTypeList.replace(idx, newDataType);
            ui->tableWidget->setItem(idx,0,new QTableWidgetItem(newDataType->name, QTableWidgetItem::Type));
            ui->tableWidget->setItem(idx,1,new QTableWidgetItem(newDataType->typedefStr,0));
        }
        delete editor;
    }
}

void QDataTypeDialog::prepareForm()
{
    //тут писать получение из базы
    try {
        myTypeList = globalDBManager->getDataTypeList();
    } catch (QString s) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), s, QMessageBox::Ok);
    }
    //тут писать заполнение формы
    foreach (const DataType *type, myTypeList){
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
        if (idx == -1)
            return;
        myTypeList.removeAt(idx);
        ui->tableWidget->removeRow(idx);
    }
}

void QDataTypeDialog::on_buttonBox_accepted()
{
    try {
        globalDBManager->setDatatypeList(myTypeList);
    } catch (QString s) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), s, QMessageBox::Ok);
    }
}

void QDataTypeDialog::on_tableWidget_doubleClicked(QModelIndex)
{
    on_pushButton_2_clicked();
}

/*!
  Нажатие на кнопку "создать массив"
  */
void QDataTypeDialog::on_pushButton_4_clicked()
{
    QDataTypeEditor *editor = QDataTypeEditor::getCreator(QDataTypeEditor::Array);
    if (editor->exec()){
        const DataType *newDataType = editor->getResult();
        myTypeList.append(newDataType);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(newDataType->name, QTableWidgetItem::Type));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(newDataType->typedefStr,0));
    }
    delete editor;
}

/*!
  Нажатие на кнопку "создать структуру"
  */
void QDataTypeDialog::on_pushButton_5_clicked()
{
    QDataTypeEditor *editor = QDataTypeEditor::getCreator(QDataTypeEditor::Struct);
    if (editor->exec()){
        const DataType *newDataType = editor->getResult();
        myTypeList.append(newDataType);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(newDataType->name,QTableWidgetItem::Type));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(newDataType->typedefStr,0));
    }
    delete editor;
}

void QDataTypeDialog::on_moveUp_clicked()
{
    if (ui->tableWidget->currentRow() == 0)
        return;

    const int currRow = ui->tableWidget->currentRow();
    const int prevRow = currRow - 1;

    swap(currRow, prevRow);
    ui->tableWidget->setCurrentCell(prevRow, 0);
}

void QDataTypeDialog::swap(int i, int j)
{
    const QString prevRowText0 = ui->tableWidget->item(i, 0)->text();
    const QString prevRowText1 = ui->tableWidget->item(i, 1)->text();

    ui->tableWidget->item(i, 0)->setText(ui->tableWidget->item(j, 0)->text());
    ui->tableWidget->item(i, 1)->setText(ui->tableWidget->item(j, 1)->text());

    ui->tableWidget->item(j, 0)->setText(prevRowText0);
    ui->tableWidget->item(j, 1)->setText(prevRowText1);

    this->myTypeList.swap(j, i);
}


void QDataTypeDialog::on_moveDown_clicked()
{
    if (ui->tableWidget->currentRow() == ui->tableWidget->rowCount()-1)
        return;

    const int currRow = ui->tableWidget->currentRow();
    const int nextRow = currRow + 1;

    swap(currRow, nextRow);
    ui->tableWidget->setCurrentCell(nextRow, 0);
}

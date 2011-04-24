#include <QtGui>
#include <QtCore>

#include "qdatatypeeditor.h"
#include "ui_qdatatypeeditor.h"
#include "datatype.h"
#include "globalvariables.h"

QDataTypeEditor::QDataTypeEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeEditor)
{
    ui->setupUi(this);
    myMode = Simple;
    indexes << "i" << "j" << "k" << "l" << "m" << "p" << "q";
}

QDataTypeEditor::QDataTypeEditor(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeEditor)
{
    ui->setupUi(this);
    myMode = mode;
    indexes << "i" << "j" << "k" << "l" << "m" << "p" << "q";
}

QDataTypeEditor::~QDataTypeEditor()
{
    delete ui;
}

void QDataTypeEditor::changeEvent(QEvent *e)
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

void QDataTypeEditor::prepareForm(DataType *type)
{

    ui->ArrType->setVisible(false);
    ui->StructType->setVisible(false);
    ui->SimpleType->setVisible(false);

    //Подготавливаем справочник типов
    if (!globalDBManager->getDataTypeList(typeList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список типов данных.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);
    foreach (DataType* type, typeList)
        ui->arrItemTypeCmbBox->addItem(type->name, type->typedefStr);

    ui->structFieldsTable->blockSignals(true);
    if (type != NULL) {
        if (type->typedefStr.contains("struct")) {
            myMode = Struct;

            QRegExp r("(\\n(\\b.+\\b)\\s+(\\b.+\\b);)");
            r.setMinimal(true);

            int pos = 0;
            while ((pos = r.indexIn(type->typedefStr, pos)) != -1) {
                ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
                ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(r.cap(2)));
                ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(r.cap(3)));
                pos += r.matchedLength();
            }

            ui->structTypeNameEdt->setText(type->name);
            ui->StructType->setVisible(true);
            ui->structTypeNameEdt->setFocus();
        } else if (type->typedefStr.contains("[")) {
            myMode = Array;

            //Парсим размерность и количество элементов
            QRegExp r("(\\[(\\d+)\\])");
            r.setMinimal(true);
            QStringList::Iterator iter = indexes.begin();
            int pos = 0;
            while ((pos = r.indexIn(type->typedefStr, pos)) != -1 && iter != indexes.end()) {
                ui->arrItemsCountTable->insertRow(ui->arrItemsCountTable->rowCount());
                ui->arrItemsCountTable->setItem(ui->arrItemsCountTable->rowCount()-1, 0, new QTableWidgetItem(*(iter++)));
                ui->arrItemsCountTable->setItem(ui->arrItemsCountTable->rowCount()-1, 1, new QTableWidgetItem(r.cap(2)));
                pos += r.matchedLength();
            }
            ui->arrDimensionSpnBox->setValue(ui->arrItemsCountTable->rowCount());

            //Парсим тип элемента
            r.setPattern("(typedef\\s+(\\b.+\\b))");
            if (r.indexIn(type->typedefStr) != -1) {
                int idx = -1;
                for (int i = 0; i < typeList.count(); i++){
                    if (r.cap(2) == typeList.at(i)->name){
                        idx = i;
                        break;
                    }
                }
                ui->arrItemTypeCmbBox->setCurrentIndex(idx);
            }
            ui->arrTypeNameEdt->setText(type->name);
            QRegExp regExp("[A-Za-z][A-Za-z1-9]{0,31}");
            ui->arrTypeNameEdt->setValidator(new QRegExpValidator(regExp, this));
            enableOkButton();
            ui->ArrType->setVisible(true);
            ui->arrTypeNameEdt->setFocus();
        } else {
            myMode = Simple;
            ui->simpTypeNameEdt->setText(type->name);
            ui->simpTypedefTxtEdt->document()->setPlainText(type->typedefStr);
            QRegExp regExp("[A-Za-z][A-Za-z1-9]{0,31}");
            ui->simpTypeNameEdt->setValidator(new QRegExpValidator(regExp, this));
            enableOkButton();
            ui->SimpleType->setVisible(true);
            ui->simpTypeNameEdt->setFocus();
        }
    } else {
        ui->ArrType->setVisible(myMode==Array);
        ui->StructType->setVisible(myMode==Struct);
        ui->SimpleType->setVisible(myMode==Simple);
    }


    //Вставляем последнюю пустую строку в таблицу структуры
    ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
    ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(""));
    ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(""));

    //Вставляем строку в таблицу размерност массива
    if (ui->arrItemsCountTable->rowCount() == 0) {
        ui->arrDimensionSpnBox->setValue(1);
    }
    //Включаем сигналы
    ui->structFieldsTable->blockSignals(false);

    myDataType = type;
}

DataType* QDataTypeEditor::getResult()
{
    return myDataType;
}

void QDataTypeEditor::on_buttonBox_accepted()
{
    switch (myMode) {
    case Simple:
        if (!ui->simpTypeNameEdt->text().isEmpty()) {
            if (myDataType == NULL) {
                myDataType = new DataType(ui->simpTypeNameEdt->text(), ui->simpTypedefTxtEdt->document()->toPlainText());
            } else {
                myDataType->name = ui->simpTypeNameEdt->text();
                myDataType->typedefStr = ui->simpTypedefTxtEdt->document()->toPlainText();
            }
        }
        else
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
        break;
    case Array:
        if (!ui->arrTypeNameEdt->text().isEmpty()) {
            QString typedefStr("typedef ");
            typedefStr.append(ui->arrItemTypeCmbBox->currentText());
            typedefStr.append(" ");
            typedefStr.append(ui->arrTypeNameEdt->text());
            for (int i = 0; i < ui->arrItemsCountTable->rowCount(); i++){
                typedefStr.append("[");
                typedefStr.append(ui->arrItemsCountTable->item(i, 1)->text());
                typedefStr.append("]");
            }
            typedefStr.append(";");
            if (myDataType == NULL) {
                myDataType = new DataType(ui->arrTypeNameEdt->text(), typedefStr);
            } else {
                myDataType->name = ui->arrTypeNameEdt->text();
                myDataType->typedefStr = typedefStr;
            }
        }
        else
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
        break;
    case Struct:
        if (!ui->structTypeNameEdt->text().isEmpty()) {
            ui->structFieldsTable->setCurrentCell(-1, -1);

            QString typedefStr(tr("struct {\n"));
            for (int i = 0; i < ui->structFieldsTable->rowCount(); i++){
                if (!ui->structFieldsTable->item(i, 0)->text().isEmpty() &&
                        !ui->structFieldsTable->item(i, 1)->text().isEmpty()) {
                    typedefStr.append(ui->structFieldsTable->item(i, 0)->text());
                    typedefStr.append(" ");
                    typedefStr.append(ui->structFieldsTable->item(i, 1)->text());
                    typedefStr.append(";\n");
                }
            }
            typedefStr.append("} ");
            typedefStr.append(ui->structTypeNameEdt->text());
            typedefStr.append(";");

            if (myDataType == NULL) {
                myDataType = new DataType(ui->structTypeNameEdt->text(), typedefStr);
            } else {
                myDataType->name = ui->structTypeNameEdt->text();
                myDataType->typedefStr = typedefStr;
            }
        }
        else
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
        break;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->structTypeNameEdt->hasAcceptableInput());
        break;
    }


}

void QDataTypeEditor::enableOkButton()
{
    switch (myMode) {
    case Simple:
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->simpTypeNameEdt->hasAcceptableInput());
        break;
    case Array:
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->arrTypeNameEdt->hasAcceptableInput());
        break;
    case Struct:
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->structTypeNameEdt->hasAcceptableInput());
        break;
    }
}

void QDataTypeEditor::on_arrDimensionSpnBox_valueChanged(int v)
{
    ui->arrItemsCountTable->setRowCount(v);
    QStringList::Iterator iter = indexes.begin();
    for (int i = 0; i < ui->arrItemsCountTable->rowCount(); i++) {
        ui->arrItemsCountTable->setItem(i, 0, new QTableWidgetItem(*(iter++)));
        if (ui->arrItemsCountTable->item(i, 1) == NULL)
            ui->arrItemsCountTable->setItem(i, 1, new QTableWidgetItem("1"));
    }
}

void QDataTypeEditor::on_structFieldsTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1){
        if (previousColumn == 0){
            ui->structFieldsTable->setCellWidget(previousRow, 0, NULL);
            ui->structFieldsTable->item(previousRow, 0)->setText(varTypeCmbBox->currentText());
            delete varTypeCmbBox;
        }
        if (previousColumn == 1){
            ui->structFieldsTable->setCellWidget(previousRow, 1, NULL);
            ui->structFieldsTable->item(previousRow, 1)->setText(varNameEdt->text());
            delete varNameEdt;
        }
    }

    if (currentColumn == 0){
        varTypeCmbBox = new QComboBox(ui->structFieldsTable);
        foreach (DataType* type, typeList)
            varTypeCmbBox->addItem(type->name);

        int idx = varTypeCmbBox->findText(ui->structFieldsTable->item(currentRow, 0)->text());
        varTypeCmbBox->setCurrentIndex(idx);

        ui->structFieldsTable->setCellWidget(currentRow, 0, varTypeCmbBox);
    }

    if (currentColumn == 1){
        varNameEdt = new QLineEdit(ui->structFieldsTable);
        varNameEdt->setText(ui->structFieldsTable->item(currentRow, 1)->text());

        QRegExp regExp("[A-Za-z][A-Za-z1-9]{0,31}");
        varNameEdt->setValidator(new QRegExpValidator(regExp, this));
        ui->structFieldsTable->setCellWidget(currentRow, 1, varNameEdt);
    }
}

void QDataTypeEditor::on_structFieldsTable_cellChanged(int, int)
{
    if ((ui->structFieldsTable->item(ui->structFieldsTable->rowCount()-1, 0)->text() != "") &&
        (ui->structFieldsTable->item(ui->structFieldsTable->rowCount()-1, 1)->text() != "")) {
        //Вставляем последнюю пустую строку
        ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
        ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(""));
        ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(""));
    }
}

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include "qdatatypeeditor.h"
#include "ui_qdatatypeeditor.h"
#include "../../src/common/VO/datatype.h"
#include "../../src/common/globalvariables.h"

QDataTypeEditor *QDataTypeEditor::getCreator(Mode mode)
{
    return new QDataTypeEditor(mode);
}

QDataTypeEditor *QDataTypeEditor::getEditor(const DataType *type)
{
    return new QDataTypeEditor(type);
}

QDataTypeEditor::QDataTypeEditor(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeEditor),
    myMode(mode)
{   
    ui->setupUi(this);
    indexes << "i" << "j" << "k" << "l" << "m" << "p" << "q";
    myDataType = NULL;
    result = NULL;

    //Подготавливаем справочник типов
    myTypeList = globalDBManager->getDataTypeList();
    foreach (const DataType *type, myTypeList)
        ui->arrItemTypeCmbBox->addItem(type->name, type->typedefStr);
    ui->structFieldsTable->blockSignals(true);

    //Вставляем последнюю пустую строку в таблицу структуры
    ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
    ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(""));
    ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(""));
    //Включаем сигналы
    ui->structFieldsTable->blockSignals(false);

    //Вставляем строку в таблицу размерност массива
    ui->arrDimensionSpnBox->setValue(1);

    ui->ArrType->setVisible(myMode == Array);
    ui->StructType->setVisible(myMode == Struct);
    ui->SimpleType->setVisible(myMode == Simple);
}

QDataTypeEditor::QDataTypeEditor(const DataType *type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeEditor)
{
    ui->setupUi(this);
    indexes << "i" << "j" << "k" << "l" << "m" << "p" << "q";
    myDataType = type;
    result = NULL;

    //Подготавливаем справочник типов
    myTypeList = globalDBManager->getDataTypeList();
    foreach (const DataType *type, myTypeList)
        ui->arrItemTypeCmbBox->addItem(type->name, type->typedefStr);

    if (type->typedefStr.contains("struct")) {
        ui->structFieldsTable->blockSignals(true);
        myMode = Struct;

        QRegExp r("(\\n(\\b.+\\b)\\s+(\\b.+\\b);)");
        r.setMinimal(true);

        //Парсим поля структуры
        int pos = 0;
        while ((pos = r.indexIn(type->typedefStr, pos)) != -1) {
            ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
            ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(r.cap(2)));
            ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(r.cap(3)));
            pos += r.matchedLength();
        }

        ui->structTypeNameEdt->setText(type->name);
        ui->structTypeNameEdt->setFocus();
        //Вставляем последнюю пустую строку в таблицу структуры
        ui->structFieldsTable->insertRow(ui->structFieldsTable->rowCount());
        ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 0, new QTableWidgetItem(""));
        ui->structFieldsTable->setItem(ui->structFieldsTable->rowCount()-1, 1, new QTableWidgetItem(""));
        //Включаем сигналы
        ui->structFieldsTable->blockSignals(false);
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
            for (int i = 0; i < myTypeList.count(); i++){
                if (r.cap(2) == myTypeList[i]->name){
                    idx = i;
                    break;
                }
            }
            ui->arrItemTypeCmbBox->setCurrentIndex(idx);
        }
        ui->arrTypeNameEdt->setText(type->name);
        QRegExp regExp("[A-Za-z][A-Za-z0-9]{0,31}");
        ui->arrTypeNameEdt->setValidator(new QRegExpValidator(regExp, this));
        enableOkButton();
        ui->arrTypeNameEdt->setFocus();
        //Вставляем строку в таблицу размерности массива
        if (ui->arrItemsCountTable->rowCount() == 0) {
            ui->arrDimensionSpnBox->setValue(1);
        }
    } else {
        myMode = Simple;
        ui->simpTypeNameEdt->setText(type->name);
        ui->simpTypedefTxtEdt->document()->setPlainText(type->typedefStr);
        QRegExp regExp("[A-Za-z][A-Za-z0-9]{0,31}");
        ui->simpTypeNameEdt->setValidator(new QRegExpValidator(regExp, this));
        enableOkButton();
        ui->simpTypeNameEdt->setFocus();
    }

    const bool readOnly = !globalDBManager->findDataTypeUsage(type->name).isEmpty();

    ui->arrTypeNameEdt->setReadOnly(readOnly);
    ui->structTypeNameEdt->setReadOnly(readOnly);
    ui->simpTypeNameEdt->setReadOnly(readOnly);

    ui->ArrType->setVisible(myMode == Array);
    ui->StructType->setVisible(myMode == Struct);
    ui->SimpleType->setVisible(myMode == Simple);
}

QDataTypeEditor::~QDataTypeEditor()
{
    delete ui;
    if (varTypeCmbBox != NULL)
        delete varTypeCmbBox;
    if (varNameEdt != NULL)
        delete varNameEdt;
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

const DataType *QDataTypeEditor::getResult() const
{
    return result;
}

void QDataTypeEditor::on_buttonBox_accepted()
{
    makeResult();
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
            varTypeCmbBox = NULL;
        }
        if (previousColumn == 1){
            ui->structFieldsTable->setCellWidget(previousRow, 1, NULL);
            ui->structFieldsTable->item(previousRow, 1)->setText(varNameEdt->text());
            delete varNameEdt;
            varNameEdt = NULL;
        }
    }

    if (currentColumn == 0){
        varTypeCmbBox = new QComboBox(ui->structFieldsTable);
        foreach (const DataType *type, myTypeList)
            varTypeCmbBox->addItem(type->name);

        int idx = varTypeCmbBox->findText(ui->structFieldsTable->item(currentRow, 0)->text());
        varTypeCmbBox->setCurrentIndex(idx);

        ui->structFieldsTable->setCellWidget(currentRow, 0, varTypeCmbBox);
    }

    if (currentColumn == 1){
        varNameEdt = new QLineEdit(ui->structFieldsTable);
        varNameEdt->setText(ui->structFieldsTable->item(currentRow, 1)->text());

        QRegExp regExp("[A-Za-z][A-Za-z0-9]{0,31}");
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

void QDataTypeEditor::makeResult()
{
    QString typedefStr;
    QString typeStr;
    switch (myMode) {
    case Simple:
        if (ui->simpTypeNameEdt->text().isEmpty()) {
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
            return;
        }
        typeStr    =  ui->simpTypeNameEdt->text();
        typedefStr =  ui->simpTypedefTxtEdt->document()->toPlainText();
        break;
    case Array:
        if (ui->arrTypeNameEdt->text().isEmpty()) {
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
            return;
        }
        typeStr    =  ui->arrTypeNameEdt->text();
        typedefStr = tr("typedef ");
        typedefStr.append(ui->arrItemTypeCmbBox->currentText());
        typedefStr.append(" ");
        typedefStr.append(ui->arrTypeNameEdt->text());
        for (int i = 0; i < ui->arrItemsCountTable->rowCount(); i++){
            typedefStr.append("[");
            typedefStr.append(ui->arrItemsCountTable->item(i, 1)->text());
            typedefStr.append("]");
        }
        typedefStr.append(";");
        break;
    case Struct:
        if (ui->structTypeNameEdt->text().isEmpty()) {
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Укажите название типа.\n"),
                                  QMessageBox::Ok);
            return;
        }
        typeStr    =  ui->structTypeNameEdt->text();
        ui->structFieldsTable->setCurrentCell(-1, -1);
        typedefStr = (tr("struct {\n"));
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
        break;
    }

    if (myDataType == NULL) {
        result = new DataType(typeStr, typedefStr);
    } else {
        result = const_cast<DataType *>(myDataType);
        result->name = typeStr;
        result->typedefStr = typedefStr;
    }

}

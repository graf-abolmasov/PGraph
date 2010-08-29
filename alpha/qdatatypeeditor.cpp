#include "qdatatypeeditor.h"
#include "ui_qdatatypeeditor.h"

QDataTypeEditor::QDataTypeEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDataTypeEditor)
{
    ui->setupUi(this);
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
    if (type != NULL) {
        ui->typeNameEdt->setText(type->name);
        ui->typedefTxtEdt->document()->setPlainText(type->typedefStr);
        myDataType = type;
    } else
        myDataType = new DataType();
}

DataType* QDataTypeEditor::getResult()
{
    return myDataType;
}

void QDataTypeEditor::on_buttonBox_accepted()
{
    myDataType->name = ui->typeNameEdt->text();
    myDataType->typedefStr = ui->typedefTxtEdt->document()->toPlainText();
}

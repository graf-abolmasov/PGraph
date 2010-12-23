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
    }
    myDataType = type;
    QRegExp regExp("[A-Za-z][A-Za-z1-9]{0,31}");
    ui->typeNameEdt->setValidator(new QRegExpValidator(regExp, this));
    enableOkButton();
    ui->typeNameEdt->setFocus();
}

DataType* QDataTypeEditor::getResult()
{
    return myDataType;
}

void QDataTypeEditor::on_buttonBox_accepted()
{
    if (!ui->typeNameEdt->text().isEmpty()) {
        if (myDataType == NULL) {
            myDataType = new DataType(ui->typeNameEdt->text(), ui->typedefTxtEdt->document()->toPlainText());
        } else {
            myDataType->name = ui->typeNameEdt->text();
            myDataType->typedefStr = ui->typedefTxtEdt->document()->toPlainText();
        }
    }
    else
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Укажите название типа.\n"),
                              QMessageBox::Ok);
}

void QDataTypeEditor::enableOkButton()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->typeNameEdt->hasAcceptableInput());
}

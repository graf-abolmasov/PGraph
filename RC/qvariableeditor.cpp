#include <QtCore>
#include <QtGui>

#include "VO/variable.h"
#include "VO/datatype.h"
#include "qvariableeditor.h"
#include "ui_qvariableeditor.h"
#include "qdatatypeeditor.h"
#include "VO/datatype.h"
#include "databasemanager.h"
#include "globalvariables.h"

QVariableEditor *QVariableEditor::getCreator()
{
    return new QVariableEditor();
}

QVariableEditor *QVariableEditor::getEditor(const Variable *var)
{
    return new QVariableEditor(var);
}

QVariableEditor::QVariableEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableEditor)
{
    ui->setupUi(this);
    myVariable = NULL;
    QRegExp regExp("[_A-Za-z][_A-Za-z0-9]{0,31}");
    ui->nameEdt->setValidator(new QRegExpValidator(regExp, this));
    ui->nameEdt->setFocus();

    //Подготавливаем справочники
    myTypeList = globalDBManager->getDataTypeList();
    foreach (const DataType *type, myTypeList)
        ui->typeCmbBox->addItem(type->name, type->typedefStr);
}

QVariableEditor::QVariableEditor(const Variable *var, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableEditor)
{
    ui->setupUi(this);
    myVariable = var;
    QRegExp regExp("[_A-Za-z][_A-Za-z0-9]{0,31}");
    ui->nameEdt->setValidator(new QRegExpValidator(regExp, this));
    ui->nameEdt->setFocus();

    //Подготавливаем справочники
    myTypeList = globalDBManager->getDataTypeList();
    foreach (const DataType *type, myTypeList)
        ui->typeCmbBox->addItem(type->name, type->typedefStr);

    ui->nameEdt->setText(var->name);
    ui->commentTxtEdt->setPlainText(var->comment);
    ui->initValueEdt->setText(var->initValue);
    ui->typeCmbBox->setCurrentIndex(myTypeList.indexOf(var->type));
    enableOkButton();
}

QVariableEditor::~QVariableEditor()
{
    delete ui;
}

void QVariableEditor::changeEvent(QEvent *e)
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

void QVariableEditor::makeResult()
{
    if (myVariable == NULL)
        result = new Variable(ui->nameEdt->text(),
                                  ui->initValueEdt->text(),
                                  ui->commentTxtEdt->document()->toPlainText(),
                                  false,
                                  myTypeList[ui->typeCmbBox->currentIndex()]);
    else {
        result = const_cast<Variable *>(myVariable);
        result->name = ui->nameEdt->text();
        result->initValue = ui->initValueEdt->text();
        result->type = myTypeList[ui->typeCmbBox->currentIndex()];
        result->comment = ui->commentTxtEdt->document()->toPlainText();
        result->isGlobal = false;
    }
}

const Variable *QVariableEditor::getResult() const
{
    return result;
}

void QVariableEditor::enableOkButton()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled((ui->nameEdt->hasAcceptableInput()) &&
                                                            (ui->typeCmbBox->currentIndex() >= 0));

}

void QVariableEditor::on_buttonBox_accepted()
{
    makeResult();
}

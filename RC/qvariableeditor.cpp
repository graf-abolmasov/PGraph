#include "qvariableeditor.h"
#include "ui_qvariableeditor.h"
#include "qdatatypeeditor.h"
#include "datatype.h"
#include "databasemanager.h"
#include <QList>
#include "globalvariables.h"


QVariableEditor::QVariableEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableEditor)
{
    ui->setupUi(this);
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

void QVariableEditor::on_pushButton_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor();
    editor->prepareForm(NULL);
    if (editor->exec()){
        prepareForm(myVariable);
    }
}

void QVariableEditor::prepareForm(Variable *var)
{
    if (!globalDBManager->getDataTypeList(typeList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список типов данных.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);
    foreach (DataType* type, typeList)
        ui->typeCmbBox->addItem(type->name, type->typedefStr);

    if (var != NULL) {
        ui->nameEdt->setText(var->name);
        ui->commentTxtEdt->setPlainText(var->comment);
        ui->initValueEdt->setText(var->initValue.toString());
        int idx = -1;
        for (int i = 0; i < typeList.count(); i++){
            if (var->type == typeList.at(i)->name){
                idx = i;
                break;
            }
        }
        ui->typeCmbBox->setCurrentIndex(idx);
    }
    myVariable = var;
    ui->nameEdt->setFocus();
}

Variable* QVariableEditor::getResult()
{
    if (!ui->nameEdt->text().isEmpty() &&
        ui->typeCmbBox->currentIndex() >= 0) {
        if (myVariable == NULL) {
            myVariable = new Variable(ui->nameEdt->text(),
                                      typeList.at(ui->typeCmbBox->currentIndex())->name,
                                      ui->initValueEdt->text(),
                                      ui->commentTxtEdt->document()->toPlainText());
        } else {
            myVariable->name = ui->nameEdt->text();
            myVariable->type = typeList.at(ui->typeCmbBox->currentIndex())->name;
            myVariable->initValue = ui->initValueEdt->text();
            myVariable->comment = ui->commentTxtEdt->document()->toPlainText();
        }
    } else
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Укажите имя и тип переменной.\n"),
                              QMessageBox::Ok);
    return myVariable;
}

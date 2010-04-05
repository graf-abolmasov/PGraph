#include "qdatatypedialog.h"
#include "ui_qdatatypedialog.h"
#include "qdatatypeeditor.h"
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

void QDataTypeDialog::on_pushButton_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor(QDataTypeEditor::wizard);
    editor->prepareForm(NULL);
    if (editor->exec()){

    }
}

void QDataTypeDialog::on_pushButton_2_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor(QDataTypeEditor::editor);
    editor->prepareForm(NULL);
    if (editor->exec()){

    }
}

bool QDataTypeDialog::prepareForm()
{
   bool result = true;
   return result;
}


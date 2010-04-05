#include "qvariabledialog.h"
#include "ui_qvariabledialog.h"
#include "qvariableeditor.h"

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

}

void QVariableDialog::on_newButton_clicked()
{
    QVariableEditor *editor = new QVariableEditor(QVariableEditor::wizard);
    editor->prepareForm(NULL);
    if (editor->exec()){

    }
}

void QVariableDialog::on_editButton_clicked()
{
    QVariableEditor *editor = new QVariableEditor(QVariableEditor::editor);
    editor->prepareForm(NULL);
    if (editor->exec()){

    }
}

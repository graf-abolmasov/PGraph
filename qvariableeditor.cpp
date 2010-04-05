#include "qvariableeditor.h"
#include "ui_qvariableeditor.h"
#include "qdatatypeeditor.h"

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

QVariableEditor::QVariableEditor(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVariableEditor)
{
    ui->setupUi(this);

    myMode = mode;
    updateInterface();
}


void QVariableEditor::on_pushButton_clicked()
{
    QDataTypeEditor *editor = new QDataTypeEditor(QDataTypeEditor::wizard);
    editor->prepareForm();
    if (editor->exec()){

    }
}

void QVariableEditor::updateInterface()
{
    if (myMode = editor){
        ui->nameEdit->setEnabled(false);
        ui->typeEdit->setEnabled(false);
    }
}

bool QVariableEditor::prepareForm(Variable var)
{
    bool result = true;
    return result;
}

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

QDataTypeEditor::QDataTypeEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QDataTypeEditor)
{
    myMode = mode;
    ui->setupUi(this);
}

void QDataTypeEditor::updateInterface()
{
    ui->wizardWidget->setVisible(false);
    ui->editorWidget->setVisible(false);
    switch (myMode){
    case wizard:
        ui->wizardWidget->setVisible(true);
        break;
    case editor:
        ui->editorWidget->setVisible(true);
        break;
    }
}

bool QDataTypeEditor::prepareForm(DataType *type)
{
    bool result = true;
    return result;
}

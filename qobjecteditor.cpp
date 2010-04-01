#include "qobjecteditor.h"
#include "ui_qobjecteditor.h"

QObjectEditor::QObjectEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QObjectEditor)
{
    ui->setupUi(this);
    ui->tab->setTabText(0, "������");
    ui->tab->setTabText(1, "���������");
    ui->tab->setTabText(2, "I-������");
    ui->tab->setTabText(3, "I-���������");

    EditButtonGroup = new QButtonGroup(ui->buttonBox);
    newButton       = new QPushButton(EditButtonGroup);
    editButton      = new QPushButton(EditButtonGroup);
    deleteButton    = new QPushButton(EditButtonGroup);
    EditButtonGroup->addButton(newButton);
    EditButtonGroup->addButton(editButton);
    EditButtonGroup->addButton(deleteButton);
}

QObjectEditor::~QObjectEditor()
{
    delete ui;
}

void QObjectEditor::changeEvent(QEvent *e)
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

bool QObjectEditor::prepareForm()
{

}

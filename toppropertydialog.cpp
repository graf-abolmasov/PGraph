#include "toppropertydialog.h"
#include "ui_toppropertydialog.h"

TopPropertyDialog::TopPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopPropertyDialog)
{
    ui->setupUi(this);
}

TopPropertyDialog::~TopPropertyDialog()
{
    delete ui;
}

void TopPropertyDialog::changeEvent(QEvent *e)
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

void TopPropertyDialog::prepareForm(TTop* top){
    ui->spnBoxHeight->setProperty("value", top->boundingRect().height());
    ui->spnBoxWidth->setProperty("value", top->boundingRect().width());
}

int TopPropertyDialog::getWidth(){
    return ui->spnBoxWidth->value();
};

int TopPropertyDialog::getHeight(){
    return ui->spnBoxHeight->value();
};


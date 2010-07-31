#include "qpredicateeditor.h"
#include "ui_qpredicateeditor.h"

QPredicateEditor::QPredicateEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
}

QPredicateEditor::~QPredicateEditor()
{
    delete ui;
}

QPredicateEditor::QPredicateEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);

    switch(mode){
    case Normal:
        ui->inlineWidget->setVisible(false);
        ui->normalWidget->setVisible(true);
        break;
    case Inline:
        ui->inlineWidget->setVisible(true);
        ui->normalWidget->setVisible(false);
        break;
    }
}

void QPredicateEditor::changeEvent(QEvent *e)
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

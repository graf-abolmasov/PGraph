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
    myMode = mode;
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

void QPredicateEditor::prepareForm(Predicate *pred)
{
    if (pred == NULL)
        myPredicate = new Predicate("", "", myMode == Normal ? Predicate::normalType : Predicate::inlineType);
    else
        myPredicate = pred;
    //заполняем форму
    switch(myMode){
    case Normal:
        ui->predicateNameEdt->setText(myPredicate->extName);
        break;
    case Inline:
        ui->ipredicateNameEdt->setText(myPredicate->extName);
        break;
    }
}

Predicate* QPredicateEditor::getResult()
{
    switch(myMode){
    case Normal:
        myPredicate->extName = ui->predicateNameEdt->text();
        break;
    case Inline:
        myPredicate->extName = ui->ipredicateNameEdt->text();
        break;
    }
    return myPredicate;
}

#include "qpredicateeditor.h"
#include "ui_qpredicateeditor.h"

QPredicateEditor::QPredicateEditor(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
}

QPredicateEditor::QPredicateEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);

    step = 1;
    myMode = mode;
    updateInterface();
}

QPredicateEditor::~QPredicateEditor()
{
    delete ui;
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

void QPredicateEditor::updateInterface()
{
    ui->inlineEditorWidget->setVisible(false);
    ui->normalEditorWidget->setVisible(false);
    ui->normalWizardStep1Widget->setVisible(false);
    ui->normalWizardStep2Widget->setVisible(false);

    switch (myMode){
    case normalEditor:
        ui->normalEditorWidget->setVisible(true);
        ui->prevButton->setVisible(false);
        ui->nextButton->setVisible(false);
        break;
    case normalWizard:
        switch (step){
        case 1:
            ui->normalWizardStep1Widget->setVisible(true);
            ui->normalWizardStep2Widget->setVisible(false);
            ui->prevButton->setEnabled(false);
            ui->nextButton->setEnabled(true);
            break;
        case 2:
            ui->normalWizardStep1Widget->setVisible(false);
            ui->normalWizardStep2Widget->setVisible(true);
            ui->prevButton->setEnabled(true);
            ui->nextButton->setEnabled(false);
            ui->predNameNormalWizardStep2Label->setText(ui->predNameNormalWizardStep1Edit->text());
            break;
        }
        break;
    case inlineWizard:
    case inlineEditor:
            ui->inlineEditorWidget->setVisible(true);
            ui->prevButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            break;
        break;
    }
}

void QPredicateEditor::on_nextButton_clicked()
{
    step++;
    updateInterface();
}

void QPredicateEditor::on_prevButton_clicked()
{
    step--;
    updateInterface();
}

bool QPredicateEditor::prepareForm(Predicate *predicate)
{
    bool result = true;
    switch (myMode){
    case inlineEditor:
    case inlineWizard:
        break;
    case normalWizard:
        break;
    case normalEditor:
        ui->predNameNormalEditorLabel->setText(predicate->name);
        break;

    }
    return result;
}

Predicate* QPredicateEditor::predicate()
{
    Predicate *result = new Predicate();
    switch (myMode){
    case inlineEditor:
    case inlineWizard:
        result->extName = ui->inlinePredicateText->document()->toPlainText();
        result->name    = ui->inlinePredicateText->document()->toPlainText();
        result->type    = Predicate::inlineType;
        break;
    case normalWizard:
        result->name = ui->predNameNormalWizardStep1Edit->text().toUtf8();
        result->type = Predicate::normalType;
        break;
    case normalEditor:

        break;
    }
    return result;
}


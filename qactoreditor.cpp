#include "qactoreditor.h"
#include "ui_qactoreditor.h"

QActorEditor::QActorEditor(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
}

QActorEditor::QActorEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QActorEditor)
{
    ui->setupUi(this);

    step = 1;
    myMode = mode;
    updateInterface();
}

QActorEditor::~QActorEditor()
{
    delete ui;
}

void QActorEditor::changeEvent(QEvent *e)
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

void QActorEditor::updateInterface()
{
    ui->inlineWizardStep1Widget->setVisible(false);
    ui->inlineWizardStep2Widget->setVisible(false);
    ui->normalEditorWidget->setVisible(false);
    ui->normalWizardStep1Widget->setVisible(false);
    ui->normalWizardStep2Widget->setVisible(false);
    ui->normalWizardStep3Widget->setVisible(false);
    switch (myMode){
    case normalWizard:
        switch (step){
        case 1:
            ui->normalWizardStep1Widget->setVisible(true);
            ui->normalWizardStep2Widget->setVisible(false);
            ui->normalWizardStep3Widget->setVisible(false);
            ui->prevButton->setEnabled(false);
            ui->nextButton->setEnabled(true);
            break;
        case 2:
            ui->normalWizardStep1Widget->setVisible(false);
            ui->normalWizardStep2Widget->setVisible(true);
            ui->normalWizardStep3Widget->setVisible(false);
            ui->prevButton->setEnabled(true);
            ui->nextButton->setEnabled(true);
            break;
        case 3:
            ui->normalWizardStep1Widget->setVisible(false);
            ui->normalWizardStep2Widget->setVisible(false);
            ui->normalWizardStep3Widget->setVisible(true);
            ui->prevButton->setEnabled(true);
            ui->nextButton->setEnabled(false);
            break;
        }
        break;
    case inlineWizard:
    case inlineEditor:
        switch (step){
        case 1:
            ui->inlineWizardStep1Widget->setVisible(true);
            ui->inlineWizardStep2Widget->setVisible(false);
            ui->prevButton->setEnabled(false);
            ui->nextButton->setEnabled(true);
            break;
        case 2:
            ui->inlineWizardStep1Widget->setVisible(false);
            ui->inlineWizardStep2Widget->setVisible(true);
            ui->prevButton->setEnabled(true);
            ui->nextButton->setEnabled(false);
            break;
        }
        break;
    }
}

void QActorEditor::on_nextButton_clicked()
{
    step++;
    updateInterface();
}

void QActorEditor::on_prevButton_clicked()
{
    step--;
    updateInterface();
}

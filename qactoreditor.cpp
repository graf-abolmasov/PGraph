#include "qactoreditor.h"
#include "ui_qactoreditor.h"
#include <QComboBox>

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
            ui->actorNameNormalWizardStep3Label->setText(ui->actorNameNormalEdit->text());
            ui->actorNameNormalWizardStep2Label->setText(ui->actorNameNormalEdit->text());
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

bool QActorEditor::prepareForm(Actor *actor)
{
    bool result = true;

    QComboBox *templateBox = new QComboBox(0);
    templateBox->addItem("Модифицируемый");
    templateBox->addItem("Исходный");
    templateBox->addItem("Еще какой-то");

    ui->parametersTableNormalWizard->insertRow(0);
    ui->parametersTableNormalWizard->insertRow(0);
    ui->parametersTableNormalWizard->setCellWidget(0,2,templateBox);
    ui->parametersTableNormalWizard->setCellWidget(1,2,templateBox);
    ui->parametersTableNormalWizard->setItem(0,0, new QTableWidgetItem("sad1"));
    ui->parametersTableNormalWizard->setItem(1,0, new QTableWidgetItem("sad2"));

    switch (myMode){
    case inlineEditor:
    case inlineWizard:
        break;
    case normalWizard:
        break;
    case normalEditor:
        ui->actorNameNormalEditorLabel->setText(actor->name);
        break;

    }
    return result;
}

Actor* QActorEditor::actor()
{
    Actor *result = new Actor();
    switch (myMode){
    case inlineEditor:
    case inlineWizard:
        result->extName = ui->textInline->document()->toPlainText();
        result->name    = ui->textInline->document()->toPlainText();
        result->type    = Actor::inlineType;
        break;
    case normalWizard:
        result->name = ui->actorNameNormalEdit->text().toUtf8();
        result->type = Actor::normalType;
        break;
    case normalEditor:

        break;
    }
    return result;
}

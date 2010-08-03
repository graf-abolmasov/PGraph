#include "qactoreditor.h"
#include "ui_qactoreditor.h"

QActorEditor::QActorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
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

QActorEditor::QActorEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QActorEditor)
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

void QActorEditor::prepareForm(Actor *actor)
{
    if (actor == NULL)
        myActor = new Actor("", "", myMode == Normal ? Actor::normalType : Actor::inlineType);
    else
        myActor = actor;
    //заполняем форму
    switch(myMode){
    case Normal:
        ui->actorNameEdt->setText(myActor->extName);
        break;
    case Inline:
        ui->iactorNameEdt->setText(myActor->extName);
        break;
    }
}

Actor* QActorEditor::getResult()
{
    switch(myMode){
    case Normal:
        myActor->extName = ui->actorNameEdt->text();
        break;
    case Inline:
        myActor->extName = ui->iactorNameEdt->text();
        break;
    }
    return myActor;
}

void QActorEditor::updateInterface()
{

}

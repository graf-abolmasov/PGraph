#include "qobjecteditor.h"
#include "ui_qobjecteditor.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QList>
#include "actor.h"
#include "qactoreditor.h"
#include "qpredicateeditor.h"
#include "predicate.h"
#include <QtCore>
#include <QLayout>

QObjectEditor::QObjectEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QObjectEditor)
{
    ui->setupUi(this);
    ui->tab->setTabText(0, tr("Акторы"));
    ui->tab->setTabText(1, tr("Предикаты"));
    ui->tab->setTabText(2, tr("I-Акторы"));
    ui->tab->setTabText(3, tr("I-Предикаты"));

    editButtonGroup = new QButtonGroup(ui->buttonBox);

    newButton       = new QPushButton(tr("Создать"), ui->buttonBox);
    editButton      = new QPushButton(tr("Изменить"), ui->buttonBox);
    deleteButton    = new QPushButton(tr("Удалить"), ui->buttonBox);

    QLayout *layout = ui->buttonBox->layout();
    layout->addWidget(newButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addItem(new QSpacerItem(90, 10));
    layout->addWidget(ui->buttonBox->buttons().at(0));
    layout->addWidget(ui->buttonBox->buttons().at(1));
    ui->buttonBox->setLayout(layout);

    editButtonGroup->addButton(newButton, 0);
    editButtonGroup->addButton(editButton, 1);
    editButtonGroup->addButton(deleteButton, 2);
    connect(newButton, SIGNAL(clicked()), this, SLOT(newButtonClicked()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));

    prepareForm();
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

/*!
  Получение необходимых данных из БД.
  Заполнение всех справочников.
*/
bool QObjectEditor::prepareForm()
{
    bool result = true;
    return result;
}

/*!
  Обработка нажатия на кнопки Создать,
  в зависимости от текущей вкладки
*/
void QObjectEditor::newButtonClicked()
{
    QActorEditor *actorEditor;
    QPredicateEditor * predEditor;

    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        actorEditor = new QActorEditor(QActorEditor::normalWizard);
        actorEditor->prepareForm(NULL);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->actor();
            actorsList.append(actor);
            ui->actorList->addItem(actor->name);
        }
        break;
    case 1: //Предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::normalWizard);
        predEditor->prepareForm(NULL);
        if (predEditor->exec()){
            Predicate *pred = predEditor->predicate();
            predicatesList.append(pred);
            ui->predicateList->addItem(pred->name);
        }
        break;
    case 2: //I-акторы
        actorEditor = new QActorEditor(QActorEditor::inlineWizard);
        actorEditor->prepareForm(NULL);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->actor();
            iActorsList.append(actor);
            ui->inlineActorList->addItem(actor->name);
        }
        break;
    case 3: //I-предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::inlineWizard);
        predEditor->prepareForm(NULL);
        if (predEditor->exec()){
            Predicate *pred = predEditor->predicate();
            iPredicateList.append(pred);
            ui->inlinePredicateList->addItem(pred->name);
        }
        break;
    }
}

void QObjectEditor::editButtonClicked()
{
    QActorEditor *actorEditor;
    QPredicateEditor * predEditor;

    int idx = 0;
    Actor* actor;
    Predicate* pred;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        actorEditor = new QActorEditor(QActorEditor::normalEditor);
        for (idx = 0; idx < actorsList.count(); idx++)
            if (actorsList.at(idx)->name == ui->actorList->selectedItems().first()->text())
                break;
        actor = actorsList.at(idx);
        actorEditor->prepareForm(actor);
        if (actorEditor->exec()){

        }
        break;
    case 1: //Предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::normalEditor);
        for (idx = 0; idx < predicatesList.count(); idx++)
            if (predicatesList.at(idx)->name == ui->predicateList->selectedItems().first()->text())
                break;
        pred = predicatesList.at(idx);
        predEditor->prepareForm(pred);
        if (predEditor->exec()){

        }
        break;
    case 2: //I-акторы
        actorEditor = new QActorEditor(QActorEditor::inlineEditor);
        for (idx = 0; idx < iActorsList.count(); idx++)
            if (iActorsList.at(idx)->name == ui->inlineActorList->selectedItems().first()->text())
                break;
        actorEditor->prepareForm(actor);
        if (actorEditor->exec()){

        }
        break;
    case 3: //I-предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::inlineEditor);
        for (idx = 0; idx < iPredicateList.count(); idx++)
            if (iPredicateList.at(idx)->name == ui->inlinePredicateList->selectedItems().first()->text())
                break;
        pred = iPredicateList.at(idx);
        predEditor->prepareForm(pred);
        if (predEditor->exec()){

        }
        break;
    }
}

void QObjectEditor::deleteButtonClicked()
{
    int idx = 0;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        for (idx = 0; idx < actorsList.count(); idx++)
            if (actorsList.at(idx)->name == ui->actorList->selectedItems().first()->text())
                break;
        delete ui->actorList->item(idx);
        delete actorsList.at(idx);
        actorsList.removeAt(idx);
        break;
    case 1: //Предикаты
        for (idx = 0; idx < predicatesList.count(); idx++)
            if (predicatesList.at(idx)->name == ui->predicateList->selectedItems().first()->text())
                break;
        delete ui->predicateList->item(idx);
        delete predicatesList.at(idx);
        predicatesList.removeAt(idx);
        break;
    case 2: //I-акторы
        for (idx = 0; idx < iActorsList.count(); idx++)
            if (iActorsList.at(idx)->name == ui->inlineActorList->selectedItems().first()->text())
                break;
        delete ui->inlineActorList->item(idx);
        delete iActorsList.at(idx);
        iActorsList.removeAt(idx);
        break;
    case 3: //I-предикаты
        for (idx = 0; idx < iPredicateList.count(); idx++)
            if (iPredicateList.at(idx)->name == ui->inlinePredicateList->selectedItems().first()->text())
                break;
        delete ui->inlinePredicateList->item(idx);
        delete iPredicateList.at(idx);
        iPredicateList.removeAt(idx);
        break;
    }
}



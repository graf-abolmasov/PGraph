#include "qobjecteditor.h"
#include "ui_qobjecteditor.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QList>
#include "actor.h"
#include "predicate.h"
#include <QtCore>
#include <QLayout>
#include "qactoreditor.h"
#include "qpredicateeditor.h"
#include "databasemanager.h"


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
    connect(newButton,    SIGNAL(clicked()), this, SLOT(newButtonClicked()));
    connect(editButton,   SIGNAL(clicked()), this, SLOT(editButtonClicked()));
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
void QObjectEditor::prepareForm()
{
    QList<Actor *> fullActorList;
    globalDBManager->getActorList(fullActorList);
    foreach (Actor* actor, fullActorList){
        if (actor->type == Actor::normalType) {
            ui->actorList->addItem(actor->extName);
            actorsList.append(actor);
        }
        if (actor->type == Actor::inlineType) {
            ui->inlineActorList->addItem(actor->extName);
            iActorsList.append(actor);
        }
    }
    QList<Predicate *> fullPredicateList;
    globalDBManager->getPredicateList(fullPredicateList);
    foreach (Predicate* pred, fullPredicateList){
        if (pred->type == Predicate::normalType) {
            ui->predicateList->addItem(pred->extName);
            predicatesList.append(pred);
        }
        if (pred->type == Predicate::inlineType) {
            ui->inlinePredicateList->addItem(pred->extName);
            iPredicateList.append(pred);
        }
    }
}

/*!
  Обработка нажатия на кнопки Создать,
  в зависимости от текущей вкладки
*/
void QObjectEditor::newButtonClicked()
{
    QActorEditor* actorEditor;
    QPredicateEditor* predEditor;

    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        actorEditor = new QActorEditor(QActorEditor::Normal);
        actorEditor->prepareForm(NULL);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->getResult();
            actorsList.append(actor);
            ui->actorList->addItem(actor->extName);
        }
        delete actorEditor;
        break;
    case 1: //Предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::Normal);
        predEditor->prepareForm(NULL);
        if (predEditor->exec()){
            Predicate *pred = predEditor->getResult();
            predicatesList.append(pred);
            ui->predicateList->addItem(pred->name);
        }
        delete predEditor;
        break;
    case 2: //I-акторы
        actorEditor = new QActorEditor(QActorEditor::Inline);
        actorEditor->prepareForm(NULL);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->getResult();
            iActorsList.append(actor);
            ui->inlineActorList->addItem(actor->extName);
        }
        delete actorEditor;
        break;
    case 3: //I-предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::Inline);
        predEditor->prepareForm(NULL);
        if (predEditor->exec()){
            Predicate *pred = predEditor->getResult();
            iPredicateList.append(pred);
            ui->inlinePredicateList->addItem(pred->name);
        }
        delete predEditor;
        break;
    }
}

void QObjectEditor::editButtonClicked()
{
    QActorEditor *actorEditor;
    QPredicateEditor * predEditor;

    int idx = -1;
    Actor* actor;
    Predicate* pred;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        actorEditor = new QActorEditor(QActorEditor::Normal);
        idx = ui->actorList->currentRow();
        if (idx != -1){
            actor = actorsList.at(idx);
            actorEditor->prepareForm(actor);
            if (actorEditor->exec()){
                actor = actorEditor->getResult();
                actorsList.replace(idx, actor);
                delete ui->actorList->item(idx);
                ui->actorList->insertItem(idx, actor->extName);
            }
        }
        delete actorEditor;
        break;
    case 1: //Предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::Normal);
        idx = ui->predicateList->currentRow();
        if (idx != -1){
            pred = predicatesList.at(idx);
            predEditor->prepareForm(pred);
            if (predEditor->exec()){
                pred = predEditor->getResult();
                predicatesList.replace(idx, pred);
                delete ui->predicateList->item(idx);
                ui->predicateList->insertItem(idx, pred->extName);
            }
        }
        delete predEditor;
        break;
    case 2: //I-акторы
        actorEditor = new QActorEditor(QActorEditor::Inline);
        idx = ui->inlineActorList->currentIndex().row();
        if (idx != -1){
            actor = iActorsList.at(idx);
            actorEditor->prepareForm(actor);
            if (actorEditor->exec()){
                actor = actorEditor->getResult();
                iActorsList.replace(idx, actor);
                delete ui->inlineActorList->item(idx);
                ui->inlineActorList->insertItem(idx, actor->extName);
            }
        }
        delete actorEditor;
        break;
    case 3: //I-предикаты
        predEditor = new QPredicateEditor(QPredicateEditor::Inline);
        idx = ui->inlinePredicateList->currentRow();
        if (idx != -1){
            pred = iPredicateList.at(idx);
            predEditor->prepareForm(pred);
            if (predEditor->exec()){
                pred = predEditor->getResult();
                iPredicateList.replace(idx, pred);
                delete ui->inlinePredicateList->item(idx);
                ui->inlinePredicateList->insertItem(idx, pred->extName);
            }
        }
        delete predEditor;
        break;
    }
}

void QObjectEditor::deleteButtonClicked()
{
    int idx = -1;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        idx = ui->actorList->currentRow();
        if (idx != -1){
            //TODO: Удалять только неиспользуемые акторы
            delete ui->actorList->item(idx);
            //delete actorsList.at(idx);
            actorsList.removeAt(idx);
        }
        break;
    case 1: //Предикаты
        idx = ui->predicateList->currentRow();
        if (idx != -1){
            //TODO: Удалять только неиспользуемые предикаты
            delete ui->predicateList->item(idx);
            predicatesList.removeAt(idx);
        }
        break;
    case 2: //I-акторы
        idx = ui->actorList->currentRow();
        if (idx != -1){
            delete ui->inlineActorList->item(idx);
            //delete iActorsList.at(idx);
            iActorsList.removeAt(idx);
        }
        break;
    case 3: //I-предикаты
        idx = ui->inlinePredicateList->currentRow();
        if (idx != -1){
            delete ui->inlinePredicateList->item(idx);
            iPredicateList.removeAt(idx);
        }
        break;
    }
}



void QObjectEditor::on_buttonBox_accepted()
{
    QList<Actor*> fullActorList;
    fullActorList.append(actorsList);
    fullActorList.append(iActorsList);
    globalDBManager->saveActorList(fullActorList);
    QList<Predicate*> fullPredicateList;
    fullPredicateList.append(predicatesList);
    fullPredicateList.append(iPredicateList);
    globalDBManager->savePredicateList(fullPredicateList);
}

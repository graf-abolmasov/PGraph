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
#include "globalvariables.h"
#include "actor.h"
#include "predicate.h"

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
    delete newButton;
    delete editButton;
    delete deleteButton;
    delete editButtonGroup;
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
    try {
        QList<Actor *> fullActorList = globalDBManager->getActorList();
        foreach (Actor *actor, fullActorList){
            if (actor->type == Actor::NormalType) {
                ui->actorList->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));
                myActorsList.append(actor);
            } else if (actor->type == Actor::InlineType) {
                ui->inlineActorList->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));
                myInlineActorsList.append(actor);
            }
        }
        QList<const Predicate *> fullPredicateList = globalDBManager->getPredicateList();
        foreach (const Predicate *pred, fullPredicateList){
            if (pred->type == Predicate::NormalType) {
                ui->predicateList->addItem(QString(pred->extName).replace(QRegExp("(\r+|\n+)"), " "));
                myPredicatesList.append(pred);
            } else if (pred->type == Predicate::inlineType) {
                ui->inlinePredicateList->addItem(QString(pred->extName).replace(QRegExp("(\r+|\n+)"), " "));
                myInlinePredicateList.append(pred);
            }
        }
    } catch (QString s) {
        QMessageBox::critical(this, tr("Ошибка"), s, QMessageBox::Ok);
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
        actorEditor = QActorEditor::getCreator(Actor::NormalType);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->getResult();
            myActorsList.append(actor);
            ui->actorList->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));
        }
        delete actorEditor;
        break;
    case 1: //Предикаты
        predEditor = QPredicateEditor::getCreator(Predicate::NormalType);
        if (predEditor->exec()){
            const Predicate *pred = predEditor->getResult();
            myPredicatesList.append(pred);
            ui->predicateList->addItem(QString(pred->extName).replace(QRegExp("(\r+|\n+)"), " "));
        }
        delete predEditor;
        break;
    case 2: //I-акторы
        actorEditor = QActorEditor::getCreator(Actor::InlineType);
        if (actorEditor->exec()){
            Actor *actor = actorEditor->getResult();
            myInlineActorsList.append(actor);
            ui->inlineActorList->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));
        }
        delete actorEditor;
        break;
    case 3: //I-предикаты
        predEditor = QPredicateEditor::getCreator(Predicate::inlineType);
        if (predEditor->exec()){
            const Predicate *pred = predEditor->getResult();
            myInlinePredicateList.append(pred);
            ui->inlinePredicateList->addItem(QString(pred->extName).replace(QRegExp("(\r+|\n+)"), " "));
        }
        delete predEditor;
        break;
    }
}

void QObjectEditor::editButtonClicked()
{
    QActorEditor *actorEditor;
    QPredicateEditor *predEditor;

    int idx = -1;
    //Actor *actor;
    //Predicate *pred;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        idx = ui->actorList->currentRow();
        if (idx != -1){
            Actor *actor = myActorsList[idx];
            actorEditor = QActorEditor::getEditor(actor);
            if (actorEditor->exec()){
                Actor *newActor = actorEditor->getResult();
                myActorsList.replace(idx, newActor);
                delete ui->actorList->item(idx);
                ui->actorList->insertItem(idx, QString(newActor->extName).replace(QRegExp("(\r+|\n+)"), " "));
            }
            delete actorEditor;
        }
        break;
    case 1: //Предикаты
        idx = ui->predicateList->currentRow();
        if (idx != -1){
            const Predicate *predicate = myPredicatesList[idx];
            predEditor = QPredicateEditor::getEditor(predicate);
            if (predEditor->exec()){
                const Predicate *newPred = predEditor->getResult();
                myPredicatesList.replace(idx, newPred);
                delete ui->predicateList->item(idx);
                ui->predicateList->insertItem(idx, QString(newPred->extName).replace(QRegExp("(\r+|\n+)"), " "));
            }
        }
        delete predEditor;
        break;
    case 2: //I-акторы
        idx = ui->inlineActorList->currentIndex().row();
        if (idx != -1){
            Actor *actor = myInlineActorsList[idx];
            actorEditor = QActorEditor::getEditor(actor);
            if (actorEditor->exec()){
                Actor *newActor = actorEditor->getResult();
                myInlineActorsList.replace(idx, newActor);
                delete ui->inlineActorList->item(idx);
                ui->inlineActorList->insertItem(idx, QString(newActor->extName).replace(QRegExp("(\r+|\n+)"), " "));
            }
        }
        delete actorEditor;
        break;
    case 3: //I-предикаты
        idx = ui->inlinePredicateList->currentRow();
        if (idx != -1){
            const Predicate *pred = myInlinePredicateList[idx];
            predEditor = QPredicateEditor::getEditor(pred);
            if (predEditor->exec()){
                const Predicate *newPred = predEditor->getResult();
                myInlinePredicateList.replace(idx, newPred);
                delete ui->inlinePredicateList->item(idx);
                ui->inlinePredicateList->insertItem(idx, QString(newPred->extName).replace(QRegExp("(\r+|\n+)"), " "));
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
            delete myActorsList[idx];
            myActorsList.removeAt(idx);
        }
        break;
    case 1: //Предикаты
        idx = ui->predicateList->currentRow();
        if (idx != -1){
            //TODO: Удалять только неиспользуемые предикаты
            delete ui->predicateList->item(idx);
            delete myPredicatesList[idx];
            myPredicatesList.removeAt(idx);
        }
        break;
    case 2: //I-акторы
        idx = ui->inlineActorList->currentRow();
        if (idx != -1){
            delete ui->inlineActorList->item(idx);
            delete myInlineActorsList[idx];
            myInlineActorsList.removeAt(idx);
        }
        break;
    case 3: //I-предикаты
        idx = ui->inlinePredicateList->currentRow();
        if (idx != -1){
            delete ui->inlinePredicateList->item(idx);
            delete myInlinePredicateList[idx];
            myInlinePredicateList.removeAt(idx);
        }
        break;
    }
}



void QObjectEditor::on_buttonBox_accepted()
{
    QList<Actor *> fullActorList;
    fullActorList.append(myActorsList);
    fullActorList.append(myInlineActorsList);
    QList<const Predicate *> fullPredicateList;
    fullPredicateList.append(myPredicatesList);
    fullPredicateList.append(myInlinePredicateList);
    globalDBManager->setActorList(fullActorList);
    globalDBManager->setPredicateList(fullPredicateList);
}

#include "qobjecteditor.h"
#include "QtGui/QMessageBox"

#include "ui_qobjecteditor.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QList>
#include <QtCore>
#include <QLayout>
#include "qactoreditor.h"
#include "qpredicateeditor.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/predicate.h"

#include "../../src/common/DAO/actordao.h"

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
        QList<const Actor *> fullActorList = globalDBManager->getActorList();
        foreach (const Actor *actor, fullActorList){
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
            } else if (pred->type == Predicate::InlineType) {
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
            const Actor *actor = actorEditor->getResult();
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
            const Actor *actor = actorEditor->getResult();
            myInlineActorsList.append(actor);
            ui->inlineActorList->addItem(QString(actor->extName).replace(QRegExp("(\r+|\n+)"), " "));
        }
        delete actorEditor;
        break;
    case 3: //I-предикаты
        predEditor = QPredicateEditor::getCreator(Predicate::InlineType);
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
    QActorEditor *actorEditor = NULL;
    QPredicateEditor *predEditor = NULL;

    int idx = -1;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        idx = ui->actorList->currentRow();
        if (idx != -1){
            const Actor *actor = myActorsList[idx];
            actorEditor = QActorEditor::getEditor(actor);
            if (actorEditor->exec()){
                const Actor *newActor = actorEditor->getResult();
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
        break;
    case 2: //I-акторы
        idx = ui->inlineActorList->currentIndex().row();
        if (idx != -1){
            const Actor *actor = myInlineActorsList[idx];
            actorEditor = QActorEditor::getEditor(actor);
            if (actorEditor->exec()){
                const Actor *newActor = actorEditor->getResult();
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
        break;
    }
}

void QObjectEditor::deleteButtonClicked()
{
    int idx = -1;
    switch (ui->tab->currentIndex()){
    case 0: //Акторы
        idx = ui->actorList->currentRow();
        if (idx != -1) {
            const Actor *a = myActorsList[idx];
            const QStringList usageList = globalDBManager->findActorUsage(a->name);
            if (usageList.isEmpty()) {
                delete ui->actorList->item(idx);
                delete a;
                myActorsList.removeAt(idx);
            } else
                QMessageBox::warning(NULL, QObject::tr(ERR_TITLE), QObject::tr(ERR_ACTOR_USING_IN).arg(usageList.join("\r\n")));
        }
        break;
    case 1: //Предикаты
        idx = ui->predicateList->currentRow();
        if (idx != -1) {
            const Predicate *p = myPredicatesList[idx];
            const QStringList usageList = globalDBManager->findPredicateUsage(p->name);
            if (usageList.isEmpty()) {
                delete ui->predicateList->item(idx);
                delete myPredicatesList[idx];
                myPredicatesList.removeAt(idx);
            } else
                QMessageBox::warning(NULL, QObject::tr(ERR_TITLE), QObject::tr(ERR_PREDICATE_USING_IN).arg(usageList.join("\r\n")));
        }
        break;
    case 2: //I-акторы
        idx = ui->inlineActorList->currentRow();
        if (idx != -1) {
            const Actor *a = myInlineActorsList[idx];
            const QStringList usageList = globalDBManager->findActorUsage(a->name);
            if (usageList.isEmpty()) {
                delete ui->inlineActorList->item(idx);
                delete a;
                myInlineActorsList.removeAt(idx);
            } else
                QMessageBox::warning(NULL, QObject::tr(ERR_TITLE), QObject::tr(ERR_ACTOR_USING_IN).arg(usageList.join("\r\n")));
        }
        break;
    case 3: //I-предикаты
        idx = ui->inlinePredicateList->currentRow();
        if (idx != -1) {
            const Predicate *p = myInlinePredicateList[idx];
            const QStringList usageList = globalDBManager->findPredicateUsage(p->name);
            if (usageList.isEmpty()) {
                delete ui->inlinePredicateList->item(idx);
                delete myInlinePredicateList[idx];
                myInlinePredicateList.removeAt(idx);
            } else
                QMessageBox::warning(NULL, QObject::tr(ERR_TITLE), QObject::tr(ERR_PREDICATE_USING_IN).arg(usageList.join("\r\n")));
        }
        break;
    }
}

void QObjectEditor::save()
{
    QList<const Actor *> fullActorList;
    fullActorList.append(myActorsList);
    fullActorList.append(myInlineActorsList);
    QList<const Predicate *> fullPredicateList;
    fullPredicateList.append(myPredicatesList);
    fullPredicateList.append(myInlinePredicateList);
    globalDBManager->setActorList(fullActorList);
    globalDBManager->setPredicateList(fullPredicateList);
}

void QObjectEditor::on_buttonBox_accepted()
{
    save();
    accept();
}

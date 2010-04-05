#include "qobjecteditor.h"
#include "ui_qobjecteditor.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QList>
#include "actor.h"
#include "qactoreditor.h"

QObjectEditor::QObjectEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QObjectEditor)
{
    ui->setupUi(this);
    ui->tab->setTabText(0, tr("Акторы"));
    ui->tab->setTabText(1, tr("Предикаты"));
    ui->tab->setTabText(2, tr("I-акторы"));
    ui->tab->setTabText(3, tr("I-предикаты"));

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
  ��������� ����������� ������ �� ��.
  ���������� ���� ������������.
*/
bool QObjectEditor::prepareForm()
{

}

/*!
  ��������� ������� �� ������ �������,
  � ����������� �� ������� �������
*/
void QObjectEditor::newButtonClicked()
{
    QActorEditor *actorEditor;
    switch (ui->tab->currentIndex()){
    case 0: //������
        actorEditor = new QActorEditor(QActorEditor::normalWizard);
        if (actorEditor->exec()){

        }
        break;
    case 1: //���������
        break;
    case 2: //I-������
        actorEditor = new QActorEditor(QActorEditor::inlineWizard);
        if (actorEditor->exec()){

        }
        break;
    case 3: //I-���������
        break;
    }
}

void QObjectEditor::editButtonClicked()
{
    QActorEditor *actorEditor;
    switch (ui->tab->currentIndex()){
    case 0: //������
        actorEditor = new QActorEditor(QActorEditor::normalEditor);
        if (actorEditor->exec()){

        }
        break;
    case 1: //���������
        break;
    case 2: //I-������
        actorEditor = new QActorEditor(QActorEditor::inlineEditor);
        if (actorEditor->exec()){

        }
        break;
    case 3: //I-���������
        break;
    }
}

void QObjectEditor::deleteButtonClicked()
{
    switch (ui->tab->currentIndex()){
    case 0: //������
        break;
    case 1: //���������
        break;
    case 2: //I-������
        break;
    case 3: //I-���������
        break;
    }
}



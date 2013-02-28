#include <QtGui>
#include <QtWidgets>

#include "qpredicateeditor.h"
#include "ui_qpredicateeditor.h"
#include "qvariabledialog.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/predicate.h"
#include "../../src/common/VO/basemodule.h"
#include "../../src/common/VO/variable.h"
#include "../../src/common/qgraphsettings.h"

QPredicateEditor::QPredicateEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
}

QPredicateEditor::~QPredicateEditor()
{
    delete ui;
    if (varEditBtn != NULL)
        delete varEditBtn;
    if (varWidget != NULL)
        delete varWidget;
    if (varLayout != NULL)
        delete varLayout;
    if (paramTypeCmbBox != NULL)
        delete paramTypeCmbBox;
}

QPredicateEditor *QPredicateEditor::getCreator(const Predicate::Type &mode)
{
    return new QPredicateEditor(mode);
}

QPredicateEditor *QPredicateEditor::getEditor(const Predicate *predicate)
{
    return new QPredicateEditor(predicate);
}

QPredicateEditor::QPredicateEditor(const Predicate::Type mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
    prepareForm(new Predicate("", "", mode, NULL, QList<const Variable *>()));
}
QPredicateEditor::QPredicateEditor(const Predicate *predicate, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
    prepareForm(predicate);
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

void QPredicateEditor::prepareForm(const Predicate *predicate)
{
    myPredicate = predicate;
    tempPre = new Predicate(myPredicate->name, myPredicate->extName, myPredicate->type, myPredicate->baseModule, myPredicate->variableList);
    //заполняем форму
    myVariableList = globalDBManager->getVariableList();
    QStringList varnames;
    foreach (const Variable *var, myVariableList)
        varnames << var->name;
    myCompleter = new QCompleter(varnames, this);
    myCompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    myCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    myCompleter->setWrapAround(true);
    ui->inlineModuleTxtEdt->setCompleter(myCompleter);
    switch(tempPre->type){
    case Predicate::NormalType:
        myModuleList = globalDBManager->getBaseModuleList();
        ui->inlineWidget->setVisible(false);
        ui->predicateNameEdt->setText(myPredicate->extName);
        ui->predicateNameEdt->setValidator(new QRegExpValidator(QRegExp(tr("[A-Za-zА-Яа-я0-9 ]{0,255}")), this));
        foreach(const BaseModule* baseModule, myModuleList){
            ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
            if (myPredicate != NULL && myPredicate->baseModule == baseModule){
                ui->baseModuleList->blockSignals(true);
                ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                for (int i = 0; i < myModuleList.at(ui->baseModuleList->currentRow())->parameterList.count(); i++){
                    BaseModuleParameter parameter = myModuleList.at(ui->baseModuleList->currentRow())->parameterList[i];
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.name));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.type));
                    ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myPredicate->variableList[i] == NULL ? "N/A" : myPredicate->variableList[i]->name));
                }
                ui->baseModuleList->blockSignals(false);
             }
        }
        break;
    case Predicate::InlineType:
        ui->normalWidget->setVisible(false);
        ui->inlineModuleTxtEdt->blockSignals(true);
        ui->inlineModuleTxtEdt->setPlainText(myPredicate->extName);
        ui->inlineModuleTxtEdt->blockSignals(false);
        break;
    }
}

const Predicate *QPredicateEditor::getResult() const
{
    return result;
}

void QPredicateEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList[currentRow]->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);
    tempPre->variableList.clear();
    for (int i = 0; i < myModuleList[currentRow]->parameterList.count(); i++){
        tempPre->variableList.append(NULL);
        BaseModuleParameter parameter = myModuleList[currentRow]->parameterList[i];
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.name));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.type));
        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem("N/A"));
    }
    tempPre->baseModule = myModuleList[currentRow];
}

void QPredicateEditor::on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1) {
        ui->paramsNormalTable->setCellWidget(previousRow, 2, NULL);
        delete varWidget;
    }
    if (currentRow == -1) return;
    varWidget = new QWidget(ui->paramsNormalTable);
    varLayout = new QHBoxLayout(varWidget);
    varEditBtn = new QToolButton(varWidget);
    varLayout->setContentsMargins(0, 0, 0, 0);
    varLayout->addStretch();
    varLayout->addWidget(varEditBtn);
    varEditBtn->setText("...");

    connect(varEditBtn, SIGNAL(clicked()), this, SLOT(showVariableDialog()));
    ui->paramsNormalTable->setCellWidget(currentRow, 2, varWidget);

    ui->descriptionPOLbl_2->setText("");
    if (tempPre->variableList[currentRow] != NULL) {
        ui->descriptionPOLbl_2->setText(tempPre->variableList[currentRow]->comment);
    }
    ui->descriptionBMLbl_2->setText(myModuleList[ui->baseModuleList->currentRow()]->parameterList[currentRow].comment);
}

void QPredicateEditor::showVariableDialog()
{
    QVariableDialog dialog;
    if (dialog.exec()){
        tempPre->variableList.replace(ui->paramsNormalTable->currentRow(), dialog.getVariable());
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(dialog.getVariable()->name);
        ui->descriptionPOLbl_2->setText(dialog.getVariable()->comment);
    }
}

void QPredicateEditor::on_buttonBox_accepted()
{
    if (makeResult())
        accept();
}

void QPredicateEditor::on_inlineModuleTxtEdt_textChanged()
{
    tempPre->variableList.clear();
    for (int i = 0; i < myVariableList.count(); i++)
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList[i]->name + "(\\s|\\b|\\W)", Qt::CaseSensitive)))
            tempPre->variableList.append(myVariableList.at(i));
}

bool QPredicateEditor::makeResult()
{
    switch(tempPre->type){
    case Predicate::NormalType:
        tempPre->extName = ui->predicateNameEdt->text();
        break;
    case Predicate::InlineType:
        tempPre->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        break;
    }

    tempPre->name = "P" + getCRC(tempPre->extName.toUtf8());

    const QStringList errors = tempPre->validate();
    if (!errors.isEmpty()) {
        QMessageBox::warning(0, tr("Ошибка"), errors.join("\n"));
        return false;
    }

    result = const_cast<Predicate *>(myPredicate);
    result->name = tempPre->name;
    result->extName = tempPre->extName;
    result->type = tempPre->type;
    result->baseModule = tempPre->baseModule;
    result->variableList = tempPre->variableList;
    delete tempPre;
    return true;
}

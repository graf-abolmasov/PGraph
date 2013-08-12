#include "qactoreditor.h"
#include <QtWidgets>
#include "ui_qactoreditor.h"
#include "qvariabledialog.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/basemodule.h"
#include "../../src/common/qgraphsettings.h"
#include <QtGui>

QActorEditor *QActorEditor::getCreator(const Actor::Type &mode)
{
    return new QActorEditor(mode);
}

QActorEditor *QActorEditor::getEditor(const Actor *actor)
{
    return new QActorEditor(actor);
}

QActorEditor::QActorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
}

void QActorEditor::on_iconLabel_clicked() {
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Открыть картинку..."),
                                                    "",
                                                    tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    if (fileName.isEmpty())
        return;
    QPixmap img(fileName);
    if (!img.isNull() && QFileInfo(fileName).size() < 10000) {
        if (tempActor != NULL) {
            tempActor->icon = img;
        }
        ui->iconLabel->setPixmap(img);
    } else QMessageBox::critical(NULL, tr("Ошибка"), tr("Размер картинки не должен превышать 10 кб"));
}


QActorEditor::QActorEditor(const Actor *actor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
    prepareForm(actor);
}

QActorEditor::QActorEditor(const Actor::Type &mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
    prepareForm(new Actor("", "", mode, NULL, QList<const Variable *>(), QStringList(), QPixmap()));
}

QActorEditor::~QActorEditor()
{
    delete ui;
    if (varEditBtn != NULL)
        delete varEditBtn;
    if (varWidget != NULL)
        delete varWidget;
    if (varLayout != NULL)
        delete varLayout;
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

void QActorEditor::prepareForm(const Actor *actor)
{
    myActor = actor;
    tempActor = new Actor(myActor->name, myActor->extName, myActor->type, myActor->baseModule, myActor->variableList, myActor->varAccModeList, myActor->icon);
    tempActor->icon = myActor->icon;

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

    switch(tempActor->type) {
    case Actor::NormalType:
        ui->actorNameEdt->setText(tempActor->extName);
        //Получаем список базовых модулей
        myModuleList = globalDBManager->getBaseModuleList();
        ui->baseModuleList->blockSignals(true);
        foreach(const BaseModule *baseModule, myModuleList) {
            ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
            if (tempActor->baseModule == baseModule) {
                ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                for (int i = 0; i < myModuleList[ui->baseModuleList->currentRow()]->parameterList.count(); i++){
                    BaseModuleParameter parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i];
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.name));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.type));
                    ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(tempActor->variableList[i] == NULL ? "N/A" : tempActor->variableList[i]->name));
                }
            }
        }
        ui->baseModuleList->blockSignals(false);
        ui->actorNameEdt->setFocus();
        ui->inlineWidget->setVisible(false);
        break;
    case Actor::InlineType:
        ui->inlineModuleTxtEdt->blockSignals(true);
        ui->inlineModuleTxtEdt->setPlainText(tempActor->extName);
        for (int i = 0; i < tempActor->variableList.count(); i++){
            ui->paramsInlineTable->insertRow(i);
            ui->paramsInlineTable->setItem(i, 0, new QTableWidgetItem(tempActor->variableList[i]->name));
            ui->paramsInlineTable->setItem(i, 1, new QTableWidgetItem(tempActor->variableList[i]->type->name));
            ui->paramsInlineTable->setItem(i, 2, new QTableWidgetItem(tempActor->varAccModeList[i]));
        }
        //Получаем список перемерных
        ui->inlineModuleTxtEdt->blockSignals(false);
        ui->inlineModuleTxtEdt->setFocus();
        ui->normalWidget->setVisible(false);
        break;
    case Actor::GraphType:
        break;
    }
    QRegExp regExp(tr("[A-Za-z1-9А-Яа-я ]{1,255}"));
    ui->actorNameEdt->setValidator(new QRegExpValidator(regExp, this));
    if (!myActor->icon.isNull())
        ui->iconLabel->setPixmap(myActor->icon);
}

const Actor *QActorEditor::getResult()
{
    return result;
}

void QActorEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList[currentRow]->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);

    tempActor->variableList.clear();
    tempActor->varAccModeList.clear();
    for (int i = 0; i < myModuleList[currentRow]->parameterList.count(); i++){
        tempActor->variableList.append(NULL);
        BaseModuleParameter parameter = myModuleList[currentRow]->parameterList[i];
        tempActor->varAccModeList.append(parameter.accessMode);
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.name));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.type));
        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem("N/A"));
    }
}

void QActorEditor::on_paramsNormalTable_currentCellChanged(int currentRow, int, int previousRow, int)
{
    if (previousRow != -1) {
        ui->paramsNormalTable->setCellWidget(previousRow, 2, NULL);
        delete varWidget;
        varWidget = NULL;
    }
    if (currentRow == -1)
        return;
    varWidget = new QWidget(ui->paramsNormalTable);
    varLayout = new QHBoxLayout(varWidget);
    varEditBtn = new QToolButton(varWidget);
    varLayout->setContentsMargins(0, 0, 0, 0);
    varLayout->addStretch();
    varLayout->addWidget(varEditBtn);
    varEditBtn->setText("...");

    connect(varEditBtn, SIGNAL(clicked()), this, SLOT(showVariableEditor()));
    ui->paramsNormalTable->setCellWidget(currentRow, 2, varWidget);

    ui->descriptionPOLbl_2->setText("");
    if (tempActor->variableList[currentRow] != NULL)
        ui->descriptionPOLbl_2->setText(tempActor->variableList[currentRow]->comment);
    ui->descriptionBMLbl_2->setText(myModuleList[ui->baseModuleList->currentRow()]->parameterList.at(currentRow).comment);
}

void QActorEditor::showVariableEditor()
{
    QVariableDialog dialog;
    const Variable *var;
    if (dialog.exec() && (var = dialog.getVariable())){
        tempActor->variableList[ui->paramsNormalTable->currentRow()] = var;
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(var->name);
        ui->descriptionPOLbl_2->setText(var->comment);
    }
}

void QActorEditor::on_inlineModuleTxtEdt_textChanged()
{
    ui->paramsInlineTable->clearContents();
    ui->paramsInlineTable->setRowCount(0);
    int p = 0;
    tempActor->variableList.clear();
    tempActor->varAccModeList.clear();
    for (int i = 0; i < myVariableList.count(); i++){
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList[i]->name + "(\\s|\\b|\\W)", Qt::CaseSensitive))){
            ui->paramsInlineTable->insertRow(p);
            tempActor->variableList.append(myVariableList[i]);
            tempActor->varAccModeList.append(tr("Модифицируемый"));
            ui->paramsInlineTable->setItem(p, 0, new QTableWidgetItem(myVariableList[i]->name));
            ui->paramsInlineTable->setItem(p, 1, new QTableWidgetItem(myVariableList[i]->type->name));
//            ui->paramsInlineTable->setItem(p, 2, new QTableWidgetItem("N/A"));
            ui->paramsInlineTable->setItem(p, 2, new QTableWidgetItem(tr("Модифицируемый")));
            p++;
        }
    }
    tempActor->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
}

void QActorEditor::on_paramsInlineTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1) {
        if (previousColumn == 2){
            ui->paramsInlineTable->setCellWidget(previousRow, 2, NULL);
            ui->paramsInlineTable->item(previousRow, 2)->setText(paramTypeCmbBox->currentText());
            tempActor->varAccModeList.replace(previousRow, paramTypeCmbBox->currentText());
            delete paramTypeCmbBox;
        }
    }

    if (currentColumn == 2) {
        paramTypeCmbBox = new QComboBox(ui->paramsInlineTable);
        paramTypeCmbBox->addItem(tr("Исходный"));
        paramTypeCmbBox->addItem(tr("Модифицируемый"));
        paramTypeCmbBox->addItem(tr("Вычисляемый"));

        int idx = paramTypeCmbBox->findText(ui->paramsInlineTable->item(currentRow, 2)->text());
        paramTypeCmbBox->setCurrentIndex(idx);
        ui->paramsInlineTable->setCellWidget(currentRow, 2, paramTypeCmbBox);
    }
}

/*!
  \brief Реакция на OK
*/
void QActorEditor::on_buttonBox_accepted()
{
    ui->paramsInlineTable->setCurrentCell(-1, -1);
    if (makeResult())
        accept();
}

/*!
  \brief Проверяет введенные данные и применяет сделанные изменения.
*/
bool QActorEditor::makeResult()
{
    switch(tempActor->type) {
    case Actor::NormalType:
        tempActor->extName = ui->actorNameEdt->text();
        tempActor->baseModule = myModuleList[ui->baseModuleList->currentRow()];
        break;
    case Actor::InlineType:
        tempActor->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        break;
    case Actor::GraphType:;
    }
    tempActor->name = "A" + getCRC(tempActor->extName.toUtf8());

    const QStringList errors = tempActor->validate();
    if (!errors.isEmpty()) {
        QMessageBox::warning(0, tr("Ошибка"), errors.join("\n"));
        return false;
    }

    result = const_cast<Actor *>(myActor);
    result->name = tempActor->name;
    result->extName = tempActor->extName;
    result->baseModule = tempActor->baseModule;
    result->icon = tempActor->icon;
    result->type = tempActor->type;
    result->varAccModeList = tempActor->varAccModeList;
    result->variableList = tempActor->variableList;
    result->icon = tempActor->icon;
    globalDBManager->saveActorPicture(result->name, tempActor->icon);
    delete tempActor;
    return true;
}

void QActorEditor::on_buttonBox_rejected()
{
    reject();
}

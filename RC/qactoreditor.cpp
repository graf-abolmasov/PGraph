#include "qactoreditor.h"
#include "ui_qactoreditor.h"
#include "databasemanager.h"
#include "qvariabledialog.h"
#include "commonutils.h"
#include "globalvariables.h"
#include "actor.h"
#include "basemodule.h"
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
    prepareForm(new Actor("", "", mode, NULL, QList<const Variable *>(), QStringList(), QImage()));
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
                    QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter[1]));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter[0]));
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
        QList<const Variable *> actorVariables = tempActor->variableList;
        for (int i = 0; i < actorVariables.count(); i++){
            ui->paramsInlineTable->insertRow(i);
            ui->paramsInlineTable->setItem(i, 0, new QTableWidgetItem(actorVariables[i]->name));
            ui->paramsInlineTable->setItem(i, 1, new QTableWidgetItem(actorVariables[i]->type->name));
            ui->paramsInlineTable->setItem(i, 2, new QTableWidgetItem(tempActor->varAccModeList[i]));
        }
        //Получаем список перемерных
        ui->inlineModuleTxtEdt->blockSignals(false);
        ui->inlineModuleTxtEdt->setFocus();
        ui->normalWidget->setVisible(false);
        break;
    }
    QRegExp regExp(tr("[A-Za-z1-9А-Яа-я ]{1,255}"));
    ui->actorNameEdt->setValidator(new QRegExpValidator(regExp, this));
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
        QStringList parameter = myModuleList[currentRow]->parameterList[i].split(";;");
        tempActor->varAccModeList.append(parameter[2]);
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter[1]));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter[0]));
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
    ui->descriptionBMLbl_2->setText(myModuleList[ui->baseModuleList->currentRow()]->parameterList.at(currentRow).split(";;").at(3));
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
            tempActor->varAccModeList.append("");
            ui->paramsInlineTable->setItem(p, 0, new QTableWidgetItem(myVariableList[i]->name));
            ui->paramsInlineTable->setItem(p, 1, new QTableWidgetItem(myVariableList[i]->type->name));
            ui->paramsInlineTable->setItem(p, 2, new QTableWidgetItem("N/A"));
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
    }
    tempActor->name = "A" + getCRC(tempActor->extName.toUtf8());

    if (tempActor->extName.isEmpty()) {
        QString mes = tempActor->type == Actor::InlineType ? tr("Тело inline-актора не может быть пустым") : tr("Необходимо указать название актора");
        QMessageBox::warning(NULL, tr("Ошибка"), mes);
        return false;
    }

    if (tempActor->type == Actor::NormalType && tempActor->baseModule == NULL) {
        QMessageBox::warning(NULL, tr("Ошибка"), tr("Необходимо указать базовый модуль"));
        return false;
    }

    if (tempActor->varAccModeList.size() > 0 && tempActor->varAccModeList.contains(tr(""))) {
        QMessageBox::warning(NULL, tr("Ошибка"), tr("Необходимо указать модификаторы доступа для всех переменных"));
        return false;
    }

    if (tempActor->variableList.size() > 0 && tempActor->variableList.contains(NULL)) {
        QMessageBox::warning(NULL, tr("Ошибка"), tr("Необходимо указать переменные для всех параметров базового модуля"));
        return false;
    }

    QSettings myLocSettings("graph.ini", QSettings::IniFormat);
    QDir currentDir;
    currentDir.setCurrent(myLocSettings.value("Location/BaseDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString());

    QByteArray outputData;
    QFile output;

    // Создаем сpp файл
    switch(tempActor->type) {
    case Actor::NormalType:
        // Генерируем актор
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int ");
        outputData.append(tempActor->name.toUtf8());
        outputData.append("(TPOData *D)\r\n");
        outputData.append("{\r\n");
        // Инициализуем данные
        for (int i = 0; i < tempActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(tr("\t%1 %2 = D->%3;\r\n").arg(parameter[0]).arg(parameter[1]).arg(tempActor->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\tint result = ");
        outputData.append(tempActor->baseModule->unicName);
        outputData.append("(");
        for(int i = 0; i < tempActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("&%1, ")).arg(parameter[1]).toUtf8());
        }
        if (tempActor->variableList.count() > 0)
            outputData.remove(outputData.length()-2,2);
        outputData.append(");\r\n\r\n");
        // сохраняем данные
        for(int i = 0; i < tempActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("\tD->%1 = %2;\n")).arg(tempActor->variableList[i]->name).arg(parameter[1]).toUtf8());
        }
        // выход
        outputData.append("\r\n\r\n\treturn result;\r\n}");
        break;
    case Actor::InlineType:
        //генерируем с++ файл
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int " + tempActor->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(tempActor->extName);
        for (int i = 0; i < tempActor->variableList.count(); i++)
            code.replace(QRegExp("(\\b)" + tempActor->variableList[i]->name + "(\\b)", Qt::CaseSensitive), "(D->" + tempActor->variableList[i]->name + ")");
        outputData.append("  " + code + "\r\n");
        outputData.append("  return 1;\r\n");
        outputData.append("}\r\n");
        break;
    }

    output.setFileName(currentDir.canonicalPath() + "/" + tempActor->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData);
    output.close();

    result = const_cast<Actor *>(myActor);
    result->name = tempActor->name;
    result->extName = tempActor->extName;
    result->baseModule = tempActor->baseModule;
    result->icon = tempActor->icon;
    result->type = tempActor->type;
    result->varAccModeList = tempActor->varAccModeList;
    result->variableList = tempActor->variableList;
    delete tempActor;
    return true;
}

void QActorEditor::on_buttonBox_rejected()
{
    reject();
}

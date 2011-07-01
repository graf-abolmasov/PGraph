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

QActorEditor *QActorEditor::getEditor(Actor *actor)
{
    return new QActorEditor(actor);
}

QActorEditor::QActorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
}

QActorEditor::QActorEditor(Actor *actor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
    ui->baseModuleList->setEnabled(false);
    prepareForm(actor);
}

QActorEditor::QActorEditor(const Actor::Type &mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QActorEditor)
{
    ui->setupUi(this);
    prepareForm(new Actor("", "", mode, "", QList<const Variable *>(), QStringList(), QImage()));
}

QActorEditor::~QActorEditor()
{
    delete ui;
    delete varEditBtn;
    delete varWidget;
    delete varLayout;
    delete paramTypeCmbBox;
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

void QActorEditor::prepareForm(Actor *actor)
{
    QList<const Variable *> newVariableList;
    QStringList newAMList;
    myActor = actor;
    myMode == actor->type;

    //заполняем форму
    switch(myMode) {
    case Actor::NormalType:
        ui->actorNameEdt->setText(myActor->extName);
        //Получаем список базовых модулей
        myModuleList = globalDBManager->getBaseModuleList();
        ui->baseModuleList->blockSignals(true);
        foreach(const BaseModule *baseModule, myModuleList) {
            ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
            if (myActor->baseModule == baseModule->unicName) {
                ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                for (int i = 0; i < myModuleList[ui->baseModuleList->currentRow()]->parameterList.count(); i++){
                    QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter[1]));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter[0]));
                    ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myActor->variableList[i] == NULL ? "N/A" : myActor->variableList[i]->name));
                }
            }
        }
        ui->baseModuleList->blockSignals(false);
        ui->actorNameEdt->setFocus();
        break;
    case Actor::InlineType:
        ui->inlineModuleTxtEdt->blockSignals(true);
        ui->inlineModuleTxtEdt->setPlainText(myActor->extName);
        QList<const Variable *> actorVariables = myActor->variableList;
        for (int i = 0; i < actorVariables.count(); i++){
            ui->paramsInlineTable->insertRow(i);
            ui->paramsInlineTable->setItem(i, 0, new QTableWidgetItem(actorVariables[i]->name));
            ui->paramsInlineTable->setItem(i, 1, new QTableWidgetItem(actorVariables[i]->type->name));
            ui->paramsInlineTable->setItem(i, 2, new QTableWidgetItem(myActor->varAccModeList[i]));
        }
        //Получаем список перемерных
        ui->inlineModuleTxtEdt->blockSignals(false);
        myVariableList = globalDBManager->getVariableList();
        ui->inlineModuleTxtEdt->setFocus();
        break;
    }
    QRegExp regExp(tr("[A-Za-z1-9А-Яа-я ]{1,255}"));
    ui->actorNameEdt->setValidator(new QRegExpValidator(regExp, this));
}

Actor* QActorEditor::getResult()
{
    return myActor;
}

void QActorEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList[currentRow]->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);

    myActor->variableList.clear();
    myActor->varAccModeList.clear();
    for (int i = 0; i < myModuleList[currentRow]->parameterList.count(); i++){
        myActor->variableList.append(NULL);
        QStringList parameter = myModuleList[currentRow]->parameterList[i].split(";;");
        myActor->varAccModeList.append(parameter[2]);
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

    connect(varEditBtn, SIGNAL(clicked()), this, SLOT(on_varEditBtn_clicked()));
    ui->paramsNormalTable->setCellWidget(currentRow, 2, varWidget);

    ui->descriptionPOLbl_2->setText("");
    ui->descriptionPOLbl_2->setText(myActor->variableList[currentRow]->comment);
    ui->descriptionBMLbl_2->setText(myModuleList[ui->baseModuleList->currentRow()]->parameterList.at(currentRow).split(";;").at(3));
}

void QActorEditor::on_varEditBtn_clicked()
{
    QVariableDialog dialog;
    if (dialog.exec()){
        const Variable *var = dialog.getVariable();
        myActor->variableList[ui->paramsNormalTable->currentRow()] = var;
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(var->name);
        ui->descriptionPOLbl_2->setText(var->comment);
    }
}

void QActorEditor::on_inlineModuleTxtEdt_textChanged()
{
    ui->paramsInlineTable->clearContents();
    ui->paramsInlineTable->setRowCount(0);
    int p = 0;
    myActor->variableList.clear();
    myActor->varAccModeList.clear();
    for (int i = 0; i < myVariableList.count(); i++){
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList[i]->name + "(\\s|\\b|\\W)", Qt::CaseSensitive))){
            ui->paramsInlineTable->insertRow(p);
            myActor->variableList.append(myVariableList[i]);
            myActor->varAccModeList.append("");
            ui->paramsInlineTable->setItem(p, 0, new QTableWidgetItem(myVariableList[i]->name));
            ui->paramsInlineTable->setItem(p, 1, new QTableWidgetItem(myVariableList[i]->type->name));
            ui->paramsInlineTable->setItem(p, 2, new QTableWidgetItem("N/A"));
            p++;
        }
    }
}

void QActorEditor::on_paramsInlineTable_currentCellChanged(int currentRow, int, int previousRow, int)
{
    if (previousRow != -1){
        ui->paramsInlineTable->setCellWidget(previousRow, 2, NULL);
        ui->paramsInlineTable->item(previousRow, 2)->setText(paramTypeCmbBox->currentText());
        myActor->varAccModeList.replace(previousRow, paramTypeCmbBox->currentText());
        delete paramTypeCmbBox;
    }

    paramTypeCmbBox = new QComboBox(ui->paramsInlineTable);
    paramTypeCmbBox->addItem(tr("Исходный"));
    paramTypeCmbBox->addItem(tr("Модифицируемый"));
    paramTypeCmbBox->addItem(tr("Вычисляемый"));

    if (ui->paramsInlineTable->item(currentRow, 2) != NULL)
        paramTypeCmbBox->setCurrentIndex(paramTypeCmbBox->findText(ui->paramsInlineTable->item(currentRow, 2)->text()));
    ui->paramsInlineTable->setCellWidget(currentRow, 2, paramTypeCmbBox);
}

void QActorEditor::on_okButton_clicked()
{
    if (validate())
        accept();
}

bool QActorEditor::validate()
{
    return true;
}

void QActorEditor::on_QActorEditor_accepted()
{
    QSettings myLocSettings("graph.ini", QSettings::IniFormat);
    QDir currentDir;
    currentDir.setCurrent(myLocSettings.value("Location/BaseDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString());

    QByteArray outputData;
    QFile output;

    switch(myMode){
    case Actor::NormalType:
        myActor->extName = ui->actorNameEdt->text();
        myActor->baseModule = myModuleList[ui->baseModuleList->currentRow()]->unicName;
        myActor->name = "A" + getCRC(myActor->extName.toUtf8());

        // Генерируем актор
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int ");
        outputData.append(myActor->name.toUtf8());
        outputData.append("(TPOData *D)\r\n");
        outputData.append("{\r\n");
        // Инициализуем данные
        for(int i = 0; i < myActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("\t%1 %2 = D->%3;\r\n")).arg(parameter[0]).arg(parameter[1]).arg(myActor->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\tint result = ");
        outputData.append(myActor->baseModule);
        outputData.append("(");
        for(int i = 0; i < myActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("&%1, ")).arg(parameter[1]).toUtf8());
        }
        if (myActor->variableList.count() > 0)
            outputData.remove(outputData.length()-2,2);
        outputData.append(");\r\n\r\n");
        // сохраняем данные
        for(int i = 0; i < myActor->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("\tD->%1 = %2;\n")).arg(myActor->variableList[i]->name).arg(parameter[1]).toUtf8());
        }
        // выход
        outputData.append("\r\n\r\n\treturn result;\r\n}");

        output.setFileName(currentDir.canonicalPath() + "/" + myActor->name + ".cpp");
        output.open(QFile::WriteOnly);
        output.write(outputData);
        output.close();

        break;
    case Actor::InlineType:
        myActor->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        ui->paramsInlineTable->setCurrentCell(-1, -1);
        myActor->name = "A" + getCRC(myActor->extName.toUtf8());
        //генерируем с++ файл

        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int " + myActor->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(myActor->extName);
        for (int i = 0; i < myActor->variableList.count(); i++){
            code.replace(QRegExp("(\\b)" + myActor->variableList[i]->name + "(\\b)", Qt::CaseSensitive), "(D->" + myActor->variableList[i]->name + ")");
        }
        outputData.append("  " + code + "\r\n");
        outputData.append("  return 1;\r\n");
        outputData.append("}\r\n");

        output.setFileName(currentDir.canonicalPath() + "/" + myActor->name + ".cpp");
        output.open(QFile::WriteOnly);
        output.write(outputData);
        output.close();
        break;
    }
}

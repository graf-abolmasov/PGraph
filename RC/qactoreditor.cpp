#include "qactoreditor.h"
#include "ui_qactoreditor.h"
#include "databasemanager.h"
#include "qvariabledialog.h"
#include "commonutils.h"
#include "globalvariables.h"

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
    QList<Variable*> newVariableList;
    QStringList newAMList;
    if (actor == NULL){
        myActor = new Actor("", "", myMode == Normal ? Actor::NormalType : Actor::InlineType, "", newVariableList, newAMList, QImage());
    } else {
        myActor = actor;
        ui->baseModuleList->setEnabled(false);
    }
    //заполняем форму
    switch(myMode){
    case Normal:
        ui->actorNameEdt->setText(myActor->extName);
        //Получаем список базовых модулей
        if (globalDBManager->getRegisteredModules(myModuleList)) {
            ui->baseModuleList->blockSignals(true);
            foreach(BaseModule* baseModule, myModuleList) {
                ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
                if (myActor->baseModule == baseModule->unicName) {
                    ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                    for (int i = 0; i < myModuleList.at(ui->baseModuleList->currentRow())->parameterList.count(); i++){
                        QStringList parameter = myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(i).split(";;");
                        ui->paramsNormalTable->insertRow(i);
                        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
                        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
                        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myActor->variableList.at(i) == NULL ? "N/A" : myActor->variableList.at(i)->name));
                    }
                }
            }
            ui->baseModuleList->blockSignals(false);
        } else QMessageBox::critical(NULL,
                                     QObject::tr("Ошибка"),
                                     QObject::tr("Не удалось получить список базовых модулей.\n") + globalDBManager->lastError().databaseText(),
                                     QMessageBox::Ok);
        ui->actorNameEdt->setFocus();
        break;
    case Inline:
        ui->inlineModuleTxtEdt->blockSignals(true);
        ui->inlineModuleTxtEdt->setPlainText(myActor->extName);
        for (int i = 0; i < myActor->variableList.count(); i++){
            ui->paramsInlineTable->insertRow(i);
            ui->paramsInlineTable->setItem(i, 0, new QTableWidgetItem(myActor->variableList.at(i)->name));
            ui->paramsInlineTable->setItem(i, 1, new QTableWidgetItem(myActor->variableList.at(i)->type));
            ui->paramsInlineTable->setItem(i, 2, new QTableWidgetItem(myActor->varAccModeList.at(i)));
        }
        //Получаем список перемерных
        if (!globalDBManager->getVariableList(myVariableList))
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Не удалось получить список переменных.\n") + globalDBManager->lastError().databaseText(),
                                  QMessageBox::Ok);
        ui->inlineModuleTxtEdt->blockSignals(false);
        ui->inlineModuleTxtEdt->setFocus();
        break;
    }
    QRegExp regExp(tr("[A-Za-z1-9А-Яа-я ]{1,255}"));
    ui->actorNameEdt->setValidator(new QRegExpValidator(regExp, this));
    enableOkButton();
}

Actor* QActorEditor::getResult()
{
    return myActor;
}

void QActorEditor::updateInterface()
{

}

void QActorEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList.at(currentRow)->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);
    myActor->variableList.clear();
    myActor->varAccModeList.clear();
    for (int i = 0; i < myModuleList.at(currentRow)->parameterList.count(); i++){
        myActor->variableList.append(NULL);
        QStringList parameter = myModuleList.at(currentRow)->parameterList.at(i).split(";;");
        myActor->varAccModeList.append(parameter.at(2));
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem("N/A"));
    }
}

void QActorEditor::on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
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
    if (myActor->variableList.at(currentRow) != NULL) {
        ui->descriptionPOLbl_2->setText(myActor->variableList.at(currentRow)->comment);
    }
    ui->descriptionBMLbl_2->setText(myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(currentRow).split(";;").at(3));
}

void QActorEditor::on_varEditBtn_clicked()
{
    QVariableDialog dialog;
    dialog.prepareForm();
    if (dialog.exec()){
        myActor->variableList.replace(ui->paramsNormalTable->currentRow(), dialog.getVariable());
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(dialog.getVariable()->name);
        ui->descriptionPOLbl_2->setText(dialog.getVariable()->comment);
    }
}

void QActorEditor::on_buttonBox_accepted()
{
    QSettings myLocSettings("graph.ini", QSettings::IniFormat);
    QDir currentDir;
    currentDir.setCurrent(myLocSettings.value("Location/BaseDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString());

    QByteArray outputData;
    QFile output;

    switch(myMode){
    case Normal:
        myActor->extName = ui->actorNameEdt->text();
        myActor->baseModule = myModuleList.at(ui->baseModuleList->currentRow())->unicName;
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
    case Inline:
        myActor->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        ui->paramsInlineTable->setCurrentCell(-1, -1);
        myActor->name = "A" + getCRC(myActor->extName.toUtf8());
        //генерируем с++ файл

        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int " + myActor->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(myActor->extName);
        for (int i = 0; i < myActor->variableList.count(); i++){
            code.replace(QRegExp("(\\b)" + myActor->variableList.at(i)->name + "(\\b)", Qt::CaseSensitive), "(D->" + myActor->variableList.at(i)->name + ")");
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

void QActorEditor::on_inlineModuleTxtEdt_textChanged()
{
    ui->paramsInlineTable->clearContents();
    ui->paramsInlineTable->setRowCount(0);
    int p = 0;
    myActor->variableList.clear();
    myActor->varAccModeList.clear();
    for (int i = 0; i < myVariableList.count(); i++){
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList.at(i)->name + "(\\s|\\b|\\W)", Qt::CaseSensitive))){
            ui->paramsInlineTable->insertRow(p);
            myActor->variableList.append(myVariableList.at(i));
            myActor->varAccModeList.append("");
            ui->paramsInlineTable->setItem(p, 0, new QTableWidgetItem(myVariableList.at(i)->name));
            ui->paramsInlineTable->setItem(p, 1, new QTableWidgetItem(myVariableList.at(i)->type));
            ui->paramsInlineTable->setItem(p, 2, new QTableWidgetItem("N/A"));
            p++;
        }
    }
}

void QActorEditor::on_paramsInlineTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
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

void QActorEditor::enableOkButton()
{
    switch(myMode){
    case Normal:
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->actorNameEdt->hasAcceptableInput());
        break;
    case Inline:
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->inlineModuleTxtEdt->document()->toPlainText().isEmpty());
        break;
    }
}

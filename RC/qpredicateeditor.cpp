#include <QtGui>

#include "qpredicateeditor.h"
#include "ui_qpredicateeditor.h"
#include "databasemanager.h"
#include "qvariabledialog.h"
#include "commonutils.h"
#include "globalvariables.h"
#include "predicate.h"
#include "basemodule.h"
#include "variable.h"

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
    myModuleList = globalDBManager->getBaseModuleList();
    switch(tempPre->type){
    case Predicate::NormalType:
        ui->inlineWidget->setVisible(false);
        ui->predicateNameEdt->setText(myPredicate->extName);
        foreach(const BaseModule* baseModule, myModuleList){
            ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
            if (myPredicate != NULL && myPredicate->baseModule == baseModule){
                ui->baseModuleList->blockSignals(true);
                ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                for (int i = 0; i < myModuleList.at(ui->baseModuleList->currentRow())->parameterList.count(); i++){
                    QStringList parameter = myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(i).split(";;");
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
                    ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myPredicate->variableList[i] == NULL ? "N/A" : myPredicate->variableList[i]->name));
                }
                ui->baseModuleList->blockSignals(false);
             }
        }
        break;
    case Predicate::inlineType:
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
        QStringList parameter = myModuleList[currentRow]->parameterList[i].split(";;");
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter[1]));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter[0]));
        ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem("N/A"));
    }
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

    connect(varEditBtn, SIGNAL(clicked()), this, SLOT(on_varEditBtn_clicked()));
    ui->paramsNormalTable->setCellWidget(currentRow, 2, varWidget);

    ui->descriptionPOLbl_2->setText("");
    if (tempPre->variableList[currentRow] != NULL) {
        ui->descriptionPOLbl_2->setText(myPredicate->variableList[currentRow]->comment);
    }
    ui->descriptionBMLbl_2->setText(myModuleList[ui->baseModuleList->currentRow()]->parameterList[currentRow].split(";;")[03]);
}

void QPredicateEditor::on_varEditBtn_clicked()
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
    makeResult();
}

void QPredicateEditor::on_inlineModuleTxtEdt_textChanged()
{
    tempPre->variableList.clear();
    for (int i = 0; i < myVariableList.count(); i++){
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList[i]->name + "(\\s|\\b|\\W)", Qt::CaseSensitive))){
            tempPre->variableList.append(myVariableList.at(i));
        }
    }
}

void QPredicateEditor::makeResult()
{
    QSettings myLocSettings("graph.ini", QSettings::IniFormat);
    QDir currentDir;
    currentDir.setCurrent(myLocSettings.value("Location/BaseDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString());

    QByteArray outputData;
    QFile output;

    switch(tempPre->type){
    case Predicate::NormalType:
        if (ui->predicateNameEdt->text().isEmpty()) {
            QMessageBox::critical(NULL,
                                  QObject::tr("Ошибка"),
                                  QObject::tr("Введие название предиката.\n") + globalDBManager->lastError().databaseText(),
                                  QMessageBox::Ok);
            return;
        }

        tempPre->name = "P" + getCRC(tempPre->extName.toUtf8());
        tempPre->extName = ui->predicateNameEdt->text();
        tempPre->baseModule = myModuleList[ui->baseModuleList->currentRow()];

        // Генерируем актор
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int ");
        outputData.append(tempPre->name.toUtf8());
        outputData.append("(TPOData *D)\r\n");
        outputData.append("{\r\n");
        // Инициализуем данные
        for(int i = 0; i < tempPre->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("\t%1 %2 = D->%3;\r\n")).arg(parameter[0]).arg(parameter[1]).arg(tempPre->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\tint result = ");
        outputData.append(tempPre->baseModule->name);
        outputData.append("(");
        for(int i = 0; i < tempPre->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("&%1, ")).arg(parameter[1]).toUtf8());
        }
        if (tempPre->variableList.count() > 0)
            outputData.remove(outputData.length()-2,2);
        outputData.append(");\r\n\r\n");
        // сохраняем данные
        for(int i = 0; i < tempPre->variableList.count(); i++) {
            QStringList parameter = myModuleList[ui->baseModuleList->currentRow()]->parameterList[i].split(";;");
            outputData.append(QString(tr("\tD->%1 = %2;\n")).arg(tempPre->variableList[i]->name).arg(parameter[1]).toUtf8());
        }
        // выход
        outputData.append("\r\n\r\n\treturn result;\r\n}");
        break;
    case Predicate::inlineType:
        tempPre->name = "P" + getCRC(tempPre->extName.toUtf8());
        tempPre->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        //генерируем с++ файл
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int " + tempPre->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(tempPre->extName);
        for (int i = 0; i < tempPre->variableList.count(); i++){
            code.replace(QRegExp("(\\b)" + tempPre->variableList.at(i)->name + "(\\b)", Qt::CaseSensitive), "D->" + tempPre->variableList.at(i)->name);
        }
        //code.replace(QRegExp("\\n"), "\n  ");
        outputData.append("  return (" + code + ")\r\n");
        outputData.append("}\r\n");
        break;
    }

    output.setFileName(currentDir.canonicalPath() + "/" + tempPre->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData);
    output.close();

    result = const_cast<Predicate *>(myPredicate);
    result->name = tempPre->name;
    result->extName = tempPre->extName;
    result->type = tempPre->type;
    result->baseModule = tempPre->baseModule;
    result->variableList = tempPre->variableList;
    delete tempPre;
}

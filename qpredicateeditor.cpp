#include "qpredicateeditor.h"
#include "ui_qpredicateeditor.h"
#include "databasemanager.h"
#include "qvariabledialog.h"
#include "commonutils.h"

QPredicateEditor::QPredicateEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPredicateEditor)
{
    ui->setupUi(this);
}

QPredicateEditor::~QPredicateEditor()
{
    delete ui;
}

QPredicateEditor::QPredicateEditor(Mode mode, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::QPredicateEditor)
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

void QPredicateEditor::prepareForm(Predicate *pred)
{
    QList<Variable*> newVariableList;
    if (pred == NULL)
        myPredicate = new Predicate("", "", myMode == Normal ? Predicate::normalType : Predicate::inlineType, "",  newVariableList);
    else
        myPredicate = pred;
    //заполняем форму
    switch(myMode){
    case Normal:
        ui->predicateNameEdt->setText(myPredicate->extName);
        globalDBManager->getRegisteredModules(myModuleList);
        foreach(BaseModule* baseModule, myModuleList){
            ui->baseModuleList->insertItem(ui->baseModuleList->count(), baseModule->name);
            if (myPredicate->baseModule == baseModule->unicName){
                ui->baseModuleList->blockSignals(true);
                ui->baseModuleList->setCurrentRow(ui->baseModuleList->count()-1);
                for (int i = 0; i < myModuleList.at(ui->baseModuleList->currentRow())->parameterList.count(); i++){
                    QStringList parameter = myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(i).split(";;");
                    ui->paramsNormalTable->insertRow(i);
                    ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
                    ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
                    ui->paramsNormalTable->setItem(i, 2, new QTableWidgetItem(myPredicate->variableList.at(i) == NULL ? "N/A" : myPredicate->variableList.at(i)->name));
                }
                ui->baseModuleList->blockSignals(false);
             }
        }
        break;
    case Inline:
        globalDBManager->getVariableList(myVariableList);
        ui->inlineModuleTxtEdt->blockSignals(true);
        ui->inlineModuleTxtEdt->setPlainText(myPredicate->extName);
        ui->inlineModuleTxtEdt->blockSignals(false);
        break;
    }
}

Predicate* QPredicateEditor::getResult()
{
    return myPredicate;
}

void QPredicateEditor::on_baseModuleList_currentRowChanged(int currentRow)
{
    ui->descriptionLbl_2->setText(myModuleList.at(currentRow)->comment);
    ui->paramsNormalTable->clearContents();
    ui->paramsNormalTable->setRowCount(0);
    myPredicate->variableList.clear();
    for (int i = 0; i < myModuleList.at(currentRow)->parameterList.count(); i++){
        myPredicate->variableList.append(NULL);
        QStringList parameter = myModuleList.at(currentRow)->parameterList.at(i).split(";;");
        ui->paramsNormalTable->insertRow(i);
        ui->paramsNormalTable->setItem(i, 0, new QTableWidgetItem(parameter.at(1)));
        ui->paramsNormalTable->setItem(i, 1, new QTableWidgetItem(parameter.at(0)));
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
    if (myPredicate->variableList.at(currentRow) != NULL) {
        ui->descriptionPOLbl_2->setText(myPredicate->variableList.at(currentRow)->comment);
    }
    ui->descriptionBMLbl_2->setText(myModuleList.at(ui->baseModuleList->currentRow())->parameterList.at(currentRow).split(";;").at(3));
}

void QPredicateEditor::on_varEditBtn_clicked()
{
    QVariableDialog dialog;
    dialog.prepareForm();
    if (dialog.exec()){
        myPredicate->variableList.replace(ui->paramsNormalTable->currentRow(), dialog.getVariable());
        ui->paramsNormalTable->item(ui->paramsNormalTable->currentRow(), 2)->setText(dialog.getVariable()->name);
        ui->descriptionPOLbl_2->setText(dialog.getVariable()->comment);
    }
}

void QPredicateEditor::on_buttonBox_accepted()
{
    switch(myMode){
    case Normal:
        myPredicate->extName = ui->predicateNameEdt->text();
        myPredicate->baseModule = myModuleList.at(ui->baseModuleList->currentRow())->unicName;
        myPredicate->name = "P" + QString::number(getCRC(myPredicate->extName.toUtf8().data(), sizeof(myPredicate->extName.toUtf8().data())), 16).toUpper();
        break;
    case Inline:
        myPredicate->extName = ui->inlineModuleTxtEdt->document()->toPlainText();
        myPredicate->name = "P" + QString::number(getCRC(myPredicate->extName.toUtf8().data(), sizeof(myPredicate->extName.toUtf8().data())), 16).toUpper();
        //генерируем с++ файл
        QFile output(QApplication::applicationDirPath() + "\\C\\" + myPredicate->name + ".cpp");
        output.open(QFile::WriteOnly);
        QByteArray outputData;
        outputData.append("#include \"stype.h\"\r\n");
        outputData.append("int " + myPredicate->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(myPredicate->extName);
        for (int i = 0; i < myPredicate->variableList.count(); i++){
            code.replace(QRegExp("(\\b)" + myPredicate->variableList.at(i)->name + "(\\b)", Qt::CaseSensitive), "D->" + myPredicate->variableList.at(i)->name);
        }
        //code.replace(QRegExp("\\n"), "\n  ");
        outputData.append("  " + code + "\r\n");
        outputData.append("  return 1;\r\n");
        outputData.append("}\r\n");
        output.write(outputData);
        output.close();
        break;
    }
}

void QPredicateEditor::on_inlineModuleTxtEdt_textChanged()
{
    myPredicate->variableList.clear();
    for (int i = 0; i < myVariableList.count(); i++){
        if (ui->inlineModuleTxtEdt->document()->toPlainText().contains(QRegExp("(\\s|\\b|\\W)" + myVariableList.at(i)->name + "(\\s|\\b|\\W)", Qt::CaseSensitive))){
            myPredicate->variableList.append(myVariableList.at(i));
        }
    }
}

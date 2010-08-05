#include "qmoduleregister.h"
#include "ui_qmoduleregister.h"
#include "databasemanager.h"
#include "commonutils.h"


QModuleRegister::QModuleRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QModuleRegister)
{
    ui->setupUi(this);
    ui->parametersTable->setColumnWidth(0, 70);
    ui->parametersTable->setColumnWidth(1, 60);
    ui->parametersTable->setColumnWidth(3, 90);

    QStringList filters;
    filters << "*.c" << "*.C";
    workingDir.setNameFilters(filters);
    workingDir.setFilter(QDir::Files);
    workingDir.setCurrent(QApplication::applicationDirPath() + "\\C\\");
    fileList = workingDir.entryInfoList();
}

QModuleRegister::~QModuleRegister()
{
    delete ui;
}

void QModuleRegister::changeEvent(QEvent *e)
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

void QModuleRegister::prepareForm()
{
    QStringList moduleList;
    globalDBManager->getRegisteredModules(moduleList);
    int i = 0;
    while (i < fileList.count()){
        //если уже зарегистрирован, то удаляем его из списка файлов
        if (moduleList.contains(fileList.at(i).baseName())){
            fileList.removeAt(i);
            continue;
        }
        //иначе парсим и добавляем на форму
        QFile file(fileList.at(i).absoluteFilePath());
        file.open(QFile::ReadOnly);
        QString buff(file.readAll());
        if (buff.contains(fileList.at(i).baseName(), Qt::CaseSensitive))
            ui->fileList->addItem(fileList.at(i).fileName());
        file.close();
        i++;
    }
}

void QModuleRegister::on_fileList_currentRowChanged(int currentRow)
{
    ui->parametersTable->clearContents();
    ui->parametersTable->setRowCount(0);
    QFile file(fileList.at(currentRow).absoluteFilePath());
    file.open(QFile::ReadOnly);
    QString buff(file.readAll());
    int start = buff.indexOf(fileList.at(currentRow).baseName() + "(", 0, Qt::CaseSensitive) + fileList.at(currentRow).baseName().length() + 1;
    int end   = buff.indexOf(")", start, Qt::CaseSensitive);
    QString signature(buff.mid(start, end-start));
    QStringList paramsList = signature.split(QRegExp(",{1,}\\s*"));
    for (int i = 0; i < paramsList.count(); i++){
        ui->parametersTable->insertRow(i);
        QString paramName = paramsList.at(i).split(QRegExp("\\s{1,}")).at(1);
        QString paramType = paramsList.at(i).split(QRegExp("\\s{1,}")).at(0);
        ui->parametersTable->setItem(i, 0, new QTableWidgetItem(paramName));
        ui->parametersTable->setItem(i, 1, new QTableWidgetItem(paramType));
        ui->parametersTable->setItem(i, 2, new QTableWidgetItem(" "));
        ui->parametersTable->setItem(i, 3, new QTableWidgetItem(" "));
    }
    file.close();
}

void QModuleRegister::on_parametersTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1){
        ui->parametersTable->setCellWidget(previousRow, 2, NULL);
        //ui->parametersTable->setCellWidget(previousRow, 3, NULL);
        ui->parametersTable->setItem(previousRow, 2, new QTableWidgetItem(paramTypeCmbBox->currentText()));
        //ui->parametersTable->setItem(previousRow, 3, new QTableWidgetItem(paramCommentEdt->document()->toPlainText()));
        //delete paramCommentEdt;
        delete paramTypeCmbBox;
    }
    /*paramCommentEdt = new QTextEdit(ui->parametersTable);
    if (ui->parametersTable->item(currentRow, 3) != NULL)
        paramCommentEdt->setPlainText(ui->parametersTable->item(currentRow, 3)->text());
    ui->parametersTable->setCellWidget(currentRow, 3, paramCommentEdt);*/
    paramTypeCmbBox = new QComboBox(ui->parametersTable);
    paramTypeCmbBox->addItem(tr("Исходный"));
    paramTypeCmbBox->addItem(tr("Модифицируемый"));
    paramTypeCmbBox->addItem(tr("Вычисляемый"));
    if (ui->parametersTable->item(currentRow, 2) != NULL)
        paramTypeCmbBox->setCurrentIndex(paramTypeCmbBox->findText(ui->parametersTable->item(currentRow, 2)->text()));
    ui->parametersTable->setCellWidget(currentRow, 2, paramTypeCmbBox);
}

void QModuleRegister::on_buttonBox_accepted()
{
    QStringList paramList;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        paramList.append(ui->parametersTable->item(i, 0)->text() + ";;" +
                         ui->parametersTable->item(i, 1)->text() + ";;" +
                         ui->parametersTable->item(i, 2)->text() + ";;" +
                         ui->parametersTable->item(i, 3)->text());
    }

    globalDBManager->registerModule("S" + QString::number(getCRC(ui->fileList->currentItem()->text().toAscii().data(), sizeof(ui->fileList->currentItem()->text().toAscii().data())), 16).toUpper(),
                                    fileList.at(ui->fileList->currentRow()).baseName(),
                                    ui->commentTxtEdt->document()->toPlainText(), paramList);

}


void QModuleRegister::on_buttonBox_clicked(QAbstractButton* button)
{
    ui->parametersTable->setCurrentCell(-1, -1);
    bool readyToSave = (ui->parametersTable->rowCount() > 0);
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        if ((ui->parametersTable->item(i, 0)->text() == "") &&
            (ui->parametersTable->item(i, 1)->text() == "") &&
            (ui->parametersTable->item(i, 2)->text() == "")){
            readyToSave = false;
            break;
        }
    }

    if (readyToSave) accept();
}

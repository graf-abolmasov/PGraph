#include <QtCore>
#include <QtGui>

#include "qmoduleregister.h"
#include "ui_qmoduleregister.h"
#include "databasemanager.h"
#include "commonutils.h"
#include "basemodule.h"
#include "globalvariables.h"

QModuleRegister::QModuleRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QModuleRegister)
{
    ui->setupUi(this);
    ui->parametersTable->setColumnWidth(0, 70);
    ui->parametersTable->setColumnWidth(1, 60);
    ui->parametersTable->setColumnWidth(3, 90);

    QStringList filters;
    filters << tr("*.c") << tr("*.C");
    workingDir.setNameFilters(filters);
    workingDir.setFilter(QDir::Files);
    QSettings myLocSettings("graph.ini", QSettings::IniFormat);
    workingDir.setCurrent(myLocSettings.value("Location/BaseDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString());
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
    QList<BaseModule*> tempList;
    if (!globalDBManager->getRegisteredModules(tempList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось получить список базовых модулей.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);;
    for (int i = 0; i < tempList.count(); i++)
        moduleList.append(tempList.at(i)->name);
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



void QModuleRegister::on_parametersTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1){
        if (previousColumn == 2){
            ui->parametersTable->setCellWidget(previousRow, 2, NULL);
            ui->parametersTable->item(previousRow, 2)->setText(paramTypeCmbBox->currentText());
            delete paramTypeCmbBox;
        }
        if (previousColumn == 3){
            ui->parametersTable->setCellWidget(previousRow, 3, NULL);
            ui->parametersTable->item(previousRow, 3)->setText(paramCommentEdt->document()->toPlainText());
            delete paramCommentEdt;
        }
    }

    if (currentColumn == 2){
        paramTypeCmbBox = new QComboBox(ui->parametersTable);
        paramTypeCmbBox->addItem(tr("Исходный"));
        paramTypeCmbBox->addItem(tr("Модифицируемый"));
        paramTypeCmbBox->addItem(tr("Вычисляемый"));
        if (ui->parametersTable->item(currentRow, 2) != NULL)
            paramTypeCmbBox->setCurrentIndex(paramTypeCmbBox->findText(ui->parametersTable->item(currentRow, 2)->text()));
        ui->parametersTable->setCellWidget(currentRow, 2, paramTypeCmbBox);
    }

    if (currentColumn == 3){
        paramCommentEdt = new QTextEdit(ui->parametersTable);
        if (ui->parametersTable->item(currentRow, 3) != NULL)
            paramCommentEdt->setPlainText(ui->parametersTable->item(currentRow, 3)->text());
        ui->parametersTable->setCellWidget(currentRow, 3, paramCommentEdt);
    }
}

void QModuleRegister::on_buttonBox_accepted()
{

    ui->parametersTable->setCurrentCell(-1, -1);
    bool readyToSave = true;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        if ((ui->parametersTable->item(i, 0)->text().isEmpty()) ||
            (ui->parametersTable->item(i, 1)->text().isEmpty()) ||
            (ui->parametersTable->item(i, 2)->text().isEmpty())){
            readyToSave = false;
            break;
        }
    }

    if (!readyToSave) {
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Необходимо указать класс и коментарий к переменной.\n"),
                              QMessageBox::Ok);
        return;
    }

    QStringList paramList;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        paramList.append(ui->parametersTable->item(i, 0)->text() + ";;" +
                         ui->parametersTable->item(i, 1)->text() + ";;" +
                         ui->parametersTable->item(i, 2)->text() + ";;" +
                         ui->parametersTable->item(i, 3)->text());
    }

    if (ui->fileList->count() == 0)
        return;

    QString uniqName = "S" + getCRC(ui->fileList->currentItem()->text().toUtf8());
    if (!globalDBManager->registerModule(uniqName,
                                    fileList.at(ui->fileList->currentRow()).baseName(),
                                    ui->commentTxtEdt->document()->toPlainText(), paramList))
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось зарегистрировать модуль.\n") + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);

    // Читаем файл
    QFile input(fileList.at(ui->fileList->currentRow()).absoluteFilePath());
    input.open(QFile::ReadOnly);
    QString buff(input.readAll());
    input.close();

    // Ищим заголовок (без регэкспа способ ужасен :((
    int start = buff.indexOf(fileList.at(ui->fileList->currentRow()).baseName() + "(", 0, Qt::CaseSensitive) + fileList.at(ui->fileList->currentRow()).baseName().length() + 1;
    int end   = buff.indexOf(")", start, Qt::CaseSensitive);
    QString signature(buff.mid(start, end-start));
    QByteArray outputData;
    outputData.append("#include \"graph.h\"\r\nextern int ");
    outputData.append(fileList.at(ui->fileList->currentRow()).baseName() + "(" + signature + ");\r\n");
    outputData.append("int " + uniqName + "(" + signature + ")\r\n{\r\n");

    outputData.append("\treturn " + fileList.at(ui->fileList->currentRow()).baseName() + "(");
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        QString paramName = ui->parametersTable->item(i, 0)->text();
        outputData.append(paramName + ", ");
    }
    if (ui->parametersTable->rowCount() > 0)
        outputData.remove(outputData.size()-2, 2);
    outputData.append(");\r\n");
    outputData.append("}\r\n");
    QFile output(uniqName + ".CPP");
    output.open(QFile::WriteOnly);
    output.write(outputData);
    output.close();
}

void QModuleRegister::on_fileList_currentRowChanged(int currentRow)
{
    ui->parametersTable->clearContents();
    ui->parametersTable->setRowCount(0);
    QFile file(fileList.at(currentRow).absoluteFilePath());
    file.open(QFile::ReadOnly);
    QString buff(file.readAll());
    file.close();
    int start = buff.indexOf(fileList.at(currentRow).baseName() + "(", 0, Qt::CaseSensitive) + fileList.at(currentRow).baseName().length() + 1;
    int end   = buff.indexOf(")", start, Qt::CaseSensitive);
    QString signature(buff.mid(start, end-start));
    if (signature.isEmpty()) return;
    QStringList paramsList = signature.split(QRegExp(",{1,}\\s*"));
    for (int i = 0; i < paramsList.count(); i++){
        ui->parametersTable->insertRow(i);
        QString paramName = paramsList.at(i).split(QRegExp("\\*?\\s{1,}\\*?")).at(1);
        QString paramType = paramsList.at(i).split(QRegExp("\\*?\\s{1,}\\*?")).at(0);
        ui->parametersTable->setItem(i, 0, new QTableWidgetItem(paramName));
        ui->parametersTable->setItem(i, 1, new QTableWidgetItem(paramType));
        ui->parametersTable->setItem(i, 2, new QTableWidgetItem(""));
        ui->parametersTable->setItem(i, 3, new QTableWidgetItem(""));
    }
}

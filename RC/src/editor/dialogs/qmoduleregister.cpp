#include <QtCore>
#include <QtGui>

#include "qmoduleregister.h"
#include "ui_qmoduleregister.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/VO/basemodule.h"
#include "../../src/common/globalvariables.h"
#include "../../src/common/qgraphsettings.h"

QModuleRegister::QModuleRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QModuleRegister)
{
    ui->setupUi(this);
    ui->parametersTable->setColumnWidth(0, 70);
    ui->parametersTable->setColumnWidth(1, 60);
    ui->parametersTable->setColumnWidth(3, 90);
    myBaseDirectory = QGraphSettings::getBaseDirectory();
    QDir workingDir(myBaseDirectory);

    QStringList filters;
    filters << tr("*.c") << tr("*.C");
    workingDir.setNameFilters(filters);
    workingDir.setFilter(QDir::Files);
    fileList = workingDir.entryInfoList();
    prepareForm();
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
    QList<const BaseModule *> tempList;
    tempList = globalDBManager->getBaseModuleList();

    for (int i = 0; i < tempList.count(); i++)
        moduleList.append(tempList[i]->name);
    int i = 0;
    while (i < fileList.count()){
        //если уже зарегистрирован, то удаляем его из списка файлов
        if (moduleList.contains(fileList[i].baseName())){
            fileList.removeAt(i);
            continue;
        }
        //иначе парсим и добавляем на форму
        QFile file(fileList.at(i).absoluteFilePath());
        file.open(QFile::ReadOnly);
        QString buff(file.readAll());
        if (buff.contains(fileList.at(i).baseName(), Qt::CaseSensitive))
            ui->fileList->addItem(fileList[i].fileName());
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
    if (ui->fileList->count() == 0) {
        accept();
        return;
    }

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

    QList<BaseModuleParameter> paramList;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        paramList.append(BaseModuleParameter(ui->parametersTable->item(i, 2)->text(),
                                             ui->parametersTable->item(i, 0)->text(),
                                             ui->parametersTable->item(i, 1)->text(),
                                             ui->parametersTable->item(i, 3)->text()));
    }

    QString uniqName = "S" + getCRC(ui->fileList->currentItem()->text().toUtf8());
    const BaseModule *newBaseModule = new BaseModule(fileList.at(ui->fileList->currentRow()).baseName(),
                                                     uniqName, ui->commentTxtEdt->document()->toPlainText(), paramList);
    globalDBManager->registerModuleDB(newBaseModule);
    // Читаем файл
    QFile input(fileList.at(ui->fileList->currentRow()).canonicalFilePath());
    input.open(QFile::ReadOnly);
    QString buff(input.readAll());
    input.close();

    const QString function = fileList.at(ui->fileList->currentRow()).baseName();
    QRegExp rx(function + "\\((.*)\\)");
    rx.setMinimal(true);
    rx.indexIn(buff, 0);
    QString signature = rx.cap(1).simplified();
    QByteArray outputData;
    outputData.append("#include \"utype.h\"\r\n");
    rx.setPattern("(#include\\s*[\"<].*[\">])");
    QStringList includes;
    int pos = 0;
    while ((pos = rx.indexIn(buff, pos)) != -1) {
        includes << rx.cap(1);
        pos += rx.matchedLength();
    }
    outputData.append(includes.join("\r\n").toUtf8());
    outputData.append("PROJECT_BEGIN_NAMESPACE\r\n");
    outputData.append("\r\nextern \"C\" int ");
    outputData.append(function + "(" + signature + ");\r\n");
    outputData.append("int " + uniqName + "(" + signature + ")\r\n{\r\n");

    outputData.append("\treturn " + function + "(");
    QStringList params;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++)
        params << ui->parametersTable->item(i, 0)->text();
    outputData.append(params.join(", ").toUtf8());
    outputData.append(");\r\n");
    outputData.append("}\r\n");
    outputData.append("PROJECT_END_NAMESPACE\r\n");

    QFile output(myBaseDirectory + "/" + uniqName + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData);
    output.close();

    accept();
}

void QModuleRegister::on_fileList_currentRowChanged(int currentRow)
{
    ui->parametersTable->clearContents();
    ui->parametersTable->setRowCount(0);
    QFile file(fileList.at(currentRow).absoluteFilePath());
    file.open(QFile::ReadOnly);
    QString buff(file.readAll());
    file.close();
    const QString function = fileList.at(currentRow).baseName();
    int start = buff.indexOf(function + "(", 0, Qt::CaseSensitive) + function.length() + 1;
    int end   = buff.indexOf(")", start, Qt::CaseSensitive);
    QString signature(buff.mid(start, end-start));
    if (signature.isEmpty())
        return;
    QStringList paramsList = signature.split(QRegExp(",{1,}\\s*"));
    for (int i = 0; i < paramsList.count(); i++){
        ui->parametersTable->insertRow(i);
        const QStringList param = paramsList.at(i).split(QRegExp("(\\s*\\*)|(\\*\\s*)"));
        QString paramName = param.at(1).simplified();
        QString paramType = param.at(0).simplified();
        ui->parametersTable->setItem(i, 0, new QTableWidgetItem(paramName));
        const QString accMode = paramType.contains("const") ? tr("Исходный") : tr("Модифицируемый");
        ui->parametersTable->setItem(i, 1, new QTableWidgetItem(paramType.remove(QRegExp("const")).simplified()));
        ui->parametersTable->setItem(i, 2, new QTableWidgetItem(accMode));
        ui->parametersTable->setItem(i, 3, new QTableWidgetItem(""));
    }
}

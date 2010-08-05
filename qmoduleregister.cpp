#include "qmoduleregister.h"
#include "ui_qmoduleregister.h"


QModuleRegister::QModuleRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QModuleRegister)
{
    ui->setupUi(this);

    QStringList filters;
    filters << "*.c" << "*.C";
    workingDir.setNameFilters(filters);
    workingDir.setFilter(QDir::Files);
    workingDir.setCurrent(QApplication::applicationDirPath() + "\\C\\");
    fileList = workingDir.entryInfoList();
   //paramTypeCmbBox = new QComboBox(ui->parametersTable);

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
    for (int i = 0; i < fileList.count(); i++){
        QFile file(fileList.at(i).absoluteFilePath());
        file.open(QFile::ReadOnly);
        QString buff(file.readAll());
        if (buff.contains(fileList.at(i).baseName(), Qt::CaseSensitive))
            ui->fileList->addItem(fileList.at(i).fileName());
        file.close();
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
    }
    return;

}

void QModuleRegister::on_parametersTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (previousRow != -1){
        ui->parametersTable->setCellWidget(previousRow, 2, NULL);
        ui->parametersTable->setItem(previousRow, 2, new QTableWidgetItem(paramTypeCmbBox->currentText()));
        delete paramTypeCmbBox;
    }
    paramTypeCmbBox = new QComboBox(ui->parametersTable);
    paramTypeCmbBox->addItem(tr("Исходный"));
    paramTypeCmbBox->addItem(tr("Модифицируемый"));
    paramTypeCmbBox->addItem(tr("Вычисляемый"));
    if (ui->parametersTable->item(currentRow, 2) != NULL)
        paramTypeCmbBox->setCurrentIndex(paramTypeCmbBox->findText(ui->parametersTable->item(currentRow, 2)->text()));
    ui->parametersTable->setCellWidget(currentRow, 2, paramTypeCmbBox);

}

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

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

    QList<BaseModuleParameter> paramList;
    for (int i = 0; i < ui->parametersTable->rowCount(); i++){
        paramList.append(BaseModuleParameter(ui->parametersTable->item(i, 2)->text(),
                                             ui->parametersTable->item(i, 0)->text(),
                                             ui->parametersTable->item(i, 1)->text(),
                                             ui->parametersTable->item(i, 3)->text()));
    }

    const QString baseName = myFileInfo.baseName();
    QString uniqName = "S" + getCRC(baseName.toUtf8());
    QFile file(myFileInfo.canonicalFilePath());
    file.open(QFile::ReadOnly);
    const BaseModule *newBaseModule = new BaseModule(baseName, uniqName, ui->commentTxtEdt->document()->toPlainText(), paramList, QString(file.readAll()));
    file.close();
    globalDBManager->registerModuleDB(newBaseModule);
    accept();
}

void QModuleRegister::on_selectFileButton_clicked()
{
    QString fileName;
    if (!(fileName = QFileDialog::getOpenFileName(this, tr("Выберите файл бвазового модуля"), "", "C/C++ Sources (*.c .*C);;Any (*.*)")).isEmpty()) {
        myFileInfo = QFileInfo(fileName);
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        QString buff(file.readAll());
        file.close();
        const QString function = myFileInfo.baseName();
        const int start = buff.indexOf(function + "(", 0, Qt::CaseSensitive) + function.length() + 1;
        const int end   = buff.indexOf(")", start, Qt::CaseSensitive);
        const QString signature(buff.mid(start, end-start));
        if (signature.isEmpty()) {
            QMessageBox::warning(this, tr(ERR_TITLE), tr("Неправильный файл базового модуля"));
            return;
        }
        QStringList paramsList = signature.split(QRegExp(",{1,}\\s*"));
        for (int i = 0; i < paramsList.count(); i++) {
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
        ui->fileNamelabel->setText(myFileInfo.canonicalFilePath());
        ui->commentTxtEdt->setEnabled(true);
        ui->commentLbl->setEnabled(true);
        ui->parametersLbl->setEnabled(true);
        ui->parametersTable->setEnabled(true);
    } else {
        ui->commentTxtEdt->setEnabled(false);
        ui->commentLbl->setEnabled(false);
        ui->parametersLbl->setEnabled(false);
        ui->parametersTable->setEnabled(false);
    }
}

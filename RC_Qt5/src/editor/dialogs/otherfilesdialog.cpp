#include "../../src/editor/dialogs/otherfilesdialog.h"
#include "../../src/common/globalvariables.h"
#include "ui_otherfilesdialog.h"

OtherFilesDialog::OtherFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherFilesDialog)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("othfls");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setFilter(QString("project_id=%1").arg(QString::number(globalDBManager->getProjectId())));
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("Название"));

    ui->filesTable->setModel(model);
    ui->filesTable->hideColumn(1);
}

OtherFilesDialog::~OtherFilesDialog()
{
    delete ui;
    delete model;
}

void OtherFilesDialog::on_addButton_clicked()
{
    QSqlField project_id("PROJECT_ID", QVariant::Int);
    project_id.setValue(globalDBManager->getProjectId());
    QSqlRecord newRec;
    newRec.append(project_id);
    model->insertRecord(0, newRec);
}

void OtherFilesDialog::on_removeButton_clicked()
{
    model->removeRow(ui->filesTable->selectionModel()->currentIndex().row());
    ui->filesTable->selectRow(-1);
}

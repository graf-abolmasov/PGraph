#include "projectdialog.h"
#include <QtWidgets>
#include "ui_projectdialog.h"
#include "../../src/common/VO/graphvo.h"
#include "../../src/common/globalvariables.h"

ProjectDialog::ProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDialog)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("project");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    model->setHeaderData(1, Qt::Horizontal, tr("Project name"));

    ui->projectsTable->setModel(model);
    ui->projectsTable->hideColumn(0);
    ui->projectsTable->selectRow(0);
}

ProjectDialog::~ProjectDialog()
{
    delete ui;
    delete model;
}

void ProjectDialog::on_createProjectBtn_clicked()
{
    QSqlRecord newRec;
    QSqlQuery q("select max(project_id) from project;");
    q.exec();
    q.next();
    const int max = q.value(0).toInt();
    QSqlField f("PROJECT_NAME", QVariant::String);
    f.setValue(tr("New project %1").arg(QString::number(max)));
    newRec.append(f);
    model->insertRecord(0, newRec);
}

bool ProjectDialog::openProject()
{
    model->database().transaction();
    model->database().commit();
    const int cr = currentRow();
    if (cr < 0) {
        QMessageBox::warning(this, tr("Error"), tr("Select project"));
        return false;
    }
    const int projectId = model->record(cr).value("PROJECT_ID").toInt();
    globalDBManager->openProjectDB(projectId);
    return true;
}

void ProjectDialog::on_buttonBox_accepted()
{
    if (openProject())
        accept();
}

void ProjectDialog::on_delProjectBtn_clicked()
{
    const int cr = currentRow();
    const QString projectName = model->record(cr).value("PROJECT_NAME").toString();

    if (QMessageBox::question(this, tr("Question"),
                              tr("Do you want to remove project - \"%1\"?").arg(projectName),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::No) == QMessageBox::Yes) {
        ui->projectsTable->selectRow(-1);
        ui->projectsTable->selectionModel()->reset();
        ui->projectsTable->hideRow(cr);
        model->removeRow(cr);
    }
}

int ProjectDialog::currentRow() const
{
    return ui->projectsTable->selectionModel()->currentIndex().row();
}

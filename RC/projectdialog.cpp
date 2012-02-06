#include "projectdialog.h"
#include "ui_projectdialog.h"
#include "graph.h"
#include "globalvariables.h"

ProjectDialog::ProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDialog)
{
    ui->setupUi(this);
    QList<Project> projects = globalDBManager->getProjectListDB();
    ui->projectsTable->setRowCount(projects.length());
    int i = 0;
    foreach (Project project, projects) {
        QTableWidgetItem *newItem = new QTableWidgetItem(project.name);
        newItem->setData(Qt::UserRole, project.id);
        ui->projectsTable->setItem(i++, 0, newItem);
    }

}

ProjectDialog::~ProjectDialog()
{
    delete ui;
}

void ProjectDialog::on_openProjectBtn_clicked()
{
    on_buttonBox_accepted();
    accept();
}

void ProjectDialog::on_buttonBox_accepted()
{
    int projectId = ui->projectsTable->currentItem()->data(Qt::UserRole).toInt();
    globalDBManager->openProjectDB(projectId);
}

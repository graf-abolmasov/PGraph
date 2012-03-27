#include "projectdialog.h"
#include "ui_projectdialog.h"
#include "../../src/common/VO/graph.h"
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

    model->setHeaderData(1, Qt::Horizontal, tr("Название"));

    ui->projectsTable->setModel(model);
    ui->projectsTable->hideColumn(0);
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
    f.setValue(tr("Новый проект %1").arg(QString::number(max)));
    newRec.append(f);
    model->insertRecord(-1, newRec);
}

void ProjectDialog::on_projectsTable_doubleClicked(const QModelIndex &index)
{
//    openProject();
}

void ProjectDialog::openProject()
{
    model->database().transaction();
    model->database().commit();
    const int cr = currentRow();
    if (cr < 0) {
        QMessageBox::warning(this, tr(ERR_TITLE), tr("Выберите проект"));
        return;
    }
    const int projectId = model->record(cr).value("PROJECT_ID").toInt();
    globalDBManager->openProjectDB(projectId);
    accept();
}

void ProjectDialog::on_buttonBox_accepted()
{
    openProject();
}

void ProjectDialog::on_delProjectBtn_clicked()
{
    model->removeRow(currentRow());
    ui->projectsTable->selectRow(-1);
}

int ProjectDialog::currentRow() const
{
    return ui->projectsTable->selectionModel()->currentIndex().row();
}

void ProjectDialog::on_cloneProjectBtn_clicked()
{
    const int cr = currentRow();
    if (cr < 0) {
        QMessageBox::warning(this, tr(ERR_TITLE), tr("Выберите проект"));
        return;
    }
    const int projectId = model->record(cr).value("PROJECT_ID").toInt();
    const QString projectName = model->record(cr).value("PROJECT_NAME").toString() + tr(" (копия)");
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    globalDBManager->cloneProjectDB(projectId, projectName, QString(), QString());
    QApplication::restoreOverrideCursor();
}

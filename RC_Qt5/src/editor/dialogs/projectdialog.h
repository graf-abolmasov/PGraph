#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QtGui>
#include <QtSql>
#include <QDialog>

namespace Ui {
    class ProjectDialog;
}

class ProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectDialog(QWidget *parent = 0);
    ~ProjectDialog();

private slots:
    void on_createProjectBtn_clicked();

    void on_projectsTable_doubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_delProjectBtn_clicked();

    void on_cloneProjectBtn_clicked();

private:
    Ui::ProjectDialog *ui;

    QSqlTableModel *model;

    void openProject();
    int currentRow() const;
};

#endif // PROJECTDIALOG_H

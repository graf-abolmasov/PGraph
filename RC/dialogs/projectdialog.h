#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QtGui>

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
    void on_openProjectBtn_clicked();
    void on_buttonBox_accepted();

    void on_cloneProjectBtn_clicked();

private:
    Ui::ProjectDialog *ui;
};

#endif // PROJECTDIALOG_H

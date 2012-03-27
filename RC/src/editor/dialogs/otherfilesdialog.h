#ifndef OTHERFILESDIALOG_H
#define OTHERFILESDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
class OtherFilesDialog;
}

class OtherFilesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OtherFilesDialog(QWidget *parent = 0);
    ~OtherFilesDialog();
    
private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

private:
    Ui::OtherFilesDialog *ui;

    QSqlTableModel *model;
};

#endif // OTHERFILESDIALOG_H

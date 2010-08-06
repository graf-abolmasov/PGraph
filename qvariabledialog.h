#ifndef QVARIABLEDIALOG_H
#define QVARIABLEDIALOG_H

#include <QDialog>
#include "variable.h"
#include <QtCore>

namespace Ui {
    class QVariableDialog;
}

class QVariableDialog : public QDialog {
    Q_OBJECT
public:
    explicit QVariableDialog(QWidget *parent = 0);
    ~QVariableDialog();
    void prepareForm();
    Variable* getVariable();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableDialog *ui;
    QList<Variable* > myVariableList;


private slots:
    void on_buttonBox_accepted();
    void on_variablesTable_doubleClicked(QModelIndex index);
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_newButton_clicked();
};

#endif // QVARIABLEDIALOG_H

#ifndef QVARIABLEDIALOG_H
#define QVARIABLEDIALOG_H

#include <QDialog>
#include "variable.h"
#include <QtCore>
#include <QtGui>

namespace Ui {
    class QVariableDialog;
}

class QVariableDialog : public QDialog {
    Q_OBJECT
public:
    explicit QVariableDialog(QWidget *parent = 0);
    ~QVariableDialog();
    void prepareForm(QString filter = "");
    Variable* getVariable();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableDialog *ui;
    QList<Variable* > myVariableList;
    QComboBox* varTypeCmbBox;
    QList<DataType*> myTypeList;
    Variable* myVar;

private slots:
    void on_variablesTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_variablesTable_cellChanged(int row, int column);
    void on_buttonBox_accepted();
    //void on_variablesTable_doubleClicked(QModelIndex index);
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_newButton_clicked();
};

#endif // QVARIABLEDIALOG_H

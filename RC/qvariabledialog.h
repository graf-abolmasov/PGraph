#ifndef QVARIABLEDIALOG_H
#define QVARIABLEDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QList>

namespace Ui {
    class QVariableDialog;
}

QT_BEGIN_NAMESPACE
class QComboBox;
class QEvent;
QT_END_NAMESPACE

class Variable;
class DataType;

class QVariableDialog : public QDialog {
    Q_OBJECT
public:
    explicit QVariableDialog(QWidget *parent = 0);
    ~QVariableDialog();
    const Variable *getVariable() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableDialog *ui;
    QList<const Variable *> myVariableList;
    QList<const DataType *> myTypeList;
    QComboBox *varTypeCmbBox;
    const Variable *myVariable;

private slots:
    void on_variablesTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_variablesTable_cellChanged(int row, int column);
    void on_buttonBox_accepted();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_newButton_clicked();
};

#endif // QVARIABLEDIALOG_H

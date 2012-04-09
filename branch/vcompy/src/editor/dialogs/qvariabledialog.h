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
    QVariableDialog(const bool &editOnDblClick = false, QWidget *parent = 0);
    ~QVariableDialog();
    const Variable *getVariable() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableDialog *ui;
    QList<const Variable *> myVariableList;
    QList<const Variable *> myVariableListMod;
    QList<const Variable *> myVariableListDel;
    QList<const DataType *> myTypeList;
    QComboBox *varTypeCmbBox;
    const Variable *myVariable;

private slots:
    void variablesTableCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_variablesTable_cellChanged(int row, int column);
    void on_buttonBox_accepted();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_newButton_clicked();
    void tableDblClick(int row, int column);
};

#endif // QVARIABLEDIALOG_H

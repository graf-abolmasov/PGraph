#ifndef QVARIABLEEDITOR_H
#define QVARIABLEEDITOR_H

#include <QtGui/QDialog>

namespace Ui {
    class QVariableEditor;
}

class Variable;
class DataType;

class QVariableEditor : public QDialog {
    Q_OBJECT
public:

    explicit QVariableEditor(QWidget *parent = 0);
    ~QVariableEditor();

    void prepareForm(Variable *var);
    Variable *getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableEditor *ui;

    Variable *myVariable;
    QList<DataType *> typeList;

private slots:
    void on_pushButton_clicked();
    void enableOkButton();
};

#endif // QVARIABLEEDITOR_H

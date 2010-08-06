#ifndef QVARIABLEEDITOR_H
#define QVARIABLEEDITOR_H
#include "variable.h"

#include <QDialog>

namespace Ui {
    class QVariableEditor;
}

class QVariableEditor : public QDialog {
    Q_OBJECT
public:

    explicit QVariableEditor(QWidget *parent = 0);
    ~QVariableEditor();

    void prepareForm(Variable *var);
    Variable* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableEditor *ui;

    Variable* myVariable;
    QList<DataType*> typeList;
    void updateInterface();

private slots:
    void on_pushButton_clicked();
};

#endif // QVARIABLEEDITOR_H

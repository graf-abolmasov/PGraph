#ifndef QVARIABLEEDITOR_H
#define QVARIABLEEDITOR_H

#include <QDialog>

namespace Ui {
    class QVariableEditor;
}

class Variable;
class DataType;

class QVariableEditor : public QDialog {
    Q_OBJECT
public:

    static QVariableEditor *getCreator();
    static QVariableEditor *getEditor(const Variable *var);

    ~QVariableEditor();

    const Variable *getResult() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableEditor *ui;
    const Variable *myVariable;
    Variable *result;
    QList<const DataType *> myTypeList;

    void makeResult();

    explicit QVariableEditor(QWidget *parent = 0);
    QVariableEditor(const Variable *var, QWidget *parent = 0);
private slots:
    void enableOkButton();
    void on_buttonBox_accepted();
};

#endif // QVARIABLEEDITOR_H

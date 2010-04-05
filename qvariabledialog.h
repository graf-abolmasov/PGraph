#ifndef QVARIABLEDIALOG_H
#define QVARIABLEDIALOG_H

#include <QDialog>

namespace Ui {
    class QVariableDialog;
}

class QVariableDialog : public QDialog {
    Q_OBJECT
public:
    explicit QVariableDialog(QWidget *parent = 0);
    ~QVariableDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QVariableDialog *ui;

    void prepareForm();

private slots:
    void on_editButton_clicked();
    void on_newButton_clicked();
};

#endif // QVARIABLEDIALOG_H

#ifndef QDATATYPEDIALOG_H
#define QDATATYPEDIALOG_H

#include <QDialog>

namespace Ui {
    class QDataTypeDialog;
}

class QDataTypeDialog : public QDialog {
    Q_OBJECT
public:
    explicit QDataTypeDialog(QWidget *parent = 0);
    ~QDataTypeDialog();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeDialog *ui;
    bool prepareForm();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // QDATATYPEDIALOG_H

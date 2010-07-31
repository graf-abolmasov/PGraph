#ifndef QDATATYPEDIALOG_H
#define QDATATYPEDIALOG_H

#include <QDialog>
#include "datatype.h"

namespace Ui {
    class QDataTypeDialog;
}

class QDataTypeDialog : public QDialog {
    Q_OBJECT
public:
    explicit QDataTypeDialog(QWidget *parent = 0);
    ~QDataTypeDialog();
    void prepareForm();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeDialog *ui;
    QList<DataType*> typeList;

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // QDATATYPEDIALOG_H

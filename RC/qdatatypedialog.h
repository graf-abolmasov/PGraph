#ifndef QDATATYPEDIALOG_H
#define QDATATYPEDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QModelIndex>

namespace Ui {
    class QDataTypeDialog;
}

class DataType;

class QDataTypeDialog : public QDialog {
    Q_OBJECT
public:
    static QDataTypeDialog *getDialog();
    ~QDataTypeDialog();

protected:
    void changeEvent(QEvent *e);

private:
    explicit QDataTypeDialog(QWidget *parent = 0);
    Ui::QDataTypeDialog *ui;
    QList<const DataType *> myTypeList;
    void prepareForm();

private slots:
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_tableWidget_doubleClicked(QModelIndex index);
    void on_buttonBox_accepted();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // QDATATYPEDIALOG_H

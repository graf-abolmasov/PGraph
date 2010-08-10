#ifndef QOPENGRAPHDIALOG_H
#define QOPENGRAPHDIALOG_H

#include <QDialog>

namespace Ui {
    class QOpenGraphDialog;
}

class QOpenGraphDialog : public QDialog {
    Q_OBJECT
public:
    QOpenGraphDialog(QWidget *parent = 0);
    ~QOpenGraphDialog();
    QString getResult();
    void prepareForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QOpenGraphDialog *ui;
};

#endif // QOPENGRAPHDIALOG_H

#ifndef QSAVEGRAPHDIALOG_H
#define QSAVEGRAPHDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class QSaveGraphDialog;
}

class QSaveGraphDialog : public QDialog {
    Q_OBJECT
public:
    QSaveGraphDialog(QWidget *parent = 0);
    ~QSaveGraphDialog();
    QString getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QSaveGraphDialog *ui;

};

#endif // QSAVEGRAPHDIALOG_H

#ifndef TOPPROPERTYDIALOG_H
#define TOPPROPERTYDIALOG_H

#include <QDialog>
#include "qdiagramitem.h"

namespace Ui {
    class TopPropertyDialog;
}

class TopPropertyDialog : public QDialog {
    Q_OBJECT
public:
    TopPropertyDialog(QWidget *parent = 0);
    void prepareForm(QTop* top);
    ~TopPropertyDialog();
    QTop* getResult();

protected:
    void changeEvent(QEvent *e);

public slots:
    void widthChanged(int w);
    void heightChanged(int h);

private:
    Ui::TopPropertyDialog *ui;
    QTop* myTop;
    QList<Actor*> myActorList;
};

#endif // TOPPROPERTYDIALOG_H

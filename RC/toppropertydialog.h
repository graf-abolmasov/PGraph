#ifndef TOPPROPERTYDIALOG_H
#define TOPPROPERTYDIALOG_H

#include <QDialog>
#include "qnormaltop.h"

namespace Ui {
    class TopPropertyDialog;
}

class TopPropertyDialog : public QDialog {
    Q_OBJECT
public:
    TopPropertyDialog(QWidget *parent = 0);
    void prepareForm(QNormalTop* top);
    ~TopPropertyDialog();
    QNormalTop* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TopPropertyDialog *ui;
    QNormalTop* myTop;
    QList<Actor*> myActorList;

private slots:
    void on_actorsListWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_actorsListWidget_currentRowChanged(int currentRow);
};

#endif // TOPPROPERTYDIALOG_H

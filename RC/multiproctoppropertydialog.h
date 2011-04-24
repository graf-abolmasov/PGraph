#ifndef MULTIPROCTOPPROPERTYDIALOG_H
#define MULTIPROCTOPPROPERTYDIALOG_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class Actor;
class QMultiProcTop;

namespace Ui {
    class MultiProcTopPropertyDialog;
}

class MultiProcTopPropertyDialog : public QDialog {
    Q_OBJECT
public:
    MultiProcTopPropertyDialog(QWidget *parent = 0);
    ~MultiProcTopPropertyDialog();
    void prepareForm(QMultiProcTop *top);
    QMultiProcTop *getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MultiProcTopPropertyDialog *ui;
    QMultiProcTop *myTop;
    QList<Actor *> myActorList;

private slots:
    void on_buttonBox_accepted();
    void on_actorsListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_actorsListWidget_currentRowChanged(int currentRow);
};

#endif // MULTIPROCTOPPROPERTYDIALOG_H

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
    static MultiProcTopPropertyDialog *getDialog(QMultiProcTop *top);
    ~MultiProcTopPropertyDialog();
    QMultiProcTop *getResult();

protected:
    void changeEvent(QEvent *e);

private:
    MultiProcTopPropertyDialog(QWidget *parent = 0);
    void prepareForm(QMultiProcTop *top);

    Ui::MultiProcTopPropertyDialog *ui;
    QMultiProcTop *theirTop;
    QList<Actor *> myActorList;

private slots:
    void on_buttonBox_accepted();
    void on_actorsListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_actorsListWidget_currentRowChanged(int currentRow);
};

#endif // MULTIPROCTOPPROPERTYDIALOG_H

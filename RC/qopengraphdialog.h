#ifndef QOPENGRAPHDIALOG_H
#define QOPENGRAPHDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class QOpenGraphDialog;
}

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class Graph;

class QOpenGraphDialog : public QDialog {
    Q_OBJECT
public:
    QOpenGraphDialog(QWidget *parent = 0);
    ~QOpenGraphDialog();
    Graph* getResult();
    void prepareForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QOpenGraphDialog *ui;
    QList<Graph *> myGraphList;

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // QOPENGRAPHDIALOG_H

#ifndef QOPENGRAPHDIALOG_H
#define QOPENGRAPHDIALOG_H

#include <QDialog>
#include "graph.h"

namespace Ui {
    class QOpenGraphDialog;
}

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
    QList<Graph* > myGraphList;

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // QOPENGRAPHDIALOG_H

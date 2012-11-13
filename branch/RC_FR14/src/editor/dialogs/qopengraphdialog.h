#ifndef QOPENGRAPHDIALOG_H
#define QOPENGRAPHDIALOG_H

#include <QtGui/QDialog>
#include <QString>
#include "../../src/common/VO/graphvo.h"

namespace Ui {
    class QOpenGraphDialog;
}

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class QOpenGraphDialog : public QDialog {
    Q_OBJECT
public:
    QOpenGraphDialog(QWidget *parent = 0);
    ~QOpenGraphDialog();
    QString getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QOpenGraphDialog *ui;
    QList<const Graph *> myGraphList;

    void prepareForm();
private slots:
    void enableOkButton(int currentRow);
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // QOPENGRAPHDIALOG_H

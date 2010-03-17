#ifndef TOPPROPERTYDIALOG_H
#define TOPPROPERTYDIALOG_H

#include "qdialog.h"
#include "qdiagramitem.h"

namespace Ui {
    class TopPropertyDialog;
}

class TopPropertyDialog : public QDialog {
    Q_OBJECT
public:
    TopPropertyDialog(QWidget *parent = 0);
    void prepareForm(TTop* top);
    ~TopPropertyDialog();
    int getWidth();
    int getHeight();

protected:
    void changeEvent(QEvent *e);

public slots:
    void widthChanged(int w);
    void heightChanged(int h);

private:
    Ui::TopPropertyDialog *ui;
    TTop* myTop;
};

#endif // TOPPROPERTYDIALOG_H

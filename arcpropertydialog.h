#ifndef ARCPROPERTYDIALOG_H
#define ARCPROPERTYDIALOG_H

#include <QDialog>
#include "qarc.h"

namespace Ui {
    class ArcPropertyDialog;
}

class ArcPropertyDialog : public QDialog {
    Q_OBJECT
public:
    ArcPropertyDialog(QWidget *parent = 0);
    ~ArcPropertyDialog();
    void prepareForm(TArc* arc);
    TArc* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArcPropertyDialog *ui;
    TArc* myArc;
};

#endif // ARCPROPERTYDIALOG_H

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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArcPropertyDialog *ui;
};

#endif // ARCPROPERTYDIALOG_H

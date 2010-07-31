#ifndef QMODULEREGISTER_H
#define QMODULEREGISTER_H

#include <QDialog>

namespace Ui {
    class QModuleRegister;
}

class QModuleRegister : public QDialog {
    Q_OBJECT
public:
    QModuleRegister(QWidget *parent = 0);
    ~QModuleRegister();
    void prepareForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QModuleRegister *ui;
};

#endif // QMODULEREGISTER_H

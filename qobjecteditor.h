#ifndef QOBJECTEDITOR_H
#define QOBJECTEDITOR_H

#include <QDialog>
#include <QToolButton>

namespace Ui {
    class QObjectEditor;
}

class QObjectEditor : public QDialog {
    Q_OBJECT
public:
    explicit QObjectEditor(QWidget *parent = 0);
    bool prepareForm();
    ~QObjectEditor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QObjectEditor *ui;
    QPushButton *newButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QButtonGroup *EditButtonGroup;
};

#endif // QOBJECTEDITOR_H

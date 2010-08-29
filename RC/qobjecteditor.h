#ifndef QOBJECTEDITOR_H
#define QOBJECTEDITOR_H

#include <QDialog>
#include <QToolButton>
#include <actor.h>
#include <predicate.h>

namespace Ui {
    class QObjectEditor;
}

class QObjectEditor : public QDialog {
    Q_OBJECT
public:
    explicit QObjectEditor(QWidget *parent = 0);
    void prepareForm();
    ~QObjectEditor();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_accepted();
    void newButtonClicked();
    void editButtonClicked();
    void deleteButtonClicked();

private:
    Ui::QObjectEditor *ui;
    QPushButton *newButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QButtonGroup *editButtonGroup;

    QList<Actor *> actorsList;
    QList<Predicate *> predicatesList;
    QList<Actor *> iActorsList;
    QList<Predicate *> iPredicateList;
};

#endif // QOBJECTEDITOR_H

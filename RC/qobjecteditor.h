#ifndef QOBJECTEDITOR_H
#define QOBJECTEDITOR_H

#include <QtGui/QDialog>

namespace Ui {
    class QObjectEditor;
}

QT_BEGIN_NAMESPACE
class QButtonGroup;
QT_END_NAMESPACE

class Actor;
class Predicate;

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

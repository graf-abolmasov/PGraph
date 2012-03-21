#ifndef QOBJECTEDITOR_H
#define QOBJECTEDITOR_H

#include <QtGui/QDialog>
#include <QPointer>

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
    QPointer<QPushButton>  newButton;
    QPointer<QPushButton>  editButton;
    QPointer<QPushButton>  deleteButton;
    QPointer<QButtonGroup> editButtonGroup;

    QList<const Actor *> myActorsList;
    QList<const Actor *> myInlineActorsList;
    QList<const Predicate *> myPredicatesList;
    QList<const Predicate *> myInlinePredicateList;

    void save();
};

#endif // QOBJECTEDITOR_H

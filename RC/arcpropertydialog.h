#ifndef ARCPROPERTYDIALOG_H
#define ARCPROPERTYDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ArcPropertyDialog;
}

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class QArc;
class Predicate;

class ArcPropertyDialog : public QDialog {
    Q_OBJECT
public:
    ArcPropertyDialog(QWidget *parent = 0);
    ~ArcPropertyDialog();
    void prepareForm(QArc* arc);
    QArc* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArcPropertyDialog *ui;
    QArc *myArc;
    QList<Predicate *> myPredicateList;

private slots:
    void on_predicateList_itemDoubleClicked(QListWidgetItem* item);
    void on_buttonBox_accepted();
};

#endif // ARCPROPERTYDIALOG_H

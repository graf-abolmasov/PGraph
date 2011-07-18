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
    static ArcPropertyDialog *getDialog(QArc *arc);
    ~ArcPropertyDialog();
    bool makeResult();
    QArc *getResult() const;

protected:
    void changeEvent(QEvent *e);

private:
    ArcPropertyDialog(QWidget *parent = 0);
    void prepareForm(QArc *arc);

    QArc                    *myArc;
    Ui::ArcPropertyDialog   *ui;
    QList<const Predicate *> myPredicateList;
private slots:
    void on_buttonBox_accepted();
    void on_predicateList_itemDoubleClicked(QListWidgetItem* item);
    void on_predicateList_currentRowChanged(int currentRow);
};

#endif // ARCPROPERTYDIALOG_H

#ifndef ARCPROPERTYDIALOG_H
#define ARCPROPERTYDIALOG_H

#include <QtGui>

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

    QPointer<QWidget>     varWidget;
    QPointer<QHBoxLayout> varLayout;
    QPointer<QToolButton> varEditBtn;
private slots:
    void on_buttonBox_accepted();
    void on_predicateList_itemDoubleClicked(QListWidgetItem* item);
    void on_predicateList_currentRowChanged(int currentRow);
    void on_predicateList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void showPredicateEditor();
};

#endif // ARCPROPERTYDIALOG_H

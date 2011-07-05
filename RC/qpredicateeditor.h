#ifndef QPREDICATEEDITOR_H
#define QPREDICATEEDITOR_H

#include <QtGui/QDialog>
#include <QtCore/QPointer>
#include "predicate.h"

namespace Ui {
    class QPredicateEditor;
}

QT_BEGIN_NAMESPACE
class QToolButton;
class QHBoxLayout;
class QComboBox;
QT_END_NAMESPACE

class BaseModule;
class Variable;

class QPredicateEditor : public QDialog {
    Q_OBJECT
public:
    static QPredicateEditor *getCreator(const Predicate::Type &mode);
    static QPredicateEditor *getEditor(const Predicate *predicate);
    const Predicate *getResult() const;
    ~QPredicateEditor();

protected:
    void changeEvent(QEvent *e);

private:
    QPredicateEditor(QWidget *parent = 0);
    QPredicateEditor(const Predicate::Type mode, QWidget *parent = 0);
    QPredicateEditor(const Predicate *predicate, QWidget *parent = 0);

    Predicate *result;
    Predicate *tempPre;
    const Predicate *myPredicate;
    Ui::QPredicateEditor *ui;
    QList<const Variable *>   myVariableList;
    QList<const BaseModule *> myModuleList;
    QPointer<QWidget>     varWidget;
    QPointer<QComboBox>   paramTypeCmbBox;
    QPointer<QHBoxLayout> varLayout;
    QPointer<QToolButton> varEditBtn;

    void prepareForm(const Predicate *predicate);
    void makeResult();
    void enableOkButton();
private slots:
    void on_inlineModuleTxtEdt_textChanged();
    void on_buttonBox_accepted();
    void on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_baseModuleList_currentRowChanged(int currentRow);
    void showVariableDialog();
};

#endif // QPREDICATEEDITOR_H

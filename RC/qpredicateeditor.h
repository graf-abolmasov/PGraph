#ifndef QPREDICATEEDITOR_H
#define QPREDICATEEDITOR_H

#include <QtGui/QDialog>

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
class Predicate;

class QPredicateEditor : public QDialog {
    Q_OBJECT
public:
    enum Mode {Normal, Inline};
    QPredicateEditor(QWidget *parent = 0);
    QPredicateEditor(Mode mode, QWidget *parent = 0);
    ~QPredicateEditor();

    void prepareForm(Predicate *pred);
    Predicate *getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QPredicateEditor *ui;
    Predicate* myPredicate;
    Mode myMode;
    QList<BaseModule *> myModuleList;
    QList<Variable *> myVariableList;
    QToolButton *varEditBtn;
    QWidget *varWidget;
    QHBoxLayout *varLayout;
    QComboBox  *paramTypeCmbBox;

private slots:
    void on_inlineModuleTxtEdt_textChanged();
    void on_buttonBox_accepted();
    void on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_baseModuleList_currentRowChanged(int currentRow);
    void on_varEditBtn_clicked();
};

#endif // QPREDICATEEDITOR_H

#ifndef QPREDICATEEDITOR_H
#define QPREDICATEEDITOR_H

#include <QDialog>
#include "predicate.h"

namespace Ui {
    class QPredicateEditor;
}

class QPredicateEditor : public QDialog {
    Q_OBJECT
public:
    enum Mode {Normal, Inline};
    QPredicateEditor(QWidget *parent = 0);
    QPredicateEditor(Mode mode, QWidget *parent = 0);
    ~QPredicateEditor();

    void prepareForm(Predicate* pred);
    Predicate* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QPredicateEditor *ui;
    Predicate* myPredicate;
    Mode myMode;
};

#endif // QPREDICATEEDITOR_H

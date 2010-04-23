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

    enum Mode {normalEditor, inlineEditor, normalWizard, inlineWizard};

    explicit QPredicateEditor(QWidget *parent = 0);
    QPredicateEditor(Mode mode, QWidget *parent = 0);
    ~QPredicateEditor();

    bool prepareForm(Predicate* pred);
    Predicate* predicate();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QPredicateEditor *ui;
    void updateInterface();
    Mode myMode;
    int step;

private slots:
    void on_nextButton_clicked();
    void on_prevButton_clicked();
};

#endif // QPREDICATEEDITOR_H

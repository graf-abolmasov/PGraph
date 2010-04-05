#ifndef QACTOREDITOR_H
#define QACTOREDITOR_H

#include <QDialog>

namespace Ui {
    class QActorEditor;
}

class QActorEditor : public QDialog {
    Q_OBJECT
public:

    enum Mode {normalEditor, inlineEditor, normalWizard, inlineWizard};

    explicit QActorEditor(QWidget *parent = 0);
    QActorEditor(Mode mode, QWidget *parent = 0);
    ~QActorEditor();

protected:
    void changeEvent(QEvent *e);

private:
    void updateInterface();

    Ui::QActorEditor *ui;
    Mode myMode;
    int step;

private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
};

#endif // QACTOREDITOR_H

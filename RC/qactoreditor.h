#ifndef QActorEditor_H
#define QActorEditor_H

#include <QDialog>
#include "actor.h"
#include "basemodule.h"
#include <QtGui>

namespace Ui {
    class QActorEditor;
}

class QActorEditor : public QDialog {
    Q_OBJECT
public:
    enum Mode {Normal, Inline};
    QActorEditor(QWidget *parent = 0);
    QActorEditor(Mode mode, QWidget *parent = 0);
    ~QActorEditor();
    void prepareForm(Actor *actor);
    Actor* getResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QActorEditor *ui;
    void updateInterface();
    Actor* myActor;
    Mode myMode;
    QList<BaseModule*> myModuleList;
    QList<Variable* > myVariableList;
    QToolButton* varEditBtn;
    QWidget* varWidget;
    QHBoxLayout* varLayout;
    QComboBox* paramTypeCmbBox;


private slots:
    void on_paramsInlineTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_inlineModuleTxtEdt_textChanged();
    void on_buttonBox_accepted();
    void on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_baseModuleList_currentRowChanged(int currentRow);
    void on_varEditBtn_clicked();
    void enableOkButton();
};

#endif // QActorEditor_H

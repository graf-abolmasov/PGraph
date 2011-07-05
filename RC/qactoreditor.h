#ifndef QActorEditor_H
#define QActorEditor_H

#include <QtGui/QDialog>
#include <QtCore/QPointer>
#include "actor.h"
#include "basemodule.h"
#include "variable.h"

namespace Ui {
    class QActorEditor;
}

QT_BEGIN_NAMESPACE
class QToolButton;
class QHBoxLayout;
class QComboBox;
QT_END_NAMESPACE

class QActorEditor : public QDialog {
    Q_OBJECT
public:
    static QActorEditor *getCreator(const Actor::Type &mode);
    static QActorEditor *getEditor(const Actor *actor);

    ~QActorEditor();
    const Actor *getResult();

protected:
    void changeEvent(QEvent *e);

private:
    QActorEditor(QWidget *parent = 0);
    QActorEditor(const Actor::Type &mode, QWidget *parent = 0);
    QActorEditor(const Actor *actor, QWidget *parent = 0);
    Ui::QActorEditor *ui;
    const Actor *myActor;
    Actor *result;
    Actor *tempActor;
    Actor::Type myMode;
    QList<const BaseModule *> myModuleList;
    QList<const Variable *> myVariableList;
    QPointer<QToolButton> varEditBtn;
    QPointer<QWidget> varWidget;
    QPointer<QHBoxLayout> varLayout;
    QPointer<QComboBox> paramTypeCmbBox;

    void prepareForm(const Actor *actor);
    bool validate();
    void makeResult();
private slots:
    void on_paramsInlineTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_inlineModuleTxtEdt_textChanged();
    void on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_baseModuleList_currentRowChanged(int currentRow);
    void showVariableEditor();
    void on_buttonBox_accepted();
};

#endif // QActorEditor_H

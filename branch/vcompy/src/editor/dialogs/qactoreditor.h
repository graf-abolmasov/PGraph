#ifndef QActorEditor_H
#define QActorEditor_H

#include <QtGui/QDialog>
#include <QtCore/QPointer>
#include "../../src/common/VO/actor.h"
#include "../../src/common/VO/basemodule.h"
#include "../../src/common/VO/variable.h"

namespace Ui {
    class QActorEditor;
}

QT_BEGIN_NAMESPACE
class QToolButton;
class QHBoxLayout;
class QComboBox;
class QCompleter;
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

    Actor *result;
    Actor *tempActor;
    const Actor *myActor;
    Ui::QActorEditor *ui;
    QCompleter *myCompleter;
    QList<const Variable *>   myVariableList;
    QList<const BaseModule *> myModuleList;
    QComboBox *paramTypeCmbBox;

    QPointer<QWidget>     varWidget;
    QPointer<QHBoxLayout> varLayout;
    QPointer<QToolButton> varEditBtn;

    void prepareForm(const Actor *actor);
    bool makeResult();
private slots:
    void on_paramsInlineTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_inlineModuleTxtEdt_textChanged();
    void on_paramsNormalTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_baseModuleList_currentRowChanged(int currentRow);
    void showVariableEditor();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // QActorEditor_H

#ifndef QActorEditor_H
#define QActorEditor_H

#include <QDialog>
#include "actor.h"

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
};

#endif // QActorEditor_H

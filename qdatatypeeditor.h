#ifndef QDATATYPEEDITOR_H
#define QDATATYPEEDITOR_H

#include <QDialog>
#include "datatype.h"

namespace Ui {
    class QDataTypeEditor;
}

class QDataTypeEditor : public QDialog {
    Q_OBJECT
public:

    enum Mode {wizard, editor};

    explicit QDataTypeEditor(QWidget *parent = 0);
     QDataTypeEditor(Mode mode, QWidget *parent = 0);
    ~QDataTypeEditor();

    bool prepareForm(DataType *type);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeEditor *ui;
    Mode myMode;

    void updateInterface();
};

#endif // QDATATYPEEDITOR_H

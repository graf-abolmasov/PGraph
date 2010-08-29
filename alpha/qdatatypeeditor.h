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
    explicit QDataTypeEditor(QWidget *parent = 0);
    ~QDataTypeEditor();
    DataType* getResult();
    void prepareForm(DataType *type);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeEditor *ui;
    DataType* myDataType;

private slots:
    void on_buttonBox_accepted();
};

#endif // QDATATYPEEDITOR_H

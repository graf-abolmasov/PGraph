#ifndef QDATATYPEEDITOR_H
#define QDATATYPEEDITOR_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "datatype.h"
#include "globalvariables.h"

namespace Ui {
    class QDataTypeEditor;
}

class QDataTypeEditor : public QDialog {
    Q_OBJECT

public:
    enum Mode {Simple, Array, Struct};
    explicit QDataTypeEditor(QWidget *parent = 0);
    QDataTypeEditor(Mode mode, QWidget *parent = 0);
    ~QDataTypeEditor();
    DataType* getResult();
    void prepareForm(DataType *type);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeEditor *ui;
    DataType* myDataType;
    Mode myMode;
    QStringList indexes;
    QList<DataType*> typeList;
    QComboBox* varTypeCmbBox;
    QLineEdit* varNameEdt;

private slots:
    void on_structFieldsTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_arrDimensionSpnBox_valueChanged(int );
    void on_buttonBox_accepted();
    void enableOkButton();
    void on_structFieldsTable_cellChanged(int row, int column);
};

#endif // QDATATYPEEDITOR_H

#ifndef QDATATYPEEDITOR_H
#define QDATATYPEEDITOR_H

#include <QDialog>
#include <QPointer>
#include "../../src/common/VO/datatype.h"

namespace Ui {
    class QDataTypeEditor;
}

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
QT_END_NAMESPACE

class QDataTypeEditor : public QDialog {
    Q_OBJECT

public:
    enum Mode {Simple, Array, Struct};

    static QDataTypeEditor *getCreator(Mode mode);
    static QDataTypeEditor *getEditor(const DataType *type);

    explicit QDataTypeEditor(QWidget *parent = 0);
    ~QDataTypeEditor();
    const DataType *getResult() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QDataTypeEditor *ui;
    Mode myMode;
    QStringList indexes;
    QList<const DataType *> myTypeList;
    const DataType *myDataType;
    DataType *result;
    QPointer<QComboBox> varTypeCmbBox;
    QPointer<QLineEdit> varNameEdt;

    void makeResult();

    QDataTypeEditor(Mode mode, QWidget *parent = 0);
    QDataTypeEditor(const DataType *type, QWidget *parent = 0);
private slots:
    void on_structFieldsTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_arrDimensionSpnBox_valueChanged(int );
    void on_buttonBox_accepted();
    void enableOkButton();
    void on_structFieldsTable_cellChanged(int row, int column);
};

#endif // QDATATYPEEDITOR_H

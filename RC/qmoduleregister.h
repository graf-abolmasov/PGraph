#ifndef QMODULEREGISTER_H
#define QMODULEREGISTER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui {
    class QModuleRegister;
}

class QModuleRegister : public QDialog {
    Q_OBJECT
public:
    QModuleRegister(QWidget *parent = 0);
    ~QModuleRegister();
    void prepareForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QModuleRegister *ui;
    QDir workingDir;
    QFileInfoList fileList;
    QComboBox* paramTypeCmbBox;
    QTextEdit* paramCommentEdt;

private slots:
    void on_fileList_currentRowChanged(int currentRow);
    void on_buttonBox_accepted();
    void on_parametersTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
};

#endif // QMODULEREGISTER_H

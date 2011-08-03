#ifndef QMODULEREGISTER_H
#define QMODULEREGISTER_H

#include <QtGui/QDialog>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>

namespace Ui {
    class QModuleRegister;
}

QT_BEGIN_NAMESPACE
class QComboBox;
class QTextEdit;
QT_END_NAMESPACE

class QModuleRegister : public QDialog {
    Q_OBJECT

public:
    QModuleRegister(QWidget *parent = 0);
    ~QModuleRegister();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QModuleRegister *ui;
    QFileInfoList fileList;
    QComboBox *paramTypeCmbBox;
    QTextEdit *paramCommentEdt;

    void prepareForm();
private slots:
    void on_fileList_currentRowChanged(int currentRow);
    void on_buttonBox_accepted();
    void on_parametersTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
};

#endif // QMODULEREGISTER_H

#ifndef QCODEEDITOR_H
#define QCODEEDITOR_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QCompleter;
QT_END_NAMESPACE

class QCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    QCodeEditor(QWidget *parent = 0);
    ~QCodeEditor();

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    QCompleter *c;
};

#endif // QCODEEDITOR_H

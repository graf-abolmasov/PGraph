#ifndef QCLICKABLELABLE_H
#define QCLICKABLELABLE_H

#include <QLabel>

class QClickableLabel : public QLabel
{

    Q_OBJECT

public:
    QClickableLabel(QWidget *parent = 0);
    explicit QClickableLabel(const QString &text = "", QWidget *parent = 0);
    ~QClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
};

#endif // QCLICKABLELABLE_H

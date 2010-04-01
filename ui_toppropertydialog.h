/********************************************************************************
** Form generated from reading UI file 'toppropertydialog.ui'
**
** Created: Mon 29. Mar 22:48:20 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPPROPERTYDIALOG_H
#define UI_TOPPROPERTYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopPropertyDialog
{
public:
    QDialogButtonBox *buttonBox;
    QListView *listView;
    QPushButton *moveUpButton;
    QPushButton *moveDownButton;
    QWidget *layoutWidget;
    QHBoxLayout *_2;
    QLabel *lblWidth;
    QSpinBox *spnBoxWidth;
    QLabel *lblHeight;
    QSpinBox *spnBoxHeight;

    void setupUi(QDialog *TopPropertyDialog)
    {
        if (TopPropertyDialog->objectName().isEmpty())
            TopPropertyDialog->setObjectName(QString::fromUtf8("TopPropertyDialog"));
        TopPropertyDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(TopPropertyDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        listView = new QListView(TopPropertyDialog);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(10, 20, 331, 131));
        moveUpButton = new QPushButton(TopPropertyDialog);
        moveUpButton->setObjectName(QString::fromUtf8("moveUpButton"));
        moveUpButton->setGeometry(QRect(350, 20, 41, 61));
        moveDownButton = new QPushButton(TopPropertyDialog);
        moveDownButton->setObjectName(QString::fromUtf8("moveDownButton"));
        moveDownButton->setGeometry(QRect(350, 90, 41, 61));
        layoutWidget = new QWidget(TopPropertyDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 160, 361, 22));
        _2 = new QHBoxLayout(layoutWidget);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        lblWidth = new QLabel(layoutWidget);
        lblWidth->setObjectName(QString::fromUtf8("lblWidth"));
        lblWidth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lblWidth);

        spnBoxWidth = new QSpinBox(layoutWidget);
        spnBoxWidth->setObjectName(QString::fromUtf8("spnBoxWidth"));
        spnBoxWidth->setSingleStep(10);

        _2->addWidget(spnBoxWidth);

        lblHeight = new QLabel(layoutWidget);
        lblHeight->setObjectName(QString::fromUtf8("lblHeight"));
        lblHeight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lblHeight);

        spnBoxHeight = new QSpinBox(layoutWidget);
        spnBoxHeight->setObjectName(QString::fromUtf8("spnBoxHeight"));
        spnBoxHeight->setSingleStep(10);

        _2->addWidget(spnBoxHeight);


        retranslateUi(TopPropertyDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TopPropertyDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TopPropertyDialog, SLOT(reject()));
        QObject::connect(spnBoxWidth, SIGNAL(valueChanged(int)), TopPropertyDialog, SLOT(widthChanged(int)));
        QObject::connect(spnBoxHeight, SIGNAL(valueChanged(int)), TopPropertyDialog, SLOT(heightChanged(int)));

        QMetaObject::connectSlotsByName(TopPropertyDialog);
    } // setupUi

    void retranslateUi(QDialog *TopPropertyDialog)
    {
        TopPropertyDialog->setWindowTitle(QApplication::translate("TopPropertyDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        moveUpButton->setText(QApplication::translate("TopPropertyDialog", "up", 0, QApplication::UnicodeUTF8));
        moveDownButton->setText(QApplication::translate("TopPropertyDialog", "down", 0, QApplication::UnicodeUTF8));
        lblWidth->setText(QApplication::translate("TopPropertyDialog", "\320\250\320\270\321\200\320\270\320\275\320\260", 0, QApplication::UnicodeUTF8));
        lblHeight->setText(QApplication::translate("TopPropertyDialog", "\320\222\321\213\321\201\320\276\321\202\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TopPropertyDialog: public Ui_TopPropertyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPPROPERTYDIALOG_H

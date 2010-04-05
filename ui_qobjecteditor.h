/********************************************************************************
** Form generated from reading UI file 'qobjecteditor.ui'
**
** Created: Fri 2. Apr 10:52:05 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QOBJECTEDITOR_H
#define UI_QOBJECTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QObjectEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tab;
    QWidget *tab_1;
    QHBoxLayout *horizontalLayout;
    QListWidget *objectList;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *tab_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QObjectEditor)
    {
        if (QObjectEditor->objectName().isEmpty())
            QObjectEditor->setObjectName(QString::fromUtf8("QObjectEditor"));
        QObjectEditor->resize(535, 505);
        verticalLayout = new QVBoxLayout(QObjectEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tab = new QTabWidget(QObjectEditor);
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setStyleSheet(QString::fromUtf8(""));
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        horizontalLayout = new QHBoxLayout(tab_1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        objectList = new QListWidget(tab_1);
        objectList->setObjectName(QString::fromUtf8("objectList"));

        horizontalLayout->addWidget(objectList);

        tab->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tab->addTab(tab_4, QString());

        verticalLayout->addWidget(tab);

        buttonBox = new QDialogButtonBox(QObjectEditor);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QObjectEditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), QObjectEditor, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QObjectEditor, SLOT(reject()));

        tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QObjectEditor);
    } // setupUi

    void retranslateUi(QDialog *QObjectEditor)
    {
        QObjectEditor->setWindowTitle(QApplication::translate("QObjectEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tab_1), QApplication::translate("QObjectEditor", "Tab 1", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tab_2), QApplication::translate("QObjectEditor", "Tab 2", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tab_3), QApplication::translate("QObjectEditor", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206\320\260", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tab_4), QApplication::translate("QObjectEditor", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QObjectEditor: public Ui_QObjectEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QOBJECTEDITOR_H

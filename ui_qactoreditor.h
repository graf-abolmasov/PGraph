/********************************************************************************
** Form generated from reading UI file 'qactoreditor.ui'
**
** Created: Sat 3. Apr 20:19:46 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QACTOREDITOR_H
#define UI_QACTOREDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QActorEditor
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *normalEditorWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_3;
    QLabel *lblDescParams;
    QFrame *frame_4;
    QLabel *lblDesc;
    QFrame *frame;
    QLabel *moduleName;
    QLabel *label;
    QFrame *frame_2;
    QTableWidget *tableWidget;
    QLabel *label_3;
    QWidget *inlineWizardStep1Widget;
    QPlainTextEdit *textInline;
    QLabel *label_2;
    QLabel *label_6;
    QGraphicsView *inlineActorIcon;
    QPushButton *loadInlineIconButton;
    QPushButton *clearInlineIconButton;
    QWidget *inlineWizardStep2Widget;
    QTableWidget *parameterInline;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QWidget *normalWizardStep1Widget;
    QLabel *label_4;
    QLineEdit *normalActorName;
    QLabel *label_5;
    QGraphicsView *normalActorIcon;
    QPushButton *loadNormalIconButton;
    QPushButton *clearNormalIconButton;
    QWidget *normalWizardStep2Widget;
    QWidget *normalWizardStep3Widget;

    void setupUi(QDialog *QActorEditor)
    {
        if (QActorEditor->objectName().isEmpty())
            QActorEditor->setObjectName(QString::fromUtf8("QActorEditor"));
        QActorEditor->resize(530, 500);
        QActorEditor->setModal(false);
        buttonBox = new QDialogButtonBox(QActorEditor);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(360, 470, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        normalEditorWidget = new QWidget(QActorEditor);
        normalEditorWidget->setObjectName(QString::fromUtf8("normalEditorWidget"));
        normalEditorWidget->setGeometry(QRect(0, 0, 531, 471));
        horizontalLayoutWidget = new QWidget(normalEditorWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 380, 511, 81));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_3 = new QFrame(horizontalLayoutWidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        lblDescParams = new QLabel(frame_3);
        lblDescParams->setObjectName(QString::fromUtf8("lblDescParams"));
        lblDescParams->setGeometry(QRect(10, 10, 231, 61));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblDescParams->sizePolicy().hasHeightForWidth());
        lblDescParams->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        lblDescParams->setFont(font);
        lblDescParams->setScaledContents(true);
        lblDescParams->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblDescParams->setWordWrap(true);
        lblDescParams->setTextInteractionFlags(Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(frame_3);

        frame_4 = new QFrame(horizontalLayoutWidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        lblDesc = new QLabel(frame_4);
        lblDesc->setObjectName(QString::fromUtf8("lblDesc"));
        lblDesc->setGeometry(QRect(10, 10, 231, 61));
        sizePolicy.setHeightForWidth(lblDesc->sizePolicy().hasHeightForWidth());
        lblDesc->setSizePolicy(sizePolicy);
        lblDesc->setFont(font);
        lblDesc->setScaledContents(true);
        lblDesc->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblDesc->setWordWrap(true);
        lblDesc->setTextInteractionFlags(Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(frame_4);

        frame = new QFrame(normalEditorWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 511, 71));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        moduleName = new QLabel(frame);
        moduleName->setObjectName(QString::fromUtf8("moduleName"));
        moduleName->setGeometry(QRect(120, 20, 371, 41));
        moduleName->setFont(font);
        moduleName->setTextFormat(Qt::PlainText);
        moduleName->setScaledContents(true);
        moduleName->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        moduleName->setWordWrap(true);
        moduleName->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextEditable|Qt::TextEditorInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 79, 16));
        label->setFont(font);
        label->setFrameShape(QFrame::NoFrame);
        label->setTextFormat(Qt::AutoText);
        label->setScaledContents(true);
        frame_2 = new QFrame(normalEditorWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 110, 511, 261));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        tableWidget = new QTableWidget(frame_2);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 11, 491, 241));
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        label_3 = new QLabel(normalEditorWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 90, 104, 16));
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::NoFrame);
        label_3->setTextFormat(Qt::AutoText);
        label_3->setScaledContents(true);
        inlineWizardStep1Widget = new QWidget(QActorEditor);
        inlineWizardStep1Widget->setObjectName(QString::fromUtf8("inlineWizardStep1Widget"));
        inlineWizardStep1Widget->setGeometry(QRect(0, 0, 531, 471));
        textInline = new QPlainTextEdit(inlineWizardStep1Widget);
        textInline->setObjectName(QString::fromUtf8("textInline"));
        textInline->setGeometry(QRect(10, 30, 511, 291));
        label_2 = new QLabel(inlineWizardStep1Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 129, 16));
        label_2->setFont(font);
        label_6 = new QLabel(inlineWizardStep1Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 330, 41, 16));
        label_6->setFont(font);
        inlineActorIcon = new QGraphicsView(inlineWizardStep1Widget);
        inlineActorIcon->setObjectName(QString::fromUtf8("inlineActorIcon"));
        inlineActorIcon->setGeometry(QRect(20, 360, 100, 90));
        loadInlineIconButton = new QPushButton(inlineWizardStep1Widget);
        loadInlineIconButton->setObjectName(QString::fromUtf8("loadInlineIconButton"));
        loadInlineIconButton->setGeometry(QRect(130, 360, 75, 23));
        clearInlineIconButton = new QPushButton(inlineWizardStep1Widget);
        clearInlineIconButton->setObjectName(QString::fromUtf8("clearInlineIconButton"));
        clearInlineIconButton->setGeometry(QRect(130, 390, 75, 23));
        inlineWizardStep2Widget = new QWidget(QActorEditor);
        inlineWizardStep2Widget->setObjectName(QString::fromUtf8("inlineWizardStep2Widget"));
        inlineWizardStep2Widget->setGeometry(QRect(0, 0, 531, 471));
        parameterInline = new QTableWidget(inlineWizardStep2Widget);
        if (parameterInline->columnCount() < 3)
            parameterInline->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        parameterInline->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        parameterInline->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        parameterInline->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        parameterInline->setObjectName(QString::fromUtf8("parameterInline"));
        parameterInline->setGeometry(QRect(10, 10, 511, 451));
        prevButton = new QPushButton(QActorEditor);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setGeometry(QRect(190, 470, 75, 23));
        nextButton = new QPushButton(QActorEditor);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(270, 470, 75, 23));
        normalWizardStep1Widget = new QWidget(QActorEditor);
        normalWizardStep1Widget->setObjectName(QString::fromUtf8("normalWizardStep1Widget"));
        normalWizardStep1Widget->setGeometry(QRect(0, 0, 531, 471));
        label_4 = new QLabel(normalWizardStep1Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 30, 23, 16));
        label_4->setFont(font);
        normalActorName = new QLineEdit(normalWizardStep1Widget);
        normalActorName->setObjectName(QString::fromUtf8("normalActorName"));
        normalActorName->setGeometry(QRect(50, 30, 451, 20));
        label_5 = new QLabel(normalWizardStep1Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 70, 41, 16));
        label_5->setFont(font);
        normalActorIcon = new QGraphicsView(normalWizardStep1Widget);
        normalActorIcon->setObjectName(QString::fromUtf8("normalActorIcon"));
        normalActorIcon->setGeometry(QRect(20, 100, 100, 90));
        loadNormalIconButton = new QPushButton(normalWizardStep1Widget);
        loadNormalIconButton->setObjectName(QString::fromUtf8("loadNormalIconButton"));
        loadNormalIconButton->setGeometry(QRect(140, 100, 75, 23));
        clearNormalIconButton = new QPushButton(normalWizardStep1Widget);
        clearNormalIconButton->setObjectName(QString::fromUtf8("clearNormalIconButton"));
        clearNormalIconButton->setGeometry(QRect(140, 130, 75, 23));
        normalWizardStep2Widget = new QWidget(QActorEditor);
        normalWizardStep2Widget->setObjectName(QString::fromUtf8("normalWizardStep2Widget"));
        normalWizardStep2Widget->setGeometry(QRect(0, 0, 531, 471));
        normalWizardStep3Widget = new QWidget(QActorEditor);
        normalWizardStep3Widget->setObjectName(QString::fromUtf8("normalWizardStep3Widget"));
        normalWizardStep3Widget->setGeometry(QRect(0, 0, 531, 471));

        retranslateUi(QActorEditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), QActorEditor, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QActorEditor, SLOT(reject()));

        QMetaObject::connectSlotsByName(QActorEditor);
    } // setupUi

    void retranslateUi(QDialog *QActorEditor)
    {
        QActorEditor->setWindowTitle(QApplication::translate("QActorEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        lblDescParams->setText(QApplication::translate("QActorEditor", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262 \320\274\320\276\320\264\321\203\320\273\321\217", 0, QApplication::UnicodeUTF8));
        lblDesc->setText(QApplication::translate("QActorEditor", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\274\320\276\320\264\321\203\320\273\321\217", 0, QApplication::UnicodeUTF8));
        moduleName->setText(QApplication::translate("QActorEditor", "TextLabel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QActorEditor", "\320\230\320\274\321\217 \320\276\320\261\321\212\320\265\320\272\321\202\320\260:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("QActorEditor", "\320\235\320\276\320\262\321\213\320\271 \321\201\321\202\320\276\320\273\320\261\320\265\321\206", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("QActorEditor", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("QActorEditor", "\320\224\320\260\320\275\320\275\320\276\320\265 \320\237\320\236", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QActorEditor", "\320\237\320\260\321\201\320\277\320\276\321\200\321\202 \320\276\320\261\321\212\320\265\320\272\321\202\320\260:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QActorEditor", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\274\320\276\320\264\321\203\320\273\321\217", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("QActorEditor", "\320\230\320\272\320\276\320\275\320\272\320\260", 0, QApplication::UnicodeUTF8));
        loadInlineIconButton->setText(QApplication::translate("QActorEditor", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        clearInlineIconButton->setText(QApplication::translate("QActorEditor", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = parameterInline->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("QActorEditor", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = parameterInline->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("QActorEditor", "\320\242\320\270\320\277", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = parameterInline->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("QActorEditor", "\320\232\320\273\320\260\321\201\321\201", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("QActorEditor", "<<", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("QActorEditor", ">>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("QActorEditor", "\320\230\320\274\321\217", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("QActorEditor", "\320\230\320\272\320\276\320\275\320\272\320\260", 0, QApplication::UnicodeUTF8));
        loadNormalIconButton->setText(QApplication::translate("QActorEditor", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        clearNormalIconButton->setText(QApplication::translate("QActorEditor", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QActorEditor: public Ui_QActorEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QACTOREDITOR_H

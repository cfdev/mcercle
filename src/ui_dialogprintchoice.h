/********************************************************************************
** Form generated from reading UI file 'dialogprintchoice.ui'
**
** Created: Sun 27. May 09:31:35 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPRINTCHOICE_H
#define UI_DIALOGPRINTCHOICE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogPrintChoice
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_printer;
    QLabel *label_print;
    QRadioButton *radioButton_printer;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_pdf;
    QRadioButton *radioButton_pdf;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_path;
    QLineEdit *lineEdit_path;
    QToolButton *toolButton_path;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogPrintChoice)
    {
        if (DialogPrintChoice->objectName().isEmpty())
            DialogPrintChoice->setObjectName(QString::fromUtf8("DialogPrintChoice"));
        DialogPrintChoice->resize(442, 239);
        verticalLayout_2 = new QVBoxLayout(DialogPrintChoice);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(DialogPrintChoice);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_printer = new QHBoxLayout();
        horizontalLayout_printer->setObjectName(QString::fromUtf8("horizontalLayout_printer"));
        label_print = new QLabel(groupBox);
        label_print->setObjectName(QString::fromUtf8("label_print"));
        label_print->setPixmap(QPixmap(QString::fromUtf8(":/app/print")));

        horizontalLayout_printer->addWidget(label_print);

        radioButton_printer = new QRadioButton(groupBox);
        radioButton_printer->setObjectName(QString::fromUtf8("radioButton_printer"));

        horizontalLayout_printer->addWidget(radioButton_printer);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_printer->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_printer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_pdf = new QLabel(groupBox);
        label_pdf->setObjectName(QString::fromUtf8("label_pdf"));
        label_pdf->setPixmap(QPixmap(QString::fromUtf8(":/app/pdf")));

        horizontalLayout_2->addWidget(label_pdf);

        radioButton_pdf = new QRadioButton(groupBox);
        radioButton_pdf->setObjectName(QString::fromUtf8("radioButton_pdf"));

        horizontalLayout_2->addWidget(radioButton_pdf);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_2);


        gridLayout_2->addLayout(verticalLayout_3, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(-1);
#endif
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        label_path = new QLabel(DialogPrintChoice);
        label_path->setObjectName(QString::fromUtf8("label_path"));

        horizontalLayout->addWidget(label_path);

        lineEdit_path = new QLineEdit(DialogPrintChoice);
        lineEdit_path->setObjectName(QString::fromUtf8("lineEdit_path"));
        lineEdit_path->setEnabled(true);

        horizontalLayout->addWidget(lineEdit_path);

        toolButton_path = new QToolButton(DialogPrintChoice);
        toolButton_path->setObjectName(QString::fromUtf8("toolButton_path"));

        horizontalLayout->addWidget(toolButton_path);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(DialogPrintChoice);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(DialogPrintChoice);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPrintChoice, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPrintChoice, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogPrintChoice);
    } // setupUi

    void retranslateUi(QDialog *DialogPrintChoice)
    {
        DialogPrintChoice->setWindowTitle(QApplication::translate("DialogPrintChoice", "Type d'impression", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DialogPrintChoice", "Quel type d'impression voulez-vous r\303\251aliser ?", 0, QApplication::UnicodeUTF8));
        label_print->setText(QString());
        radioButton_printer->setText(QApplication::translate("DialogPrintChoice", "Imprimante Physique", 0, QApplication::UnicodeUTF8));
        label_pdf->setText(QString());
        radioButton_pdf->setText(QApplication::translate("DialogPrintChoice", "Dans un fichier au format PDF", 0, QApplication::UnicodeUTF8));
        label_path->setText(QApplication::translate("DialogPrintChoice", "Chemin du fichier :", 0, QApplication::UnicodeUTF8));
        toolButton_path->setText(QApplication::translate("DialogPrintChoice", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPrintChoice: public Ui_DialogPrintChoice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPRINTCHOICE_H

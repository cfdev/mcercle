/********************************************************************************
** Form generated from reading UI file 'dialoginvoicelist.ui'
**
** Created: Fri 3. May 08:11:32 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGINVOICELIST_H
#define UI_DIALOGINVOICELIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogInvoiceList
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDateEdit *dateEdit;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableWidget_Invoices;
    QLabel *label_Information;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_print;
    QPushButton *pushButton_ok;

    void setupUi(QDialog *DialogInvoiceList)
    {
        if (DialogInvoiceList->objectName().isEmpty())
            DialogInvoiceList->setObjectName(QString::fromUtf8("DialogInvoiceList"));
        DialogInvoiceList->resize(627, 462);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        DialogInvoiceList->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogInvoiceList);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DialogInvoiceList);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        line = new QFrame(DialogInvoiceList);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(DialogInvoiceList);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        dateEdit = new QDateEdit(DialogInvoiceList);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setMinimumSize(QSize(150, 0));
        dateEdit->setAlignment(Qt::AlignCenter);
        dateEdit->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableWidget_Invoices = new QTableWidget(DialogInvoiceList);
        tableWidget_Invoices->setObjectName(QString::fromUtf8("tableWidget_Invoices"));

        verticalLayout->addWidget(tableWidget_Invoices);


        verticalLayout_2->addLayout(verticalLayout);

        label_Information = new QLabel(DialogInvoiceList);
        label_Information->setObjectName(QString::fromUtf8("label_Information"));

        verticalLayout_2->addWidget(label_Information);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_print = new QPushButton(DialogInvoiceList);
        pushButton_print->setObjectName(QString::fromUtf8("pushButton_print"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/print"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_print->setIcon(icon1);

        horizontalLayout_2->addWidget(pushButton_print);

        pushButton_ok = new QPushButton(DialogInvoiceList);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/quit"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_ok->setIcon(icon2);

        horizontalLayout_2->addWidget(pushButton_ok);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(DialogInvoiceList);

        QMetaObject::connectSlotsByName(DialogInvoiceList);
    } // setupUi

    void retranslateUi(QDialog *DialogInvoiceList)
    {
        DialogInvoiceList->setWindowTitle(QApplication::translate("DialogInvoiceList", "Livre des recettes", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogInvoiceList", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Livre des recettes</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogInvoiceList", "Choix du mois pour la liste des factures :", 0, QApplication::UnicodeUTF8));
        dateEdit->setDisplayFormat(QApplication::translate("DialogInvoiceList", "MM/yyyy", 0, QApplication::UnicodeUTF8));
        label_Information->setText(QApplication::translate("DialogInvoiceList", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/app/help\" width=\"24\" height=\"24\" /><span style=\" font-size:8pt;\"> </span><span style=\" font-size:9pt;\">Seulement les factures qui ont le status &quot;R\303\251gl\303\251e&quot; sont list\303\251es ici !</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_print->setText(QApplication::translate("DialogInvoiceList", "Imprimer", 0, QApplication::UnicodeUTF8));
        pushButton_ok->setText(QApplication::translate("DialogInvoiceList", "Fermer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogInvoiceList: public Ui_DialogInvoiceList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGINVOICELIST_H

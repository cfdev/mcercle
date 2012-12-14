/********************************************************************************
** Form generated from reading UI file 'dialogprovidersedit.ui'
**
** Created: Fri 14. Dec 20:28:52 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPROVIDERSEDIT_H
#define UI_DIALOGPROVIDERSEDIT_H

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
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogProvidersEdit
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *GroupBox;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_phone;
    QLineEdit *lineEdit_name;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_fax;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit_city;
    QLineEdit *lineEdit_zipCode;
    QLabel *label_8;
    QLabel *label_10;
    QLineEdit *lineEdit_contact;
    QLabel *label_6;
    QLineEdit *lineEdit_email;
    QLabel *label_11;
    QLineEdit *lineEdit_country;
    QLabel *label_7;
    QLineEdit *lineEdit_add1;
    QLineEdit *lineEdit_add2;
    QLineEdit *lineEdit_add3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogProvidersEdit)
    {
        if (DialogProvidersEdit->objectName().isEmpty())
            DialogProvidersEdit->setObjectName(QString::fromUtf8("DialogProvidersEdit"));
        DialogProvidersEdit->resize(448, 343);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        DialogProvidersEdit->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogProvidersEdit);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        GroupBox = new QGroupBox(DialogProvidersEdit);
        GroupBox->setObjectName(QString::fromUtf8("GroupBox"));
        verticalLayout = new QVBoxLayout(GroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(GroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        lineEdit_phone = new QLineEdit(GroupBox);
        lineEdit_phone->setObjectName(QString::fromUtf8("lineEdit_phone"));
        lineEdit_phone->setMaxLength(24);

        gridLayout->addWidget(lineEdit_phone, 2, 1, 1, 1);

        lineEdit_name = new QLineEdit(GroupBox);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));
        lineEdit_name->setMaxLength(64);

        gridLayout->addWidget(lineEdit_name, 0, 1, 1, 1);

        label_2 = new QLabel(GroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(GroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 3, 1, 1);

        lineEdit_fax = new QLineEdit(GroupBox);
        lineEdit_fax->setObjectName(QString::fromUtf8("lineEdit_fax"));
        lineEdit_fax->setMaxLength(24);

        gridLayout->addWidget(lineEdit_fax, 2, 4, 1, 1);

        label_4 = new QLabel(GroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_5 = new QLabel(GroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 3, 1, 1);

        lineEdit_city = new QLineEdit(GroupBox);
        lineEdit_city->setObjectName(QString::fromUtf8("lineEdit_city"));
        lineEdit_city->setMaxLength(64);

        gridLayout->addWidget(lineEdit_city, 3, 1, 1, 1);

        lineEdit_zipCode = new QLineEdit(GroupBox);
        lineEdit_zipCode->setObjectName(QString::fromUtf8("lineEdit_zipCode"));
        lineEdit_zipCode->setMaxLength(10);

        gridLayout->addWidget(lineEdit_zipCode, 3, 4, 1, 1);

        label_8 = new QLabel(GroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 0, 2, 1, 1);

        label_10 = new QLabel(GroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 1, 0, 1, 1);

        lineEdit_contact = new QLineEdit(GroupBox);
        lineEdit_contact->setObjectName(QString::fromUtf8("lineEdit_contact"));

        gridLayout->addWidget(lineEdit_contact, 1, 1, 1, 1);

        label_6 = new QLabel(GroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 3, 1, 1);

        lineEdit_email = new QLineEdit(GroupBox);
        lineEdit_email->setObjectName(QString::fromUtf8("lineEdit_email"));
        lineEdit_email->setMaxLength(128);

        gridLayout->addWidget(lineEdit_email, 1, 4, 1, 1);

        label_11 = new QLabel(GroupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 4, 0, 1, 1);

        lineEdit_country = new QLineEdit(GroupBox);
        lineEdit_country->setObjectName(QString::fromUtf8("lineEdit_country"));
        lineEdit_country->setMaxLength(64);

        gridLayout->addWidget(lineEdit_country, 4, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        label_7 = new QLabel(GroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout->addWidget(label_7);

        lineEdit_add1 = new QLineEdit(GroupBox);
        lineEdit_add1->setObjectName(QString::fromUtf8("lineEdit_add1"));
        lineEdit_add1->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_add1);

        lineEdit_add2 = new QLineEdit(GroupBox);
        lineEdit_add2->setObjectName(QString::fromUtf8("lineEdit_add2"));
        lineEdit_add2->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_add2);

        lineEdit_add3 = new QLineEdit(GroupBox);
        lineEdit_add3->setObjectName(QString::fromUtf8("lineEdit_add3"));
        lineEdit_add3->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_add3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_9 = new QLabel(GroupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_3->addWidget(label_9);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(GroupBox);

        buttonBox = new QDialogButtonBox(DialogProvidersEdit);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);

        QWidget::setTabOrder(lineEdit_name, lineEdit_contact);
        QWidget::setTabOrder(lineEdit_contact, lineEdit_email);
        QWidget::setTabOrder(lineEdit_email, lineEdit_phone);
        QWidget::setTabOrder(lineEdit_phone, lineEdit_fax);
        QWidget::setTabOrder(lineEdit_fax, lineEdit_city);
        QWidget::setTabOrder(lineEdit_city, lineEdit_zipCode);
        QWidget::setTabOrder(lineEdit_zipCode, lineEdit_country);
        QWidget::setTabOrder(lineEdit_country, lineEdit_add1);
        QWidget::setTabOrder(lineEdit_add1, lineEdit_add2);
        QWidget::setTabOrder(lineEdit_add2, lineEdit_add3);
        QWidget::setTabOrder(lineEdit_add3, buttonBox);

        retranslateUi(DialogProvidersEdit);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogProvidersEdit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogProvidersEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogProvidersEdit);
    } // setupUi

    void retranslateUi(QDialog *DialogProvidersEdit)
    {
        DialogProvidersEdit->setWindowTitle(QApplication::translate("DialogProvidersEdit", "Services communs", 0, QApplication::UnicodeUTF8));
        GroupBox->setTitle(QApplication::translate("DialogProvidersEdit", "Fournisseur", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogProvidersEdit", "T\303\251l :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogProvidersEdit", "Nom :", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogProvidersEdit", "Fax :", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogProvidersEdit", "Ville :", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogProvidersEdit", "Code Postal :", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DialogProvidersEdit", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#aa0000;\">*</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DialogProvidersEdit", "Contact : ", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogProvidersEdit", "Email :", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DialogProvidersEdit", "Pays :", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogProvidersEdit", "Adresse :", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DialogProvidersEdit", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#aa0000;\">* Champs obligatoires</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogProvidersEdit: public Ui_DialogProvidersEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPROVIDERSEDIT_H

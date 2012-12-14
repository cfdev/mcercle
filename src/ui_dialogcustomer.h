/********************************************************************************
** Form generated from reading UI file 'dialogcustomer.ui'
**
** Created: Thu 13. Dec 18:04:11 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCUSTOMER_H
#define UI_DIALOGCUSTOMER_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogCustomer
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QFrame *line;
    QGroupBox *groupBox_type;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton_people;
    QRadioButton *radioButton_compagny;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *label_civ;
    QLabel *label_3;
    QLabel *label_Email;
    QLabel *label_2;
    QLineEdit *lineEdit_LastName;
    QComboBox *comboBoxGender;
    QLineEdit *lineEdit_PhoneNumber;
    QLineEdit *lineEdit_Email;
    QLabel *label_lastname;
    QLabel *label_12;
    QLineEdit *lineEdit_FirstName;
    QLabel *label_asterix_ltname;
    QLabel *label_date;
    QLabel *label_7;
    QLabel *label_9;
    QLineEdit *lineEdit_City;
    QLabel *label_11;
    QDateEdit *dateEdit;
    QLineEdit *lineEdit_MobilePhone;
    QLineEdit *lineEdit_Profession;
    QLineEdit *lineEdit_ZipCode;
    QLineEdit *lineEdit_compagny;
    QLabel *label_compagny;
    QLabel *label;
    QLineEdit *lineEdit_country;
    QLabel *label_8;
    QLineEdit *lineEdit_Add1;
    QLineEdit *lineEdit_Add2;
    QLineEdit *lineEdit_Add3;
    QLabel *label_10;
    QLineEdit *lineEdit_Comments;
    QCheckBox *checkBox_promo;
    QLabel *label_13;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCustomer)
    {
        if (DialogCustomer->objectName().isEmpty())
            DialogCustomer->setObjectName(QString::fromUtf8("DialogCustomer"));
        DialogCustomer->setWindowModality(Qt::NonModal);
        DialogCustomer->resize(563, 559);
        DialogCustomer->setMinimumSize(QSize(500, 0));
        DialogCustomer->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/customers"), QSize(), QIcon::Normal, QIcon::Off);
        DialogCustomer->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogCustomer);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(DialogCustomer);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelTitle);

        line = new QFrame(DialogCustomer);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        groupBox_type = new QGroupBox(DialogCustomer);
        groupBox_type->setObjectName(QString::fromUtf8("groupBox_type"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_type);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton_people = new QRadioButton(groupBox_type);
        radioButton_people->setObjectName(QString::fromUtf8("radioButton_people"));
        radioButton_people->setIcon(icon);
        radioButton_people->setIconSize(QSize(24, 24));
        radioButton_people->setChecked(true);

        horizontalLayout->addWidget(radioButton_people);

        radioButton_compagny = new QRadioButton(groupBox_type);
        radioButton_compagny->setObjectName(QString::fromUtf8("radioButton_compagny"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/compagny"), QSize(), QIcon::Normal, QIcon::Off);
        radioButton_compagny->setIcon(icon1);
        radioButton_compagny->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(radioButton_compagny);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox_type);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(-1, 5, -1, 5);
        label_6 = new QLabel(DialogCustomer);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        label_civ = new QLabel(DialogCustomer);
        label_civ->setObjectName(QString::fromUtf8("label_civ"));

        gridLayout->addWidget(label_civ, 2, 0, 1, 1);

        label_3 = new QLabel(DialogCustomer);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        label_Email = new QLabel(DialogCustomer);
        label_Email->setObjectName(QString::fromUtf8("label_Email"));

        gridLayout->addWidget(label_Email, 4, 0, 1, 1);

        label_2 = new QLabel(DialogCustomer);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(label_2, 5, 0, 1, 1);

        lineEdit_LastName = new QLineEdit(DialogCustomer);
        lineEdit_LastName->setObjectName(QString::fromUtf8("lineEdit_LastName"));
        lineEdit_LastName->setMaxLength(64);

        gridLayout->addWidget(lineEdit_LastName, 0, 1, 1, 1);

        comboBoxGender = new QComboBox(DialogCustomer);
        comboBoxGender->setObjectName(QString::fromUtf8("comboBoxGender"));
        comboBoxGender->setMaxVisibleItems(10);

        gridLayout->addWidget(comboBoxGender, 2, 1, 1, 1);

        lineEdit_PhoneNumber = new QLineEdit(DialogCustomer);
        lineEdit_PhoneNumber->setObjectName(QString::fromUtf8("lineEdit_PhoneNumber"));
        lineEdit_PhoneNumber->setMaxLength(24);

        gridLayout->addWidget(lineEdit_PhoneNumber, 3, 1, 1, 1);

        lineEdit_Email = new QLineEdit(DialogCustomer);
        lineEdit_Email->setObjectName(QString::fromUtf8("lineEdit_Email"));
        lineEdit_Email->setMaxLength(128);

        gridLayout->addWidget(lineEdit_Email, 4, 1, 1, 1);

        label_lastname = new QLabel(DialogCustomer);
        label_lastname->setObjectName(QString::fromUtf8("label_lastname"));

        gridLayout->addWidget(label_lastname, 0, 3, 1, 1);

        label_12 = new QLabel(DialogCustomer);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_12->setFont(font1);

        gridLayout->addWidget(label_12, 0, 2, 1, 1);

        lineEdit_FirstName = new QLineEdit(DialogCustomer);
        lineEdit_FirstName->setObjectName(QString::fromUtf8("lineEdit_FirstName"));
        lineEdit_FirstName->setMaxLength(64);

        gridLayout->addWidget(lineEdit_FirstName, 0, 4, 1, 1);

        label_asterix_ltname = new QLabel(DialogCustomer);
        label_asterix_ltname->setObjectName(QString::fromUtf8("label_asterix_ltname"));

        gridLayout->addWidget(label_asterix_ltname, 0, 5, 1, 1);

        label_date = new QLabel(DialogCustomer);
        label_date->setObjectName(QString::fromUtf8("label_date"));
        label_date->setMinimumSize(QSize(0, 0));
        label_date->setScaledContents(false);
        label_date->setMargin(0);

        gridLayout->addWidget(label_date, 2, 3, 1, 1);

        label_7 = new QLabel(DialogCustomer);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 3, 1, 1);

        label_9 = new QLabel(DialogCustomer);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 4, 3, 1, 1);

        lineEdit_City = new QLineEdit(DialogCustomer);
        lineEdit_City->setObjectName(QString::fromUtf8("lineEdit_City"));
        lineEdit_City->setMaxLength(64);

        gridLayout->addWidget(lineEdit_City, 5, 1, 1, 1);

        label_11 = new QLabel(DialogCustomer);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 5, 3, 1, 1);

        dateEdit = new QDateEdit(DialogCustomer);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setDate(QDate(1970, 1, 1));
        dateEdit->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit, 2, 4, 1, 1);

        lineEdit_MobilePhone = new QLineEdit(DialogCustomer);
        lineEdit_MobilePhone->setObjectName(QString::fromUtf8("lineEdit_MobilePhone"));
        lineEdit_MobilePhone->setMaxLength(24);

        gridLayout->addWidget(lineEdit_MobilePhone, 3, 4, 1, 1);

        lineEdit_Profession = new QLineEdit(DialogCustomer);
        lineEdit_Profession->setObjectName(QString::fromUtf8("lineEdit_Profession"));
        lineEdit_Profession->setMaxLength(128);

        gridLayout->addWidget(lineEdit_Profession, 4, 4, 1, 1);

        lineEdit_ZipCode = new QLineEdit(DialogCustomer);
        lineEdit_ZipCode->setObjectName(QString::fromUtf8("lineEdit_ZipCode"));
        lineEdit_ZipCode->setMaxLength(10);
        lineEdit_ZipCode->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lineEdit_ZipCode, 5, 4, 1, 1);

        lineEdit_compagny = new QLineEdit(DialogCustomer);
        lineEdit_compagny->setObjectName(QString::fromUtf8("lineEdit_compagny"));
        lineEdit_compagny->setMaxLength(128);

        gridLayout->addWidget(lineEdit_compagny, 1, 1, 1, 1);

        label_compagny = new QLabel(DialogCustomer);
        label_compagny->setObjectName(QString::fromUtf8("label_compagny"));

        gridLayout->addWidget(label_compagny, 1, 0, 1, 1);

        label = new QLabel(DialogCustomer);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 6, 0, 1, 1);

        lineEdit_country = new QLineEdit(DialogCustomer);
        lineEdit_country->setObjectName(QString::fromUtf8("lineEdit_country"));
        lineEdit_country->setMaxLength(64);

        gridLayout->addWidget(lineEdit_country, 6, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        label_8 = new QLabel(DialogCustomer);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout->addWidget(label_8);

        lineEdit_Add1 = new QLineEdit(DialogCustomer);
        lineEdit_Add1->setObjectName(QString::fromUtf8("lineEdit_Add1"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_Add1->sizePolicy().hasHeightForWidth());
        lineEdit_Add1->setSizePolicy(sizePolicy);
        lineEdit_Add1->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_Add1);

        lineEdit_Add2 = new QLineEdit(DialogCustomer);
        lineEdit_Add2->setObjectName(QString::fromUtf8("lineEdit_Add2"));
        lineEdit_Add2->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_Add2);

        lineEdit_Add3 = new QLineEdit(DialogCustomer);
        lineEdit_Add3->setObjectName(QString::fromUtf8("lineEdit_Add3"));
        lineEdit_Add3->setMaxLength(128);

        verticalLayout->addWidget(lineEdit_Add3);

        label_10 = new QLabel(DialogCustomer);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout->addWidget(label_10);

        lineEdit_Comments = new QLineEdit(DialogCustomer);
        lineEdit_Comments->setObjectName(QString::fromUtf8("lineEdit_Comments"));
        lineEdit_Comments->setMaxLength(256);

        verticalLayout->addWidget(lineEdit_Comments);

        checkBox_promo = new QCheckBox(DialogCustomer);
        checkBox_promo->setObjectName(QString::fromUtf8("checkBox_promo"));

        verticalLayout->addWidget(checkBox_promo);


        verticalLayout_2->addLayout(verticalLayout);

        label_13 = new QLabel(DialogCustomer);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout_2->addWidget(label_13);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(DialogCustomer);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);

        QWidget::setTabOrder(radioButton_people, radioButton_compagny);
        QWidget::setTabOrder(radioButton_compagny, lineEdit_LastName);
        QWidget::setTabOrder(lineEdit_LastName, lineEdit_FirstName);
        QWidget::setTabOrder(lineEdit_FirstName, lineEdit_compagny);
        QWidget::setTabOrder(lineEdit_compagny, comboBoxGender);
        QWidget::setTabOrder(comboBoxGender, dateEdit);
        QWidget::setTabOrder(dateEdit, lineEdit_PhoneNumber);
        QWidget::setTabOrder(lineEdit_PhoneNumber, lineEdit_MobilePhone);
        QWidget::setTabOrder(lineEdit_MobilePhone, lineEdit_Email);
        QWidget::setTabOrder(lineEdit_Email, lineEdit_Profession);
        QWidget::setTabOrder(lineEdit_Profession, lineEdit_City);
        QWidget::setTabOrder(lineEdit_City, lineEdit_ZipCode);
        QWidget::setTabOrder(lineEdit_ZipCode, lineEdit_country);
        QWidget::setTabOrder(lineEdit_country, lineEdit_Add1);
        QWidget::setTabOrder(lineEdit_Add1, lineEdit_Add2);
        QWidget::setTabOrder(lineEdit_Add2, lineEdit_Add3);
        QWidget::setTabOrder(lineEdit_Add3, lineEdit_Comments);
        QWidget::setTabOrder(lineEdit_Comments, checkBox_promo);
        QWidget::setTabOrder(checkBox_promo, buttonBox);

        retranslateUi(DialogCustomer);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogCustomer, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogCustomer, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogCustomer);
    } // setupUi

    void retranslateUi(QDialog *DialogCustomer)
    {
        DialogCustomer->setWindowTitle(QApplication::translate("DialogCustomer", "Client", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("DialogCustomer", "Ajout/Edit Client", 0, QApplication::UnicodeUTF8));
        groupBox_type->setTitle(QApplication::translate("DialogCustomer", "Type de client", 0, QApplication::UnicodeUTF8));
        radioButton_people->setText(QApplication::translate("DialogCustomer", "Particulier", 0, QApplication::UnicodeUTF8));
        radioButton_compagny->setText(QApplication::translate("DialogCustomer", "Soci\303\251t\303\251", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogCustomer", "T\303\251l Fixe :", 0, QApplication::UnicodeUTF8));
        label_civ->setText(QApplication::translate("DialogCustomer", "Civilit\303\251 :", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogCustomer", "Nom :", 0, QApplication::UnicodeUTF8));
        label_Email->setText(QApplication::translate("DialogCustomer", "Email :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogCustomer", "Ville :", 0, QApplication::UnicodeUTF8));
        label_lastname->setText(QApplication::translate("DialogCustomer", "Pr\303\251nom :", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DialogCustomer", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#aa0000;\">*</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_asterix_ltname->setText(QApplication::translate("DialogCustomer", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#aa0000;\">*</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_date->setText(QApplication::translate("DialogCustomer", "Date de\n"
"Naissance :", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogCustomer", "Mobile :", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DialogCustomer", "Activit\303\251 :", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DialogCustomer", "Code Postal :", 0, QApplication::UnicodeUTF8));
        dateEdit->setDisplayFormat(QApplication::translate("DialogCustomer", "dd/MM/yyyy", 0, QApplication::UnicodeUTF8));
        label_compagny->setText(QApplication::translate("DialogCustomer", "Forme Juridique :", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogCustomer", "Pays :", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DialogCustomer", "Adresse :", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DialogCustomer", "Commentaires :", 0, QApplication::UnicodeUTF8));
        checkBox_promo->setText(QApplication::translate("DialogCustomer", "Accept de re\303\247evoir des emails promotionnels", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("DialogCustomer", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#aa0000;\">* Champs obligatoires</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogCustomer: public Ui_DialogCustomer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCUSTOMER_H

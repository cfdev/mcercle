/********************************************************************************
** Form generated from reading UI file 'dialogsettings.ui'
**
** Created: Tue Apr 3 11:50:08 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSETTINGS_H
#define UI_DIALOGSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSettings
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_compagny;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_logo;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_15;
    QPushButton *pushButton_Logo;
    QPushButton *pushButton_ClearImage;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLineEdit *lineEdit_sName;
    QLineEdit *lineEdit_sEmail;
    QLineEdit *lineEdit_sCity;
    QLabel *label_6;
    QLineEdit *lineEdit_sNum;
    QLineEdit *lineEdit_sFax;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_8;
    QLabel *label_11;
    QLineEdit *lineEdit_sWebSite;
    QLineEdit *lineEdit_sPhone;
    QLineEdit *lineEdit_sZipCode;
    QLabel *label_16;
    QLineEdit *lineEdit_sCapital;
    QCheckBox *checkBox_TAX;
    QLabel *label_17;
    QLabel *label_10;
    QLineEdit *lineEdit_sAdd1;
    QLineEdit *lineEdit_sAdd2;
    QLineEdit *lineEdit_sAdd3;
    QWidget *tab_Database;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_hostName;
    QLabel *label_2;
    QLineEdit *lineEdit_port;
    QLineEdit *lineEdit_databaseName;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_login;
    QLabel *label_5;
    QLineEdit *lineEdit_password;
    QToolButton *toolButton_BaseSelect;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_18;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_state;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_connect;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogSettings)
    {
        if (DialogSettings->objectName().isEmpty())
            DialogSettings->setObjectName(QString::fromUtf8("DialogSettings"));
        DialogSettings->resize(506, 447);
        DialogSettings->setMinimumSize(QSize(500, 425));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/preference"), QSize(), QIcon::Normal, QIcon::Off);
        DialogSettings->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(DialogSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(DialogSettings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setIconSize(QSize(24, 24));
        tab_compagny = new QWidget();
        tab_compagny->setObjectName(QString::fromUtf8("tab_compagny"));
        horizontalLayout_2 = new QHBoxLayout(tab_compagny);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_logo = new QLabel(tab_compagny);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));

        horizontalLayout_3->addWidget(label_logo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_15 = new QLabel(tab_compagny);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        verticalLayout_5->addWidget(label_15);

        pushButton_Logo = new QPushButton(tab_compagny);
        pushButton_Logo->setObjectName(QString::fromUtf8("pushButton_Logo"));

        verticalLayout_5->addWidget(pushButton_Logo);

        pushButton_ClearImage = new QPushButton(tab_compagny);
        pushButton_ClearImage->setObjectName(QString::fromUtf8("pushButton_ClearImage"));

        verticalLayout_5->addWidget(pushButton_ClearImage);


        horizontalLayout_3->addLayout(verticalLayout_5);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(tab_compagny);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        lineEdit_sName = new QLineEdit(tab_compagny);
        lineEdit_sName->setObjectName(QString::fromUtf8("lineEdit_sName"));
        lineEdit_sName->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sName, 1, 2, 1, 1);

        lineEdit_sEmail = new QLineEdit(tab_compagny);
        lineEdit_sEmail->setObjectName(QString::fromUtf8("lineEdit_sEmail"));
        lineEdit_sEmail->setMaxLength(128);

        gridLayout_2->addWidget(lineEdit_sEmail, 4, 2, 1, 1);

        lineEdit_sCity = new QLineEdit(tab_compagny);
        lineEdit_sCity->setObjectName(QString::fromUtf8("lineEdit_sCity"));
        lineEdit_sCity->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sCity, 5, 2, 1, 1);

        label_6 = new QLabel(tab_compagny);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 3, 1, 1);

        lineEdit_sNum = new QLineEdit(tab_compagny);
        lineEdit_sNum->setObjectName(QString::fromUtf8("lineEdit_sNum"));
        lineEdit_sNum->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sNum, 1, 4, 1, 1);

        lineEdit_sFax = new QLineEdit(tab_compagny);
        lineEdit_sFax->setObjectName(QString::fromUtf8("lineEdit_sFax"));
        lineEdit_sFax->setMaxLength(24);

        gridLayout_2->addWidget(lineEdit_sFax, 3, 4, 1, 1);

        label_9 = new QLabel(tab_compagny);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 3, 0, 1, 1);

        label_12 = new QLabel(tab_compagny);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 3, 3, 1, 1);

        label_13 = new QLabel(tab_compagny);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 4, 0, 1, 1);

        label_14 = new QLabel(tab_compagny);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 4, 3, 1, 1);

        label_8 = new QLabel(tab_compagny);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 5, 0, 1, 1);

        label_11 = new QLabel(tab_compagny);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 5, 3, 1, 1);

        lineEdit_sWebSite = new QLineEdit(tab_compagny);
        lineEdit_sWebSite->setObjectName(QString::fromUtf8("lineEdit_sWebSite"));
        lineEdit_sWebSite->setMaxLength(256);

        gridLayout_2->addWidget(lineEdit_sWebSite, 4, 4, 1, 1);

        lineEdit_sPhone = new QLineEdit(tab_compagny);
        lineEdit_sPhone->setObjectName(QString::fromUtf8("lineEdit_sPhone"));
        lineEdit_sPhone->setMaxLength(24);

        gridLayout_2->addWidget(lineEdit_sPhone, 3, 2, 1, 1);

        lineEdit_sZipCode = new QLineEdit(tab_compagny);
        lineEdit_sZipCode->setObjectName(QString::fromUtf8("lineEdit_sZipCode"));
        lineEdit_sZipCode->setMaxLength(10);

        gridLayout_2->addWidget(lineEdit_sZipCode, 5, 4, 1, 1);

        label_16 = new QLabel(tab_compagny);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 2, 0, 1, 1);

        lineEdit_sCapital = new QLineEdit(tab_compagny);
        lineEdit_sCapital->setObjectName(QString::fromUtf8("lineEdit_sCapital"));

        gridLayout_2->addWidget(lineEdit_sCapital, 2, 2, 1, 1);

        checkBox_TAX = new QCheckBox(tab_compagny);
        checkBox_TAX->setObjectName(QString::fromUtf8("checkBox_TAX"));
        checkBox_TAX->setEnabled(false);

        gridLayout_2->addWidget(checkBox_TAX, 2, 4, 1, 1);

        label_17 = new QLabel(tab_compagny);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 2, 3, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);

        label_10 = new QLabel(tab_compagny);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_4->addWidget(label_10);

        lineEdit_sAdd1 = new QLineEdit(tab_compagny);
        lineEdit_sAdd1->setObjectName(QString::fromUtf8("lineEdit_sAdd1"));
        lineEdit_sAdd1->setMaxLength(128);

        verticalLayout_4->addWidget(lineEdit_sAdd1);

        lineEdit_sAdd2 = new QLineEdit(tab_compagny);
        lineEdit_sAdd2->setObjectName(QString::fromUtf8("lineEdit_sAdd2"));
        lineEdit_sAdd2->setMaxLength(128);

        verticalLayout_4->addWidget(lineEdit_sAdd2);

        lineEdit_sAdd3 = new QLineEdit(tab_compagny);
        lineEdit_sAdd3->setObjectName(QString::fromUtf8("lineEdit_sAdd3"));
        lineEdit_sAdd3->setMaxLength(128);

        verticalLayout_4->addWidget(lineEdit_sAdd3);


        horizontalLayout_2->addLayout(verticalLayout_4);

        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/compagny"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_compagny, icon1, QString());
        tab_Database = new QWidget();
        tab_Database->setObjectName(QString::fromUtf8("tab_Database"));
        verticalLayout_3 = new QVBoxLayout(tab_Database);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(tab_Database);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_hostName = new QLineEdit(tab_Database);
        lineEdit_hostName->setObjectName(QString::fromUtf8("lineEdit_hostName"));

        gridLayout->addWidget(lineEdit_hostName, 0, 1, 1, 1);

        label_2 = new QLabel(tab_Database);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_port = new QLineEdit(tab_Database);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        gridLayout->addWidget(lineEdit_port, 1, 1, 1, 1);

        lineEdit_databaseName = new QLineEdit(tab_Database);
        lineEdit_databaseName->setObjectName(QString::fromUtf8("lineEdit_databaseName"));

        gridLayout->addWidget(lineEdit_databaseName, 2, 1, 1, 1);

        label_3 = new QLabel(tab_Database);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(tab_Database);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        lineEdit_login = new QLineEdit(tab_Database);
        lineEdit_login->setObjectName(QString::fromUtf8("lineEdit_login"));

        gridLayout->addWidget(lineEdit_login, 3, 1, 1, 1);

        label_5 = new QLabel(tab_Database);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        lineEdit_password = new QLineEdit(tab_Database);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_password, 4, 1, 1, 1);

        toolButton_BaseSelect = new QToolButton(tab_Database);
        toolButton_BaseSelect->setObjectName(QString::fromUtf8("toolButton_BaseSelect"));

        gridLayout->addWidget(toolButton_BaseSelect, 2, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_18 = new QLabel(tab_Database);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_4->addWidget(label_18);

        horizontalSpacer_3 = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        label_state = new QLabel(tab_Database);
        label_state->setObjectName(QString::fromUtf8("label_state"));

        horizontalLayout_4->addWidget(label_state);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pushButton_connect = new QPushButton(tab_Database);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));

        horizontalLayout_4->addWidget(pushButton_connect);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);

        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/database"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_Database, icon2, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(DialogSettings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(lineEdit_sName, lineEdit_sNum);
        QWidget::setTabOrder(lineEdit_sNum, lineEdit_sPhone);
        QWidget::setTabOrder(lineEdit_sPhone, lineEdit_sFax);
        QWidget::setTabOrder(lineEdit_sFax, lineEdit_sEmail);
        QWidget::setTabOrder(lineEdit_sEmail, lineEdit_sWebSite);
        QWidget::setTabOrder(lineEdit_sWebSite, lineEdit_sCity);
        QWidget::setTabOrder(lineEdit_sCity, lineEdit_sZipCode);
        QWidget::setTabOrder(lineEdit_sZipCode, lineEdit_sAdd1);
        QWidget::setTabOrder(lineEdit_sAdd1, lineEdit_sAdd2);
        QWidget::setTabOrder(lineEdit_sAdd2, lineEdit_sAdd3);
        QWidget::setTabOrder(lineEdit_sAdd3, buttonBox);
        QWidget::setTabOrder(buttonBox, lineEdit_password);
        QWidget::setTabOrder(lineEdit_password, lineEdit_port);
        QWidget::setTabOrder(lineEdit_port, lineEdit_login);
        QWidget::setTabOrder(lineEdit_login, lineEdit_hostName);
        QWidget::setTabOrder(lineEdit_hostName, lineEdit_databaseName);

        retranslateUi(DialogSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogSettings, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogSettings);
    } // setupUi

    void retranslateUi(QDialog *DialogSettings)
    {
        DialogSettings->setWindowTitle(QApplication::translate("DialogSettings", "Configuration", 0, QApplication::UnicodeUTF8));
        label_logo->setText(QApplication::translate("DialogSettings", "Logo", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("DialogSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" text-decoration: underline;\">Logo</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_Logo->setText(QApplication::translate("DialogSettings", "Parcourir", 0, QApplication::UnicodeUTF8));
        pushButton_ClearImage->setText(QApplication::translate("DialogSettings", "Effacer", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogSettings", "Nom: ", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogSettings", "Siret: ", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DialogSettings", "Tel: ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DialogSettings", "Fax: ", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("DialogSettings", "Email: ", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DialogSettings", "Site internet: ", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DialogSettings", "Ville: ", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DialogSettings", "Code Postal: ", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DialogSettings", "Capital: ", 0, QApplication::UnicodeUTF8));
        checkBox_TAX->setText(QString());
        label_17->setText(QApplication::translate("DialogSettings", "Assujetti \303\240 la TVA: ", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DialogSettings", "Adresse :", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_compagny), QApplication::translate("DialogSettings", "Soci\303\251t\303\251", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogSettings", "Adresse: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogSettings", "Port: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogSettings", "Nom de la base: ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogSettings", "Identifiant: ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogSettings", "Mot de pass: ", 0, QApplication::UnicodeUTF8));
        toolButton_BaseSelect->setText(QApplication::translate("DialogSettings", "...", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("DialogSettings", "Etat:", 0, QApplication::UnicodeUTF8));
        label_state->setText(QApplication::translate("DialogSettings", "...", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("DialogSettings", "Se connecter", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_Database), QApplication::translate("DialogSettings", "Base de donn\303\251es", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogSettings: public Ui_DialogSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSETTINGS_H

/********************************************************************************
** Form generated from reading UI file 'dialogsettings.ui'
**
** Created: Sat 1. Dec 18:09:11 2012
**      by: Qt User Interface Compiler version 4.8.2
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
#include <QtGui/QComboBox>
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
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QCheckBox *checkBox_TAX;
    QLineEdit *lineEdit_sPhone;
    QLabel *label_6;
    QLineEdit *lineEdit_sName;
    QLabel *label_14;
    QLabel *label_16;
    QLineEdit *lineEdit_sNum;
    QLineEdit *lineEdit_sCapital;
    QLabel *label_9;
    QLabel *label_12;
    QLineEdit *lineEdit_sFax;
    QLabel *label_17;
    QLabel *label_13;
    QLineEdit *lineEdit_sEmail;
    QLabel *label_8;
    QLineEdit *lineEdit_sWebSite;
    QLineEdit *lineEdit_sCity;
    QLabel *label_11;
    QLineEdit *lineEdit_sZipCode;
    QLabel *label_10;
    QLineEdit *lineEdit_sAdd1;
    QLineEdit *lineEdit_sAdd2;
    QLineEdit *lineEdit_sAdd3;
    QWidget *tab_bank;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_3;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *lineEdit_codeGuichet;
    QLabel *label_21;
    QLineEdit *lineEdit_codeBanque;
    QLineEdit *lineEdit_numCompte;
    QLabel *label_22;
    QLineEdit *lineEdit_keyRIB;
    QLabel *label_23;
    QLineEdit *lineEdit_Address;
    QGridLayout *gridLayout_4;
    QLabel *label_24;
    QLineEdit *lineEdit_IBAN1;
    QLineEdit *lineEdit_IBAN2;
    QLineEdit *lineEdit_IBAN3;
    QLineEdit *lineEdit_IBAN4;
    QLineEdit *lineEdit_IBAN6;
    QLineEdit *lineEdit_IBAN7;
    QLineEdit *lineEdit_IBAN5;
    QLineEdit *lineEdit_IBAN9;
    QLineEdit *lineEdit_IBAN8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_25;
    QLineEdit *lineEdit_BIC;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
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
    QLabel *label_26;
    QComboBox *comboBox_dbase;
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
        DialogSettings->resize(500, 412);
        DialogSettings->setMinimumSize(QSize(500, 0));
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

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(tab_compagny);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        checkBox_TAX = new QCheckBox(tab_compagny);
        checkBox_TAX->setObjectName(QString::fromUtf8("checkBox_TAX"));
        checkBox_TAX->setEnabled(false);

        gridLayout_2->addWidget(checkBox_TAX, 1, 3, 1, 1);

        lineEdit_sPhone = new QLineEdit(tab_compagny);
        lineEdit_sPhone->setObjectName(QString::fromUtf8("lineEdit_sPhone"));
        lineEdit_sPhone->setMaxLength(24);

        gridLayout_2->addWidget(lineEdit_sPhone, 2, 1, 1, 1);

        label_6 = new QLabel(tab_compagny);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 2, 1, 1);

        lineEdit_sName = new QLineEdit(tab_compagny);
        lineEdit_sName->setObjectName(QString::fromUtf8("lineEdit_sName"));
        lineEdit_sName->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sName, 0, 1, 1, 1);

        label_14 = new QLabel(tab_compagny);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 3, 2, 1, 1);

        label_16 = new QLabel(tab_compagny);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 1, 0, 1, 1);

        lineEdit_sNum = new QLineEdit(tab_compagny);
        lineEdit_sNum->setObjectName(QString::fromUtf8("lineEdit_sNum"));
        lineEdit_sNum->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sNum, 0, 3, 1, 1);

        lineEdit_sCapital = new QLineEdit(tab_compagny);
        lineEdit_sCapital->setObjectName(QString::fromUtf8("lineEdit_sCapital"));

        gridLayout_2->addWidget(lineEdit_sCapital, 1, 1, 1, 1);

        label_9 = new QLabel(tab_compagny);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 2, 0, 1, 1);

        label_12 = new QLabel(tab_compagny);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 2, 2, 1, 1);

        lineEdit_sFax = new QLineEdit(tab_compagny);
        lineEdit_sFax->setObjectName(QString::fromUtf8("lineEdit_sFax"));
        lineEdit_sFax->setMaxLength(24);

        gridLayout_2->addWidget(lineEdit_sFax, 2, 3, 1, 1);

        label_17 = new QLabel(tab_compagny);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 1, 2, 1, 1);

        label_13 = new QLabel(tab_compagny);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 3, 0, 1, 1);

        lineEdit_sEmail = new QLineEdit(tab_compagny);
        lineEdit_sEmail->setObjectName(QString::fromUtf8("lineEdit_sEmail"));
        lineEdit_sEmail->setMaxLength(128);

        gridLayout_2->addWidget(lineEdit_sEmail, 3, 1, 1, 1);

        label_8 = new QLabel(tab_compagny);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 4, 0, 1, 1);

        lineEdit_sWebSite = new QLineEdit(tab_compagny);
        lineEdit_sWebSite->setObjectName(QString::fromUtf8("lineEdit_sWebSite"));
        lineEdit_sWebSite->setMaxLength(256);

        gridLayout_2->addWidget(lineEdit_sWebSite, 3, 3, 1, 1);

        lineEdit_sCity = new QLineEdit(tab_compagny);
        lineEdit_sCity->setObjectName(QString::fromUtf8("lineEdit_sCity"));
        lineEdit_sCity->setMaxLength(64);

        gridLayout_2->addWidget(lineEdit_sCity, 4, 1, 1, 1);

        label_11 = new QLabel(tab_compagny);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 4, 2, 1, 1);

        lineEdit_sZipCode = new QLineEdit(tab_compagny);
        lineEdit_sZipCode->setObjectName(QString::fromUtf8("lineEdit_sZipCode"));
        lineEdit_sZipCode->setMaxLength(10);

        gridLayout_2->addWidget(lineEdit_sZipCode, 4, 3, 1, 1);


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
        tab_bank = new QWidget();
        tab_bank->setObjectName(QString::fromUtf8("tab_bank"));
        verticalLayout_7 = new QVBoxLayout(tab_bank);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_19 = new QLabel(tab_bank);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_3->addWidget(label_19, 0, 0, 1, 1);

        label_20 = new QLabel(tab_bank);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_3->addWidget(label_20, 2, 0, 1, 1);

        lineEdit_codeGuichet = new QLineEdit(tab_bank);
        lineEdit_codeGuichet->setObjectName(QString::fromUtf8("lineEdit_codeGuichet"));
        lineEdit_codeGuichet->setMaxLength(5);
        lineEdit_codeGuichet->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lineEdit_codeGuichet, 2, 1, 1, 1);

        label_21 = new QLabel(tab_bank);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_3->addWidget(label_21, 1, 0, 1, 1);

        lineEdit_codeBanque = new QLineEdit(tab_bank);
        lineEdit_codeBanque->setObjectName(QString::fromUtf8("lineEdit_codeBanque"));
        lineEdit_codeBanque->setMaxLength(5);
        lineEdit_codeBanque->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lineEdit_codeBanque, 0, 1, 1, 1);

        lineEdit_numCompte = new QLineEdit(tab_bank);
        lineEdit_numCompte->setObjectName(QString::fromUtf8("lineEdit_numCompte"));
        lineEdit_numCompte->setMaxLength(11);
        lineEdit_numCompte->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lineEdit_numCompte, 1, 1, 1, 1);

        label_22 = new QLabel(tab_bank);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_3->addWidget(label_22, 3, 0, 1, 1);

        lineEdit_keyRIB = new QLineEdit(tab_bank);
        lineEdit_keyRIB->setObjectName(QString::fromUtf8("lineEdit_keyRIB"));
        lineEdit_keyRIB->setMaxLength(2);
        lineEdit_keyRIB->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lineEdit_keyRIB, 3, 1, 1, 1);

        label_23 = new QLabel(tab_bank);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_3->addWidget(label_23, 4, 0, 1, 1);

        lineEdit_Address = new QLineEdit(tab_bank);
        lineEdit_Address->setObjectName(QString::fromUtf8("lineEdit_Address"));
        lineEdit_Address->setMaxLength(128);

        gridLayout_3->addWidget(lineEdit_Address, 4, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_3);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_24 = new QLabel(tab_bank);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_4->addWidget(label_24, 0, 0, 1, 1);

        lineEdit_IBAN1 = new QLineEdit(tab_bank);
        lineEdit_IBAN1->setObjectName(QString::fromUtf8("lineEdit_IBAN1"));
        lineEdit_IBAN1->setMaxLength(4);
        lineEdit_IBAN1->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN1, 1, 0, 1, 1);

        lineEdit_IBAN2 = new QLineEdit(tab_bank);
        lineEdit_IBAN2->setObjectName(QString::fromUtf8("lineEdit_IBAN2"));
        lineEdit_IBAN2->setMaxLength(4);
        lineEdit_IBAN2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN2, 1, 1, 1, 1);

        lineEdit_IBAN3 = new QLineEdit(tab_bank);
        lineEdit_IBAN3->setObjectName(QString::fromUtf8("lineEdit_IBAN3"));
        lineEdit_IBAN3->setMaxLength(4);
        lineEdit_IBAN3->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN3, 1, 2, 1, 1);

        lineEdit_IBAN4 = new QLineEdit(tab_bank);
        lineEdit_IBAN4->setObjectName(QString::fromUtf8("lineEdit_IBAN4"));
        lineEdit_IBAN4->setMaxLength(4);
        lineEdit_IBAN4->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN4, 1, 3, 1, 1);

        lineEdit_IBAN6 = new QLineEdit(tab_bank);
        lineEdit_IBAN6->setObjectName(QString::fromUtf8("lineEdit_IBAN6"));
        lineEdit_IBAN6->setMaxLength(4);
        lineEdit_IBAN6->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN6, 1, 5, 1, 1);

        lineEdit_IBAN7 = new QLineEdit(tab_bank);
        lineEdit_IBAN7->setObjectName(QString::fromUtf8("lineEdit_IBAN7"));
        lineEdit_IBAN7->setMaxLength(4);
        lineEdit_IBAN7->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN7, 1, 6, 1, 1);

        lineEdit_IBAN5 = new QLineEdit(tab_bank);
        lineEdit_IBAN5->setObjectName(QString::fromUtf8("lineEdit_IBAN5"));
        lineEdit_IBAN5->setMaxLength(4);
        lineEdit_IBAN5->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN5, 1, 4, 1, 1);

        lineEdit_IBAN9 = new QLineEdit(tab_bank);
        lineEdit_IBAN9->setObjectName(QString::fromUtf8("lineEdit_IBAN9"));
        lineEdit_IBAN9->setMaxLength(2);
        lineEdit_IBAN9->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN9, 2, 1, 1, 1);

        lineEdit_IBAN8 = new QLineEdit(tab_bank);
        lineEdit_IBAN8->setObjectName(QString::fromUtf8("lineEdit_IBAN8"));
        lineEdit_IBAN8->setMaxLength(4);
        lineEdit_IBAN8->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lineEdit_IBAN8, 2, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_25 = new QLabel(tab_bank);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_6->addWidget(label_25);

        lineEdit_BIC = new QLineEdit(tab_bank);
        lineEdit_BIC->setObjectName(QString::fromUtf8("lineEdit_BIC"));
        lineEdit_BIC->setMaxLength(11);
        lineEdit_BIC->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(lineEdit_BIC);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_6->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);


        verticalLayout_7->addLayout(verticalLayout_6);

        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/revenue"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_bank, icon2, QString());
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

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lineEdit_hostName = new QLineEdit(tab_Database);
        lineEdit_hostName->setObjectName(QString::fromUtf8("lineEdit_hostName"));

        gridLayout->addWidget(lineEdit_hostName, 1, 1, 1, 1);

        label_2 = new QLabel(tab_Database);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEdit_port = new QLineEdit(tab_Database);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        gridLayout->addWidget(lineEdit_port, 2, 1, 1, 1);

        lineEdit_databaseName = new QLineEdit(tab_Database);
        lineEdit_databaseName->setObjectName(QString::fromUtf8("lineEdit_databaseName"));

        gridLayout->addWidget(lineEdit_databaseName, 3, 1, 1, 1);

        label_3 = new QLabel(tab_Database);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_4 = new QLabel(tab_Database);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        lineEdit_login = new QLineEdit(tab_Database);
        lineEdit_login->setObjectName(QString::fromUtf8("lineEdit_login"));

        gridLayout->addWidget(lineEdit_login, 4, 1, 1, 1);

        label_5 = new QLabel(tab_Database);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        lineEdit_password = new QLineEdit(tab_Database);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_password, 5, 1, 1, 1);

        toolButton_BaseSelect = new QToolButton(tab_Database);
        toolButton_BaseSelect->setObjectName(QString::fromUtf8("toolButton_BaseSelect"));

        gridLayout->addWidget(toolButton_BaseSelect, 3, 2, 1, 1);

        label_26 = new QLabel(tab_Database);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout->addWidget(label_26, 0, 0, 1, 1);

        comboBox_dbase = new QComboBox(tab_Database);
        comboBox_dbase->setObjectName(QString::fromUtf8("comboBox_dbase"));

        gridLayout->addWidget(comboBox_dbase, 0, 1, 1, 1);


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

        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/database"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_Database, icon3, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(DialogSettings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(tabWidget, pushButton_Logo);
        QWidget::setTabOrder(pushButton_Logo, pushButton_ClearImage);
        QWidget::setTabOrder(pushButton_ClearImage, lineEdit_sName);
        QWidget::setTabOrder(lineEdit_sName, lineEdit_sNum);
        QWidget::setTabOrder(lineEdit_sNum, lineEdit_sCapital);
        QWidget::setTabOrder(lineEdit_sCapital, checkBox_TAX);
        QWidget::setTabOrder(checkBox_TAX, lineEdit_sPhone);
        QWidget::setTabOrder(lineEdit_sPhone, lineEdit_sFax);
        QWidget::setTabOrder(lineEdit_sFax, lineEdit_sEmail);
        QWidget::setTabOrder(lineEdit_sEmail, lineEdit_sWebSite);
        QWidget::setTabOrder(lineEdit_sWebSite, lineEdit_sCity);
        QWidget::setTabOrder(lineEdit_sCity, lineEdit_sZipCode);
        QWidget::setTabOrder(lineEdit_sZipCode, lineEdit_sAdd1);
        QWidget::setTabOrder(lineEdit_sAdd1, lineEdit_sAdd2);
        QWidget::setTabOrder(lineEdit_sAdd2, lineEdit_sAdd3);
        QWidget::setTabOrder(lineEdit_sAdd3, lineEdit_codeBanque);
        QWidget::setTabOrder(lineEdit_codeBanque, lineEdit_numCompte);
        QWidget::setTabOrder(lineEdit_numCompte, lineEdit_codeGuichet);
        QWidget::setTabOrder(lineEdit_codeGuichet, lineEdit_keyRIB);
        QWidget::setTabOrder(lineEdit_keyRIB, lineEdit_Address);
        QWidget::setTabOrder(lineEdit_Address, lineEdit_IBAN1);
        QWidget::setTabOrder(lineEdit_IBAN1, lineEdit_IBAN2);
        QWidget::setTabOrder(lineEdit_IBAN2, lineEdit_IBAN3);
        QWidget::setTabOrder(lineEdit_IBAN3, lineEdit_IBAN4);
        QWidget::setTabOrder(lineEdit_IBAN4, lineEdit_IBAN5);
        QWidget::setTabOrder(lineEdit_IBAN5, lineEdit_IBAN6);
        QWidget::setTabOrder(lineEdit_IBAN6, lineEdit_IBAN7);
        QWidget::setTabOrder(lineEdit_IBAN7, lineEdit_IBAN8);
        QWidget::setTabOrder(lineEdit_IBAN8, lineEdit_IBAN9);
        QWidget::setTabOrder(lineEdit_IBAN9, lineEdit_BIC);
        QWidget::setTabOrder(lineEdit_BIC, comboBox_dbase);
        QWidget::setTabOrder(comboBox_dbase, lineEdit_hostName);
        QWidget::setTabOrder(lineEdit_hostName, lineEdit_port);
        QWidget::setTabOrder(lineEdit_port, lineEdit_databaseName);
        QWidget::setTabOrder(lineEdit_databaseName, toolButton_BaseSelect);
        QWidget::setTabOrder(toolButton_BaseSelect, lineEdit_login);
        QWidget::setTabOrder(lineEdit_login, lineEdit_password);
        QWidget::setTabOrder(lineEdit_password, pushButton_connect);
        QWidget::setTabOrder(pushButton_connect, buttonBox);

        retranslateUi(DialogSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogSettings, SLOT(reject()));

        tabWidget->setCurrentIndex(2);


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
        checkBox_TAX->setText(QString());
        label_6->setText(QApplication::translate("DialogSettings", "Siret: ", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DialogSettings", "Site internet: ", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DialogSettings", "Capital: ", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DialogSettings", "Tel: ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DialogSettings", "Fax: ", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("DialogSettings", "Assujetti \303\240 la TVA: ", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("DialogSettings", "Email: ", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DialogSettings", "Ville: ", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DialogSettings", "Code Postal: ", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DialogSettings", "Adresse :", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_compagny), QApplication::translate("DialogSettings", "Soci\303\251t\303\251", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("DialogSettings", "Code Banque :", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("DialogSettings", "Code Guichet :", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("DialogSettings", "Num\303\251ro de compte :", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("DialogSettings", "Cl\303\251 RIB :", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("DialogSettings", "Domiciliation :", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("DialogSettings", "IBAN :", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("DialogSettings", "BIC :", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_bank), QApplication::translate("DialogSettings", "Relev\303\251 d'identit\303\251 bancaire", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogSettings", "Adresse : ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogSettings", "Port : ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogSettings", "Nom de la base : ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogSettings", "Identifiant : ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogSettings", "Mot de pass : ", 0, QApplication::UnicodeUTF8));
        toolButton_BaseSelect->setText(QApplication::translate("DialogSettings", "...", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("DialogSettings", "Base de donn\303\251es :", 0, QApplication::UnicodeUTF8));
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

/********************************************************************************
** Form generated from reading UI file 'customerView.ui'
**
** Created: Fri 25. Jan 18:33:04 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERVIEW_H
#define UI_CUSTOMERVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customerView
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QFrame *line;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *toolBut_Add;
    QToolButton *toolBut_Edit;
    QToolButton *toolButton_Del;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *comboBoxFiltre;
    QLineEdit *lineEdit_Search;
    QToolButton *toolBut_search;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_12;
    QLabel *LabelPa;
    QLineEdit *lineEdit_page;
    QLabel *label_pageMax;
    QSpacerItem *horizontalSpacer_7;
    QToolButton *toolButton_pagePrev;
    QToolButton *toolButton_pageNext;
    QTabWidget *tabWidget_Customer;
    QWidget *tab_file;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_13;
    QToolButton *toolButton_clipboard;
    QSpacerItem *horizontalSpacer_8;
    QLabel *labelInfoCustomer;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_services;
    QVBoxLayout *verticalLayout_11;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton_addService;
    QToolButton *toolButton_editService;
    QToolButton *toolButton_delService;
    QToolButton *toolButton_printServ;
    QSpacerItem *horizontalSpacer_6;
    QTableWidget *tableWidget_Services;
    QWidget *tab_estimates;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_11;
    QToolButton *toolButton_addProposal;
    QToolButton *toolButton_editProposal;
    QToolButton *toolButton_delProposal;
    QToolButton *toolButton_printPro;
    QSpacerItem *horizontalSpacer_5;
    QTableWidget *tableWidget_Proposals;
    QWidget *tab_invoices;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QToolButton *toolButton_addInvoice;
    QToolButton *toolButton_editInvoice;
    QToolButton *toolButton_delInvoice;
    QToolButton *toolButton_printInv;
    QSpacerItem *horizontalSpacer_4;
    QTableWidget *tableWidget_Invoices;
    QLabel *labelCustomersNumber;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *customerView)
    {
        if (customerView->objectName().isEmpty())
            customerView->setObjectName(QString::fromUtf8("customerView"));
        customerView->resize(640, 509);
        horizontalLayout = new QHBoxLayout(customerView);
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(6);
#endif
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(customerView);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);
        labelTitle->setMargin(5);

        verticalLayout->addWidget(labelTitle);

        line = new QFrame(customerView);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_4->setContentsMargins(-1, -1, 5, -1);
        toolBut_Add = new QToolButton(customerView);
        toolBut_Add->setObjectName(QString::fromUtf8("toolBut_Add"));
        toolBut_Add->setMinimumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/adduser"), QSize(), QIcon::Normal, QIcon::Off);
        toolBut_Add->setIcon(icon);
        toolBut_Add->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(toolBut_Add);

        toolBut_Edit = new QToolButton(customerView);
        toolBut_Edit->setObjectName(QString::fromUtf8("toolBut_Edit"));
        toolBut_Edit->setMinimumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        toolBut_Edit->setIcon(icon1);
        toolBut_Edit->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(toolBut_Edit);

        toolButton_Del = new QToolButton(customerView);
        toolButton_Del->setObjectName(QString::fromUtf8("toolButton_Del"));
        toolButton_Del->setEnabled(true);
        toolButton_Del->setMinimumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_Del->setIcon(icon2);
        toolButton_Del->setIconSize(QSize(16, 16));

        horizontalLayout_4->addWidget(toolButton_Del);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        comboBoxFiltre = new QComboBox(customerView);
        comboBoxFiltre->setObjectName(QString::fromUtf8("comboBoxFiltre"));

        horizontalLayout_5->addWidget(comboBoxFiltre);

        lineEdit_Search = new QLineEdit(customerView);
        lineEdit_Search->setObjectName(QString::fromUtf8("lineEdit_Search"));
        lineEdit_Search->setMaxLength(128);

        horizontalLayout_5->addWidget(lineEdit_Search);

        toolBut_search = new QToolButton(customerView);
        toolBut_search->setObjectName(QString::fromUtf8("toolBut_search"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/search"), QSize(), QIcon::Normal, QIcon::Off);
        toolBut_search->setIcon(icon3);
        toolBut_search->setIconSize(QSize(24, 24));

        horizontalLayout_5->addWidget(toolBut_search);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        tableWidget = new QTableWidget(customerView);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(350, 0));
        tableWidget->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_4->addWidget(tableWidget);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        LabelPa = new QLabel(customerView);
        LabelPa->setObjectName(QString::fromUtf8("LabelPa"));

        horizontalLayout_12->addWidget(LabelPa);

        lineEdit_page = new QLineEdit(customerView);
        lineEdit_page->setObjectName(QString::fromUtf8("lineEdit_page"));
        lineEdit_page->setMaximumSize(QSize(50, 16777215));
        lineEdit_page->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(lineEdit_page);

        label_pageMax = new QLabel(customerView);
        label_pageMax->setObjectName(QString::fromUtf8("label_pageMax"));

        horizontalLayout_12->addWidget(label_pageMax);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_7);

        toolButton_pagePrev = new QToolButton(customerView);
        toolButton_pagePrev->setObjectName(QString::fromUtf8("toolButton_pagePrev"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/prev"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_pagePrev->setIcon(icon4);
        toolButton_pagePrev->setIconSize(QSize(16, 16));

        horizontalLayout_12->addWidget(toolButton_pagePrev);

        toolButton_pageNext = new QToolButton(customerView);
        toolButton_pageNext->setObjectName(QString::fromUtf8("toolButton_pageNext"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/app/next"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_pageNext->setIcon(icon5);

        horizontalLayout_12->addWidget(toolButton_pageNext);


        verticalLayout_4->addLayout(horizontalLayout_12);


        horizontalLayout_2->addLayout(verticalLayout_4);

        tabWidget_Customer = new QTabWidget(customerView);
        tabWidget_Customer->setObjectName(QString::fromUtf8("tabWidget_Customer"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget_Customer->sizePolicy().hasHeightForWidth());
        tabWidget_Customer->setSizePolicy(sizePolicy1);
        tabWidget_Customer->setTabShape(QTabWidget::Rounded);
        tabWidget_Customer->setIconSize(QSize(24, 24));
        tab_file = new QWidget();
        tab_file->setObjectName(QString::fromUtf8("tab_file"));
        horizontalLayout_6 = new QHBoxLayout(tab_file);
        horizontalLayout_6->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        scrollArea = new QScrollArea(tab_file);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 256, 303));
        verticalLayout_9 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        toolButton_clipboard = new QToolButton(scrollAreaWidgetContents);
        toolButton_clipboard->setObjectName(QString::fromUtf8("toolButton_clipboard"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/app/venteComptoire"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_clipboard->setIcon(icon6);
        toolButton_clipboard->setIconSize(QSize(24, 24));

        horizontalLayout_13->addWidget(toolButton_clipboard);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_8);


        verticalLayout_8->addLayout(horizontalLayout_13);

        labelInfoCustomer = new QLabel(scrollAreaWidgetContents);
        labelInfoCustomer->setObjectName(QString::fromUtf8("labelInfoCustomer"));
        labelInfoCustomer->setMargin(5);

        verticalLayout_8->addWidget(labelInfoCustomer);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_3);


        verticalLayout_9->addLayout(verticalLayout_8);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);


        horizontalLayout_6->addLayout(verticalLayout_5);

        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/app/customer_info"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Customer->addTab(tab_file, icon7, QString());
        tab_services = new QWidget();
        tab_services->setObjectName(QString::fromUtf8("tab_services"));
        verticalLayout_11 = new QVBoxLayout(tab_services);
        verticalLayout_11->setContentsMargins(3, 3, 3, 3);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        toolButton_addService = new QToolButton(tab_services);
        toolButton_addService->setObjectName(QString::fromUtf8("toolButton_addService"));
        toolButton_addService->setMinimumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_addService->setIcon(icon8);
        toolButton_addService->setIconSize(QSize(24, 24));

        horizontalLayout_3->addWidget(toolButton_addService);

        toolButton_editService = new QToolButton(tab_services);
        toolButton_editService->setObjectName(QString::fromUtf8("toolButton_editService"));
        toolButton_editService->setMinimumSize(QSize(32, 32));
        toolButton_editService->setIcon(icon1);
        toolButton_editService->setIconSize(QSize(24, 24));

        horizontalLayout_3->addWidget(toolButton_editService);

        toolButton_delService = new QToolButton(tab_services);
        toolButton_delService->setObjectName(QString::fromUtf8("toolButton_delService"));
        toolButton_delService->setMinimumSize(QSize(32, 32));
        toolButton_delService->setIcon(icon2);
        toolButton_delService->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(toolButton_delService);

        toolButton_printServ = new QToolButton(tab_services);
        toolButton_printServ->setObjectName(QString::fromUtf8("toolButton_printServ"));
        toolButton_printServ->setMinimumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/app/print"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_printServ->setIcon(icon9);
        toolButton_printServ->setIconSize(QSize(22, 22));

        horizontalLayout_3->addWidget(toolButton_printServ);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout_10->addLayout(horizontalLayout_3);

        tableWidget_Services = new QTableWidget(tab_services);
        tableWidget_Services->setObjectName(QString::fromUtf8("tableWidget_Services"));

        verticalLayout_10->addWidget(tableWidget_Services);


        verticalLayout_11->addLayout(verticalLayout_10);

        tabWidget_Customer->addTab(tab_services, QString());
        tab_estimates = new QWidget();
        tab_estimates->setObjectName(QString::fromUtf8("tab_estimates"));
        horizontalLayout_10 = new QHBoxLayout(tab_estimates);
        horizontalLayout_10->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        toolButton_addProposal = new QToolButton(tab_estimates);
        toolButton_addProposal->setObjectName(QString::fromUtf8("toolButton_addProposal"));
        toolButton_addProposal->setMinimumSize(QSize(32, 32));
        toolButton_addProposal->setIcon(icon8);
        toolButton_addProposal->setIconSize(QSize(24, 24));

        horizontalLayout_11->addWidget(toolButton_addProposal);

        toolButton_editProposal = new QToolButton(tab_estimates);
        toolButton_editProposal->setObjectName(QString::fromUtf8("toolButton_editProposal"));
        toolButton_editProposal->setMinimumSize(QSize(32, 32));
        toolButton_editProposal->setIcon(icon1);
        toolButton_editProposal->setIconSize(QSize(24, 24));

        horizontalLayout_11->addWidget(toolButton_editProposal);

        toolButton_delProposal = new QToolButton(tab_estimates);
        toolButton_delProposal->setObjectName(QString::fromUtf8("toolButton_delProposal"));
        toolButton_delProposal->setMinimumSize(QSize(32, 32));
        toolButton_delProposal->setIcon(icon2);
        toolButton_delProposal->setIconSize(QSize(16, 16));

        horizontalLayout_11->addWidget(toolButton_delProposal);

        toolButton_printPro = new QToolButton(tab_estimates);
        toolButton_printPro->setObjectName(QString::fromUtf8("toolButton_printPro"));
        toolButton_printPro->setMinimumSize(QSize(32, 32));
        toolButton_printPro->setIcon(icon9);
        toolButton_printPro->setIconSize(QSize(22, 22));

        horizontalLayout_11->addWidget(toolButton_printPro);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_5);


        verticalLayout_7->addLayout(horizontalLayout_11);

        tableWidget_Proposals = new QTableWidget(tab_estimates);
        tableWidget_Proposals->setObjectName(QString::fromUtf8("tableWidget_Proposals"));

        verticalLayout_7->addWidget(tableWidget_Proposals);


        horizontalLayout_10->addLayout(verticalLayout_7);

        tabWidget_Customer->addTab(tab_estimates, QString());
        tab_invoices = new QWidget();
        tab_invoices->setObjectName(QString::fromUtf8("tab_invoices"));
        verticalLayout_3 = new QVBoxLayout(tab_invoices);
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        toolButton_addInvoice = new QToolButton(tab_invoices);
        toolButton_addInvoice->setObjectName(QString::fromUtf8("toolButton_addInvoice"));
        toolButton_addInvoice->setMinimumSize(QSize(32, 32));
        toolButton_addInvoice->setIcon(icon8);
        toolButton_addInvoice->setIconSize(QSize(24, 24));

        horizontalLayout_9->addWidget(toolButton_addInvoice);

        toolButton_editInvoice = new QToolButton(tab_invoices);
        toolButton_editInvoice->setObjectName(QString::fromUtf8("toolButton_editInvoice"));
        toolButton_editInvoice->setMinimumSize(QSize(32, 32));
        toolButton_editInvoice->setIcon(icon1);
        toolButton_editInvoice->setIconSize(QSize(24, 24));

        horizontalLayout_9->addWidget(toolButton_editInvoice);

        toolButton_delInvoice = new QToolButton(tab_invoices);
        toolButton_delInvoice->setObjectName(QString::fromUtf8("toolButton_delInvoice"));
        toolButton_delInvoice->setMinimumSize(QSize(32, 32));
        toolButton_delInvoice->setIcon(icon2);
        toolButton_delInvoice->setIconSize(QSize(16, 16));

        horizontalLayout_9->addWidget(toolButton_delInvoice);

        toolButton_printInv = new QToolButton(tab_invoices);
        toolButton_printInv->setObjectName(QString::fromUtf8("toolButton_printInv"));
        toolButton_printInv->setMinimumSize(QSize(32, 32));
        toolButton_printInv->setIcon(icon9);
        toolButton_printInv->setIconSize(QSize(22, 22));

        horizontalLayout_9->addWidget(toolButton_printInv);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_9);

        tableWidget_Invoices = new QTableWidget(tab_invoices);
        tableWidget_Invoices->setObjectName(QString::fromUtf8("tableWidget_Invoices"));

        verticalLayout_2->addWidget(tableWidget_Invoices);


        verticalLayout_3->addLayout(verticalLayout_2);

        tabWidget_Customer->addTab(tab_invoices, QString());

        horizontalLayout_2->addWidget(tabWidget_Customer);


        verticalLayout->addLayout(horizontalLayout_2);

        labelCustomersNumber = new QLabel(customerView);
        labelCustomersNumber->setObjectName(QString::fromUtf8("labelCustomersNumber"));

        verticalLayout->addWidget(labelCustomersNumber);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(customerView);

        tabWidget_Customer->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(customerView);
    } // setupUi

    void retranslateUi(QWidget *customerView)
    {
        customerView->setWindowTitle(QApplication::translate("customerView", "Form", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("customerView", "Clients", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolBut_Add->setToolTip(QApplication::translate("customerView", "Ajouter un client", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBut_Add->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolBut_Edit->setToolTip(QApplication::translate("customerView", "Modifier le client", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBut_Edit->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_Del->setToolTip(QApplication::translate("customerView", "Supprimer le client", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_Del->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_Search->setToolTip(QApplication::translate("customerView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Chercher un client...</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        toolBut_search->setToolTip(QApplication::translate("customerView", "Chercher un client", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBut_search->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        LabelPa->setText(QApplication::translate("customerView", "Page", 0, QApplication::UnicodeUTF8));
        label_pageMax->setText(QApplication::translate("customerView", "/", 0, QApplication::UnicodeUTF8));
        toolButton_pagePrev->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        toolButton_pageNext->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_clipboard->setToolTip(QApplication::translate("customerView", "Copier dans le presse papier", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_clipboard->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        labelInfoCustomer->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        tabWidget_Customer->setTabText(tabWidget_Customer->indexOf(tab_file), QApplication::translate("customerView", "Fiche", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_addService->setToolTip(QApplication::translate("customerView", "Ajouter un service", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_addService->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolButton_editService->setToolTip(QApplication::translate("customerView", "Editer un service", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editService->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolButton_delService->setToolTip(QApplication::translate("customerView", "Supprimer un service", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_delService->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolButton_printServ->setToolTip(QApplication::translate("customerView", "imprimer un service", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_printServ->setText(QString());
        tabWidget_Customer->setTabText(tabWidget_Customer->indexOf(tab_services), QApplication::translate("customerView", "Services", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_addProposal->setToolTip(QApplication::translate("customerView", "Cr\303\251er un devis", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_addProposal->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editProposal->setToolTip(QApplication::translate("customerView", "Modifier le devis s\303\251lectionn\303\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editProposal->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_delProposal->setToolTip(QApplication::translate("customerView", "Supprimer le devis s\303\251lectionn\303\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_delProposal->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_printPro->setToolTip(QApplication::translate("customerView", "Imprimer le devis", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_printPro->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        tabWidget_Customer->setTabText(tabWidget_Customer->indexOf(tab_estimates), QApplication::translate("customerView", "Devis / Propositions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_addInvoice->setToolTip(QApplication::translate("customerView", "Cr\303\251er une facture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_addInvoice->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editInvoice->setToolTip(QApplication::translate("customerView", "Modifier la facture s\303\251lectionn\303\251e", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editInvoice->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_delInvoice->setToolTip(QApplication::translate("customerView", "Supprimer la facture s\303\251lectionn\303\251e", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_delInvoice->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_printInv->setToolTip(QApplication::translate("customerView", "Imprimer la facture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_printInv->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        tabWidget_Customer->setTabText(tabWidget_Customer->indexOf(tab_invoices), QApplication::translate("customerView", "Factures", 0, QApplication::UnicodeUTF8));
        labelCustomersNumber->setText(QApplication::translate("customerView", "Nombre de clients :", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customerView: public Ui_customerView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERVIEW_H

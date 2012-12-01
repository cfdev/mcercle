/********************************************************************************
** Form generated from reading UI file 'customer.ui'
**
** Created: Mon 14. Jun 14:20:56 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMER_H
#define UI_CUSTOMER_H

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
#include <QtGui/QPushButton>
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
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *comboBoxFiltre;
    QLineEdit *lineEdit_Search;
    QToolButton *toolBut_search;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *tableWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QLabel *labelInfoCustomer;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_4;
    QWidget *tab_2;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushBut_pdfFacture;
    QPushButton *pushBut_printFacture;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *customerView)
    {
        if (customerView->objectName().isEmpty())
            customerView->setObjectName(QString::fromUtf8("customerView"));
        customerView->resize(640, 335);
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
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/search"), QSize(), QIcon::Normal, QIcon::Off);
        toolBut_search->setIcon(icon2);

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


        horizontalLayout_2->addLayout(verticalLayout_4);

        tabWidget = new QTabWidget(customerView);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(24, 24));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_6 = new QHBoxLayout(tab);
        horizontalLayout_6->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        labelInfoCustomer = new QLabel(tab);
        labelInfoCustomer->setObjectName(QString::fromUtf8("labelInfoCustomer"));
        labelInfoCustomer->setMargin(5);

        verticalLayout_5->addWidget(labelInfoCustomer);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        horizontalLayout_6->addLayout(verticalLayout_5);

        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/customer_info"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab, icon3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushBut_pdfFacture = new QPushButton(tab_3);
        pushBut_pdfFacture->setObjectName(QString::fromUtf8("pushBut_pdfFacture"));

        horizontalLayout_3->addWidget(pushBut_pdfFacture);

        pushBut_printFacture = new QPushButton(tab_3);
        pushBut_printFacture->setObjectName(QString::fromUtf8("pushBut_printFacture"));

        horizontalLayout_3->addWidget(pushBut_printFacture);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout_2);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout_2->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(customerView);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(customerView);
    } // setupUi

    void retranslateUi(QWidget *customerView)
    {
        customerView->setWindowTitle(QApplication::translate("customerView", "Form", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("customerView", "Clients", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolBut_Add->setToolTip(QApplication::translate("customerView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Ajouter un client</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBut_Add->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolBut_Edit->setToolTip(QApplication::translate("customerView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Editer le client s\303\251lectionn\303\251</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBut_Edit->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
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
        toolBut_search->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        labelInfoCustomer->setText(QApplication::translate("customerView", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("customerView", "Fiche", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("customerView", "Commandes", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("customerView", "Devis / Pro forma", 0, QApplication::UnicodeUTF8));
        pushBut_pdfFacture->setText(QApplication::translate("customerView", "Export PDF", 0, QApplication::UnicodeUTF8));
        pushBut_printFacture->setText(QApplication::translate("customerView", "Imprimer", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("customerView", "Factures", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customerView: public Ui_customerView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMER_H

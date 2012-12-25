/********************************************************************************
** Form generated from reading UI file 'dialoginvoice.ui'
**
** Created: Tue 25. Dec 16:28:15 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGINVOICE_H
#define UI_DIALOGINVOICE_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogInvoice
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QFrame *line;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_description;
    QLabel *label_3;
    QLineEdit *lineEdit_code;
    QDateEdit *dateEdit_DATE;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_state;
    QComboBox *comboBox_State;
    QLabel *label_payment;
    QComboBox *comboBox_TYPE_PAYMENT;
    QLabel *label_date;
    QDateEdit *dateEdit_valid;
    QLabel *label_datevalid;
    QLabel *label_delivery;
    QDateEdit *dateEdit_delivery;
    QLabel *label_delay;
    QSpinBox *spinBox_delayDelivery;
    QLabel *label_link;
    QLineEdit *lineEdit_linkCode;
    QLabel *label_partpayment;
    QDoubleSpinBox *doubleSpinBox_partPAYMENT;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_select;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget_select;
    QWidget *tabService;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *tableWidget_selectService;
    QWidget *tabProduct;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *productLayout;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_2;
    QToolButton *toolButton_add;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_invoice;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_but;
    QToolButton *toolButton_addFreeline;
    QToolButton *toolButton_rm;
    QToolButton *toolButton_up;
    QToolButton *toolButton_dn;
    QSpacerItem *horizontalSpacer_3;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_Total;
    QHBoxLayout *horizontalLayout_button;
    QPushButton *pushButton_createInv;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *DialogInvoice)
    {
        if (DialogInvoice->objectName().isEmpty())
            DialogInvoice->setObjectName(QString::fromUtf8("DialogInvoice"));
        DialogInvoice->resize(635, 484);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        DialogInvoice->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogInvoice);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(DialogInvoice);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);
        labelTitle->setMargin(0);

        verticalLayout->addWidget(labelTitle);

        line = new QFrame(DialogInvoice);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DialogInvoice);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(DialogInvoice);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_description = new QLineEdit(DialogInvoice);
        lineEdit_description->setObjectName(QString::fromUtf8("lineEdit_description"));
        lineEdit_description->setMaxLength(255);

        gridLayout->addWidget(lineEdit_description, 0, 1, 1, 1);

        label_3 = new QLabel(DialogInvoice);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        lineEdit_code = new QLineEdit(DialogInvoice);
        lineEdit_code->setObjectName(QString::fromUtf8("lineEdit_code"));
        lineEdit_code->setEnabled(false);
        lineEdit_code->setMaxLength(255);
        lineEdit_code->setReadOnly(false);

        gridLayout->addWidget(lineEdit_code, 1, 1, 1, 1);

        dateEdit_DATE = new QDateEdit(DialogInvoice);
        dateEdit_DATE->setObjectName(QString::fromUtf8("dateEdit_DATE"));
        dateEdit_DATE->setDate(QDate(2000, 1, 1));
        dateEdit_DATE->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit_DATE, 1, 3, 1, 1);

        dateTimeEdit = new QDateTimeEdit(DialogInvoice);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setEnabled(false);
        dateTimeEdit->setReadOnly(false);
        dateTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(dateTimeEdit, 0, 3, 1, 1);

        label_state = new QLabel(DialogInvoice);
        label_state->setObjectName(QString::fromUtf8("label_state"));

        gridLayout->addWidget(label_state, 0, 4, 1, 1);

        comboBox_State = new QComboBox(DialogInvoice);
        comboBox_State->setObjectName(QString::fromUtf8("comboBox_State"));

        gridLayout->addWidget(comboBox_State, 0, 5, 1, 1);

        label_payment = new QLabel(DialogInvoice);
        label_payment->setObjectName(QString::fromUtf8("label_payment"));

        gridLayout->addWidget(label_payment, 1, 4, 1, 1);

        comboBox_TYPE_PAYMENT = new QComboBox(DialogInvoice);
        comboBox_TYPE_PAYMENT->setObjectName(QString::fromUtf8("comboBox_TYPE_PAYMENT"));

        gridLayout->addWidget(comboBox_TYPE_PAYMENT, 1, 5, 1, 1);

        label_date = new QLabel(DialogInvoice);
        label_date->setObjectName(QString::fromUtf8("label_date"));

        gridLayout->addWidget(label_date, 1, 2, 1, 1);

        dateEdit_valid = new QDateEdit(DialogInvoice);
        dateEdit_valid->setObjectName(QString::fromUtf8("dateEdit_valid"));
        dateEdit_valid->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit_valid, 2, 5, 1, 1);

        label_datevalid = new QLabel(DialogInvoice);
        label_datevalid->setObjectName(QString::fromUtf8("label_datevalid"));

        gridLayout->addWidget(label_datevalid, 2, 4, 1, 1);

        label_delivery = new QLabel(DialogInvoice);
        label_delivery->setObjectName(QString::fromUtf8("label_delivery"));

        gridLayout->addWidget(label_delivery, 2, 2, 1, 1);

        dateEdit_delivery = new QDateEdit(DialogInvoice);
        dateEdit_delivery->setObjectName(QString::fromUtf8("dateEdit_delivery"));
        dateEdit_delivery->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit_delivery, 2, 3, 1, 1);

        label_delay = new QLabel(DialogInvoice);
        label_delay->setObjectName(QString::fromUtf8("label_delay"));

        gridLayout->addWidget(label_delay, 3, 2, 1, 1);

        spinBox_delayDelivery = new QSpinBox(DialogInvoice);
        spinBox_delayDelivery->setObjectName(QString::fromUtf8("spinBox_delayDelivery"));
        spinBox_delayDelivery->setMaximum(9999);

        gridLayout->addWidget(spinBox_delayDelivery, 3, 3, 1, 1);

        label_link = new QLabel(DialogInvoice);
        label_link->setObjectName(QString::fromUtf8("label_link"));

        gridLayout->addWidget(label_link, 2, 0, 1, 1);

        lineEdit_linkCode = new QLineEdit(DialogInvoice);
        lineEdit_linkCode->setObjectName(QString::fromUtf8("lineEdit_linkCode"));
        lineEdit_linkCode->setEnabled(false);

        gridLayout->addWidget(lineEdit_linkCode, 2, 1, 1, 1);

        label_partpayment = new QLabel(DialogInvoice);
        label_partpayment->setObjectName(QString::fromUtf8("label_partpayment"));

        gridLayout->addWidget(label_partpayment, 3, 4, 1, 1);

        doubleSpinBox_partPAYMENT = new QDoubleSpinBox(DialogInvoice);
        doubleSpinBox_partPAYMENT->setObjectName(QString::fromUtf8("doubleSpinBox_partPAYMENT"));
        doubleSpinBox_partPAYMENT->setMaximum(1e+06);

        gridLayout->addWidget(doubleSpinBox_partPAYMENT, 3, 5, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_select = new QGroupBox(DialogInvoice);
        groupBox_select->setObjectName(QString::fromUtf8("groupBox_select"));
        verticalLayout_3 = new QVBoxLayout(groupBox_select);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget_select = new QTabWidget(groupBox_select);
        tabWidget_select->setObjectName(QString::fromUtf8("tabWidget_select"));
        tabWidget_select->setIconSize(QSize(24, 24));
        tabWidget_select->setDocumentMode(false);
        tabService = new QWidget();
        tabService->setObjectName(QString::fromUtf8("tabService"));
        verticalLayout_6 = new QVBoxLayout(tabService);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        tableWidget_selectService = new QTableWidget(tabService);
        tableWidget_selectService->setObjectName(QString::fromUtf8("tableWidget_selectService"));

        verticalLayout_6->addWidget(tableWidget_selectService);

        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/venteComptoire"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_select->addTab(tabService, icon1, QString());
        tabProduct = new QWidget();
        tabProduct->setObjectName(QString::fromUtf8("tabProduct"));
        verticalLayout_7 = new QVBoxLayout(tabProduct);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        productLayout = new QHBoxLayout();
        productLayout->setObjectName(QString::fromUtf8("productLayout"));

        verticalLayout_7->addLayout(productLayout);

        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/products"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_select->addTab(tabProduct, icon2, QString());

        verticalLayout_3->addWidget(tabWidget_select);


        horizontalLayout->addWidget(groupBox_select);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        toolButton_add = new QToolButton(DialogInvoice);
        toolButton_add->setObjectName(QString::fromUtf8("toolButton_add"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/next"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_add->setIcon(icon3);
        toolButton_add->setIconSize(QSize(24, 24));

        verticalLayout_5->addWidget(toolButton_add);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_5);

        groupBox_invoice = new QGroupBox(DialogInvoice);
        groupBox_invoice->setObjectName(QString::fromUtf8("groupBox_invoice"));
        verticalLayout_4 = new QVBoxLayout(groupBox_invoice);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_but = new QHBoxLayout();
        horizontalLayout_but->setObjectName(QString::fromUtf8("horizontalLayout_but"));
        toolButton_addFreeline = new QToolButton(groupBox_invoice);
        toolButton_addFreeline->setObjectName(QString::fromUtf8("toolButton_addFreeline"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_addFreeline->setIcon(icon4);
        toolButton_addFreeline->setIconSize(QSize(24, 24));

        horizontalLayout_but->addWidget(toolButton_addFreeline);

        toolButton_rm = new QToolButton(groupBox_invoice);
        toolButton_rm->setObjectName(QString::fromUtf8("toolButton_rm"));
        toolButton_rm->setMinimumSize(QSize(32, 24));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_rm->setIcon(icon5);
        toolButton_rm->setIconSize(QSize(16, 24));

        horizontalLayout_but->addWidget(toolButton_rm);

        toolButton_up = new QToolButton(groupBox_invoice);
        toolButton_up->setObjectName(QString::fromUtf8("toolButton_up"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/app/up"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_up->setIcon(icon6);
        toolButton_up->setIconSize(QSize(24, 24));

        horizontalLayout_but->addWidget(toolButton_up);

        toolButton_dn = new QToolButton(groupBox_invoice);
        toolButton_dn->setObjectName(QString::fromUtf8("toolButton_dn"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/app/dn"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_dn->setIcon(icon7);
        toolButton_dn->setIconSize(QSize(24, 24));

        horizontalLayout_but->addWidget(toolButton_dn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_but->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_but);

        tableWidget = new QTableWidget(groupBox_invoice);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout_4->addWidget(tableWidget);


        horizontalLayout->addWidget(groupBox_invoice);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_Total = new QLabel(DialogInvoice);
        label_Total->setObjectName(QString::fromUtf8("label_Total"));

        horizontalLayout_2->addWidget(label_Total);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_button = new QHBoxLayout();
        horizontalLayout_button->setObjectName(QString::fromUtf8("horizontalLayout_button"));
        pushButton_createInv = new QPushButton(DialogInvoice);
        pushButton_createInv->setObjectName(QString::fromUtf8("pushButton_createInv"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/app/ok"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_createInv->setIcon(icon8);

        horizontalLayout_button->addWidget(pushButton_createInv);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_button->addItem(horizontalSpacer_2);

        pushButton_ok = new QPushButton(DialogInvoice);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setAutoDefault(false);

        horizontalLayout_button->addWidget(pushButton_ok);

        pushButton_cancel = new QPushButton(DialogInvoice);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/app/quit"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_cancel->setIcon(icon9);
        pushButton_cancel->setAutoDefault(false);

        horizontalLayout_button->addWidget(pushButton_cancel);


        verticalLayout_2->addLayout(horizontalLayout_button);

        QWidget::setTabOrder(lineEdit_description, lineEdit_code);
        QWidget::setTabOrder(lineEdit_code, dateTimeEdit);
        QWidget::setTabOrder(dateTimeEdit, dateEdit_DATE);
        QWidget::setTabOrder(dateEdit_DATE, comboBox_State);
        QWidget::setTabOrder(comboBox_State, comboBox_TYPE_PAYMENT);
        QWidget::setTabOrder(comboBox_TYPE_PAYMENT, tableWidget);
        QWidget::setTabOrder(tableWidget, toolButton_add);
        QWidget::setTabOrder(toolButton_add, pushButton_ok);
        QWidget::setTabOrder(pushButton_ok, pushButton_cancel);

        retranslateUi(DialogInvoice);

        tabWidget_select->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogInvoice);
    } // setupUi

    void retranslateUi(QDialog *DialogInvoice)
    {
        DialogInvoice->setWindowTitle(QString());
        labelTitle->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogInvoice", "Description :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogInvoice", "Code :", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogInvoice", "Date de cr\303\251ation :", 0, QApplication::UnicodeUTF8));
        dateEdit_DATE->setDisplayFormat(QApplication::translate("DialogInvoice", "dd/MM/yyyy", 0, QApplication::UnicodeUTF8));
        dateTimeEdit->setDisplayFormat(QApplication::translate("DialogInvoice", "dd/MM/yyyy HH:mm", 0, QApplication::UnicodeUTF8));
        label_state->setText(QApplication::translate("DialogInvoice", "Etat :", 0, QApplication::UnicodeUTF8));
        label_payment->setText(QApplication::translate("DialogInvoice", "R\303\251glement :", 0, QApplication::UnicodeUTF8));
        label_date->setText(QApplication::translate("DialogInvoice", "Date :", 0, QApplication::UnicodeUTF8));
        dateEdit_valid->setDisplayFormat(QApplication::translate("DialogInvoice", "dd/MM/yyyy", 0, QApplication::UnicodeUTF8));
        label_datevalid->setText(QApplication::translate("DialogInvoice", "Fin de validit\303\251 :", 0, QApplication::UnicodeUTF8));
        label_delivery->setText(QApplication::translate("DialogInvoice", "Date de livraison :", 0, QApplication::UnicodeUTF8));
        dateEdit_delivery->setDisplayFormat(QApplication::translate("DialogInvoice", "dd/MM/yyyy", 0, QApplication::UnicodeUTF8));
        label_delay->setText(QApplication::translate("DialogInvoice", "Dur\303\251e de livraison\n"
"(en jours) :", 0, QApplication::UnicodeUTF8));
        label_link->setText(QApplication::translate("DialogInvoice", "Facture associ\303\251e : ", 0, QApplication::UnicodeUTF8));
        label_partpayment->setText(QApplication::translate("DialogInvoice", "Accompte :", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_partPAYMENT->setPrefix(QString());
        doubleSpinBox_partPAYMENT->setSuffix(QApplication::translate("DialogInvoice", " \342\202\254", 0, QApplication::UnicodeUTF8));
        groupBox_select->setTitle(QString());
        tabWidget_select->setTabText(tabWidget_select->indexOf(tabService), QApplication::translate("DialogInvoice", "Services", 0, QApplication::UnicodeUTF8));
        tabWidget_select->setTabText(tabWidget_select->indexOf(tabProduct), QApplication::translate("DialogInvoice", "Produits", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_add->setToolTip(QApplication::translate("DialogInvoice", "Ajouter un service ou un produit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_add->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
        groupBox_invoice->setTitle(QApplication::translate("DialogInvoice", "Facture", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_addFreeline->setToolTip(QApplication::translate("DialogInvoice", "Ajouter une ligne d'\303\251dition libre", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_addFreeline->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_rm->setToolTip(QApplication::translate("DialogInvoice", "Supprimer un article", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_rm->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_up->setToolTip(QApplication::translate("DialogInvoice", "Monter l'article", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_up->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_dn->setToolTip(QApplication::translate("DialogInvoice", "Descendre l'article", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_dn->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
        label_Total->setText(QApplication::translate("DialogInvoice", "Total: 0", 0, QApplication::UnicodeUTF8));
        pushButton_createInv->setText(QApplication::translate("DialogInvoice", "Cr\303\251er une facture", 0, QApplication::UnicodeUTF8));
        pushButton_ok->setText(QApplication::translate("DialogInvoice", "...", 0, QApplication::UnicodeUTF8));
        pushButton_cancel->setText(QApplication::translate("DialogInvoice", "Annuler", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogInvoice: public Ui_DialogInvoice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGINVOICE_H

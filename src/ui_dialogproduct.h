/********************************************************************************
** Form generated from reading UI file 'dialogproduct.ui'
**
** Created: Sun 27. May 09:31:35 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPRODUCT_H
#define UI_DIALOGPRODUCT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
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
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogProduct
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QFrame *line;
    QGridLayout *gridLayout;
    QToolButton *toolButton_editCategorie;
    QLabel *label_3;
    QToolButton *toolButton_editProvider;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_titletax;
    QComboBox *comboBox_providers;
    QLabel *label_7;
    QComboBox *comboBox_tax;
    QLabel *label_tax;
    QLineEdit *lineEdit_stock;
    QLineEdit *lineEdit_code;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_price;
    QDoubleSpinBox *doubleSpinBox_buyingPrice;
    QLineEdit *lineEdit_description;
    QComboBox *comboBox_categories;
    QLineEdit *lineEdit_stockAlert;
    QLabel *label_8;
    QLabel *label_5;
    QLabel *label;
    QToolButton *toolButton_autoCode;
    QToolButton *toolButton_editTax;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton_OK;
    QSpacerItem *horizontalSpacer;
    QRadioButton *radioButton_Discontinued;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_add_edit;
    QPushButton *pushButton_close;

    void setupUi(QDialog *DialogProduct)
    {
        if (DialogProduct->objectName().isEmpty())
            DialogProduct->setObjectName(QString::fromUtf8("DialogProduct"));
        DialogProduct->resize(551, 303);
        DialogProduct->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/products"), QSize(), QIcon::Normal, QIcon::Off);
        DialogProduct->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogProduct);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(DialogProduct);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);
        labelTitle->setMargin(0);

        verticalLayout->addWidget(labelTitle);

        line = new QFrame(DialogProduct);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        toolButton_editCategorie = new QToolButton(DialogProduct);
        toolButton_editCategorie->setObjectName(QString::fromUtf8("toolButton_editCategorie"));
        toolButton_editCategorie->setMinimumSize(QSize(25, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_editCategorie->setIcon(icon1);

        gridLayout->addWidget(toolButton_editCategorie, 1, 6, 1, 1);

        label_3 = new QLabel(DialogProduct);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 4, 1, 1);

        toolButton_editProvider = new QToolButton(DialogProduct);
        toolButton_editProvider->setObjectName(QString::fromUtf8("toolButton_editProvider"));
        toolButton_editProvider->setMinimumSize(QSize(25, 25));
        toolButton_editProvider->setIcon(icon1);

        gridLayout->addWidget(toolButton_editProvider, 0, 6, 1, 1);

        label_6 = new QLabel(DialogProduct);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 4, 1, 1);

        label_2 = new QLabel(DialogProduct);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 4, 1, 1);

        label_titletax = new QLabel(DialogProduct);
        label_titletax->setObjectName(QString::fromUtf8("label_titletax"));

        gridLayout->addWidget(label_titletax, 3, 1, 1, 1);

        comboBox_providers = new QComboBox(DialogProduct);
        comboBox_providers->setObjectName(QString::fromUtf8("comboBox_providers"));
        comboBox_providers->setMinimumSize(QSize(100, 0));
        comboBox_providers->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(comboBox_providers, 0, 5, 1, 1);

        label_7 = new QLabel(DialogProduct);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 2, 1, 1, 1);

        comboBox_tax = new QComboBox(DialogProduct);
        comboBox_tax->setObjectName(QString::fromUtf8("comboBox_tax"));
        comboBox_tax->setMinimumSize(QSize(100, 0));
        comboBox_tax->setMaximumSize(QSize(16777215, 16777215));
        comboBox_tax->setEditable(true);

        gridLayout->addWidget(comboBox_tax, 3, 2, 1, 1);

        label_tax = new QLabel(DialogProduct);
        label_tax->setObjectName(QString::fromUtf8("label_tax"));

        gridLayout->addWidget(label_tax, 2, 6, 1, 1);

        lineEdit_stock = new QLineEdit(DialogProduct);
        lineEdit_stock->setObjectName(QString::fromUtf8("lineEdit_stock"));
        lineEdit_stock->setMinimumSize(QSize(100, 0));
        lineEdit_stock->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_stock->setMaxLength(5);
        lineEdit_stock->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lineEdit_stock, 4, 2, 1, 1);

        lineEdit_code = new QLineEdit(DialogProduct);
        lineEdit_code->setObjectName(QString::fromUtf8("lineEdit_code"));
        lineEdit_code->setMinimumSize(QSize(100, 0));
        lineEdit_code->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_code->setMaxLength(64);

        gridLayout->addWidget(lineEdit_code, 1, 2, 1, 1);

        label_9 = new QLabel(DialogProduct);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 0, 1, 1, 1);

        doubleSpinBox_price = new QDoubleSpinBox(DialogProduct);
        doubleSpinBox_price->setObjectName(QString::fromUtf8("doubleSpinBox_price"));
        doubleSpinBox_price->setAlignment(Qt::AlignCenter);
        doubleSpinBox_price->setMaximum(999999);

        gridLayout->addWidget(doubleSpinBox_price, 2, 5, 1, 1);

        doubleSpinBox_buyingPrice = new QDoubleSpinBox(DialogProduct);
        doubleSpinBox_buyingPrice->setObjectName(QString::fromUtf8("doubleSpinBox_buyingPrice"));
        doubleSpinBox_buyingPrice->setAlignment(Qt::AlignCenter);
        doubleSpinBox_buyingPrice->setMaximum(999999);

        gridLayout->addWidget(doubleSpinBox_buyingPrice, 2, 2, 1, 1);

        lineEdit_description = new QLineEdit(DialogProduct);
        lineEdit_description->setObjectName(QString::fromUtf8("lineEdit_description"));
        lineEdit_description->setMinimumSize(QSize(100, 0));
        lineEdit_description->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_description->setMaxLength(256);

        gridLayout->addWidget(lineEdit_description, 0, 2, 1, 1);

        comboBox_categories = new QComboBox(DialogProduct);
        comboBox_categories->setObjectName(QString::fromUtf8("comboBox_categories"));
        comboBox_categories->setMinimumSize(QSize(100, 0));
        comboBox_categories->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(comboBox_categories, 1, 5, 1, 1);

        lineEdit_stockAlert = new QLineEdit(DialogProduct);
        lineEdit_stockAlert->setObjectName(QString::fromUtf8("lineEdit_stockAlert"));
        lineEdit_stockAlert->setMinimumSize(QSize(100, 0));
        lineEdit_stockAlert->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_stockAlert->setMaxLength(5);
        lineEdit_stockAlert->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lineEdit_stockAlert, 4, 5, 1, 1);

        label_8 = new QLabel(DialogProduct);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 1, 4, 1, 1);

        label_5 = new QLabel(DialogProduct);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 1, 1, 1);

        label = new QLabel(DialogProduct);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        toolButton_autoCode = new QToolButton(DialogProduct);
        toolButton_autoCode->setObjectName(QString::fromUtf8("toolButton_autoCode"));
        toolButton_autoCode->setMinimumSize(QSize(25, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/config"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_autoCode->setIcon(icon2);

        gridLayout->addWidget(toolButton_autoCode, 1, 3, 1, 1);

        toolButton_editTax = new QToolButton(DialogProduct);
        toolButton_editTax->setObjectName(QString::fromUtf8("toolButton_editTax"));
        toolButton_editTax->setMinimumSize(QSize(25, 25));
        toolButton_editTax->setIcon(icon1);

        gridLayout->addWidget(toolButton_editTax, 3, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        groupBox = new QGroupBox(DialogProduct);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton_OK = new QRadioButton(groupBox);
        radioButton_OK->setObjectName(QString::fromUtf8("radioButton_OK"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/On"), QSize(), QIcon::Normal, QIcon::Off);
        radioButton_OK->setIcon(icon3);
        radioButton_OK->setChecked(true);

        horizontalLayout->addWidget(radioButton_OK);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        radioButton_Discontinued = new QRadioButton(groupBox);
        radioButton_Discontinued->setObjectName(QString::fromUtf8("radioButton_Discontinued"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/quit"), QSize(), QIcon::Normal, QIcon::Off);
        radioButton_Discontinued->setIcon(icon4);

        horizontalLayout->addWidget(radioButton_Discontinued);


        horizontalLayout_3->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_add_edit = new QPushButton(DialogProduct);
        pushButton_add_edit->setObjectName(QString::fromUtf8("pushButton_add_edit"));

        horizontalLayout_2->addWidget(pushButton_add_edit);

        pushButton_close = new QPushButton(DialogProduct);
        pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));
        pushButton_close->setIcon(icon4);

        horizontalLayout_2->addWidget(pushButton_close);


        verticalLayout_2->addLayout(horizontalLayout_2);

        QWidget::setTabOrder(lineEdit_description, comboBox_providers);
        QWidget::setTabOrder(comboBox_providers, toolButton_editProvider);
        QWidget::setTabOrder(toolButton_editProvider, lineEdit_code);
        QWidget::setTabOrder(lineEdit_code, toolButton_autoCode);
        QWidget::setTabOrder(toolButton_autoCode, comboBox_categories);
        QWidget::setTabOrder(comboBox_categories, toolButton_editCategorie);
        QWidget::setTabOrder(toolButton_editCategorie, doubleSpinBox_buyingPrice);
        QWidget::setTabOrder(doubleSpinBox_buyingPrice, doubleSpinBox_price);
        QWidget::setTabOrder(doubleSpinBox_price, comboBox_tax);
        QWidget::setTabOrder(comboBox_tax, toolButton_editTax);
        QWidget::setTabOrder(toolButton_editTax, lineEdit_stock);
        QWidget::setTabOrder(lineEdit_stock, lineEdit_stockAlert);
        QWidget::setTabOrder(lineEdit_stockAlert, radioButton_OK);
        QWidget::setTabOrder(radioButton_OK, radioButton_Discontinued);
        QWidget::setTabOrder(radioButton_Discontinued, pushButton_add_edit);
        QWidget::setTabOrder(pushButton_add_edit, pushButton_close);

        retranslateUi(DialogProduct);

        QMetaObject::connectSlotsByName(DialogProduct);
    } // setupUi

    void retranslateUi(QDialog *DialogProduct)
    {
        DialogProduct->setWindowTitle(QApplication::translate("DialogProduct", "Produit", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("DialogProduct", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editCategorie->setToolTip(QApplication::translate("DialogProduct", "Editer les cat\303\251gories", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editCategorie->setText(QApplication::translate("DialogProduct", "...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogProduct", "Prix de vente:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editProvider->setToolTip(QApplication::translate("DialogProduct", "Editer les fournisseurs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editProvider->setText(QApplication::translate("DialogProduct", "...", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogProduct", "Stock Alerte :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogProduct", "Founisseur: ", 0, QApplication::UnicodeUTF8));
        label_titletax->setText(QApplication::translate("DialogProduct", "Tax (%) :", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogProduct", "Prix d'achat:  ", 0, QApplication::UnicodeUTF8));
        label_tax->setText(QApplication::translate("DialogProduct", "HT", 0, QApplication::UnicodeUTF8));
        lineEdit_stock->setInputMask(QString());
        lineEdit_stock->setText(QString());
        label_9->setText(QApplication::translate("DialogProduct", "Nom: ", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_price->setSuffix(QApplication::translate("DialogProduct", " \342\202\254", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_buyingPrice->setSuffix(QApplication::translate("DialogProduct", " \342\202\254", 0, QApplication::UnicodeUTF8));
        lineEdit_stockAlert->setInputMask(QString());
        lineEdit_stockAlert->setText(QString());
        label_8->setText(QApplication::translate("DialogProduct", "Cat\303\251gorie: ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogProduct", "Stock :", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogProduct", "Code :", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_autoCode->setToolTip(QApplication::translate("DialogProduct", "G\303\251n\303\251rer un code", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_autoCode->setText(QApplication::translate("DialogProduct", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editTax->setToolTip(QApplication::translate("DialogProduct", "Editer les TAX", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editTax->setText(QApplication::translate("DialogProduct", "...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DialogProduct", "Etat / Status", 0, QApplication::UnicodeUTF8));
        radioButton_OK->setText(QApplication::translate("DialogProduct", "En vente", 0, QApplication::UnicodeUTF8));
        radioButton_Discontinued->setText(QApplication::translate("DialogProduct", "Abandonn\303\251", 0, QApplication::UnicodeUTF8));
        pushButton_add_edit->setText(QApplication::translate("DialogProduct", "Ajouter", 0, QApplication::UnicodeUTF8));
        pushButton_close->setText(QApplication::translate("DialogProduct", "Fermer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogProduct: public Ui_DialogProduct {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPRODUCT_H

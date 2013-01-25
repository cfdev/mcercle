/********************************************************************************
** Form generated from reading UI file 'productView.ui'
**
** Created: Fri 25. Jan 20:29:52 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTVIEW_H
#define UI_PRODUCTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
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

class Ui_productView
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labeltitle;
    QFrame *line_top;
    QHBoxLayout *horizontalLayout_buttons;
    QToolButton *toolButton_addProduct;
    QToolButton *toolButton_editProduct;
    QToolButton *toolButton_remove;
    QToolButton *toolButton_print;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_product;
    QVBoxLayout *verticalLayout_product;
    QHBoxLayout *horizontalLayout_search;
    QComboBox *comboBoxFiltre;
    QLineEdit *lineEdit_searchProduct;
    QToolButton *toolButton_searchProduct;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_list;
    QCheckBox *checkBox_viewDiscontinued;
    QTableWidget *tableWidget_products;
    QHBoxLayout *horizontalLayout_page;
    QLabel *label_2;
    QLineEdit *lineEdit_page;
    QLabel *label_pageMax;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *toolButton_pagePrev;
    QToolButton *toolButton_pageNext;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QToolButton *toolButton_clipboard;
    QToolButton *toolButton_printInfo;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelInfoProduct;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_image;
    QFrame *line_2;
    QLabel *label_provider;
    QSpacerItem *verticalSpacer_2;
    QLabel *labelProductsNumber;

    void setupUi(QWidget *productView)
    {
        if (productView->objectName().isEmpty())
            productView->setObjectName(QString::fromUtf8("productView"));
        productView->resize(640, 518);
        productView->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/products"), QSize(), QIcon::Normal, QIcon::Off);
        productView->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(productView);
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labeltitle = new QLabel(productView);
        labeltitle->setObjectName(QString::fromUtf8("labeltitle"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        labeltitle->setFont(font);
        labeltitle->setAlignment(Qt::AlignCenter);
        labeltitle->setMargin(5);

        verticalLayout->addWidget(labeltitle);

        line_top = new QFrame(productView);
        line_top->setObjectName(QString::fromUtf8("line_top"));
        line_top->setFrameShape(QFrame::HLine);
        line_top->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_top);

        horizontalLayout_buttons = new QHBoxLayout();
        horizontalLayout_buttons->setObjectName(QString::fromUtf8("horizontalLayout_buttons"));
        toolButton_addProduct = new QToolButton(productView);
        toolButton_addProduct->setObjectName(QString::fromUtf8("toolButton_addProduct"));
        toolButton_addProduct->setMinimumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_addProduct->setIcon(icon1);
        toolButton_addProduct->setIconSize(QSize(24, 24));

        horizontalLayout_buttons->addWidget(toolButton_addProduct);

        toolButton_editProduct = new QToolButton(productView);
        toolButton_editProduct->setObjectName(QString::fromUtf8("toolButton_editProduct"));
        toolButton_editProduct->setMinimumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_editProduct->setIcon(icon2);
        toolButton_editProduct->setIconSize(QSize(24, 24));

        horizontalLayout_buttons->addWidget(toolButton_editProduct);

        toolButton_remove = new QToolButton(productView);
        toolButton_remove->setObjectName(QString::fromUtf8("toolButton_remove"));
        toolButton_remove->setMinimumSize(QSize(32, 32));
        toolButton_remove->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_remove->setIcon(icon3);
        toolButton_remove->setIconSize(QSize(16, 24));

        horizontalLayout_buttons->addWidget(toolButton_remove);

        toolButton_print = new QToolButton(productView);
        toolButton_print->setObjectName(QString::fromUtf8("toolButton_print"));
        toolButton_print->setMinimumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/print"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_print->setIcon(icon4);
        toolButton_print->setIconSize(QSize(24, 24));

        horizontalLayout_buttons->addWidget(toolButton_print);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_buttons->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_buttons);

        horizontalLayout_product = new QHBoxLayout();
        horizontalLayout_product->setObjectName(QString::fromUtf8("horizontalLayout_product"));
        verticalLayout_product = new QVBoxLayout();
        verticalLayout_product->setObjectName(QString::fromUtf8("verticalLayout_product"));
        horizontalLayout_search = new QHBoxLayout();
        horizontalLayout_search->setObjectName(QString::fromUtf8("horizontalLayout_search"));
        comboBoxFiltre = new QComboBox(productView);
        comboBoxFiltre->setObjectName(QString::fromUtf8("comboBoxFiltre"));

        horizontalLayout_search->addWidget(comboBoxFiltre);

        lineEdit_searchProduct = new QLineEdit(productView);
        lineEdit_searchProduct->setObjectName(QString::fromUtf8("lineEdit_searchProduct"));
        lineEdit_searchProduct->setMinimumSize(QSize(0, 0));
        lineEdit_searchProduct->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_search->addWidget(lineEdit_searchProduct);

        toolButton_searchProduct = new QToolButton(productView);
        toolButton_searchProduct->setObjectName(QString::fromUtf8("toolButton_searchProduct"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/app/search"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_searchProduct->setIcon(icon5);
        toolButton_searchProduct->setIconSize(QSize(24, 24));

        horizontalLayout_search->addWidget(toolButton_searchProduct);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_search->addItem(horizontalSpacer_2);


        verticalLayout_product->addLayout(horizontalLayout_search);

        verticalLayout_list = new QVBoxLayout();
        verticalLayout_list->setObjectName(QString::fromUtf8("verticalLayout_list"));
        checkBox_viewDiscontinued = new QCheckBox(productView);
        checkBox_viewDiscontinued->setObjectName(QString::fromUtf8("checkBox_viewDiscontinued"));

        verticalLayout_list->addWidget(checkBox_viewDiscontinued);

        tableWidget_products = new QTableWidget(productView);
        tableWidget_products->setObjectName(QString::fromUtf8("tableWidget_products"));
        tableWidget_products->setMinimumSize(QSize(350, 0));
        tableWidget_products->setSizeIncrement(QSize(10, 10));
        tableWidget_products->setFrameShadow(QFrame::Sunken);
        tableWidget_products->setAlternatingRowColors(true);

        verticalLayout_list->addWidget(tableWidget_products);


        verticalLayout_product->addLayout(verticalLayout_list);

        horizontalLayout_page = new QHBoxLayout();
        horizontalLayout_page->setObjectName(QString::fromUtf8("horizontalLayout_page"));
        label_2 = new QLabel(productView);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_page->addWidget(label_2);

        lineEdit_page = new QLineEdit(productView);
        lineEdit_page->setObjectName(QString::fromUtf8("lineEdit_page"));
        lineEdit_page->setMaximumSize(QSize(50, 16777215));
        lineEdit_page->setAlignment(Qt::AlignCenter);

        horizontalLayout_page->addWidget(lineEdit_page);

        label_pageMax = new QLabel(productView);
        label_pageMax->setObjectName(QString::fromUtf8("label_pageMax"));

        horizontalLayout_page->addWidget(label_pageMax);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_page->addItem(horizontalSpacer_3);

        toolButton_pagePrev = new QToolButton(productView);
        toolButton_pagePrev->setObjectName(QString::fromUtf8("toolButton_pagePrev"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/app/prev"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_pagePrev->setIcon(icon6);

        horizontalLayout_page->addWidget(toolButton_pagePrev);

        toolButton_pageNext = new QToolButton(productView);
        toolButton_pageNext->setObjectName(QString::fromUtf8("toolButton_pageNext"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/app/next"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_pageNext->setIcon(icon7);

        horizontalLayout_page->addWidget(toolButton_pageNext);


        verticalLayout_product->addLayout(horizontalLayout_page);


        horizontalLayout_product->addLayout(verticalLayout_product);

        tabWidget = new QTabWidget(productView);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setIconSize(QSize(24, 24));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_5 = new QHBoxLayout(tab);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 242, 357));
        verticalLayout_5 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, -1, -1);
        toolButton_clipboard = new QToolButton(scrollAreaWidgetContents);
        toolButton_clipboard->setObjectName(QString::fromUtf8("toolButton_clipboard"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/app/venteComptoire"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_clipboard->setIcon(icon8);
        toolButton_clipboard->setIconSize(QSize(24, 24));

        horizontalLayout_7->addWidget(toolButton_clipboard);

        toolButton_printInfo = new QToolButton(scrollAreaWidgetContents);
        toolButton_printInfo->setObjectName(QString::fromUtf8("toolButton_printInfo"));
        toolButton_printInfo->setIcon(icon4);
        toolButton_printInfo->setIconSize(QSize(24, 24));

        horizontalLayout_7->addWidget(toolButton_printInfo);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_7);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelInfoProduct = new QLabel(scrollAreaWidgetContents);
        labelInfoProduct->setObjectName(QString::fromUtf8("labelInfoProduct"));

        horizontalLayout_2->addWidget(labelInfoProduct);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        label_image = new QLabel(scrollAreaWidgetContents);
        label_image->setObjectName(QString::fromUtf8("label_image"));
        label_image->setScaledContents(false);

        horizontalLayout_2->addWidget(label_image);


        verticalLayout_4->addLayout(horizontalLayout_2);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_2);

        label_provider = new QLabel(scrollAreaWidgetContents);
        label_provider->setObjectName(QString::fromUtf8("label_provider"));

        verticalLayout_4->addWidget(label_provider);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        verticalLayout_5->addLayout(verticalLayout_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);


        horizontalLayout_5->addLayout(verticalLayout_3);

        tabWidget->addTab(tab, QString());

        horizontalLayout_product->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout_product);

        labelProductsNumber = new QLabel(productView);
        labelProductsNumber->setObjectName(QString::fromUtf8("labelProductsNumber"));

        verticalLayout->addWidget(labelProductsNumber);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(productView);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(productView);
    } // setupUi

    void retranslateUi(QWidget *productView)
    {
        productView->setWindowTitle(QApplication::translate("productView", "Form", 0, QApplication::UnicodeUTF8));
        labeltitle->setText(QApplication::translate("productView", "Produits", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_addProduct->setToolTip(QApplication::translate("productView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ajouter un produit</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_addProduct->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_editProduct->setToolTip(QApplication::translate("productView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Modifier le produit s\303\251lectionn\303\251</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_editProduct->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_remove->setToolTip(QApplication::translate("productView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Supprimer le produit s\303\251lectionn\303\251</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_remove->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_print->setToolTip(QApplication::translate("productView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Imprimer l'\303\251tat du stock produit</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_print->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        toolButton_searchProduct->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        checkBox_viewDiscontinued->setText(QApplication::translate("productView", "Afficher aussi les produits abandonn\303\251s", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("productView", "Page", 0, QApplication::UnicodeUTF8));
        lineEdit_page->setInputMask(QString());
        lineEdit_page->setText(QString());
        label_pageMax->setText(QApplication::translate("productView", "/", 0, QApplication::UnicodeUTF8));
        toolButton_pagePrev->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        toolButton_pageNext->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_clipboard->setToolTip(QApplication::translate("productView", "Copier dans le presse papier", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_clipboard->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolButton_printInfo->setToolTip(QApplication::translate("productView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Imprimer la fiche produit</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_printInfo->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        labelInfoProduct->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        label_image->setText(QString());
        label_provider->setText(QApplication::translate("productView", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("productView", "Fiche", 0, QApplication::UnicodeUTF8));
        labelProductsNumber->setText(QApplication::translate("productView", "Nombre de produits :", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class productView: public Ui_productView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTVIEW_H

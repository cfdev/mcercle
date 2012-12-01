/********************************************************************************
** Form generated from reading UI file 'product.ui'
**
** Created: Fri 11. Jun 10:46:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCT_H
#define UI_PRODUCT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_product
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labeltitle;
    QFrame *line;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *product)
    {
        if (product->objectName().isEmpty())
            product->setObjectName(QString::fromUtf8("product"));
        product->resize(400, 300);
        horizontalLayout = new QHBoxLayout(product);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labeltitle = new QLabel(product);
        labeltitle->setObjectName(QString::fromUtf8("labeltitle"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        labeltitle->setFont(font);
        labeltitle->setAlignment(Qt::AlignCenter);
        labeltitle->setMargin(5);

        verticalLayout->addWidget(labeltitle);

        line = new QFrame(product);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(product);

        QMetaObject::connectSlotsByName(product);
    } // setupUi

    void retranslateUi(QWidget *product)
    {
        product->setWindowTitle(QApplication::translate("product", "Form", 0, QApplication::UnicodeUTF8));
        labeltitle->setText(QApplication::translate("product", "Produits", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class product: public Ui_product {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCT_H

/********************************************************************************
** Form generated from reading UI file 'dialogtax.ui'
**
** Created: Fri 3. May 08:11:31 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGTAX_H
#define UI_DIALOGTAX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogTax
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_tax;
    QLabel *label_2;
    QLineEdit *lineEdit_description;
    QPushButton *pushButton_add;
    QPushButton *pushButton_edit;
    QPushButton *pushButton_del;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogTax)
    {
        if (DialogTax->objectName().isEmpty())
            DialogTax->setObjectName(QString::fromUtf8("DialogTax"));
        DialogTax->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        DialogTax->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(DialogTax);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget = new QTableWidget(DialogTax);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        horizontalLayout->addWidget(tableWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(DialogTax);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        doubleSpinBox_tax = new QDoubleSpinBox(DialogTax);
        doubleSpinBox_tax->setObjectName(QString::fromUtf8("doubleSpinBox_tax"));
        doubleSpinBox_tax->setMinimumSize(QSize(100, 0));
        doubleSpinBox_tax->setAlignment(Qt::AlignCenter);
        doubleSpinBox_tax->setSingleStep(0.1);

        verticalLayout_2->addWidget(doubleSpinBox_tax);

        label_2 = new QLabel(DialogTax);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        lineEdit_description = new QLineEdit(DialogTax);
        lineEdit_description->setObjectName(QString::fromUtf8("lineEdit_description"));
        lineEdit_description->setMaxLength(128);

        verticalLayout_2->addWidget(lineEdit_description);

        pushButton_add = new QPushButton(DialogTax);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_add->setIcon(icon1);

        verticalLayout_2->addWidget(pushButton_add);

        pushButton_edit = new QPushButton(DialogTax);
        pushButton_edit->setObjectName(QString::fromUtf8("pushButton_edit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_edit->setIcon(icon2);

        verticalLayout_2->addWidget(pushButton_edit);

        pushButton_del = new QPushButton(DialogTax);
        pushButton_del->setObjectName(QString::fromUtf8("pushButton_del"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_del->setIcon(icon3);

        verticalLayout_2->addWidget(pushButton_del);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(DialogTax);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogTax);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogTax, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogTax, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogTax);
    } // setupUi

    void retranslateUi(QDialog *DialogTax)
    {
        DialogTax->setWindowTitle(QApplication::translate("DialogTax", "Edition des Tax", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogTax", "Tax :", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_tax->setSuffix(QApplication::translate("DialogTax", " %", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogTax", "Description :", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("DialogTax", "Ajouter", 0, QApplication::UnicodeUTF8));
        pushButton_edit->setText(QApplication::translate("DialogTax", "Modifier", 0, QApplication::UnicodeUTF8));
        pushButton_del->setText(QApplication::translate("DialogTax", "Supprimer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogTax: public Ui_DialogTax {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGTAX_H

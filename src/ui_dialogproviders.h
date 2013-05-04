/********************************************************************************
** Form generated from reading UI file 'dialogproviders.ui'
**
** Created: Fri 3. May 08:11:31 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPROVIDERS_H
#define UI_DIALOGPROVIDERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogProviders
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_add;
    QPushButton *pushButton_edit;
    QPushButton *pushButton_delete;
    QTableWidget *tableWidget_providers;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogProviders)
    {
        if (DialogProviders->objectName().isEmpty())
            DialogProviders->setObjectName(QString::fromUtf8("DialogProviders"));
        DialogProviders->resize(465, 273);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/providers"), QSize(), QIcon::Normal, QIcon::Off);
        DialogProviders->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogProviders);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(DialogProviders);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_add = new QPushButton(groupBox);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_add->setIcon(icon1);

        horizontalLayout_2->addWidget(pushButton_add);

        pushButton_edit = new QPushButton(groupBox);
        pushButton_edit->setObjectName(QString::fromUtf8("pushButton_edit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_edit->setIcon(icon2);

        horizontalLayout_2->addWidget(pushButton_edit);

        pushButton_delete = new QPushButton(groupBox);
        pushButton_delete->setObjectName(QString::fromUtf8("pushButton_delete"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_delete->setIcon(icon3);

        horizontalLayout_2->addWidget(pushButton_delete);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tableWidget_providers = new QTableWidget(groupBox);
        tableWidget_providers->setObjectName(QString::fromUtf8("tableWidget_providers"));

        verticalLayout_3->addWidget(tableWidget_providers);


        verticalLayout_2->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(DialogProviders);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);

        QWidget::setTabOrder(pushButton_add, pushButton_edit);
        QWidget::setTabOrder(pushButton_edit, tableWidget_providers);
        QWidget::setTabOrder(tableWidget_providers, buttonBox);

        retranslateUi(DialogProviders);

        QMetaObject::connectSlotsByName(DialogProviders);
    } // setupUi

    void retranslateUi(QDialog *DialogProviders)
    {
        DialogProviders->setWindowTitle(QApplication::translate("DialogProviders", "Fournisseurs", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        pushButton_add->setText(QApplication::translate("DialogProviders", "Ajouter", 0, QApplication::UnicodeUTF8));
        pushButton_edit->setText(QApplication::translate("DialogProviders", "Modifier", 0, QApplication::UnicodeUTF8));
        pushButton_delete->setText(QApplication::translate("DialogProviders", "Supprimer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogProviders: public Ui_DialogProviders {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPROVIDERS_H

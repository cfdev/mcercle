/********************************************************************************
** Form generated from reading UI file 'dialogservicesedit.ui'
**
** Created: Fri 3. May 08:11:31 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSERVICESEDIT_H
#define UI_DIALOGSERVICESEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogServicesEdit
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_IntCom;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_name;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_Inter;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_add;
    QPushButton *pushButton_edit;
    QPushButton *pushButton_del;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_titletax;
    QComboBox *comboBox_tax;
    QLabel *LabelName;
    QLineEdit *lineEdit_Name;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_tax;
    QTextEdit *textEdit_Desc;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogServicesEdit)
    {
        if (DialogServicesEdit->objectName().isEmpty())
            DialogServicesEdit->setObjectName(QString::fromUtf8("DialogServicesEdit"));
        DialogServicesEdit->resize(582, 439);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        DialogServicesEdit->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(DialogServicesEdit);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_IntCom = new QGroupBox(DialogServicesEdit);
        groupBox_IntCom->setObjectName(QString::fromUtf8("groupBox_IntCom"));
        verticalLayout_6 = new QVBoxLayout(groupBox_IntCom);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_name = new QHBoxLayout();
        horizontalLayout_name->setObjectName(QString::fromUtf8("horizontalLayout_name"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget_Inter = new QTableWidget(groupBox_IntCom);
        tableWidget_Inter->setObjectName(QString::fromUtf8("tableWidget_Inter"));

        verticalLayout_2->addWidget(tableWidget_Inter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_add = new QPushButton(groupBox_IntCom);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_add->setIcon(icon1);

        horizontalLayout->addWidget(pushButton_add);

        pushButton_edit = new QPushButton(groupBox_IntCom);
        pushButton_edit->setObjectName(QString::fromUtf8("pushButton_edit"));
        pushButton_edit->setIcon(icon);

        horizontalLayout->addWidget(pushButton_edit);

        pushButton_del = new QPushButton(groupBox_IntCom);
        pushButton_del->setObjectName(QString::fromUtf8("pushButton_del"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_del->setIcon(icon2);

        horizontalLayout->addWidget(pushButton_del);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_name->addLayout(verticalLayout_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(groupBox_IntCom);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        dateTimeEdit = new QDateTimeEdit(groupBox_IntCom);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setCalendarPopup(true);

        gridLayout->addWidget(dateTimeEdit, 1, 1, 1, 1);

        label_titletax = new QLabel(groupBox_IntCom);
        label_titletax->setObjectName(QString::fromUtf8("label_titletax"));

        gridLayout->addWidget(label_titletax, 2, 0, 1, 1);

        comboBox_tax = new QComboBox(groupBox_IntCom);
        comboBox_tax->setObjectName(QString::fromUtf8("comboBox_tax"));

        gridLayout->addWidget(comboBox_tax, 2, 1, 1, 1);

        LabelName = new QLabel(groupBox_IntCom);
        LabelName->setObjectName(QString::fromUtf8("LabelName"));

        gridLayout->addWidget(LabelName, 0, 0, 1, 1);

        lineEdit_Name = new QLineEdit(groupBox_IntCom);
        lineEdit_Name->setObjectName(QString::fromUtf8("lineEdit_Name"));
        lineEdit_Name->setMaxLength(255);

        gridLayout->addWidget(lineEdit_Name, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_IntCom);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(groupBox_IntCom);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setMaximum(999999);

        gridLayout->addWidget(doubleSpinBox, 3, 1, 1, 1);

        label_tax = new QLabel(groupBox_IntCom);
        label_tax->setObjectName(QString::fromUtf8("label_tax"));

        gridLayout->addWidget(label_tax, 3, 2, 1, 1);


        verticalLayout_5->addLayout(gridLayout);

        textEdit_Desc = new QTextEdit(groupBox_IntCom);
        textEdit_Desc->setObjectName(QString::fromUtf8("textEdit_Desc"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit_Desc->sizePolicy().hasHeightForWidth());
        textEdit_Desc->setSizePolicy(sizePolicy);
        textEdit_Desc->setMinimumSize(QSize(0, 0));
        textEdit_Desc->setSizeIncrement(QSize(0, 0));
        textEdit_Desc->setBaseSize(QSize(0, 0));

        verticalLayout_5->addWidget(textEdit_Desc);


        horizontalLayout_name->addLayout(verticalLayout_5);


        verticalLayout_6->addLayout(horizontalLayout_name);


        verticalLayout->addWidget(groupBox_IntCom);

        buttonBox = new QDialogButtonBox(DialogServicesEdit);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogServicesEdit);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogServicesEdit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogServicesEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogServicesEdit);
    } // setupUi

    void retranslateUi(QDialog *DialogServicesEdit)
    {
        DialogServicesEdit->setWindowTitle(QApplication::translate("DialogServicesEdit", "Edition des services", 0, QApplication::UnicodeUTF8));
        groupBox_IntCom->setTitle(QApplication::translate("DialogServicesEdit", "Interventions G\303\251n\303\251rales", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("DialogServicesEdit", "Ajouter", 0, QApplication::UnicodeUTF8));
        pushButton_edit->setText(QApplication::translate("DialogServicesEdit", "Modifier", 0, QApplication::UnicodeUTF8));
        pushButton_del->setText(QApplication::translate("DialogServicesEdit", "Supprimer", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogServicesEdit", "Date et heure: ", 0, QApplication::UnicodeUTF8));
        dateTimeEdit->setDisplayFormat(QApplication::translate("DialogServicesEdit", "dd/MM/yyyy HH:mm", 0, QApplication::UnicodeUTF8));
        label_titletax->setText(QApplication::translate("DialogServicesEdit", "Tax:", 0, QApplication::UnicodeUTF8));
        LabelName->setText(QApplication::translate("DialogServicesEdit", "Nom: ", 0, QApplication::UnicodeUTF8));
        lineEdit_Name->setText(QString());
        label_2->setText(QApplication::translate("DialogServicesEdit", "Prix: ", 0, QApplication::UnicodeUTF8));
        doubleSpinBox->setSuffix(QApplication::translate("DialogServicesEdit", " \342\202\254", 0, QApplication::UnicodeUTF8));
        label_tax->setText(QApplication::translate("DialogServicesEdit", "HT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogServicesEdit: public Ui_DialogServicesEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSERVICESEDIT_H

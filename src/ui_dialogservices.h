/********************************************************************************
** Form generated from reading UI file 'dialogservices.ui'
**
** Created: Mon 31. Dec 09:36:31 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSERVICES_H
#define UI_DIALOGSERVICES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogServices
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_serviceComm;
    QComboBox *comboBox_servComm;
    QToolButton *toolButton_EditServiceComm;
    QGroupBox *groupBox_Int;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_name;
    QLabel *LabelName;
    QLineEdit *lineEdit_Name;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_tax;
    QLabel *label_titletax;
    QComboBox *comboBox_tax;
    QTextEdit *textEdit_Desc;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogServices)
    {
        if (DialogServices->objectName().isEmpty())
            DialogServices->setObjectName(QString::fromUtf8("DialogServices"));
        DialogServices->resize(401, 337);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        DialogServices->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogServices);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(DialogServices);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelTitle);

        line = new QFrame(DialogServices);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_serviceComm = new QLabel(DialogServices);
        label_serviceComm->setObjectName(QString::fromUtf8("label_serviceComm"));

        horizontalLayout_2->addWidget(label_serviceComm);

        comboBox_servComm = new QComboBox(DialogServices);
        comboBox_servComm->setObjectName(QString::fromUtf8("comboBox_servComm"));

        horizontalLayout_2->addWidget(comboBox_servComm);

        toolButton_EditServiceComm = new QToolButton(DialogServices);
        toolButton_EditServiceComm->setObjectName(QString::fromUtf8("toolButton_EditServiceComm"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_EditServiceComm->setIcon(icon1);

        horizontalLayout_2->addWidget(toolButton_EditServiceComm);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox_Int = new QGroupBox(DialogServices);
        groupBox_Int->setObjectName(QString::fromUtf8("groupBox_Int"));
        verticalLayout_6 = new QVBoxLayout(groupBox_Int);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_name = new QHBoxLayout();
        horizontalLayout_name->setObjectName(QString::fromUtf8("horizontalLayout_name"));
        LabelName = new QLabel(groupBox_Int);
        LabelName->setObjectName(QString::fromUtf8("LabelName"));

        horizontalLayout_name->addWidget(LabelName);

        lineEdit_Name = new QLineEdit(groupBox_Int);
        lineEdit_Name->setObjectName(QString::fromUtf8("lineEdit_Name"));
        lineEdit_Name->setMaxLength(255);

        horizontalLayout_name->addWidget(lineEdit_Name);


        verticalLayout_6->addLayout(horizontalLayout_name);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(groupBox_Int);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        dateTimeEdit = new QDateTimeEdit(groupBox_Int);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setCalendarPopup(true);

        gridLayout->addWidget(dateTimeEdit, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox_Int);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(groupBox_Int);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setMaximum(999999);

        gridLayout->addWidget(doubleSpinBox, 1, 3, 1, 1);

        label_tax = new QLabel(groupBox_Int);
        label_tax->setObjectName(QString::fromUtf8("label_tax"));

        gridLayout->addWidget(label_tax, 1, 4, 1, 1);

        label_titletax = new QLabel(groupBox_Int);
        label_titletax->setObjectName(QString::fromUtf8("label_titletax"));

        gridLayout->addWidget(label_titletax, 2, 0, 1, 1);

        comboBox_tax = new QComboBox(groupBox_Int);
        comboBox_tax->setObjectName(QString::fromUtf8("comboBox_tax"));

        gridLayout->addWidget(comboBox_tax, 2, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout);

        textEdit_Desc = new QTextEdit(groupBox_Int);
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


        verticalLayout_6->addLayout(verticalLayout_5);


        verticalLayout->addWidget(groupBox_Int);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(DialogServices);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(DialogServices);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogServices, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogServices, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogServices);
    } // setupUi

    void retranslateUi(QDialog *DialogServices)
    {
        DialogServices->setWindowTitle(QApplication::translate("DialogServices", "Services", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("DialogServices", "...", 0, QApplication::UnicodeUTF8));
        label_serviceComm->setText(QApplication::translate("DialogServices", "Charger un service\n"
"g\303\251n\303\251ral :", 0, QApplication::UnicodeUTF8));
        toolButton_EditServiceComm->setText(QApplication::translate("DialogServices", "...", 0, QApplication::UnicodeUTF8));
        groupBox_Int->setTitle(QApplication::translate("DialogServices", "Service personnalis\303\251", 0, QApplication::UnicodeUTF8));
        LabelName->setText(QApplication::translate("DialogServices", "Nom: ", 0, QApplication::UnicodeUTF8));
        lineEdit_Name->setText(QString());
        label_3->setText(QApplication::translate("DialogServices", "Date et heure: ", 0, QApplication::UnicodeUTF8));
        dateTimeEdit->setDisplayFormat(QApplication::translate("DialogServices", "dd/MM/yyyy HH:mm", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogServices", "Prix: ", 0, QApplication::UnicodeUTF8));
        doubleSpinBox->setSuffix(QApplication::translate("DialogServices", " \342\202\254", 0, QApplication::UnicodeUTF8));
        label_tax->setText(QApplication::translate("DialogServices", "HT", 0, QApplication::UnicodeUTF8));
        label_titletax->setText(QApplication::translate("DialogServices", "Tax:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogServices: public Ui_DialogServices {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSERVICES_H

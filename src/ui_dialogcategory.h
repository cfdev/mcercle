/********************************************************************************
** Form generated from reading UI file 'dialogcategory.ui'
**
** Created: Mon 31. Dec 09:36:31 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCATEGORY_H
#define UI_DIALOGCATEGORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogCategory
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_name;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_color;
    QLabel *label_color;
    QToolButton *toolButton_colorDefault;
    QFrame *line;
    QPushButton *pushButton_add;
    QPushButton *pushButton_edit;
    QPushButton *pushButton_del;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCategory)
    {
        if (DialogCategory->objectName().isEmpty())
            DialogCategory->setObjectName(QString::fromUtf8("DialogCategory"));
        DialogCategory->resize(401, 295);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        DialogCategory->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(DialogCategory);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget = new QTableWidget(DialogCategory);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        horizontalLayout->addWidget(tableWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(DialogCategory);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        lineEdit_name = new QLineEdit(DialogCategory);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));

        verticalLayout_2->addWidget(lineEdit_name);

        label_2 = new QLabel(DialogCategory);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_color = new QPushButton(DialogCategory);
        pushButton_color->setObjectName(QString::fromUtf8("pushButton_color"));
        pushButton_color->setMinimumSize(QSize(0, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/color"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_color->setIcon(icon1);

        horizontalLayout_2->addWidget(pushButton_color);

        label_color = new QLabel(DialogCategory);
        label_color->setObjectName(QString::fromUtf8("label_color"));
        label_color->setStyleSheet(QString::fromUtf8(""));
        label_color->setScaledContents(false);

        horizontalLayout_2->addWidget(label_color);

        toolButton_colorDefault = new QToolButton(DialogCategory);
        toolButton_colorDefault->setObjectName(QString::fromUtf8("toolButton_colorDefault"));
        toolButton_colorDefault->setMinimumSize(QSize(25, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/quit"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_colorDefault->setIcon(icon2);

        horizontalLayout_2->addWidget(toolButton_colorDefault);


        verticalLayout_2->addLayout(horizontalLayout_2);

        line = new QFrame(DialogCategory);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        pushButton_add = new QPushButton(DialogCategory);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/insert"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_add->setIcon(icon3);

        verticalLayout_2->addWidget(pushButton_add);

        pushButton_edit = new QPushButton(DialogCategory);
        pushButton_edit->setObjectName(QString::fromUtf8("pushButton_edit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_edit->setIcon(icon4);

        verticalLayout_2->addWidget(pushButton_edit);

        pushButton_del = new QPushButton(DialogCategory);
        pushButton_del->setObjectName(QString::fromUtf8("pushButton_del"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/app/remove"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_del->setIcon(icon5);

        verticalLayout_2->addWidget(pushButton_del);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(DialogCategory);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogCategory);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogCategory, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogCategory, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogCategory);
    } // setupUi

    void retranslateUi(QDialog *DialogCategory)
    {
        DialogCategory->setWindowTitle(QApplication::translate("DialogCategory", "Cat\303\251gories", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogCategory", "Nom:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogCategory", "Couleur:", 0, QApplication::UnicodeUTF8));
        pushButton_color->setText(QString());
        label_color->setText(QString());
        toolButton_colorDefault->setText(QApplication::translate("DialogCategory", "...", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("DialogCategory", "Ajouter", 0, QApplication::UnicodeUTF8));
        pushButton_edit->setText(QApplication::translate("DialogCategory", "Modifier", 0, QApplication::UnicodeUTF8));
        pushButton_del->setText(QApplication::translate("DialogCategory", "Supprimer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogCategory: public Ui_DialogCategory {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCATEGORY_H

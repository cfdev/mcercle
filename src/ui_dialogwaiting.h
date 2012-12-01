/********************************************************************************
** Form generated from reading UI file 'dialogwaiting.ui'
**
** Created: Tue Apr 3 11:50:08 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGWAITING_H
#define UI_DIALOGWAITING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogWaiting
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;
    QLabel *label_detail;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogWaiting)
    {
        if (DialogWaiting->objectName().isEmpty())
            DialogWaiting->setObjectName(QString::fromUtf8("DialogWaiting"));
        DialogWaiting->resize(400, 133);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/process"), QSize(), QIcon::Normal, QIcon::Off);
        DialogWaiting->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(DialogWaiting);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(DialogWaiting);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        labelTitle->setTextFormat(Qt::AutoText);
        labelTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelTitle);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        progressBar = new QProgressBar(DialogWaiting);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        label_detail = new QLabel(DialogWaiting);
        label_detail->setObjectName(QString::fromUtf8("label_detail"));

        verticalLayout->addWidget(label_detail);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(DialogWaiting);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(DialogWaiting);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogWaiting, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogWaiting, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogWaiting);
    } // setupUi

    void retranslateUi(QDialog *DialogWaiting)
    {
        DialogWaiting->setWindowTitle(QApplication::translate("DialogWaiting", "Merci de patienter", 0, QApplication::UnicodeUTF8));
        labelTitle->setText(QApplication::translate("DialogWaiting", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_detail->setText(QApplication::translate("DialogWaiting", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogWaiting: public Ui_DialogWaiting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGWAITING_H

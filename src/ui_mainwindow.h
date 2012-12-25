/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 25. Dec 16:28:14 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuitter;
    QAction *actionA_propos;
    QAction *actionA_propos_de_Qt;
    QAction *actionTableau_de_bord;
    QAction *actionClients;
    QAction *actionProduits;
    QAction *actionSauvegarder;
    QAction *actionDevis;
    QAction *actionFactures;
    QAction *actionCaisse;
    QAction *actionFaire_un_mailing;
    QAction *actionCr_er_un_bon_cadeau;
    QAction *actionConfiguration;
    QAction *actionActionProvider;
    QAction *actionDebug_Add;
    QAction *actionDebug_prod;
    QAction *actionImporter_depuis_un_fichier_cvs;
    QAction *actionExporter_vers_un_fichier_cvs;
    QAction *actionLivres_des_Recettes;
    QAction *actionSauvegarder_la_base_de_donn_es_sous;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuFichier;
    QMenu *menuAide;
    QMenu *menuAffichage;
    QMenu *menuOutils;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(450, 331);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/app/logo"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAnimated(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/app/quit"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuitter->setIcon(icon1);
        actionQuitter->setVisible(true);
        actionA_propos = new QAction(MainWindow);
        actionA_propos->setObjectName(QString::fromUtf8("actionA_propos"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/app/help"), QSize(), QIcon::Normal, QIcon::Off);
        actionA_propos->setIcon(icon2);
        actionA_propos_de_Qt = new QAction(MainWindow);
        actionA_propos_de_Qt->setObjectName(QString::fromUtf8("actionA_propos_de_Qt"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/app/AboutQt"), QSize(), QIcon::Normal, QIcon::Off);
        actionA_propos_de_Qt->setIcon(icon3);
        actionTableau_de_bord = new QAction(MainWindow);
        actionTableau_de_bord->setObjectName(QString::fromUtf8("actionTableau_de_bord"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/app/board"), QSize(), QIcon::Normal, QIcon::Off);
        actionTableau_de_bord->setIcon(icon4);
        actionTableau_de_bord->setVisible(true);
        actionClients = new QAction(MainWindow);
        actionClients->setObjectName(QString::fromUtf8("actionClients"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/app/customers"), QSize(), QIcon::Normal, QIcon::Off);
        actionClients->setIcon(icon5);
        actionProduits = new QAction(MainWindow);
        actionProduits->setObjectName(QString::fromUtf8("actionProduits"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/app/products"), QSize(), QIcon::Normal, QIcon::Off);
        actionProduits->setIcon(icon6);
        actionSauvegarder = new QAction(MainWindow);
        actionSauvegarder->setObjectName(QString::fromUtf8("actionSauvegarder"));
        actionDevis = new QAction(MainWindow);
        actionDevis->setObjectName(QString::fromUtf8("actionDevis"));
        actionFactures = new QAction(MainWindow);
        actionFactures->setObjectName(QString::fromUtf8("actionFactures"));
        actionCaisse = new QAction(MainWindow);
        actionCaisse->setObjectName(QString::fromUtf8("actionCaisse"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/app/venteComptoire"), QSize(), QIcon::Normal, QIcon::Off);
        actionCaisse->setIcon(icon7);
        actionFaire_un_mailing = new QAction(MainWindow);
        actionFaire_un_mailing->setObjectName(QString::fromUtf8("actionFaire_un_mailing"));
        actionCr_er_un_bon_cadeau = new QAction(MainWindow);
        actionCr_er_un_bon_cadeau->setObjectName(QString::fromUtf8("actionCr_er_un_bon_cadeau"));
        actionConfiguration = new QAction(MainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/app/preference"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfiguration->setIcon(icon8);
        actionActionProvider = new QAction(MainWindow);
        actionActionProvider->setObjectName(QString::fromUtf8("actionActionProvider"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/app/providers"), QSize(), QIcon::Normal, QIcon::Off);
        actionActionProvider->setIcon(icon9);
        actionDebug_Add = new QAction(MainWindow);
        actionDebug_Add->setObjectName(QString::fromUtf8("actionDebug_Add"));
        actionDebug_prod = new QAction(MainWindow);
        actionDebug_prod->setObjectName(QString::fromUtf8("actionDebug_prod"));
        actionImporter_depuis_un_fichier_cvs = new QAction(MainWindow);
        actionImporter_depuis_un_fichier_cvs->setObjectName(QString::fromUtf8("actionImporter_depuis_un_fichier_cvs"));
        actionExporter_vers_un_fichier_cvs = new QAction(MainWindow);
        actionExporter_vers_un_fichier_cvs->setObjectName(QString::fromUtf8("actionExporter_vers_un_fichier_cvs"));
        actionLivres_des_Recettes = new QAction(MainWindow);
        actionLivres_des_Recettes->setObjectName(QString::fromUtf8("actionLivres_des_Recettes"));
        actionLivres_des_Recettes->setIcon(icon7);
        actionSauvegarder_la_base_de_donn_es_sous = new QAction(MainWindow);
        actionSauvegarder_la_base_de_donn_es_sous->setObjectName(QString::fromUtf8("actionSauvegarder_la_base_de_donn_es_sous"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/app/save_as"), QSize(), QIcon::Normal, QIcon::Off);
        actionSauvegarder_la_base_de_donn_es_sous->setIcon(icon10);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 450, 21));
        menuFichier = new QMenu(menuBar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        menuAide = new QMenu(menuBar);
        menuAide->setObjectName(QString::fromUtf8("menuAide"));
        menuAffichage = new QMenu(menuBar);
        menuAffichage->setObjectName(QString::fromUtf8("menuAffichage"));
        menuOutils = new QMenu(menuBar);
        menuOutils->setObjectName(QString::fromUtf8("menuOutils"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setLayoutDirection(Qt::LeftToRight);
        mainToolBar->setMovable(true);
        mainToolBar->setIconSize(QSize(32, 32));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFichier->menuAction());
        menuBar->addAction(menuAffichage->menuAction());
        menuBar->addAction(menuOutils->menuAction());
        menuBar->addAction(menuAide->menuAction());
        menuFichier->addAction(actionSauvegarder_la_base_de_donn_es_sous);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);
        menuAide->addAction(actionA_propos);
        menuAide->addAction(actionA_propos_de_Qt);
        menuAffichage->addAction(actionTableau_de_bord);
        menuAffichage->addAction(actionClients);
        menuAffichage->addAction(actionProduits);
        menuOutils->addAction(actionLivres_des_Recettes);
        menuOutils->addSeparator();
        menuOutils->addAction(actionConfiguration);
        mainToolBar->addAction(actionTableau_de_bord);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionClients);
        mainToolBar->addAction(actionProduits);
        mainToolBar->addAction(actionActionProvider);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionConfiguration);
        mainToolBar->addAction(actionA_propos);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "mcercle", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("MainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        actionA_propos->setText(QApplication::translate("MainWindow", "A propos", 0, QApplication::UnicodeUTF8));
        actionA_propos_de_Qt->setText(QApplication::translate("MainWindow", "A propos de Qt", 0, QApplication::UnicodeUTF8));
        actionTableau_de_bord->setText(QApplication::translate("MainWindow", "Tableau de bord", 0, QApplication::UnicodeUTF8));
        actionClients->setText(QApplication::translate("MainWindow", "Clients", 0, QApplication::UnicodeUTF8));
        actionProduits->setText(QApplication::translate("MainWindow", "Produits", 0, QApplication::UnicodeUTF8));
        actionSauvegarder->setText(QApplication::translate("MainWindow", "Sauvegarder", 0, QApplication::UnicodeUTF8));
        actionDevis->setText(QApplication::translate("MainWindow", "Devis", 0, QApplication::UnicodeUTF8));
        actionFactures->setText(QApplication::translate("MainWindow", "Factures", 0, QApplication::UnicodeUTF8));
        actionCaisse->setText(QApplication::translate("MainWindow", "Caisse", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCaisse->setToolTip(QApplication::translate("MainWindow", "Caisse", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFaire_un_mailing->setText(QApplication::translate("MainWindow", "Faire un mailing", 0, QApplication::UnicodeUTF8));
        actionCr_er_un_bon_cadeau->setText(QApplication::translate("MainWindow", "Cr\303\251er un bon cadeau", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        actionActionProvider->setText(QApplication::translate("MainWindow", "Fournisseurs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionActionProvider->setToolTip(QApplication::translate("MainWindow", "Fournisseurs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDebug_Add->setText(QApplication::translate("MainWindow", "DEBUG: Ajoute des clients", 0, QApplication::UnicodeUTF8));
        actionDebug_prod->setText(QApplication::translate("MainWindow", "DEBUG: Ajoute des produits", 0, QApplication::UnicodeUTF8));
        actionImporter_depuis_un_fichier_cvs->setText(QApplication::translate("MainWindow", "Importer depuis un fichier cvs", 0, QApplication::UnicodeUTF8));
        actionExporter_vers_un_fichier_cvs->setText(QApplication::translate("MainWindow", "Exporter vers un fichier cvs", 0, QApplication::UnicodeUTF8));
        actionLivres_des_Recettes->setText(QApplication::translate("MainWindow", "Livres des Recettes", 0, QApplication::UnicodeUTF8));
        actionSauvegarder_la_base_de_donn_es_sous->setText(QApplication::translate("MainWindow", "Sauvegarder la base de donn\303\251es sous...", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
        menuAide->setTitle(QApplication::translate("MainWindow", "Aide", 0, QApplication::UnicodeUTF8));
        menuAffichage->setTitle(QApplication::translate("MainWindow", "Affichage", 0, QApplication::UnicodeUTF8));
        menuOutils->setTitle(QApplication::translate("MainWindow", "Outils", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

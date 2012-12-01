/**
  This file is a part of mcercle
  Copyright (C) 2010-2012 Cyril FRAUSTI

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
//#include <QDebug>

#include "dialogsettings.h"
#include "dialogproviders.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogwaiting.h"

/**
    Constructeur de la class MainWindow
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //langage
    m_lang = QLocale::system().name().section('_', 0, 0);

    // la fenetre est maximisee par defaut
    setMinimumSize(QSize(640, 480));
    setWindowState(Qt::WindowMaximized);

    m_database = new database( m_lang, this);
    m_Settings = new Settings(this);
}


/**
    Destructeur de la class MainWindow
*/
MainWindow::~MainWindow()
{
    //sauvegarde les index des champs de recherche
    m_Settings->setPositionListSearchProduct( m_productView->getIndexSearchProduct() );

    delete ui;
    delete m_Settings;
    delete m_database;
}

/**
    Change de language
*/
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
  Initialise lapplication, base de donnees etc..
  */
void MainWindow::init(){
    // Charge le fichier de configurations
    if(!m_Settings->settingIsOk()) {
        ///TODO: QWizard pour la config du soft
        m_Settings->setDatabase_default();
        m_Settings->setSettingState(true);
    }

    //Base de donnees
    m_database->setBdd( m_Settings->getDatabase_bdd() );
    m_database->setHostName( m_Settings->getDatabase_hostName() );
    m_database->setPort( m_Settings->getDatabase_port() );
    m_database->setDatabaseName( m_Settings->getDatabase_databaseName() );
    m_database->setUserName( m_Settings->getDatabase_userName() );
    m_database->setPassword( m_Settings->getDatabase_userPassword() );
    //version 1.0
    if((m_database->connect()==database::DB_CON_OK) && (m_Settings->getDatabase_databaseName().contains(".fdb"))){
        //Si la migration s'est deroulee correctement, Mise a jour des settings!
        m_Settings->setDatabase_bdd(m_database->getBdd());
        m_Settings->setDatabase_hostName(m_database->getHostName());
        m_Settings->setDatabase_port(m_database->getPort());
        m_Settings->setDatabase_databaseName(m_database->getDatabaseName());
        m_Settings->setDatabase_userName(m_database->getUserName());
        m_Settings->setDatabase_userPassword(m_database->getPassword());
    }


    /// Construction des widgets!!
    //customers
    m_customerView = new customerView( m_database, m_lang  );
    m_customerView->hide();
    //products
    m_productView = new productView( m_database, m_lang, productView::PRODUCT_VIEW );
    m_productView->hide();
    m_productView->setIndexSearchProduct( m_Settings->getPositionListSearchProduct() );
    //board
    m_board = new board( m_database, m_lang );

    //Mis en layout
    ui->verticalLayout->addWidget( m_board );
    ui->verticalLayout->addWidget( m_customerView );
    ui->verticalLayout->addWidget( m_productView );
    ui->verticalLayout->update();

    //Menu visible uniquement en mode debug!
#ifdef QT_NO_DEBUG
    ui->actionDebug_Add->setVisible(false);
    ui->actionDebug_prod->setVisible(false);
#endif

   /* qDebug() << "MCERCLE_DEBUG :" <<QString(m_database->m_customer->m_invoice->getYearsList().at(0)+' '+
                QString::number(m_database->m_customer->m_invoice->getYearRevenue("2011"))+' '+
                QString::number(m_database->m_customer->m_invoice->getMonthRevenue("2011","8")) );*/
}


/**
    Affiche les informations au sujet de Qt
*/
void MainWindow::on_actionA_propos_de_Qt_triggered()
{
    QApplication::aboutQt();
}

/**
    Quitte l application
*/
void MainWindow::on_actionQuitter_triggered()
{
    close();
}

/**
    Affiche le tableau de bord
*/
void MainWindow::on_actionTableau_de_bord_triggered()
{
    m_board->show();
    m_board->listStockAlertToTable();
    m_board->listInvoiceAlertToTable();
    m_board->listProposalAlertToTable();
    m_board->listYear();
    m_board->listRevenuesToTable();
    m_board->calculYear();
    m_customerView->hide();
    m_productView->hide();
    ui->verticalLayout->update();
}

/**
    Affiche la gestion clients
*/
void MainWindow::on_actionClients_triggered()
{
    m_board->hide();
    m_customerView->refreshCustomersList();
    m_customerView->show();
    m_productView->hide();
    ui->verticalLayout->update();
}

/**
    Affiche la gestion produits
*/
void MainWindow::on_actionProduits_triggered()
{
    m_board->hide();
    m_customerView->hide();
    m_productView->refreshProductsList();
    m_productView->show();
    ui->verticalLayout->update();
}

/**
    Affiche la liste des fournisseurs
*/
void MainWindow::on_actionActionProvider_triggered()
{
    //Si on est pas connecte on sort
    if(!m_database->isConnected())return;

    DialogProviders *m_DialogProviders = new DialogProviders(m_database->m_product);
    m_DialogProviders->setModal(true);
    m_DialogProviders->exec();
    delete m_DialogProviders;
}

/**
    Affiche les informations de l application
*/
void MainWindow::on_actionA_propos_triggered()
{
    //fenetre a propos
    m_about = new about(m_database, this);
    m_about->setModal(true);
    m_about->show();
}

/**
    Ouvrir la fenetre de configuration
  */
void MainWindow::on_actionConfiguration_triggered()
{
   DialogSettings *m_DialogSettings = new DialogSettings(m_Settings, m_database, m_lang, this);
   m_DialogSettings->setModal(true);
   QObject::connect(m_DialogSettings, SIGNAL(askRefreshList()), this, SLOT(RefreshLists()));
   m_DialogSettings->show();
}

/**
    Rafraichir les listes clients et produits
  */
void MainWindow::RefreshLists()
{
    m_customerView->listCustomers(1);
    m_customerView->listServices();
    m_customerView->listProposals();
    m_customerView->listInvoices();
    m_productView->listProducts(1);
    m_board->listStockAlertToTable();
    m_board->listInvoiceAlertToTable();
    m_board->listProposalAlertToTable();
    m_board->listYear();
    m_board->calculYear();
    m_board->listRevenuesToTable();
}



/**
    DEBUG AJOUT DE CLIENTS *****************************************************************************
  */
void MainWindow::on_actionDebug_Add_triggered()
{
    /* TEST NUMBER OF CUSTOMERS */
    //Affichage de la fenetre d attente
    DialogWaiting* m_DialogWaiting = new DialogWaiting();
    m_DialogWaiting->setTitle(tr("<b>TEST AJOUT DE CLIENTS</b>"));
    m_DialogWaiting->setDetail(tr("<i>En cours...</i>"));
    int val = 50;
    int loop = 100;
    m_DialogWaiting->setProgressBarRange(0,val);
    m_DialogWaiting->setModal(true);
    m_DialogWaiting->show();

    for(int i=0; i < loop; i++) {
        m_DialogWaiting->setProgressBar(0);
        m_DialogWaiting->setDetail("<i>En cours..." + QString::number(i*val) + " / " + QString::number(loop*val) +"</i>");
        for(int j=0; j < val; j++){
            m_DialogWaiting->setProgressBar(j);
            m_database->m_customer->setName("TOTO"+ QString::number(i*val+j), "NAME"+ QString::number(i*val+j));
            m_database->m_customer->create();
        }
    }
    delete m_DialogWaiting;
}

void MainWindow::on_actionDebug_prod_triggered()
{
    /* TEST NUMBER OF PRODUCTS */
    //Affichage de la fenetre d attente
    DialogWaiting* m_DialogWaiting = new DialogWaiting();
    m_DialogWaiting->setTitle(tr("<b>TEST AJOUT DE PRODUITS</b>"));
    m_DialogWaiting->setDetail(tr("<i>En cours...</i>"));
    int val = 50;
    int loop = 100;
    m_DialogWaiting->setProgressBarRange(0,val);
    m_DialogWaiting->setModal(true);
    m_DialogWaiting->show();

    for(int i=0; i < loop; i++) {
        m_DialogWaiting->setProgressBar(0);
        m_DialogWaiting->setDetail("<i>En cours..." + QString::number(i*val) + " / " + QString::number(loop*val) +"</i>");
        for(int j=0; j < val; j++){
            m_DialogWaiting->setProgressBar(j);
            m_database->m_product->setCode("X"+ QString::number(i*val+j));
            m_database->m_product->setState(1);
            m_database->m_product->setName("PROD"+ QString::number(i*val+j));
            m_database->m_product->create();
        }
    }
    delete m_DialogWaiting;
}

/// ******************************************************************************************************

/**
  This file is a part of mcercle
  Copyright (C) 2010-2013 Cyril FRAUSTI

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

#include "customerView.h"
#include "ui_customerView.h"
#include "dialogcustomer.h"
#include "dialogservices.h"
#include "dialoginvoice.h"
#include "dialogprintchoice.h"
#include "dialogwaiting.h"

#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocumentFragment>
#include <QClipboard>
#include <QChar>

/**
	Constructeur de la class customerView
*/
customerView::customerView(database *pdata, QLocale &lang, QWidget *parent) :
	QWidget(parent), ui(new Ui::customerView)
{
	ui->setupUi(this);
	m_data = pdata;
	m_lang = lang;
	ui->comboBoxFiltre->addItem(tr("Noms"));
	ui->comboBoxFiltre->addItem(tr("Pr\351noms"));

	m_custPage=1;
	m_custfilter = m_custfield = "";
	ui->lineEdit_page->setText(QString::number(m_custPage));

	//afficher la liste des clients
	listCustomers(m_custPage);
	m_custId = 1;

	//Selectionne la tab fiche
	ui->tabWidget_Customer->setCurrentIndex(0);
}

/**
	Destructeur de la class customerView
*/
customerView::~customerView()
{
	delete ui;
}

void customerView::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

/**
	Ajouter un client
*/
void customerView::on_toolBut_Add_clicked()
{
	if(!m_data->isConnected())return;

	DialogCustomer *m_DialogCustomer = new DialogCustomer(m_data->m_customer);
	m_DialogCustomer->setModal(true);
	m_DialogCustomer->setTitle(tr("Ajouter un client"));
	m_DialogCustomer->exec();
	if(m_DialogCustomer->result() == QDialog::Accepted)
	{
		// Recuperation des informations de clients
		if( m_data->m_customer->create() ) listCustomers(m_custPage);
	}
	delete m_DialogCustomer;
}

/**
		Editer Un client
  */
void customerView::on_toolBut_Edit_clicked()
{
	if(!m_data->isConnected())return;

	DialogCustomer *m_DialogCustomer = new DialogCustomer(m_data->m_customer);
	m_DialogCustomer->setModal(true);
	m_DialogCustomer->setTitle(tr("Modifier le client"));
	m_DialogCustomer->loadValuesFormCustomer();
	m_DialogCustomer->exec();
	if(m_DialogCustomer->result() == QDialog::Accepted)
	{
		// Recuperation des informations de clients
		if( m_data->m_customer->update() ) listCustomers(m_custPage);
	}
	delete m_DialogCustomer;
}

/**
	Supprime un client
  */
void customerView::on_toolButton_Del_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int ret = QMessageBox::critical(this, tr("Attention"),
								   tr("Ceci est fortement d\351conseill&eacute; !<br>"
									  "Voulez-vous vraiment supprimer le client<br><br><b>")+
								   m_data->m_customer->getFirstName()+" - "+
								   m_data->m_customer->getLastName()+"</b> Ainsi que toutes ses donn&eacute;es (services,propositions,factures) ?",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		m_data->m_customer->remove();
		//rafraichir la liste des clients
		this->listCustomers(this->m_custPage);
	}
}


/**
	Page precedente
  */
void customerView::on_toolButton_pagePrev_clicked()
{
	//
	if(m_custPage>1) listCustomers(--m_custPage);
}

/**
  Page suivante
  */
void customerView::on_toolButton_pageNext_clicked()
{
	//
	if(m_custPage<m_custNbPage) listCustomers(++m_custPage);
}

/**
	fonction sur la pression de la touche entree
	de la selection d une page.
  */
void customerView::on_lineEdit_page_returnPressed()
{
	int p = ui->lineEdit_page->text().toInt();
	// Test des bornes min
	if(p < 1){
		m_custPage = 1;
		ui->lineEdit_page->setText(QString::number(m_custPage));
	}// Max
	else if(p > m_custNbPage){
		m_custPage = m_custNbPage;
		ui->lineEdit_page->setText(QString::number(m_custPage));
	}
	else m_custPage = p;

	listCustomers(m_custPage);
}

/**
	Rafraichit la liste des clients
*/
void customerView::refreshCustomersList()
{
	 listCustomersToTable(m_custPage, m_custfilter, m_custfield);
}

/**
	Acquerir tous les clients
*/
void customerView::listCustomers(int page)
{
	//show all
	listCustomersToTable(page, m_custfilter, m_custfield);
}


/**
	Trie les clients selon leurs noms
	@param string de filtrage
*/
void customerView::listCustomersFilter(int page, QString val)
{
	if(!m_data->isConnected())return;
	//show with filter
	m_custfilter = val;
	if(ui->comboBoxFiltre->currentIndex() == 0 )        m_custfield = "LASTNAME ";
	else if(ui->comboBoxFiltre->currentIndex() == 1 )   m_custfield = "FIRSTNAME ";
	listCustomersToTable(page, m_custfilter, m_custfield);
}

/**
	Affiche les clients
*/
void customerView::listCustomersToTable(int page, QString filter, QString field)
{
	customer::CustomerList clist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget->clear();
	for (int i=ui->tableWidget->rowCount()-1; i >= 0; --i)
		ui->tableWidget->removeRow(i);
	for (int i=ui->tableWidget->columnCount()-1; i>=0; --i)
		ui->tableWidget->removeColumn(i);

	//DEsactivation des boutons car aucun client nest selectionne
	ui->toolBut_Edit->setEnabled(false);
	ui->toolButton_Del->setEnabled(false);
	ui->tabWidget_Customer->setEnabled(false);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget->setSortingEnabled(false);
	//Style de la table clients
	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setColumnWidth(1, 150);
	ui->tableWidget->setColumnWidth(2, 150);
#ifdef QT_NO_DEBUG
	ui->tableWidget->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles << tr("Id") << tr("Noms") << tr("Pr\351noms");
	ui->tableWidget->setHorizontalHeaderLabels( titles );

	//Cacul en fonction de la page
	int count = m_data->m_customer->count(filter, field);
	m_custNbPage = count/CUSTOMERSMAX_PAGE +1;
	int first = CUSTOMERSMAX_PAGE;
	int skip = (page-1) * CUSTOMERSMAX_PAGE;
	ui->lineEdit_page->setText(QString::number(page));
	ui->label_pageMax->setText("/ "+ QString::number(m_custNbPage));

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->getCustomerList(clist, first, skip, filter, field);

	// list all customers
	for(unsigned int i=0; i<clist.id.size(); i++){
		QTableWidgetItem *item_ID          = new QTableWidgetItem();
		QTableWidgetItem *item_LASTNAME      = new QTableWidgetItem();
		QTableWidgetItem *item_FIRSTNAME     = new QTableWidgetItem();

		item_ID->setData(Qt::DisplayRole, clist.id.at(i));
		item_LASTNAME->setData(Qt::DisplayRole, clist.lastName.at(i));
		item_FIRSTNAME->setData(Qt::DisplayRole, clist.firstName.at(i));

		//definir le tableau
		ui->tableWidget->setRowCount(i+1);

		//remplir les champs
		ui->tableWidget->setItem(i, 0, item_ID);
		ui->tableWidget->setItem(i, 1, item_LASTNAME);
		ui->tableWidget->setItem(i, 2, item_FIRSTNAME);
	}
	ui->tableWidget->setSortingEnabled(true);
	ui->tableWidget->sortByColumn(LASTNAME_ROW, Qt::AscendingOrder);
	//Affiche le nombre de client
	ui->labelCustomersNumber->setText(tr("Nombre de clients : ")+QString::number(count));
}


/**
	Filtre la liste des clients en fonction du edit
*/
void customerView::on_toolBut_search_clicked()
{
	//filtre avec retour sur la page 1
	m_custPage=1;
	listCustomersFilter(m_custPage, ui->lineEdit_Search->text() );
}


void customerView::on_lineEdit_Search_returnPressed() {
	//filtre avec retour sur la page 1
	m_custPage=1;
	listCustomersFilter(m_custPage, ui->lineEdit_Search->text() );
}


/**
	Event de la selection d'un client
	Affiche toutes les informations client dans la tab fiche client
*/
void customerView::on_tableWidget_itemSelectionChanged(){
	loadInfoCustomer();
}

/**

  */
void customerView::loadInfoCustomer(){
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget->currentRow();
	m_custId = ui->tableWidget->item(m_index, 0)->text().toInt();
	QString info = InfoCustomer( m_custId);
	ui->labelInfoCustomer->setText( info );
	ui->labelInfoCustomer->setOpenExternalLinks ( true );
	//liste les devis
	listProposals();
	//liste les factures
	listInvoices();
	//liste les services
	m_data->m_customer->m_service->setIdCustomer(m_data->m_customer->getId());
	listServices();
	//Activation des boutons
	ui->toolBut_Edit->setEnabled(true);
	ui->toolButton_Del->setEnabled(true);
	ui->tabWidget_Customer->setEnabled(true);
}

/**
	Acquerir toutes les Informations du client Selectionne
*/
QString customerView::InfoCustomer(int id)
{
	QString Informations;
	bool ret = m_data->m_customer->loadFromID(id);
	if(ret) {
		Informations = "<html><p style=\"font-size:18px;\" align=\"center\"><b>" + m_data->m_customer->getGender() + " " +
					   m_data->m_customer->getFirstName()+" "+m_data->m_customer->getLastName()+"</b></p>";
		Informations += "<p style=\"font-size:9px;\">"+tr("Date de cr&#233;ation: ")+"<font color = blue>";
		Informations += m_data->m_customer->getCreationDate().toString(tr("dd-MM-yyyy HH:mm:ss")) + " </font></p>";

		//info
		Informations += "<p style=\"font-size:14px;font-weight:bold;color:white;background:#7594ab; \">" + tr("&nbsp;Informations :")+"</p> ";
		if(m_data->m_customer->getType() == customer::PEOPLE_TYPE){
			Informations += "<p><img src=\":/app/customers\" height=\"48\" width=\"48\" /> <br>" +tr("Type : Particulier");
			Informations += "<br>" + tr("Date de naissance: ") + "<font color = blue>";
			Informations += m_data->m_customer->getBirthday().toString(tr("dd-MM-yyyy")) + " </font></p>";
		}
		else{
			Informations += "<p><img src=\":/app/compagny\" height=\"48\" width=\"48\" /> <br>" +tr("Type : Soci&#233;t&#233;");
			Informations += "<br>" + tr("Forme Juridique: ")+"<font color = blue>";
			Informations += m_data->m_customer->getDesCompagny() + "</font></p>";
		}

		//contact
		Informations += "<p style=\"font-size:14px;font-weight:bold;color:white;background:#7594ab; \">" + tr("&nbsp;Contact :")+"</p>";
		Informations += tr("T&#233;l fixe: ")+"<font color = blue>";
		Informations += m_data->m_customer->getPhoneNumber() + "</font>";
		Informations += "&nbsp;&nbsp;" + tr("Mobile: ") + "<font color = blue>";
		Informations += m_data->m_customer->getMobileNumber() + "</font>";

		Informations += "<br><br>" + tr("Email: ");
		Informations += "<a href=\"MAILTO:";
		Informations += m_data->m_customer->getEmail() + "\">"+ m_data->m_customer->getEmail() +"</a>";;
		Informations += "&nbsp;&nbsp;" + tr("Profession: ") + "<font color = blue>";
		Informations += m_data->m_customer->getProfession() + "</font>";
		//adresse
		Informations += "<p style=\"font-size:14px;font-weight:bold;color:white;background:#7594ab; \">" + tr("&nbsp;Adresse :")+"</p>";
		Informations += m_data->m_customer->getAddress1();
		Informations += "<br>"+m_data->m_customer->getAddress2();
		Informations += "<br>"+m_data->m_customer->getAddress3();
		Informations += "<br>"+m_data->m_customer->getCity() + "&nbsp;&nbsp;";
		Informations += m_data->m_customer->getZipCode();
		Informations += "<br>"+m_data->m_customer->getCountry();

		//Si pas de code postal ni de ville pas de plan!
		if(!m_data->m_customer->getCity().isEmpty() && !m_data->m_customer->getZipCode().isEmpty()){
			Informations += "<br><a href=\"http://maps.google.com/maps?q=";
			Informations +=  m_data->m_customer->getAddress1() + " ";
			Informations +=  m_data->m_customer->getAddress2() + " ";
			Informations +=  m_data->m_customer->getAddress3() + ", ";
			Informations +=  m_data->m_customer->getCity()     + " ";
			Informations +=  m_data->m_customer->getZipCode()  + " ";
			Informations +=  m_data->m_customer->getCountry()  + " ";
			Informations +=  "\">Plan d'acc&egrave;s</a>";
		}


		Informations += "<br><br>" + tr("Commentaire: ")+"<font color = blue>";
		Informations += "<br>"+m_data->m_customer->getComments() + "</font></html> ";
	}

	return Informations;
}

/**
	Copie les informations produit dans le presse papier
  */
void customerView::on_toolButton_clipboard_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QTextDocumentFragment mt = QTextDocumentFragment::fromHtml (this->InfoCustomer(m_custId) );
	QClipboard *m_clipboard = QApplication::clipboard();
	m_clipboard->setText( mt.toPlainText() );
}

/**
	Liste tous les devis du client
*/
void customerView::listProposals()
{
	listProposalsToTable("", "");
}

/**
	Affiche les devis du client
	@param filter, filtre a appliquer
	@param field, champ ou appliquer le filtre
*/
void customerView::listProposalsToTable(QString filter, QString field) {
	proposal::ProposalList ilist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_Proposals->clear();
	for (int i=ui->tableWidget_Proposals->rowCount()-1; i >= 0; --i)
		ui->tableWidget_Proposals->removeRow(i);
	for (int i=ui->tableWidget_Proposals->columnCount()-1; i>=0; --i)
		ui->tableWidget_Proposals->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_Proposals->setSortingEnabled(false);
	//Style de la table de facture
	ui->tableWidget_Proposals->setColumnCount(6);
	ui->tableWidget_Proposals->setColumnWidth(4,250);
#ifdef QT_NO_DEBUG
	ui->tableWidget_Proposals->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Proposals->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Proposals->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Proposals->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles  << tr("Id") << tr("Codes") << tr("Date") << tr("Prix") << tr("Description")  << tr("Etat");
	ui->tableWidget_Proposals->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_proposal->getProposalList(ilist, m_data->m_customer->getId(), "TAB_PROPOSALS.DATE", filter, field);

	// list tous les devis
	for(int i=0; i<ilist.code.count(); i++){
		QTableWidgetItem *item_ID           = new QTableWidgetItem();
		QTableWidgetItem *item_CODE         = new QTableWidgetItem();
		QTableWidgetItem *item_DATE         = new QTableWidgetItem();
		QTableWidgetItem *item_PRICE        = new QTableWidgetItem();
		QTableWidgetItem *item_DESCRIPTION  = new QTableWidgetItem();
		QTableWidgetItem *item_STATE        = new QTableWidgetItem();

		item_ID->setData(Qt::DisplayRole, ilist.id.at(i));
		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i));
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i).toString(tr("dd/MM/yyyy")));
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
		item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));
		item_STATE->setIcon( m_data->m_customer->m_proposal->getIconState(ilist.state.at(i)) );
		item_STATE->setText( m_data->m_customer->m_proposal->getTextState(ilist.state.at(i)) );

		//definir le tableau
		ui->tableWidget_Proposals->setRowCount(i+1);

		//remplir les champs
		ui->tableWidget_Proposals->setItem(i, 0, item_ID);
		ui->tableWidget_Proposals->setItem(i, 1, item_CODE);
		ui->tableWidget_Proposals->setItem(i, 2, item_DATE);
		ui->tableWidget_Proposals->setItem(i, 3, item_PRICE);
		ui->tableWidget_Proposals->setItem(i, 4, item_DESCRIPTION);
		ui->tableWidget_Proposals->setItem(i, 5, item_STATE);
	}
	ui->tableWidget_Proposals->setSortingEnabled(true);
	ui->tableWidget_Proposals->selectRow(0);

	//Si pas de facture on desactive les boutons
	if( ui->tableWidget_Proposals->rowCount() <=0 ) {
		ui->toolButton_editProposal->setEnabled(false);
		ui->toolButton_delProposal->setEnabled(false);
		ui->toolButton_printPro->setEnabled(false);
	}
	else{
		ui->toolButton_editProposal->setEnabled(true);
		ui->toolButton_delProposal->setEnabled(true);
		ui->toolButton_printPro->setEnabled(true);
	}
}

/**
	Liste tous les Services du client
*/
void customerView::listServices()
{
   listServicesToTable("", "");
}

/**
	Affiche les factures du client
	@param pointer sur le query qui comprend les datas
*/
void customerView::listServicesToTable(QString filter, QString field)
{
	service::serviceList ilist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_Services->clear();
	for (int i=ui->tableWidget_Services->rowCount()-1; i >= 0; --i)
		ui->tableWidget_Services->removeRow(i);
	for (int i=ui->tableWidget_Services->columnCount()-1; i>=0; --i)
		ui->tableWidget_Services->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_Services->setSortingEnabled(false);
	//Style de la table de l intervention
	ui->tableWidget_Services->setColumnCount(4);
	ui->tableWidget_Services->setColumnWidth(1,150);
	ui->tableWidget_Services->setColumnWidth(3,250);
#ifdef QT_NO_DEBUG
	ui->tableWidget_Services->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Services->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Services->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Services->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles << tr("Id") << tr("Date") << tr("Prix") << tr("Nom");
	ui->tableWidget_Services->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_service->getServiceList(ilist, m_data->m_customer->getId(), "THEDATE", filter, field);

	// list all customers
	for(unsigned int i=0; i<ilist.id.size(); i++){
		QTableWidgetItem *item_ID      = new QTableWidgetItem();
		QTableWidgetItem *item_DATE      = new QTableWidgetItem();
		QTableWidgetItem *item_PRICE     = new QTableWidgetItem();
		QTableWidgetItem *item_NAME     = new QTableWidgetItem();

		item_ID->setData(Qt::DisplayRole, QString::number(ilist.id.at(i)));
		item_DATE->setData(Qt::DisplayRole, ilist.date.at(i).toString("dd/MM/yyyy hh:mm:ss"));
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
		item_NAME->setData(Qt::DisplayRole, ilist.name.at(i));

		//definir le tableau
		ui->tableWidget_Services->setRowCount(i+1);

		//remplir les champs
		ui->tableWidget_Services->setItem(i, 0, item_ID);
		ui->tableWidget_Services->setItem(i, 1, item_DATE);
		ui->tableWidget_Services->setItem(i, 2, item_PRICE);
		ui->tableWidget_Services->setItem(i, 3, item_NAME);
	}
	ui->tableWidget_Services->setSortingEnabled(true);
	ui->tableWidget_Services->selectRow(0);

	//Si pas de services on desactive les boutons
	if( ui->tableWidget_Services->rowCount() <=0 ) {
		ui->toolButton_editService->setEnabled(false);
		ui->toolButton_delService->setEnabled(false);
		ui->toolButton_printServ->setEnabled(false);
	}else{
		ui->toolButton_editService->setEnabled(true);
		ui->toolButton_delService->setEnabled(true);
		ui->toolButton_printServ->setEnabled(true);
	}
}


/**
	Liste tous les factures du client
*/
void customerView::listInvoices()
{
	listInvoicesToTable("", "");
}

/**
	Affiche les factures du client
	@param filter, filtre a appliquer
	@param field, champ ou appliquer le filtre
*/
void customerView::listInvoicesToTable(QString filter, QString field)
{
	invoice::InvoiceList ilist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_Invoices->clear();
	for (int i=ui->tableWidget_Invoices->rowCount()-1; i >= 0; --i)
		ui->tableWidget_Invoices->removeRow(i);
	for (int i=ui->tableWidget_Invoices->columnCount()-1; i>=0; --i)
		ui->tableWidget_Invoices->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_Invoices->setSortingEnabled(false);
	//Style de la table de facture
	ui->tableWidget_Invoices->setColumnCount(6);
	ui->tableWidget_Invoices->setColumnWidth(4,250);
#ifdef QT_NO_DEBUG
	ui->tableWidget_Invoices->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Invoices->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Invoices->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Invoices->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles  << tr("Id") << tr("Codes") << tr("Date") << tr("Prix") << tr("Description")  << tr("Etat");
	ui->tableWidget_Invoices->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_invoice->getInvoiceList(ilist,m_data->m_customer->getId(), "TAB_INVOICES.DATE", filter, field);

	// list all customers
	for(int i=0; i<ilist.code.count(); i++){
		QTableWidgetItem *item_ID           = new QTableWidgetItem();
		QTableWidgetItem *item_CODE         = new QTableWidgetItem();
		QTableWidgetItem *item_DATE         = new QTableWidgetItem();
		QTableWidgetItem *item_PRICE        = new QTableWidgetItem();
		QTableWidgetItem *item_DESCRIPTION  = new QTableWidgetItem();
		QTableWidgetItem *item_STATE        = new QTableWidgetItem();

		item_ID->setData(Qt::DisplayRole, ilist.id.at(i));
		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i));
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i).toString(tr("dd/MM/yyyy")));
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
		item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));
		item_STATE->setIcon( m_data->m_customer->m_invoice->getIconState(ilist.state.at(i)) );
		item_STATE->setText( m_data->m_customer->m_invoice->getTextState(ilist.state.at(i)) );

		//definir le tableau
		ui->tableWidget_Invoices->setRowCount(i+1);

		//remplir les champs
		ui->tableWidget_Invoices->setItem(i, 0, item_ID);
		ui->tableWidget_Invoices->setItem(i, 1, item_CODE);
		ui->tableWidget_Invoices->setItem(i, 2, item_DATE);
		ui->tableWidget_Invoices->setItem(i, 3, item_PRICE);
		ui->tableWidget_Invoices->setItem(i, 4, item_DESCRIPTION);
		ui->tableWidget_Invoices->setItem(i, 5, item_STATE);
	}
	ui->tableWidget_Invoices->setSortingEnabled(true);
	ui->tableWidget_Invoices->selectRow(0);

	//Si pas de facture on desactive les boutons
	if( ui->tableWidget_Invoices->rowCount() <=0 ) {
		ui->toolButton_editInvoice->setEnabled(false);
		ui->toolButton_delInvoice->setEnabled(false);
		ui->toolButton_printInv->setEnabled(false);
	}
	else{
		ui->toolButton_editInvoice->setEnabled(true);
		ui->toolButton_delInvoice->setEnabled(true);
		ui->toolButton_printInv->setEnabled(true);
	}
}


  /*  QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFileName("print.pdf");
	QPainter painter;
	painter.begin(&printer);

	int numberOfPages=1,lastPage=1;

	for (int page = 0; page < numberOfPages; ++page) {

		// Use the painter to draw on the page.
		painter.setPen(Qt::blue);
		painter.setFont(QFont("Arial", 30));
		painter.drawText(QPoint(30,30), "FRAUSTI Manager");

		if (page != lastPage)
			printer.newPage();
	}

	painter.end();*/


 /*   QFileDialog dialog(this);
	QString filename = dialog.getSaveFileName(this, "Save file", "order.pdf", "");
	if( !filename.isEmpty() ) {
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFileName( filename.toStdString().c_str() );
		QPainter painter;
		painter.begin(&printer);

		int numberOfPages=1,lastPage=1;

		for (int page = 0; page < numberOfPages; page++) {

			// Use the painter to draw on the page.
			painter.setPen(Qt::blue);
			painter.setFont(QFont("Arial", 30));
			painter.drawText(QPoint(30,30), "FRAUSTI Manager");

			if (page != lastPage)
				printer.newPage();
		}

		painter.end();
	}*/


/**
	Ajouter un devis
*/
void customerView::on_toolButton_addProposal_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::PROPOSAL_TYPE, DialogInvoice::ADD);
	m_DialogInvoice->setModal(true);
	m_DialogInvoice->setTitle(tr("Ajouter une proposition commerciale"));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshProposalList()), this, SLOT(listProposals()));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshInvoiceList()), this, SLOT(listInvoices()));
	m_DialogInvoice->exec();

	delete m_DialogInvoice;
}


/**
	Editer le devis
*/
void customerView::on_toolButton_editProposal_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Proposals->currentRow();
	int m_InvId = ui->tableWidget_Proposals->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_proposal->loadFromID(m_InvId);

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::PROPOSAL_TYPE, DialogInvoice::EDIT);
	m_DialogInvoice->setModal(true);
	m_DialogInvoice->setTitle(tr("Modifier une proposition commerciale"));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshProposalList()), this, SLOT(listProposals()));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshInvoiceList()), this, SLOT(listInvoices()));
	m_DialogInvoice->exec();

	delete m_DialogInvoice;
}

/**
	Supression d un devis
  */
void customerView::on_toolButton_delProposal_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Proposals->currentRow();
	int m_InvId = ui->tableWidget_Proposals->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_proposal->loadFromID(m_InvId);

	int ret = QMessageBox::critical(this, tr("Attention"),
								   tr("Ceci est fortement d\351conseill\351 !<br>"
									  "Voulez-vous vraiment supprimer la proposition commerciale<br><br><b>")+
								   m_data->m_customer->m_proposal->getCode() +" - "+
								   m_data->m_customer->m_proposal->getDescription()+"</b>",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		m_data->m_customer->m_proposal->remove();
		//rafraichir la liste les devis
		this->listProposals();
	}
}


/**
	Ajouter une intervention
*/
void customerView::on_toolButton_addService_clicked()
{
	if(!m_data->isConnected())return;

	DialogServices *m_DialogInt = new DialogServices(m_data);
	m_DialogInt->setModal(true);
	m_DialogInt->setTitle(tr("Ajouter un service"));
	m_DialogInt->exec();

	if(m_DialogInt->result() == QDialog::Accepted)
	{
		// Recuperation des informations de clients
		m_data->m_customer->m_service->create();
		//rafraichir la liste dans tout les cas pour recharger la class service
		listServices();
	}
	delete m_DialogInt;
}

/**
	Event de la selection d une intervention
*/
void customerView::on_tableWidget_Services_itemSelectionChanged()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Services->currentRow();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_service->loadFromID( ui->tableWidget_Services->item(m_index, 0)->text().toInt() );
}

/**
	Supprimer une intervention
  */
void customerView::on_toolButton_delService_clicked()
{
	if(!m_data->isConnected())return;

	int ret = QMessageBox::warning(this, tr("Attention"),
								   tr("Voulez-vous vraiment supprimer le service<br><b>")+
								   m_data->m_customer->m_service->getDate().toString("dd/MM/yyyy hh:mm:ss") +" - "+
								   m_data->m_customer->m_service->getName()+"</b><br> de la liste ?",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		m_data->m_customer->m_service->remove();
		//rafraichir la liste dans tout les cas pour recharger la class service
		listServices();
	}
}

/**
	Edite une intervention
  */
void customerView::on_toolButton_editService_clicked()
{
	if(!m_data->isConnected())return;

	DialogServices *m_DialogInt = new DialogServices(m_data);
	m_DialogInt->setModal(true);
	m_DialogInt->setTitle(tr("Modifier un service"));
	m_DialogInt->loadValuesFromService();
	m_DialogInt->exec();

	if(m_DialogInt->result() == QDialog::Accepted) {
		// Mise a jour des informations clients
		m_data->m_customer->m_service->update();
		//rafraichir la liste dans tout les cas pour recharger la class service
		listServices();
	}
	delete m_DialogInt;
}



/**
	Ajouter une facture
*/
void customerView::on_toolButton_addInvoice_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data ,DialogInvoice::INVOICE_TYPE,  DialogInvoice::ADD);
	m_DialogInvoice->setModal(true);
	m_DialogInvoice->setTitle(tr("Ajouter une facture"));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshInvoiceList()), this, SLOT(listInvoices()));
	m_DialogInvoice->exec();

	delete m_DialogInvoice;
}

/**
	Edite une facture
  */
void customerView::on_toolButton_editInvoice_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Invoices->currentRow();
	int m_InvId = ui->tableWidget_Invoices->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_invoice->loadFromID(m_InvId);

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::INVOICE_TYPE, DialogInvoice::EDIT);
	m_DialogInvoice->setModal(true);
	m_DialogInvoice->setTitle(tr("Modifier une facture"));
	QObject::connect(m_DialogInvoice, SIGNAL(askRefreshInvoiceList()), this, SLOT(listInvoices()));
	m_DialogInvoice->exec();

	delete m_DialogInvoice;
}


/**
	Supprime une facture
  */
void customerView::on_toolButton_delInvoice_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Invoices->currentRow();
	int m_InvId = ui->tableWidget_Invoices->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_invoice->loadFromID(m_InvId);

	int ret = QMessageBox::critical(this, tr("Attention"),
								   tr("Ceci est fortement d\351conseill\351 !<br>"
									  "Voulez-vous vraiment supprimer la facture<br><br><b>")+
								   m_data->m_customer->m_invoice->getCode() +" - "+
								   m_data->m_customer->m_invoice->getDescription()+"</b>",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		m_data->m_customer->m_invoice->remove();
		//rafraichir la liste des factures
		this->listInvoices();
	}
}

/**
	Imprimer le bon pour l intervention
  */
void customerView::on_toolButton_printServ_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = "SR"  + m_data->m_customer->m_service->getDate().toString("yyMMdd") +"-"+ QString::number(m_data->m_customer->m_service->getId()) ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterService(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}


void customerView::on_paintPrinterService(QPrinter *printer)
{
	QPainter painter;
	painter.begin(printer);
   // int res = printer->resolution();
   // QRect paperRect = printer->paperRect();
	QRect pageRect = printer->pageRect();
   // double coeff = paperRect.width() / 21.0; // A4
	qreal mLeft, mTop, mRight, mBottom;
	printer->getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	//painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait
	QFontMetrics fm= painter.fontMetrics();
	QFont font = painter.font();
	int wUtil = pageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases

	//Logo
	QImage logo = m_data->getLogoTable_informations();
	//Info societe
	database::Informations info;
	m_data->getInfo(info);
	QString textInfo = info.name + "\n" + tr("Tel: ") + info.phoneNumber + "\n" + tr("Fax: ") + info.faxNumber + "\n" + tr("Email: ") + info.email + "\n" + tr("Web: ") + info.webSite;
	/// Identite du client
	QString identity = m_data->m_customer->getFirstName()+" "+m_data->m_customer->getLastName()+'\n';
	identity += m_data->m_customer->getAddress1()+'\n';
	identity += m_data->m_customer->getAddress2()+'\n';
	identity += m_data->m_customer->getAddress3()+'\n';
	identity += m_data->m_customer->getZipCode()+" "+m_data->m_customer->getCity();

	//Margins Debug
	//painter.drawRect(mLeft, mTop, pageRect.width() - (mLeft+mRight), pageRect.height() - (mTop+mBottom));

	//Logo
	QRect rect = QRect(mLeft+5, mTop, logo.width(), logo.height() );
	painter.drawImage(rect, logo);

	//Info societe
	font.setPointSize(10);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect.translate( 0, rect.height()+5);
	rect = fm.boundingRect(mLeft+5,rect.top(), 0,0, Qt::AlignLeft, textInfo );
	painter.drawText( rect, textInfo);

	//Titre
	font.setPointSize(24);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect = QRect(mLeft, mTop, pageRect.width() - (mLeft+mRight),
					   fm.boundingRect( tr("Fiche d'intervention") ).height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Fiche d'intervention") );

	font.setPointSize(14);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect.translate( 0, 32);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( fm.boundingRect( tr("Code: ") + "SR"  + m_data->m_customer->m_service->getDate().toString("yyMMdd") +"-"+ QString::number(m_data->m_customer->m_service->getId()) ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + "SR"  + m_data->m_customer->m_service->getDate().toString("yyMMdd") +"-"+ QString::number(m_data->m_customer->m_service->getId()));

	font.setPointSize(12);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect.translate( 0, 20);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( fm.boundingRect( tr("Date: ")+m_data->m_customer->m_service->getDate().toString(tr("dd-MM-yyyy")) ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
					  tr("Date: ")+
					  m_data->m_customer->m_service->getDate().toString(tr("dd-MM-yyyy")) );


	/// Identite du client
	font.setPointSize(10);
	painter.setFont(font);
	fm= painter.fontMetrics();
	//rect.translate( wUtil/2, rect.bottom());
	rect = fm.boundingRect(wUtil/2, rect.bottom()+40, 0, 0, Qt::AlignLeft, tr("Client: ") );
	painter.drawText(rect, tr("Client: "));

	//rect.translate( 0, rect.height()+5);
	rect = fm.boundingRect(wUtil/2, rect.bottom()+5, 0, 0, Qt::AlignLeft, identity );
	painter.drawText(rect, identity);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + wUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	/// contenu
	//contour du contenu
	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 450;

	QString serv = tr("Intervention: ") + m_data->m_customer->m_service->getName()+"\n";
	serv += tr("R\351alis\351 le : ") + m_data->m_customer->m_service->getDate().toString(tr("dd-MM-yyyy"))+ tr(" \xE0 ")+ m_data->m_customer->m_service->getDate().toString(tr("HH:mm")) + "\n\n";
	serv += tr("D\351tails:\n");
	serv += m_data->m_customer->m_service->getDescription();
	serv += " \n";
	// Attention ladaptation avec le Qt::TextWordWrap ne fonctionne qui si il y a des espaces dans la chaine !!
	rect = fm.boundingRect(mLeft+5,rect.bottom()+40,
						   wUtil-5 ,0,
						   Qt::TextWordWrap | Qt::AlignLeft,
						   serv );

	// Si la hauteur depasse le rectangle on fixe sa taille et tant pis on perd du text...
	if(rect.height() > blockHeight)rect.setHeight(blockHeight-10);
	painter.drawText( rect, Qt::TextWordWrap, serv);

	rect = QRect(mLeft, rect.top()-10, wUtil, blockHeight);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	/// Prix
   /* QString price = tr("Prix: ") + QString::number(m_data->m_customer->m_service->getPrice())+ QChar(8364);
	if(m_data->getIsTax()){
		price += tr(" HT");
		//TODO:complete...
	}
	else{
		price += tr(" TTC");
	}
	rect.translate( wUtil/2, rect.height()+50);
	rect = fm.boundingRect(wUtil/2, rect.top(), 0, 0, Qt::AlignLeft, price );
	painter.drawText(rect, price);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + wUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius
	*/

	/// Signature Client
	QString sign = "Signature client:\n\n\n\n\n";
	rect.translate( wUtil/2, rect.height()+25);
	rect = fm.boundingRect(wUtil/2, rect.top(), 0, 0, Qt::AlignLeft, sign );
	painter.drawText(rect, sign);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + wUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	//Information pied de page
	font.setPointSize(10);
	painter.setFont(font);
	textInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	textInfo += "\n" + info.name + " - " + tr("Capital ") + info.capital + " - " + tr("Siret ") + info.num;
	rect = fm.boundingRect(mLeft, pageRect.height() - mBottom, pageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, textInfo );
	rect.translate( 0, -rect.height());

	painter.drawText( rect, Qt::AlignCenter, textInfo);
	painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + wUtil, rect.top()));

	painter.end();
}







/**
	Impression des propositions commerciales
  */
void customerView::on_toolButton_printPro_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Proposals->currentRow();
	int m_InvId = ui->tableWidget_Proposals->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_proposal->loadFromID(m_InvId);

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = m_data->m_customer->m_proposal->getCode() ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterProposal(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}

void customerView::on_paintPrinterProposal(QPrinter *printer)
{
	QPainter painter;
	painter.begin(printer);
   // int res = printer->resolution();
   // QRect paperRect = printer->paperRect();
	QRectF rect;
	QRectF pageRect = printer->pageRect();
   // double coeff = paperRect.width() / 21.0; // A4
	qreal mLeft, mTop, mRight, mBottom;
	printer->getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	QFontMetrics fm= painter.fontMetrics();
	QFont font = painter.font();
	qreal wUtil = pageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases
	//painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait

	proposal::ProposalListItems plist;
	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_proposal->getProposalItemsList(plist, "ITEM_ORDER", "", "");
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int itemPerPage;
	if(printer->orientation() == QPrinter::Landscape) itemPerPage = 4;
	else itemPerPage = 16;
	//Defini le nombre a imprimer
	int itemsToPrint = plist.name.count();
	if(itemsToPrint < itemPerPage )itemPerPage = itemsToPrint;
	if(itemPerPage== 0)itemPerPage++;
	int numberOfPage = itemsToPrint/itemPerPage;
	m_DialogWaiting->setProgressBarRange(0, numberOfPage);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	//Logo
	QImage logo = m_data->getLogoTable_informations();
	//Info societe
	database::Informations info;
	m_data->getInfo(info);
	QString textInfo = info.name + "\n";
	if(!info.phoneNumber.isEmpty()) textInfo += tr("Tel: ") + info.phoneNumber + "\n";
	if(!info.faxNumber.isEmpty())   textInfo += tr("Fax: ") + info.faxNumber + "\n";
	if(!info.email.isEmpty())       textInfo += tr("Email: ") + info.email + "\n";
	if(!info.webSite.isEmpty())     textInfo += tr("Web: ") + info.webSite;

	/// Identite du client
	QString identity = m_data->m_customer->getFirstName()+" "+m_data->m_customer->getLastName()+'\n';
	identity += m_data->m_customer->getAddress1()+'\n';
	identity += m_data->m_customer->getAddress2()+'\n';
	identity += m_data->m_customer->getAddress3()+'\n';
	identity += m_data->m_customer->getZipCode()+" "+m_data->m_customer->getCity();

	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 500;
	QRect rectContent = QRect(mLeft, 0, wUtil, blockHeight);

	//VAR Prix total
	qreal totalPrice=0;
	std::vector<float> ListTotalPrice;

	//Pied de page
	QString footerTextInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	footerTextInfo += "\n" + info.name;
	if(!info.capital.isEmpty()) footerTextInfo += " - " + tr("Capital ") + info.capital;
	if(!info.num.isEmpty())     footerTextInfo += " - " + tr("Siret ") + info.num;
	if(!m_data->getIsTax())     footerTextInfo += "\n" + tr("Dispens\351 d'immatriculation au registre du commerce et des soci\351t\351 (RCS) et au r\351pertoire des m\351tiers (RM)");

	QString pageText;
	// list all products
	for(int pIndex=0, page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		//Logo
		rect = QRect(mLeft+5, mTop, logo.width(), logo.height() );
		painter.drawImage(rect, logo);

		//Info societe
		font.setPointSize(10);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, rect.height()+5);
		rect = fm.boundingRect(mLeft+5,rect.top(), 0,0, Qt::AlignLeft, textInfo );
		painter.drawText( rect, textInfo);

		//Titre
		font.setPointSize(24);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect = QRect(mLeft, mTop, pageRect.width() - (mLeft+mRight),
						   fm.boundingRect( tr("Devis") ).height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Devis") );

		font.setPointSize(14);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 32);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Code: ") + m_data->m_customer->m_proposal->getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_data->m_customer->m_proposal->getCode());

		font.setPointSize(12);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 20);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Date: ")+m_data->m_customer->m_proposal->getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_data->m_customer->m_proposal->getUserDate().toString(tr("dd-MM-yyyy")) );
		rect.translate( 0, rect.height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date fin validit\351: ")+
						  m_data->m_customer->m_proposal->getValidDate().toString(tr("dd-MM-yyyy")) );

		/// Identite du client
		font.setPointSize(10);
		painter.setFont(font);
		fm= painter.fontMetrics();
		//rect.translate( wUtil/2, rect.bottom());
		rect = fm.boundingRect(wUtil/2, rect.bottom()+15, 0, 0, Qt::AlignLeft, tr("Client: ") );
		painter.drawText(rect, tr("Client: "));

		//rect.translate( 0, rect.height()+5);
		rect = fm.boundingRect(wUtil/2, rect.bottom()+5, 0, 0, Qt::AlignLeft, identity );
		painter.drawText(rect, identity);
		rect.translate(-5,-5);
		rect.setHeight(rect.height()+10);
		rect.setWidth(6 + mLeft + wUtil/2);
		painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

		// Contour du contenu, dessine le rectangle avec 5 de radius
		rect.translate( 0, rect.height()+25);
		rectContent.setTop( rect.top() );
		rectContent.setHeight( blockHeight );
		painter.drawRoundedRect(rectContent, 5, 5);

		/// Header
		rect.translate( 0, 5);
		//DESIGNATION 40%
		rect = fm.boundingRect(mLeft+5,rect.top(), wUtil*0.40,0, Qt::AlignLeft, tr("D\351signation") );
		painter.drawText( rect, tr("D\351signation"));
		//TVA 12%
		if(m_data->getIsTax()){
			painter.drawLine(QPoint(mLeft+wUtil*0.40, rectContent.top()),
							 QPoint(mLeft+wUtil*0.40, rectContent.bottom()) );
			rect = fm.boundingRect(mLeft+5+(wUtil*0.40),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("TVA %") );
			painter.drawText( rect, tr("TVA %") );
		}
		//REMISE 12%
		bool discount=false;
		for(int i=0; i<plist.discount.size(); i++)
			if(plist.discount.at(i) > 0){
			discount=true;
			break;
			}
		if(discount){
			painter.drawLine(QPoint(mLeft+wUtil*0.52, rectContent.top()),
							 QPoint(mLeft+wUtil*0.52, rectContent.bottom()) );
			rect = fm.boundingRect(mLeft+5+(wUtil*0.52),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Remise %"));
			painter.drawText( rect, tr("Remise %"));
		}
		//PRIX UNITAIRE 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.64, rectContent.top()),
						 QPoint(mLeft+wUtil*0.64, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+(wUtil*0.64),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Prix U. ")+ QChar(8364) );
		painter.drawText( rect, tr("Prix U. ")+ QChar(8364));
		//QUANTITE 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.76, rectContent.top()),
						 QPoint(mLeft+wUtil*0.76, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+wUtil*0.76,rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Quantit\351")  );
		painter.drawText( rect, tr("Quantit\351") );
		//TOTAL 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.88, rectContent.top()),
						 QPoint(mLeft+wUtil*0.88, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+(wUtil*0.88),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("TOTAL ")+ QChar(8364) );
		painter.drawText( rect, tr("TOTAL ")+ QChar(8364) );
		painter.drawLine(QPoint(rectContent.left(), rect.bottom()+5),
						 QPoint(rectContent.right(),rect.bottom()+5));

		rect.translate( 0, 5);
		for(int itemOnpage=0; itemOnpage<itemPerPage;){
			//sil ne reste plus a afficher on sort
			if((plist.name.count() - pIndex) <= 0)break;
			rect.translate( 0, rect.height()+5);

			//DESIGNATION 40%
			rect = fm.boundingRect(mLeft+5,rect.top(), wUtil*0.40,0, Qt::AlignLeft, plist.name.at(pIndex) );
			rect.setWidth(wUtil*0.50); //fixe la largeur
			painter.drawText( rect,  Qt::AlignLeft , plist.name.at(pIndex));

			//TVA 12%
			if(m_data->getIsTax()){
				rect = fm.boundingRect(mLeft-5+(wUtil*0.40),rect.top(), wUtil*0.12,0, Qt::AlignRight, m_lang.toString(plist.tax.at(pIndex),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(plist.tax.at(pIndex),'f',2) );
			}

			//REMISE 12%
			if(discount){
				rect = fm.boundingRect(mLeft-5+(wUtil*0.52),rect.top(), wUtil*0.12,0, Qt::AlignRight, QString::number(plist.discount.at(pIndex)) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , QString::number(plist.discount.at(pIndex)) );
			}

			//PRIX UNITAIRE 12%
			rect = fm.boundingRect(mLeft-5+(wUtil*0.64),rect.top(), wUtil*0.12,0, Qt::AlignRight, m_lang.toString(plist.price.at(pIndex),'f',2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , m_lang.toString(plist.price.at(pIndex),'f',2) );

			//QUANTITE 12%
			rect = fm.boundingRect(mLeft-5+wUtil*0.76,rect.top(), wUtil*0.12,0, Qt::AlignRight, QString::number(plist.quantity.at(pIndex)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect , Qt::AlignRight , QString::number(plist.quantity.at(pIndex)));

			//TOTAL 12%
			totalPrice = plist.price.at(pIndex)*plist.quantity.at(pIndex);
			if(plist.discount.at(pIndex)>0)totalPrice -= totalPrice*(plist.discount.at(pIndex)/100.0);
			rect = fm.boundingRect(mLeft-5+(wUtil*0.88),rect.top(), wUtil*0.12,0, Qt::AlignRight,
								   m_lang.toString(totalPrice, 'f', 2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight ,
								   m_lang.toString(totalPrice, 'f', 2) );
			ListTotalPrice.push_back(totalPrice);

			itemPrinted++;
			itemOnpage++;

			pIndex++;
			//Nombre ditem max atteind?
			if( (itemOnpage - itemPerPage) >= 0) break;
		}

		//Information pied de page
		rect = fm.boundingRect(mLeft, pageRect.height() - mBottom, pageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, footerTextInfo );
		rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, footerTextInfo);

		//Num de page
		pageText = "- " + tr("Page ") + QString::number(page) /*+ '/' + QString::number(printer->copyCount())*/ + " -";
		rect = fm.boundingRect(mLeft, rect.top(), pageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		//rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, pageText);

		//Ligne
		//rect.translate( 0, -rect.height());
		painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + wUtil, rect.top()));
		m_DialogWaiting->setProgressBar(page);

		//New page ?
		if( (itemsToPrint - itemPrinted) > 0) printer->newPage();
	}

	//TOTAL
	totalPrice=0;
	for(unsigned i=0; i<ListTotalPrice.size(); i++)
		totalPrice += ListTotalPrice.at(i);
	rect = fm.boundingRect(mLeft+5+(wUtil*0.62),rectContent.bottom()+15, wUtil*0.36,0, Qt::AlignLeft, tr("TOTAL : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect( QRect(mLeft+(wUtil*0.62),rect.top()-5, wUtil*0.36 +15, rect.height() +10), 5, 5 );
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("TOTAL : "));

	font.setBold(true);
	painter.setFont(font);
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rectContent.bottom()+15, wUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice, 'f', 2) );

	font.setBold(false);
	painter.setFont(font);
	painter.setBrush( Qt::NoBrush );

	//TOTAL TAX
	QString text;
	if(m_data->getIsTax()){
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = fm.boundingRect(mLeft, rectContent.bottom()+5 , wUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Mode de reglement
	QString typePayment;
	QString typeP = m_data->m_customer->m_proposal->getTypePayment();
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == CASH)           typePayment = tr("Espece");
	if(typeP == CHECK)          typePayment = tr("Cheque");
	if(typeP == CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == INTERBANK)      typePayment = tr("TIP");
	if(typeP == TRANSFER)       typePayment = tr("Virement");
	if(typeP == DEBIT)          typePayment = tr("Prelevement");
	if(typeP == OTHER)          typePayment = tr("Autre");
	text = tr("Mode de r\350glement : ")+typePayment;
	rect = fm.boundingRect(mLeft,rect.bottom()+5, wUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == INTERBANK)||(typeP == DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de r\350glement.");
		rect = fm.boundingRect(mLeft, rect.bottom()+5, wUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Condition de reglement
	text = tr("Conditions de r\350glement: 30% du montant total lors\nde la signature de cette proposition soit: ");
	text += m_lang.toString(totalPrice * 0.3, 'f', 2) +" "+ QChar(8364);
	rect = fm.boundingRect(mLeft,rect.bottom()+5, wUtil*0.50,0, Qt::AlignLeft, text);
	painter.drawText( rect, text);

	/// Signature Client
	text = "Signature client:\n\n\n\n\n\n";
	rect = fm.boundingRect(mLeft+(wUtil*0.62)+5, rect.bottom()+15, 0, 0, Qt::AlignLeft, text );
	painter.drawText(rect, text);
	painter.drawRoundedRect( QRect(mLeft+(wUtil*0.62),rect.top()-5, wUtil*0.36 +15, rect.height() +10), 5, 5 );

	/// RIB
	if(typeP == TRANSFER){
		database::Bank mb;
		m_data->getBank(mb);
		text = "Relev\351 d'Itentit\351 Bancaire\n\n\n\n\n\n";
		rect = fm.boundingRect(mLeft+5, rect.top(), wUtil*0.36 +15, rect.height() +10, Qt::AlignCenter, text );
		painter.drawText(rect, text);
		painter.setPen( Qt::DashLine );
		painter.drawRoundedRect( QRect(mLeft,rect.top()-5, wUtil*0.36 +15, rect.height()), 5, 5 );
		painter.setPen( Qt::SolidLine );

		font.setPointSize(8);
		painter.setFont(font);
		text = tr("Code banque: ")+mb.codeBanque+"  "+tr("Code guichet: ")+mb.codeGuichet+'\n';
		text += tr("Compte: ")+mb.numCompte+"  "+tr("Cl\351 RIB: ")+mb.keyRIB+'\n';
		text += tr("Domiciliation: ");
		text += mb.address+"\n\n";
		text += tr("IBAN: ");
		text += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
		text += tr("BIC: ") + mb.codeBIC+'\n';
		rect = fm.boundingRect(mLeft+5, rect.top()+15, wUtil*0.36 +15, rect.height(), Qt::AlignLeft, text );
		rect.setWidth(wUtil*0.36); //fixe la largeur
		painter.drawText(rect, text);
	}

	delete m_DialogWaiting;
	painter.end();
}



/**
	Impression des factures
  */
void customerView::on_toolButton_printInv_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_Invoices->currentRow();
	int m_InvId = ui->tableWidget_Invoices->item(m_index, 0)->text().toInt();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_invoice->loadFromID(m_InvId);

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = m_data->m_customer->m_invoice->getCode() ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterInvoice(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}


void customerView::on_paintPrinterInvoice(QPrinter *printer)
{
	QPainter painter;
	painter.begin(printer);
   // int res = printer->resolution();
   // QRect paperRect = printer->paperRect();
	QRectF rect;
	QRectF pageRect = printer->pageRect();
   // double coeff = paperRect.width() / 21.0; // A4
	qreal mLeft, mTop, mRight, mBottom;
	printer->getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	QFontMetrics fm= painter.fontMetrics();
	QFont font = painter.font();
	qreal wUtil = pageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases
	//painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait

	invoice::InvoiceListItems ilist;
	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_invoice->getInvoiceItemsList(ilist, "ITEM_ORDER", "", "");
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int itemPerPage;
	if(printer->orientation() == QPrinter::Landscape) itemPerPage = 8;
	else itemPerPage = 20;
	//Defini le nombre a imprimer
	int itemsToPrint = ilist.name.count();
	if(itemsToPrint < itemPerPage )itemPerPage = itemsToPrint;
	if(itemPerPage== 0)itemPerPage++;
	int numberOfPage = itemsToPrint/itemPerPage;
	m_DialogWaiting->setProgressBarRange(0, numberOfPage);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	//Logo
	QImage logo = m_data->getLogoTable_informations();
	//Info societe
	database::Informations info;
	m_data->getInfo(info);
	QString textInfo = info.name + "\n";
	if(!info.phoneNumber.isEmpty()) textInfo += tr("Tel: ") + info.phoneNumber + "\n";
	if(!info.faxNumber.isEmpty())   textInfo += tr("Fax: ") + info.faxNumber + "\n";
	if(!info.email.isEmpty())       textInfo += tr("Email: ") + info.email + "\n";
	if(!info.webSite.isEmpty())     textInfo += tr("Web: ") + info.webSite;

	/// Identite du client
	QString identity = m_data->m_customer->getFirstName()+" "+m_data->m_customer->getLastName()+'\n';
	identity += m_data->m_customer->getAddress1()+'\n';
	identity += m_data->m_customer->getAddress2()+'\n';
	identity += m_data->m_customer->getAddress3()+'\n';
	identity += m_data->m_customer->getZipCode()+" "+m_data->m_customer->getCity();

	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 500;
	QRect rectContent = QRect(mLeft, 0, wUtil, blockHeight);

	//VAR Prix total
	qreal totalPrice=0;
	std::vector<float> ListTotalPrice;

	//Pied de page
	QString footerTextInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	footerTextInfo += "\n" + info.name;
	if(!info.capital.isEmpty()) footerTextInfo += " - " + tr("Capital ") + info.capital;
	if(!info.num.isEmpty())     footerTextInfo += " - " + tr("Siret ") + info.num;
	if(!m_data->getIsTax())     footerTextInfo += "\n" + tr("Dispens\351 d'immatriculation au registre du commerce et des soci\351t\351 (RCS) et au r\351pertoire des m\351tiers (RM)");

	QString pageText;

	// list all products
	for(int pIndex=0, page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		//Logo
		rect = QRect(mLeft+5, mTop, logo.width(), logo.height() );
		painter.drawImage(rect, logo);

		//Info societe
		font.setPointSize(10);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, rect.height()+5);
		rect = fm.boundingRect(mLeft+5,rect.top(), 0,0, Qt::AlignLeft, textInfo );
		painter.drawText( rect, textInfo);

		//Titre
		font.setPointSize(24);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect = QRect(mLeft, mTop, pageRect.width() - (mLeft+mRight),
						   fm.boundingRect( tr("Facture") ).height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Facture") );

		font.setPointSize(14);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 32);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Code: ") + m_data->m_customer->m_invoice->getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_data->m_customer->m_invoice->getCode());

		if(!m_data->m_customer->m_invoice->getProposalCode().isEmpty()){
			rect.translate( 0, 20);
			rect.setHeight( fm.boundingRect( tr("Ref: ") + m_data->m_customer->m_invoice->getProposalCode() ).height() );
			painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Ref: ") + m_data->m_customer->m_invoice->getProposalCode());
		}
		font.setPointSize(12);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 20);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Date: ")+m_data->m_customer->m_invoice->getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_data->m_customer->m_invoice->getUserDate().toString(tr("dd-MM-yyyy")) );

		/// Identite du client
		font.setPointSize(10);
		painter.setFont(font);
		fm= painter.fontMetrics();
		//rect.translate( wUtil/2, rect.bottom());
		rect = fm.boundingRect(wUtil/2, rect.bottom()+15, 0, 0, Qt::AlignLeft, tr("Client: ") );
		painter.drawText(rect, tr("Client: "));

		//rect.translate( 0, rect.height()+5);
		rect = fm.boundingRect(wUtil/2, rect.bottom()+5, 0, 0, Qt::AlignLeft, identity );
		painter.drawText(rect, identity);
		rect.translate(-5,-5);
		rect.setHeight(rect.height()+10);
		rect.setWidth(6 + mLeft + wUtil/2);
		painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

		// Contour du contenu, dessine le rectangle avec 5 de radius
		rect.translate( 0, rect.height()+25);
		rectContent.setTop( rect.top() );
		rectContent.setHeight( blockHeight );
		painter.drawRoundedRect(rectContent, 5, 5);

		/// Header
		rect.translate( 0, 5);
		//DESIGNATION 40%
		rect = fm.boundingRect(mLeft+5,rect.top(), wUtil*0.40,0, Qt::AlignLeft, tr("D\351signation") );
		painter.drawText( rect, tr("D\351signation"));
		//TVA 12%
		if(m_data->getIsTax()){
			painter.drawLine(QPoint(mLeft+wUtil*0.40, rectContent.top()),
							 QPoint(mLeft+wUtil*0.40, rectContent.bottom()) );
			rect = fm.boundingRect(mLeft+5+(wUtil*0.40),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("TVA %") );
			painter.drawText( rect, tr("TVA %") );
		}
		//REMISE 12%
		bool discount=false;
		for(int i=0; i<ilist.discount.size(); i++)
			if(ilist.discount.at(i) > 0){
			discount=true;
			break;
			}
		if(discount){
			painter.drawLine(QPoint(mLeft+wUtil*0.52, rectContent.top()),
							 QPoint(mLeft+wUtil*0.52, rectContent.bottom()) );
			rect = fm.boundingRect(mLeft+5+(wUtil*0.52),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Remise %"));
			painter.drawText( rect, tr("Remise %"));
		}
		//PRIX UNITAIRE 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.64, rectContent.top()),
						 QPoint(mLeft+wUtil*0.64, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+(wUtil*0.64),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Prix U. ")+ QChar(8364) );
		painter.drawText( rect, tr("Prix U. ")+ QChar(8364));
		//QUANTITE 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.76, rectContent.top()),
						 QPoint(mLeft+wUtil*0.76, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+wUtil*0.76,rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("Quantit\351")  );
		painter.drawText( rect, tr("Quantit\351") );
		//TOTAL 12%
		painter.drawLine(QPoint(mLeft+wUtil*0.88, rectContent.top()),
						 QPoint(mLeft+wUtil*0.88, rectContent.bottom()) );
		rect = fm.boundingRect(mLeft+5+(wUtil*0.88),rect.top(), wUtil*0.12,0, Qt::AlignLeft, tr("TOTAL ")+ QChar(8364) );
		painter.drawText( rect, tr("TOTAL ")+ QChar(8364) );
		painter.drawLine(QPoint(rectContent.left(), rect.bottom()+5),
						 QPoint(rectContent.right(),rect.bottom()+5));

		rect.translate( 0, 5);
		for(int itemOnpage=0; itemOnpage<itemPerPage;){
			//sil ne reste plus a afficher on sort
			if((ilist.name.count() - pIndex) <= 0)break;
			rect.translate( 0, rect.height()+5);

			//DESIGNATION 40%
			rect = fm.boundingRect(mLeft+5,rect.top(), wUtil*0.40,0, Qt::AlignLeft, ilist.name.at(pIndex) );
			rect.setWidth(wUtil*0.50); //fixe la largeur
			painter.drawText( rect,  Qt::AlignLeft , ilist.name.at(pIndex));

			//TVA 12%
			if(m_data->getIsTax()){
				rect = fm.boundingRect(mLeft-5+(wUtil*0.40),rect.top(), wUtil*0.12,0, Qt::AlignRight, m_lang.toString(ilist.tax.at(pIndex),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(ilist.tax.at(pIndex),'f',2) );
			}

			//REMISE 12%
			if(discount){
				rect = fm.boundingRect(mLeft-5+(wUtil*0.52),rect.top(), wUtil*0.12,0, Qt::AlignRight, QString::number(ilist.discount.at(pIndex)) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , QString::number(ilist.discount.at(pIndex)) );
			}

			//PRIX UNITAIRE 12%
			rect = fm.boundingRect(mLeft-5+(wUtil*0.64),rect.top(), wUtil*0.12,0, Qt::AlignRight, m_lang.toString(ilist.price.at(pIndex),'f',2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , m_lang.toString(ilist.price.at(pIndex),'f',2) );

			//QUANTITE 12%
			rect = fm.boundingRect(mLeft-5+wUtil*0.76,rect.top(), wUtil*0.12,0, Qt::AlignRight, QString::number(ilist.quantity.at(pIndex)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect , Qt::AlignRight , QString::number(ilist.quantity.at(pIndex)));

			//TOTAL 12%
			totalPrice = ilist.price.at(pIndex)*ilist.quantity.at(pIndex);
			if(ilist.discount.at(pIndex)>0)totalPrice -= totalPrice*(ilist.discount.at(pIndex)/100.0);
			rect = fm.boundingRect(mLeft-5+(wUtil*0.88),rect.top(), wUtil*0.12,0, Qt::AlignRight,
								   m_lang.toString(totalPrice, 'f', 2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight ,
								   m_lang.toString(totalPrice, 'f', 2) );
			ListTotalPrice.push_back(totalPrice);

			itemPrinted++;
			itemOnpage++;

			pIndex++;
			//Nombre ditem max atteind?
			if( (itemOnpage - itemPerPage) >= 0) break;
		}

		//Information pied de page
		rect = fm.boundingRect(mLeft, pageRect.height() - mBottom, pageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, footerTextInfo );
		rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, footerTextInfo);

		//Num de page
		pageText = "- " + tr("Page ") + QString::number(page) /*+ '/' + QString::number(printer->copyCount())*/ + " -";
		rect = fm.boundingRect(mLeft, rect.top(), pageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		//rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, pageText);

		//Ligne
		//rect.translate( 0, -rect.height());
		painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + wUtil, rect.top()));

		m_DialogWaiting->setProgressBar(page);
		//New page ?
		if( (itemsToPrint - itemPrinted) > 0) printer->newPage();
	}

	//TOTAL
	totalPrice=0;
	for(unsigned i=0; i<ListTotalPrice.size(); i++)
		totalPrice += ListTotalPrice.at(i);
	rect = fm.boundingRect(mLeft+5+(wUtil*0.62),rectContent.bottom()+5, wUtil*0.36,0, Qt::AlignLeft, tr("TOTAL : ") );
	painter.drawText( rect, tr("TOTAL : "));
	font.setBold(true);
	painter.setFont(font);
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rectContent.bottom()+5, wUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice, 'f', 2) );
	font.setBold(false);
	painter.setFont(font);

	//ACOMPTE
	rect = fm.boundingRect(mLeft+5+(wUtil*0.62),rect.bottom()+5, wUtil*0.36,0, Qt::AlignLeft, tr("ACOMPTE : ") );
	painter.drawText( rect, tr("ACOMPTE : "));
	font.setBold(true);
	painter.setFont(font);
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rect.top(), wUtil*0.36,0, Qt::AlignRight, m_lang.toString(m_data->m_customer->m_invoice->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(m_data->m_customer->m_invoice->getPartPayment(), 'f', 2) );
	font.setBold(false);
	painter.setFont(font);

	//RESTE
	rect = fm.boundingRect(mLeft+5+(wUtil*0.62),rect.bottom()+10, wUtil*0.36,0, Qt::AlignLeft, tr("RESTE A PAYER : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect( QRect(mLeft+(wUtil*0.62),rect.top()-5, wUtil*0.36 +15,rect.height() +10), 5, 5 );
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("RESTE A PAYER : "));
	font.setBold(true);
	painter.setFont(font);
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rect.top(), wUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice-m_data->m_customer->m_invoice->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice-m_data->m_customer->m_invoice->getPartPayment(), 'f', 2) );
	font.setBold(false);
	painter.setFont(font);
	painter.setBrush( Qt::NoBrush );

	//TOTAL TAX
	QString text;
	if(m_data->getIsTax()){
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = fm.boundingRect(mLeft, rectContent.bottom()+5 , wUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Mode de reglement
	QString typePayment;
	QString typeP = m_data->m_customer->m_invoice->getTypePayment();
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == CASH)           typePayment = tr("Espece");
	if(typeP == CHECK)          typePayment = tr("Cheque");
	if(typeP == CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == INTERBANK)      typePayment = tr("TIP");
	if(typeP == TRANSFER)       typePayment = tr("Virement");
	if(typeP == DEBIT)          typePayment = tr("Prelevement");
	if(typeP == OTHER)          typePayment = tr("Autre");
	text = tr("Mode de r\350glement : ")+typePayment;
	rect = fm.boundingRect(mLeft,rect.bottom()+5, wUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == INTERBANK)||(typeP == DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de r\350glement.");
		rect = fm.boundingRect(mLeft, rect.bottom()+5, wUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	/// RIB
	if(typeP == TRANSFER){
		database::Bank mb;
		m_data->getBank(mb);
		text = "Relev\351 d'Itentit\351 Bancaire\n\n\n\n\n\n";
		rect = fm.boundingRect(mLeft+5, rect.bottom()+65, wUtil*0.36 +15, rect.height() +10, Qt::AlignCenter, text );
		painter.drawText(rect, text);
		painter.setPen( Qt::DashLine );
		painter.drawRoundedRect( QRect(mLeft,rect.top()-5, wUtil*0.36 +15, rect.height()), 5, 5 );
		painter.setPen( Qt::SolidLine );

		font.setPointSize(8);
		painter.setFont(font);
		text = tr("Code banque: ")+mb.codeBanque+"  "+tr("Code guichet: ")+mb.codeGuichet+'\n';
		text += tr("Compte: ")+mb.numCompte+"  "+tr("Cl\351 RIB: ")+mb.keyRIB+'\n';
		text += tr("Domiciliation: ");
		text += mb.address+"\n\n";
		text += tr("IBAN: ");
		text += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
		text += tr("BIC: ") + mb.codeBIC+'\n';
		rect = fm.boundingRect(mLeft+5, rect.top()+15, wUtil*0.36 +15, rect.height(), Qt::AlignLeft, text );
		rect.setWidth(wUtil*0.36); //fixe la largeur
		painter.drawText(rect, text);
	}

	delete m_DialogWaiting;
	painter.end();
}

/**
	Ouvre la fentre d edition sur un double clique
  */
void customerView::on_tableWidget_itemDoubleClicked(){
	on_toolBut_Edit_clicked();
}

void customerView::on_tableWidget_Services_itemDoubleClicked(){
	on_toolButton_editService_clicked();
}

void customerView::on_tableWidget_Proposals_itemDoubleClicked(){
	on_toolButton_editProposal_clicked();
}

void customerView::on_tableWidget_Invoices_itemDoubleClicked(){
	on_toolButton_editInvoice_clicked();
}

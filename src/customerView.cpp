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
#include "printc.h"
#include "table.h"

#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QPainter>
#include <QFileDialog>
#include <QClipboard>
#include <QChar>
#include <QCompleter>

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
	ui->comboBoxFiltre->addItem(tr("Prénoms"));
	ui->comboBoxFiltre->addItem(tr("Tel Fixe"));
	ui->comboBoxFiltre->addItem(tr("Mobile"));
	ui->comboBoxFiltre->addItem(tr("Ville"));
	
	m_custPage=1;
	m_custfilter = "";
	m_custfield = "LASTNAME ";
	ui->lineEdit_page->setText(QString::number(m_custPage));

	//afficher la liste des clients
	listCustomers(m_custPage);
	m_custId = 1;

	//Selectionne la tab fiche
	ui->tabWidget_Customer->setCurrentIndex(0);

	//Cache la suppression pour le moment trop de user supprime sans savoir
	/*ui->toolButton_Del->setVisible(false);
	ui->toolButton_delProposal->setVisible(false);
	ui->toolButton_delInvoice->setVisible(false);*/
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
								   tr("Ceci est fortement d&eacute;conseill&eacute; !<br>"
									  "Voulez-vous vraiment supprimer le client<br><br><b>")+
								   m_data->m_customer->getFirstName()+" - "+
								   m_data->m_customer->getLastName()+"</b> Ainsi que toutes ses donn&eacute;es (services, devis, factures) ?",
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

	listCustomersToTable(page, m_custfilter, m_custfield);
}

/**
 * @brief customerView::on_comboBoxFiltre_currentIndexChanged
 * @param index
 */
void customerView::on_comboBoxFiltre_currentIndexChanged(int index) {
	if(index == 0 ) m_custfield = "LASTNAME ";
	if(index == 1 ) m_custfield = "FIRSTNAME ";
	if(index == 2 ) m_custfield = "PHONENUMBER ";
	if(index == 3 ) m_custfield = "MOBILENUMBER ";
	if(index == 4 ) m_custfield = "CITY ";
}

/**
 * @brief customerView::on_lineEdit_Search_textChanged
 * @param arg1
 */
void customerView::on_lineEdit_Search_textChanged(const QString &arg1)
{
	if(m_data->isConnected()) {
		customer::CustomerList clist;
		//Recuperation des donnees presentent dans la bdd
		m_data->m_customer->getCustomerList(clist, 25, 0, arg1, m_custfield);
		QStringList wordList;
		if(m_custfield == "LASTNAME ")wordList = clist.lastName;
		if(m_custfield == "FIRSTNAME ")wordList = clist.firstName;
		if(m_custfield == "PHONENUMBER ")wordList = clist.phoneNumber;
		if(m_custfield == "MOBILENUMBER ")wordList = clist.mobileNumber;
		if(m_custfield == "CITY ")wordList = clist.city;
		
		//Supprimer les doublons
		wordList.removeDuplicates();
		QCompleter *completer = new QCompleter(wordList, this);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		ui->lineEdit_Search->setCompleter(completer);
	}
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
	titles << tr("Id") << tr("Noms") << tr("Prénoms");
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
	for(int i=0; i<clist.id.size(); i++){
		ItemOfTable *item_ID			= new ItemOfTable();
		ItemOfTable *item_LASTNAME	= new ItemOfTable();
		ItemOfTable *item_FIRSTNAME	= new ItemOfTable();

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
	ui->tableWidget_Proposals->setColumnCount(7);
	ui->tableWidget_Proposals->setColumnWidth(5,250);
#ifdef QT_NO_DEBUG
	ui->tableWidget_Proposals->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Proposals->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Proposals->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Proposals->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles  << tr("Id") << tr("Codes") << tr("Date") << tr("Prix") << tr("Prix TTC") << tr("Description")  << tr("Etat");
	ui->tableWidget_Proposals->setHorizontalHeaderLabels( titles );

	//Si pas de tva on cache la colonne
	if(!m_data->getIsTax()) {
		ui->tableWidget_Proposals->setColumnHidden(4 , true);
	}

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_proposal->getProposalList(ilist, m_data->m_customer->getId(), "TAB_PROPOSALS.DATE", filter, field);

	// list tous les devis
	for(int i=0; i<ilist.code.count(); i++){
		ItemOfTable *item_ID           = new ItemOfTable();
		ItemOfTable *item_CODE         = new ItemOfTable();
		ItemOfTable *item_DATE         = new ItemOfTable();
		ItemOfTable *item_PRICE        = new ItemOfTable();
		ItemOfTable *item_PRICE_TAX    = new ItemOfTable();
		ItemOfTable *item_DESCRIPTION  = new ItemOfTable();
		ItemOfTable *item_STATE        = new ItemOfTable();

		item_ID->setData(Qt::DisplayRole, ilist.id.at(i));
		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i));
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i)/*.toString(tr("dd/MM/yyyy"))*/ );
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
		item_PRICE_TAX->setData(Qt::DisplayRole, ilist.priceTax.at(i));
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
		ui->tableWidget_Proposals->setItem(i, 4, item_PRICE_TAX);
		ui->tableWidget_Proposals->setItem(i, 5, item_DESCRIPTION);
		ui->tableWidget_Proposals->setItem(i, 6, item_STATE);
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
	for(int i=0; i<ilist.id.size(); i++){
		ItemOfTable *item_ID		= new ItemOfTable();
		ItemOfTable *item_DATE		= new ItemOfTable();
		ItemOfTable *item_PRICE		= new ItemOfTable();
		ItemOfTable *item_NAME		= new ItemOfTable();

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
	ui->tableWidget_Invoices->setColumnCount(8);
	//ui->tableWidget_Invoices->setColumnWidth(3,125);
	ui->tableWidget_Invoices->setColumnWidth(6,250);
#ifdef QT_NO_DEBUG
	ui->tableWidget_Invoices->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Invoices->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Invoices->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Invoices->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles  << tr("Id") << tr("Codes") << tr("Date") << tr("Prix") << tr("PrixTTC") << tr("Acompte") << tr("Description")  << tr("Etat");
	ui->tableWidget_Invoices->setHorizontalHeaderLabels( titles );

	//Si pas de tva on cache la colonne
	if(!m_data->getIsTax()) {
		ui->tableWidget_Invoices->setColumnHidden(4 , true);
	}

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_invoice->getInvoiceList(ilist,m_data->m_customer->getId(), "TAB_INVOICES.DATE", filter, field);

	// list all customers
	for(int i=0; i<ilist.code.count(); i++){
		ItemOfTable *item_ID           = new ItemOfTable();
		ItemOfTable *item_CODE         = new ItemOfTable();
		ItemOfTable *item_DATE         = new ItemOfTable();
		ItemOfTable *item_PRICE        = new ItemOfTable();
		ItemOfTable *item_PRICE_TAX    = new ItemOfTable();
		ItemOfTable *item_PARTPAYMENT  = new ItemOfTable();
		ItemOfTable *item_DESCRIPTION  = new ItemOfTable();
		ItemOfTable *item_STATE        = new ItemOfTable();

		item_ID->setData(Qt::DisplayRole, ilist.id.at(i));
		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i));
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i)/*.toString(tr("dd/MM/yyyy"))*/);
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
		item_PRICE_TAX->setData(Qt::DisplayRole, ilist.priceTax.at(i));
		if(m_data->getIsTax()){
			item_PARTPAYMENT->setData(Qt::DisplayRole, ilist.part_paymentTax.at(i));
		}
		else {
			item_PARTPAYMENT->setData(Qt::DisplayRole, ilist.part_payment.at(i));
		}
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
		ui->tableWidget_Invoices->setItem(i, 4, item_PRICE_TAX);
		ui->tableWidget_Invoices->setItem(i, 5, item_PARTPAYMENT);
		ui->tableWidget_Invoices->setItem(i, 6, item_DESCRIPTION);
		ui->tableWidget_Invoices->setItem(i, 7, item_STATE);
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
									tr("Ceci est fortement d&eacute;conseill&eacute; !<br>"
									"Cela cr&eacute;era un trou dans <b>la num&eacute;rotation</b>.<br>"
									"<br>Voulez-vous vraiment supprimer le devis?<br><b>")+
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
									tr("Ceci est fortement d&eacute;conseill&eacute; !<br>"
									"Cela cr&eacute;era un trou dans <b>la num&eacute;rotation</b>.<br>"
									"<br>Voulez-vous vraiment supprimer la facture<br><b>")+
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
void customerView::on_toolButton_printServ_clicked() {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int index = ui->tableWidget_Services->currentRow();
	int Id = ui->tableWidget_Services->item(index, 0)->text().toInt();
		
	Printc print(m_data, m_lang);
	print.print_Service( Id );
}

/**
	Impression des propositions commerciales
  */
void customerView::on_toolButton_printPro_clicked() {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;
	
	int index = ui->tableWidget_Proposals->currentRow();
	int Id = ui->tableWidget_Proposals->item(index, 0)->text().toInt();
		
	Printc print(m_data, m_lang);
	print.print_Proposal( Id );
}

/**
	Impression des factures
  */
void customerView::on_toolButton_printInv_clicked() {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int index = ui->tableWidget_Invoices->currentRow();
	int Id = ui->tableWidget_Invoices->item(index, 0)->text().toInt();
	Printc print(m_data, m_lang);
	print.print_Invoice( Id );
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





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

#include "board.h"
#include "ui_board.h"

#include <QMessageBox>
#include "dialoginvoice.h"

/**
	Constructeur de la class board
*/
board::board(database *pdata, QLocale &lang, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::board){

	ui->setupUi(this);
	m_data = pdata;
	m_lang = lang;

	//Liste le tableau de bord
	listStockAlertToTable();
	listInvoiceAlertToTable();
	listProposalAlertToTable();
	listYear();
	calculYear();
	listRevenuesToTable();
}

/**
	Destructeur de la class board
*/
board::~board()
{
	delete ui;
}

void board::changeEvent(QEvent *e)
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
	Affiche les alertes du stock
*/
void board::listStockAlertToTable()
{
	product::ProductList plist;
	int state;
	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_stock->clear();
	for (int i=ui->tableWidget_stock->rowCount()-1; i >= 0; --i)
		ui->tableWidget_stock->removeRow(i);
	for (int i=ui->tableWidget_stock->columnCount()-1; i>=0; --i)
		ui->tableWidget_stock->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_stock->setSortingEnabled(false);
	//Style de la table produit
	ui->tableWidget_stock->setColumnCount(4);
	ui->tableWidget_stock->setColumnWidth(1,200);
	ui->tableWidget_stock->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_stock->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_stock->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Codes") << tr("Nom") << tr("Stock") << tr("Status");
	ui->tableWidget_stock->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_product->getProductListStockAlert(plist);

	// list all products
	for(int i=0,j=0; i<plist.code.count();i++){
		QTableWidgetItem *item_CODE      = new QTableWidgetItem();
		QTableWidgetItem *item_NAME      = new QTableWidgetItem();
		QTableWidgetItem *item_STOCK     = new QTableWidgetItem();
		QTableWidgetItem *item_STATE     = new QTableWidgetItem();

		item_CODE->setData(Qt::DisplayRole, plist.code.at(i) );
		item_NAME->setData(Qt::DisplayRole, plist.name.at(i));
		item_STOCK->setData(Qt::DisplayRole, plist.stock.at(i));

		if(plist.stock.at(i) <= plist.stockWarning.at(i)) state = 1;
		else  state = 0;

		item_STATE->setIcon( m_data->m_product->getIconState(state) );
		item_STATE->setText( m_data->m_product->getTextState(state) );

		//definir le tableau
		ui->tableWidget_stock->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_stock->setItem(j, 0, item_CODE);
		ui->tableWidget_stock->setItem(j, 1, item_NAME);
		ui->tableWidget_stock->setItem(j, 2, item_STOCK);
		ui->tableWidget_stock->setItem(j, 3, item_STATE);
		j++;
	}
	ui->tableWidget_stock->setSortingEnabled(true);
}


/**
	Affiche les alertes des facture
*/
void board::listInvoiceAlertToTable()
{
	invoice::InvoiceListAlert ilist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_InvoiceAlert->clear();
	for (int i=ui->tableWidget_InvoiceAlert->rowCount()-1; i >= 0; --i)
		ui->tableWidget_InvoiceAlert->removeRow(i);
	for (int i=ui->tableWidget_InvoiceAlert->columnCount()-1; i>=0; --i)
		ui->tableWidget_InvoiceAlert->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_InvoiceAlert->setSortingEnabled(false);
	//Style de la table
	ui->tableWidget_InvoiceAlert->setColumnCount(5);
	ui->tableWidget_InvoiceAlert->setColumnWidth(2,175); // description
	ui->tableWidget_InvoiceAlert->setColumnWidth(3,175); // Customer
	ui->tableWidget_InvoiceAlert->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_InvoiceAlert->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_InvoiceAlert->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Codes") << tr("Date") << tr("Description") << tr("Client") << tr("Status");
	ui->tableWidget_InvoiceAlert->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_invoice->getInvoiceListAlert(ilist);

	// list all products
	for(int i=0,j=0; i<ilist.code.count();i++){
		QTableWidgetItem *item_CODE      = new QTableWidgetItem();
		QTableWidgetItem *item_DATE     = new QTableWidgetItem();
		QTableWidgetItem *item_DESCRIPTION     = new QTableWidgetItem();
		QTableWidgetItem *item_CUSTOMER     = new QTableWidgetItem();
		QTableWidgetItem *item_STATE     = new QTableWidgetItem();

		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i) );
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i).toString(tr("dd/MM/yyyy")));
		item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));
		item_CUSTOMER->setData(Qt::DisplayRole, ilist.customerFirstName.at(i)+" "+ilist.customerLastName.at(i));
		// Si limite de paiment depasse alors on change licon
		if(ilist.limitPayment.at(i).addDays(1) > QDate::currentDate()){
			item_STATE->setIcon( m_data->m_customer->m_invoice->getIconState(ilist.state.at(i)) );
			item_STATE->setText( m_data->m_customer->m_invoice->getTextState(ilist.state.at(i)) );
		}
		else{
			item_STATE->setIcon( QIcon(":/app/quit") );
			item_STATE->setText( tr("Ech\351ance d\351pass\351e") );
		}
		//definir le tableau
		ui->tableWidget_InvoiceAlert->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_InvoiceAlert->setItem(j, 0, item_CODE);
		ui->tableWidget_InvoiceAlert->setItem(j, 1, item_DATE);
		ui->tableWidget_InvoiceAlert->setItem(j, 2, item_DESCRIPTION);
		ui->tableWidget_InvoiceAlert->setItem(j, 3, item_CUSTOMER);
		ui->tableWidget_InvoiceAlert->setItem(j, 4, item_STATE);
		j++;
	}
	ui->tableWidget_InvoiceAlert->setSortingEnabled(true);
}

/**
	Affiche les alertes des propositions commerciales
*/
void board::listProposalAlertToTable()
{
	proposal::ProposalListAlert list;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_ProposalAlert->clear();
	for (int i=ui->tableWidget_ProposalAlert->rowCount()-1; i >= 0; --i)
		ui->tableWidget_ProposalAlert->removeRow(i);
	for (int i=ui->tableWidget_ProposalAlert->columnCount()-1; i>=0; --i)
		ui->tableWidget_ProposalAlert->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_ProposalAlert->setSortingEnabled(false);
	//Style de la table
	ui->tableWidget_ProposalAlert->setColumnCount(4);
	ui->tableWidget_ProposalAlert->setColumnWidth(2,175); // description
	ui->tableWidget_ProposalAlert->setColumnWidth(3,175); // Customer
	ui->tableWidget_ProposalAlert->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_ProposalAlert->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_ProposalAlert->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Codes") << tr("Date") << tr("Description") << tr("Client");
	ui->tableWidget_ProposalAlert->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_proposal->getProposalListAlert(list);

	// list all products
	for(int i=0,j=0; i<list.code.count();i++){
		QTableWidgetItem *item_CODE      = new QTableWidgetItem();
		QTableWidgetItem *item_DATE     = new QTableWidgetItem();
		QTableWidgetItem *item_DESCRIPTION     = new QTableWidgetItem();
		QTableWidgetItem *item_CUSTOMER     = new QTableWidgetItem();

		item_CODE->setData(Qt::DisplayRole, list.code.at(i) );
		item_DATE->setData(Qt::DisplayRole, list.userDate.at(i).toString(tr("dd/MM/yyyy")));
		item_DESCRIPTION->setData(Qt::DisplayRole, list.description.at(i));
		item_CUSTOMER->setData(Qt::DisplayRole, list.customerFirstName.at(i)+" "+list.customerLastName.at(i));
		//definir le tableau
		ui->tableWidget_ProposalAlert->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_ProposalAlert->setItem(j, 0, item_CODE);
		ui->tableWidget_ProposalAlert->setItem(j, 1, item_DATE);
		ui->tableWidget_ProposalAlert->setItem(j, 2, item_DESCRIPTION);
		ui->tableWidget_ProposalAlert->setItem(j, 3, item_CUSTOMER);
		j++;
	}
	ui->tableWidget_InvoiceAlert->setSortingEnabled(true);
}


/**
	Affiche le CA
*/
void board::listRevenuesToTable()
{
	qreal monthServiceRevenue=0, monthProductRevenue=0;
	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_revenue->clear();
	for (int i=ui->tableWidget_revenue->rowCount()-1; i >= 0; --i)
		ui->tableWidget_revenue->removeRow(i);
	for (int i=ui->tableWidget_revenue->columnCount()-1; i>=0; --i)
		ui->tableWidget_revenue->removeColumn(i);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_revenue->setSortingEnabled(false);
	//Style de la table produit
	ui->tableWidget_revenue->setColumnCount(4);
	ui->tableWidget_revenue->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_revenue->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_revenue->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Mois") << tr("Service") << tr("Produit") << tr("Total");
	ui->tableWidget_revenue->setHorizontalHeaderLabels( titles );

	for(int i=1,j=0; i<13;i++){
		QTableWidgetItem *item_DATE     = new QTableWidgetItem();
		QTableWidgetItem *item_MonthRevenue  = new QTableWidgetItem();
		QTableWidgetItem *item_MonthServiceRevenue  = new QTableWidgetItem();
		QTableWidgetItem *item_MonthProductRevenue  = new QTableWidgetItem();

		item_DATE->setData(Qt::DisplayRole, QDate::longMonthName ( i,QDate::DateFormat) );

		monthServiceRevenue = m_data->m_customer->m_invoice->getMonthServiceRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		item_MonthServiceRevenue->setData(Qt::DisplayRole, monthServiceRevenue);

		monthProductRevenue = m_data->m_customer->m_invoice->getMonthProductRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		item_MonthProductRevenue->setData(Qt::DisplayRole, monthProductRevenue);

		item_MonthRevenue->setData(Qt::DisplayRole, monthServiceRevenue + monthProductRevenue);
		//definir le tableau
		ui->tableWidget_revenue->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_revenue->setItem(j, 0, item_DATE);
		ui->tableWidget_revenue->setItem(j, 1, item_MonthServiceRevenue);
		ui->tableWidget_revenue->setItem(j, 2, item_MonthProductRevenue);
		ui->tableWidget_revenue->setItem(j, 3, item_MonthRevenue);
		j++;
	}
}


/**
	Liste les annees des CA
  */
void board::listYear(){
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	//Recuperation des donnees presentent dans la bdd
	QStringList yearsList = m_data->m_customer->m_invoice->getYearsList();

	 ui->comboBox_yearsList->clear();
	for(int i=0; i<yearsList.count(); i++){
		ui->comboBox_yearsList->addItem( yearsList.at(i) );
	}
	//ui->comboBox_yearsList->setCurrentIndex(select);
}


/**
	calcul le CA de lannee selectionnee
  */
void board::calculYear(){
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	qreal tt = m_data->m_customer->m_invoice->getYearRevenue( ui->comboBox_yearsList->currentText() );
	ui->label_totalYear->setText( tr("<b>Total : ") + m_lang.toString(tt,'f',2) + tr(" &euro;</b>"));
}

/**
	rafraichit le calcul du CA Mois/Annee
  */
void board::on_comboBox_yearsList_currentIndexChanged() {
	calculYear();
	listRevenuesToTable();
}

/**
 * @brief board::on_tableWidget_InvoiceAlert_itemDoubleClicked
 * @param item
 */
void board::on_tableWidget_InvoiceAlert_itemDoubleClicked(QTableWidgetItem *item) {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QString code = ui->tableWidget_InvoiceAlert->item(item -> row(), 0) -> text();
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_invoice -> loadFromCode( code );

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::INVOICE_TYPE, DialogInvoice::EDIT);
	m_DialogInvoice -> setModal(true);
	m_DialogInvoice -> setTitle(tr("Modifier une facture"));
	m_DialogInvoice -> exec();

	delete m_DialogInvoice;
	//rafraichir la liste
	listInvoiceAlertToTable();
}

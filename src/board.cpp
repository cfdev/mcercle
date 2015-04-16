/**
  This file is a part of mcercle
  Copyright (C) 2010-2014 Cyril FRAUSTI

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
#include "dialoginvoice.h"
#include "table.h"



#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>

/**
	Constructeur de la class board
*/
board::board(database *pdata, QLocale &lang, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::board){

	ui->setupUi(this);
	m_data = pdata;
	m_lang = lang;

	// Creation graphique
	m_plot = new QCustomPlot(ui->tab);
	ui->verticalLayout_plot->addWidget(m_plot);
	m_caBar = new QCPBars(m_plot->xAxis, m_plot->yAxis);
	setupBarChart();

	//Liste le tableau de bord
	listStockAlertToTable();
	listInvoiceAlertToTable();
	listProposalAlertToTable();
	listYear();
	listRevenuesToTable();
}

/**
	Destructeur de la class board
*/
board::~board()
{
	delete ui;
}

/**
	Affiche les alertes du stock
*/
void board::listStockAlertToTable() {
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
		ItemOfTable *item_CODE      = new ItemOfTable();
		ItemOfTable *item_NAME      = new ItemOfTable();
		ItemOfTable *item_STOCK     = new ItemOfTable();
		ItemOfTable *item_STATE     = new ItemOfTable();

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
	ui->tableWidget_InvoiceAlert->setColumnCount(6);
	ui->tableWidget_InvoiceAlert->setColumnWidth(1,40); // id
	ui->tableWidget_InvoiceAlert->setColumnWidth(3,175); // description
	ui->tableWidget_InvoiceAlert->setColumnWidth(4,175); // Customer
	ui->tableWidget_InvoiceAlert->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_InvoiceAlert->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_InvoiceAlert->setEditTriggers(QAbstractItemView::NoEditTriggers);
#ifdef QT_NO_DEBUG
	ui->tableWidget_InvoiceAlert->setColumnHidden(1 , true); //cache la colonne ID ou DEBUG
#endif
	QStringList titles;
	titles.clear();
	titles << tr("Codes") << tr("C_Id") << tr("Date") << tr("Description") << tr("Client") << tr("Status");
	ui->tableWidget_InvoiceAlert->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_invoice->getInvoiceListAlert(ilist);
	
	// list all products
	for(int i=0,j=0; i<ilist.code.count();i++){
		ItemOfTable *item_CODE	= new ItemOfTable();
		ItemOfTable *item_C_Id	= new ItemOfTable();
		ItemOfTable *item_DATE	= new ItemOfTable();
		ItemOfTable *item_DESCRIPTION	= new ItemOfTable();
		ItemOfTable *item_CUSTOMER		= new ItemOfTable();
		ItemOfTable *item_STATE			= new ItemOfTable();

		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i) );
		item_C_Id->setData(Qt::DisplayRole, ilist.customerId.at(i) );
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i)/*.toString(tr("dd/MM/yyyy"))*/);
		item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));
		item_CUSTOMER->setData(Qt::DisplayRole, ilist.customerFirstName.at(i)+" "+ilist.customerLastName.at(i));
		// Si limite de paiment depasse alors on change licon
		if(ilist.limitPayment.at(i).addDays(1) > QDate::currentDate()){
			item_STATE->setIcon( m_data->m_customer->m_invoice->getIconState(ilist.state.at(i)) );
			item_STATE->setText( m_data->m_customer->m_invoice->getTextState(ilist.state.at(i)) );
		}
		else{
			item_STATE->setIcon( QIcon(":/app/quit") );
			item_STATE->setText( QLatin1String("Echéance dépassée") );
		}
		
		//definir le tableau
		ui->tableWidget_InvoiceAlert->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_InvoiceAlert->setItem(j, 0, item_CODE);
		ui->tableWidget_InvoiceAlert->setItem(j, 1, item_C_Id);
		ui->tableWidget_InvoiceAlert->setItem(j, 2, item_DATE);
		ui->tableWidget_InvoiceAlert->setItem(j, 3, item_DESCRIPTION);
		ui->tableWidget_InvoiceAlert->setItem(j, 4, item_CUSTOMER);
		ui->tableWidget_InvoiceAlert->setItem(j, 5, item_STATE);
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
	ui->tableWidget_ProposalAlert->setColumnCount(5);
	ui->tableWidget_ProposalAlert->setColumnWidth(1,40); // id
	ui->tableWidget_ProposalAlert->setColumnWidth(3,175); // description
	ui->tableWidget_ProposalAlert->setColumnWidth(4,175); // Customer
	ui->tableWidget_ProposalAlert->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_ProposalAlert->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_ProposalAlert->setEditTriggers(QAbstractItemView::NoEditTriggers);
#ifdef QT_NO_DEBUG
	ui->tableWidget_ProposalAlert->setColumnHidden(1 , true); //cache la colonne ID ou DEBUG
#endif
	QStringList titles;
	titles.clear();
	titles << tr("Codes") << tr("C_Id") << tr("Date") << tr("Description") << tr("Client");
	ui->tableWidget_ProposalAlert->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_data->m_customer->m_proposal->getProposalListAlert(list);

	// list all products
	for(int i=0,j=0; i<list.code.count();i++){
		ItemOfTable *item_CODE		= new ItemOfTable();
		ItemOfTable *item_C_Id		= new ItemOfTable();
		ItemOfTable *item_DATE		= new ItemOfTable();
		ItemOfTable *item_DESCRIPTION	= new ItemOfTable();
		ItemOfTable *item_CUSTOMER		= new ItemOfTable();

		item_CODE->setData(Qt::DisplayRole, list.code.at(i) );
		item_C_Id->setData(Qt::DisplayRole, list.customerId.at(i) );
		item_DATE->setData(Qt::DisplayRole, list.userDate.at(i)/*.toString(tr("dd/MM/yyyy"))*/);
		item_DESCRIPTION->setData(Qt::DisplayRole, list.description.at(i));
		item_CUSTOMER->setData(Qt::DisplayRole, list.customerFirstName.at(i)+" "+list.customerLastName.at(i));

		//definir le tableau
		ui->tableWidget_ProposalAlert->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_ProposalAlert->setItem(j, 0, item_CODE);
		ui->tableWidget_ProposalAlert->setItem(j, 1, item_C_Id);
		ui->tableWidget_ProposalAlert->setItem(j, 2, item_DATE);
		ui->tableWidget_ProposalAlert->setItem(j, 3, item_DESCRIPTION);
		ui->tableWidget_ProposalAlert->setItem(j, 4, item_CUSTOMER);
		j++;
	}
	ui->tableWidget_InvoiceAlert->setSortingEnabled(true);
}


/**
	Affiche le CA
*/
void board::listRevenuesToTable()
{
	/*QSqlQueryModel *mod = new QSqlQueryModel();
	QSqlQuery query;

	QString req = "SELECT DISTINCT TAB_INVOICES.CODE, TAB_INVOICES.ID, TAB_INVOICES.ID_REFERENCE, TAB_INVOICES.TYPE, TAB_INVOICES.PART_PAYMENT,"
		" ("
		" SELECT SUM(TAB_INVOICES_DETAILS.PRICE) FROM TAB_INVOICES_DETAILS"
		" WHERE TAB_INVOICES_DETAILS.ID_INVOICE = TAB_INVOICES.ID "
		" AND TAB_INVOICES_DETAILS.TYPE = 0 "
		" ),"
		" ("
		" SELECT SUM(TAB_INVOICES_DETAILS.PRICE) FROM TAB_INVOICES_DETAILS"
		" WHERE TAB_INVOICES_DETAILS.ID_INVOICE = TAB_INVOICES.ID "
		" AND TAB_INVOICES_DETAILS.TYPE = 1 "
		" )"
		" FROM TAB_INVOICES"
		" "
		" LEFT JOIN TAB_INVOICES_DETAILS ON TAB_INVOICES.ID = TAB_INVOICES_DETAILS.ID_INVOICE"
		" WHERE TAB_INVOICES.ID = TAB_INVOICES_DETAILS.ID_INVOICE"
		" AND TAB_INVOICES.STATE = '1'"
		" AND strftime('%m',TAB_INVOICES.PAYMENTDATE)='01'"
		" AND strftime('%Y',TAB_INVOICES.PAYMENTDATE)='2015';";

	query.prepare(req);

	if(query.exec()){
		mod -> setQuery(query);
		ui -> tableView -> setModel( mod );
	}*/

	/*qDebug()<< "=======================================================";
	qDebug()<< "Service: " << m_data->m_customer->m_invoice->calcul_partPaymentService(1);
	qDebug()<< "Produit: " << m_data->m_customer->m_invoice->calcul_partPaymentProduct(1);
	qDebug()<< "=======================================================";*/

	qreal monthServiceRevenue=0, monthProductRevenue=0, total=0;
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
		ItemOfTable *item_DATE				= new ItemOfTable();
		ItemOfTable *item_MonthRevenue		= new ItemOfTable();
		ItemOfTable *item_MonthServiceRevenue  = new ItemOfTable();
		ItemOfTable *item_MonthProductRevenue  = new ItemOfTable();

		item_DATE->setData(Qt::DisplayRole, QDate::longMonthName ( i,QDate::DateFormat) );

		monthServiceRevenue = m_data->m_customer->m_invoice->getMonthServiceRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		item_MonthServiceRevenue->setData(Qt::DisplayRole, m_lang.toString(monthServiceRevenue,'f',2) );

		monthProductRevenue = m_data->m_customer->m_invoice->getMonthProductRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		item_MonthProductRevenue->setData(Qt::DisplayRole, m_lang.toString(monthProductRevenue,'f',2) );

		total =  m_data->m_customer->m_invoice->getMonthRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		item_MonthRevenue->setData(Qt::DisplayRole, m_lang.toString(total,'f',2) );
		
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
void board::calculYear(QString year){
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	qreal price = m_data->m_customer->m_invoice->getYearRevenue( year );
	qreal ttc = m_data->m_customer->m_invoice->getYearRevenueTAX( year );
	QString txt = tr("<b>Total : ") + m_lang.toString(price,'f',2) + tr(" &euro;</b>");
	if( m_data->getIsTax() ){
		txt += " HT";
		txt += " ( "+m_lang.toString(ttc,'f',2)+" TTC) ";
	}
	ui -> label_totalYear -> setText(txt);
}

/**
	rafraichit le calcul du CA Mois/Annee
  */
void board::on_comboBox_yearsList_currentIndexChanged(QString txt) {
	calculYear( txt );
	listRevenuesToTable();
	setupBarChart();
}

/**
 * @brief board::on_tableWidget_InvoiceAlert_itemDoubleClicked
 * @param item
 */
void board::on_tableWidget_InvoiceAlert_itemDoubleClicked(QTableWidgetItem *item) {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QString codeFA = ui->tableWidget_InvoiceAlert->item(item -> row(), 0) -> text();
	int CustomerId = ui->tableWidget_InvoiceAlert->item(item -> row(), 1) -> text().toInt();
	
	m_data->m_customer->loadFromID(CustomerId);
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_invoice -> loadFromCode( codeFA );

	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::INVOICE_TYPE, DialogInvoice::EDIT);
	m_DialogInvoice -> setModal(true);
	m_DialogInvoice -> setTitle(tr("Modifier une facture"));
	m_DialogInvoice -> exec();

	delete m_DialogInvoice;
	//rafraichir la liste
	listInvoiceAlertToTable();
	//rafraichir le calcul du CA
	listRevenuesToTable();
	listYear();
}

/**
 * @brief board::on_tableWidget_ProposalAlert_itemDoubleClicked
 * @param item
 */
void board::on_tableWidget_ProposalAlert_itemDoubleClicked(QTableWidgetItem *item) {
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QString codePR = ui->tableWidget_ProposalAlert->item(item -> row(), 0) -> text();
	int CustomerId = ui->tableWidget_ProposalAlert->item(item -> row(), 1) -> text().toInt();
	
	m_data->m_customer->loadFromID(CustomerId);
	//On charge l objet en fonction de la selection
	m_data->m_customer->m_proposal -> loadFromCode( codePR );
	
	DialogInvoice *m_DialogInvoice = new DialogInvoice(m_lang, m_data, DialogInvoice::PROPOSAL_TYPE, DialogInvoice::EDIT);
	m_DialogInvoice->setModal(true);
	m_DialogInvoice->setTitle(tr("Modifier une proposition commerciale"));
	m_DialogInvoice->exec();

	delete m_DialogInvoice;
	//rafraichir la liste
	listProposalAlertToTable();
}


/**
 * @brief board::setupBarChart - affichage graphique du CA
 */
void board::setupBarChart() {
	//si un graphique existe deja
	if(m_plot->plottableCount()>1){
		m_plot->removePlottable(m_plot->plottable(1));
	}
	m_plot->setObjectName(QString::fromUtf8("m_plot"));
	m_plot->setBackground(Qt::white);
	// create empty bar chart objects:
	m_plot->addPlottable(m_caBar);
	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	m_caBar->setName("Chiffre d'affaire HT");
	pen.setColor(QColor(45, 125, 69));
	m_caBar->setPen(pen);
	m_caBar->setBrush(QColor(45, 125, 69, 50));
	m_caBar->removeFromLegend();

	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
	labels << "Jan" << "Fev" << "Mar" << "Avr" << "Mai" << "Jui" << "jui" << "Aou" << "Sep" << "Oct" << "Nov" << "Dec";
	m_plot->xAxis->setAutoTicks(false);
	m_plot->xAxis->setAutoTickLabels(false);
	m_plot->xAxis->setTickVector(ticks);
	m_plot->xAxis->setTickVectorLabels(labels);
	m_plot->xAxis->setTickLabelRotation(60);
	m_plot->xAxis->setSubTickCount(0);
	m_plot->xAxis->setTickLength(0, 4);
	m_plot->xAxis->grid()->setVisible(true);
	m_plot->xAxis->setRange(0, 13);

	// prepare y axis:
	m_plot->yAxis->setRange(0, 1000);
	m_plot->yAxis->setPadding(5); // a bit more space to the left border
	m_plot->yAxis->setLabel(QLatin1String("Chiffre d'affaire HT"));
	m_plot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	m_plot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	m_plot->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<qreal> CA;
	for(int i=1,j=0; i<13;i++){
		CA <<  m_data->m_customer->m_invoice->getMonthRevenue(ui->comboBox_yearsList->currentText(), QString::number(i));
		// Ajustage de l'échelle Y
		if(CA[j] > m_plot->yAxis->range().size())
			m_plot->yAxis->setRange(0, CA[j] + CA[j]*0.025);
		j++;
	}
	m_caBar->setData(ticks, CA);

	// setup legend:
	m_plot->legend->setVisible(false);
	/*m_plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	m_plot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	m_plot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	m_plot->legend->setFont(legendFont);
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);*/

	//Refresh
	m_plot->replot();
	m_plot->repaint();
	m_plot->rescaleAxes();
}

#include "dialoginvoicelist.h"
#include "ui_dialoginvoicelist.h"
#include "mcercle.h"
#include "dialogwaiting.h"
#include "dialogprintchoice.h"
#include "printc.h"
#include "table.h"

#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMenu>
#include <QDebug>

DialogInvoiceList::DialogInvoiceList(QLocale &lang, database *pdata, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogInvoiceList)
{
	ui -> setupUi(this);
	setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	m_data		= pdata;
	m_invoice	= pdata -> m_customer->m_invoice;
	m_isTax		= pdata -> getIsTax();
	m_lang		= lang;

	ui -> dateEdit -> setDate( QDate::currentDate());
}

DialogInvoiceList::~DialogInvoiceList() {
	delete ui;
}

/**
	Affiche les factures
	@param filter, filtre a appliquer
	@param field, champ ou appliquer le filtre
*/
void DialogInvoiceList::listInvoicesToTable(QDate mdate) {
	invoice::InvoicesBook ilist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_Invoices->clear();
	for (int i=ui->tableWidget_Invoices->rowCount()-1; i >= 0; --i)
		ui->tableWidget_Invoices->removeRow(i);
	for (int i=ui->tableWidget_Invoices->columnCount()-1; i>=0; --i)
		ui->tableWidget_Invoices->removeColumn(i);

	ui->tableWidget_Invoices->setSortingEnabled(false);
	//Style de la table de facture
	ui->tableWidget_Invoices->setColumnCount( COL_COUNT );
	ui->tableWidget_Invoices->setColumnWidth( COL_DESCRIPTION, 250 );
#ifdef QT_NO_DEBUG
	ui->tableWidget_Invoices->setColumnHidden(COL_ID , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_Invoices->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_Invoices->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_Invoices->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles  << tr("Id") << tr("selection") << tr("Date") << tr("Code Facture") << tr("Client") << tr("Description")  << tr("Montant") << tr("R\350glement");
	ui->tableWidget_Invoices->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_invoice->getInvoices(ilist, QString::number(mdate.year()), QString::number(mdate.month()));

	// list all customers
	QString typeP;
	for(int i=0; i<ilist.code.count(); i++){

		ItemOfTable *item_ID           = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_State        = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_DATE         = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_CODE         = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_CUSTOMER     = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_DESCRIPTION  = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_PRICE        = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_TYPE_PAYMENT = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		
		item_ID->setData(Qt::DisplayRole, ilist.id.at(i));
		item_State -> setData(Qt::CheckStateRole, Qt::Checked);
		item_DATE->setData(Qt::DisplayRole, ilist.userDate.at(i).toString(tr("dd/MM/yyyy")));
		item_CODE->setData(Qt::DisplayRole, ilist.code.at(i));
		if(ilist.customerFirstName.at(i).isEmpty())
			item_CUSTOMER->setData(Qt::DisplayRole, ilist.customerLastName.at(i));
		else
			item_CUSTOMER->setData(Qt::DisplayRole, ilist.customerFirstName.at(i) +" "+ilist.customerLastName.at(i));

		item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));
		item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));

		typeP="";
		if( ilist.typePayment.at(i) == MCERCLE::CASH)         typeP = tr("Espece");
		if( ilist.typePayment.at(i) == MCERCLE::CHECK)        typeP = tr("Cheque");
		if( ilist.typePayment.at(i) == MCERCLE::CREDIT_CARD)  typeP = tr("CB");
		if( ilist.typePayment.at(i) == MCERCLE::INTERBANK)    typeP = tr("TIP");
		if( ilist.typePayment.at(i) == MCERCLE::TRANSFER)     typeP = tr("Virement");
		if( ilist.typePayment.at(i) == MCERCLE::DEBIT)        typeP = tr("Prelevement");
		if( ilist.typePayment.at(i) == MCERCLE::OTHER)        typeP = tr("Autre");

		item_TYPE_PAYMENT->setData(Qt::DisplayRole, typeP);

		//definir le tableau
		ui->tableWidget_Invoices->setRowCount(i+1);

		//remplir les champs
		ui->tableWidget_Invoices->setItem(i, COL_ID, item_ID);
		ui->tableWidget_Invoices->setItem(i, COL_STATE, item_State);
		ui->tableWidget_Invoices->setItem(i, COL_DATE, item_DATE);
		ui->tableWidget_Invoices->setItem(i, COL_CODE, item_CODE);
		ui->tableWidget_Invoices->setItem(i, COL_CUSTOMER, item_CUSTOMER);
		ui->tableWidget_Invoices->setItem(i, COL_DESCRIPTION, item_DESCRIPTION);
		ui->tableWidget_Invoices->setItem(i, COL_PRICE, item_PRICE);
		ui->tableWidget_Invoices->setItem(i, COL_TYPE_PAYMENT, item_TYPE_PAYMENT);
	}
	ui->tableWidget_Invoices->setSortingEnabled(true);
	ui->tableWidget_Invoices->selectRow(0);
}

/**
	Sur le changement de Date on liste des factures
  */
void DialogInvoiceList::on_dateEdit_dateChanged(const QDate &date) {
	m_date = date;
	listInvoicesToTable(m_date);
}

/**
	Fermeture du dialog
  */
void DialogInvoiceList::on_pushButton_ok_clicked() {
	this->close();
}

/**
   Impression du livre des recettes
  */
void DialogInvoiceList::on_pushButton_print_clicked() {
	//Si on est pas connecte on sort
	if((!m_data->isConnected()) || (ui->tableWidget_Invoices->rowCount()<=0) )return;

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = tr("LIVRERECETTES-")+ m_date.toString(tr("yyyyMM")) ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinter(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}



/**
	Paint pour l apercu de l impression
  */
void DialogInvoiceList::on_paintPrinter(QPrinter *printer) {
	QPainter painter;
	painter.begin(printer);
   // int res = printer->resolution();
   // QRect paperRect = printer->paperRect();
	QRectF pageRect = printer->pageRect();
	QRectF rect;
   // double coeff = paperRect.width() / 21.0; // A4
	qreal mLeft, mTop, mRight, mBottom;
	printer->getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	qreal wUtil = pageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases

	QFontMetrics fm= painter.fontMetrics();
	QFont font = painter.font();
	painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait

	//Margins Debug
	//painter.drawRect(mLeft, mTop, pageRect.width() - (mLeft+mRight), pageRect.height() - (mTop+mBottom));

	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int itemPerPage;
	if(printer->orientation() == QPrinter::Landscape) itemPerPage = 18;
	else itemPerPage = 33;
	//Defini le nombre a imprimer
	int itemsToPrint = ui->tableWidget_Invoices->rowCount();
	if(itemsToPrint < itemPerPage )itemPerPage = itemsToPrint;

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
	/// Pied de page
	QString footerTextInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	footerTextInfo += "\n" + info.name;
	if(!info.capital.isEmpty()) footerTextInfo += " - " + tr("Capital ") + info.capital;
	if(!info.num.isEmpty())     footerTextInfo += " - " + tr("Siret ") + info.num;
	if(!m_data->getIsTax())     footerTextInfo += "\n" + tr("Dispensé d'immatriculation au registre du commerce et des société (RCS) et au répertoire des métiers (RM)");
	QString pageText;

	//defini la date de limpression
	QString sDateTime = tr("(Imprimé le ") + QDateTime::currentDateTime().toString(tr("dd-MM-yyyy HH:mm:ss")) + tr(")");
	
	// list all products
	for(int pIndex=0, page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		//Titre
		font.setPointSize(24);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect = QRect(mLeft, mTop, pageRect.width() - (mLeft+mRight),
						   fm.boundingRect( tr("Livre des recettes ")+m_date.toString(tr("MM-yyyy")) ).height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Livre des recettes ")+m_date.toString(tr("MM-yyyy")) );

		font.setPointSize(14);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 48);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( sDateTime ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, sDateTime );

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

		/// Contenu
		rect.translate( -5 , rect.height()+25 );

		/// Header
		//DATE
		rect = fm.boundingRect(mLeft+5,rect.top(), wUtil/8,0, Qt::AlignLeft, tr("Date") );
		painter.drawText( rect, tr("Date"));
		rect.setWidth(wUtil/8 -5); //fixe la largeur
		//CODE
		rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/6,0, Qt::AlignLeft, tr("Code")  );
		painter.drawText( rect, tr("Code") );
		rect.setWidth(wUtil/6 -5); //fixe la largeur
		//CLIENT
		rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/5,0, Qt::AlignLeft, tr("Client") );
		painter.drawText( rect, tr("Client"));
		rect.setWidth(wUtil/5 -5); //fixe la largeur
		//DESCIPTION
		rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/4,0, Qt::AlignLeft, tr("Description") );
		painter.drawText( rect, tr("Description") );
		rect.setWidth(wUtil/4 -5); //fixe la largeur
		//MONTANT
		rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/8,0, Qt::AlignLeft, tr("Montant") );
		painter.drawText( rect, tr("Montant") );
		rect.setWidth(wUtil/8 -5); //fixe la largeur
		//REGLEMENT
		rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/10,0, Qt::AlignLeft, tr("R\350glement") );
		painter.drawText( rect, tr("R\350glement") );
		rect.setWidth(wUtil/10 -5); //fixe la largeur

		for(int itemOnpage=0; itemOnpage<itemPerPage;){
			//sil ne reste plus a afficher on sort
			if(((ui->tableWidget_Invoices->rowCount()) - pIndex) <= 0)break;

				rect.translate( 0, rect.height()+5);
				//DATE
				rect = fm.boundingRect(mLeft+5,rect.top(), wUtil/8,0, Qt::AlignLeft, ui->tableWidget_Invoices->item(pIndex, COL_DATE)->text() );
				rect.setWidth(wUtil/8 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , ui->tableWidget_Invoices->item(pIndex, COL_DATE)->text());
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/8,rect.height()+5) );

				//CODE
				rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/6,0, Qt::AlignLeft, ui->tableWidget_Invoices->item(pIndex, COL_CODE)->text() );
				rect.setWidth(wUtil/6 -5); //fixe la largeur
				painter.drawText( rect , Qt::AlignLeft , ui->tableWidget_Invoices->item(pIndex, COL_CODE)->text());
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/6,rect.height()+5) );

				//CLIENT
				rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/5,0, Qt::AlignLeft, ui->tableWidget_Invoices->item(pIndex, COL_CUSTOMER)->text() );
				rect.setWidth(wUtil/5 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , ui->tableWidget_Invoices->item(pIndex, COL_CUSTOMER)->text());
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/5,rect.height()+5) );

				//DESCIPTION
				rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/4,0, Qt::AlignLeft, ui->tableWidget_Invoices->item(pIndex,COL_DESCRIPTION)->text() );
				rect.setWidth(wUtil/4 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , ui->tableWidget_Invoices->item(pIndex,COL_DESCRIPTION)->text() );
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/4,rect.height()+5) );

				//MONTANT
				rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/8,0, Qt::AlignLeft, m_lang.toString(ui->tableWidget_Invoices->item(pIndex,COL_PRICE)->text().toFloat(),'f',2) );
				rect.setWidth(wUtil/8 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , m_lang.toString(ui->tableWidget_Invoices->item(pIndex,COL_PRICE)->text().toFloat(),'f',2) );
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/8,rect.height()+5) );

				//REGLEMENT
				rect = fm.boundingRect(rect.right()+5,rect.top(), wUtil/10,0, Qt::AlignLeft, ui->tableWidget_Invoices->item(pIndex,COL_TYPE_PAYMENT)->text() );
				rect.setWidth(wUtil/10 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , ui->tableWidget_Invoices->item(pIndex,COL_TYPE_PAYMENT)->text());

				//Rectangle pour la ligne basse
				painter.drawRect( QRect(mLeft,rect.top()-5,wUtil,rect.height()+5) );

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
	delete m_DialogWaiting;
	painter.end();
}

/**
 * @brief DialogInvoiceList::on_toolButton_selAll_clicked
 */
void DialogInvoiceList::on_toolButton_selAll_clicked() {
	for(int i=0; i<ui -> tableWidget_Invoices -> rowCount();i++)
		ui -> tableWidget_Invoices -> item(i, COL_STATE)->setCheckState(Qt::Checked);
}

/**
 * @brief DialogInvoiceList::on_toolButton_unSel_clicked
 */
void DialogInvoiceList::on_toolButton_unSel_clicked() {
	for(int i=0; i<ui -> tableWidget_Invoices -> rowCount();i++)
		ui -> tableWidget_Invoices -> item(i, COL_STATE)->setCheckState(Qt::Unchecked);
}

/**
 * @brief get list of Diagrams is selected
 * @return this list of Diagrams
 */
QList<int> DialogInvoiceList::list_of_InvoiceSelected() {
	QList<int> listID;
	for(int i=0; i<ui -> tableWidget_Invoices -> rowCount();i++){
		if(ui -> tableWidget_Invoices -> item(i, COL_STATE)->checkState()){
			listID.push_back( ui -> tableWidget_Invoices -> item(i, COL_ID)->text().toInt() );
		}
	}
	return listID;
}

/**
 * @brief DialogInvoiceList::on_pushButton_printAll_clicked
 */
void DialogInvoiceList::on_pushButton_printAll_clicked() {
	QList <int>ListOfID = list_of_InvoiceSelected();
	if(ListOfID.count()>0) {
		Printc *mPrintc = new Printc(m_data,m_lang,this);
		mPrintc -> print_InvoicesList( ListOfID );
	}
}


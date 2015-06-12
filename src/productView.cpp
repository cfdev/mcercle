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

#include "productView.h"
#include "ui_productView.h"
#include "dialogproduct.h"
#include "dialogproviders.h"
#include "dialogwaiting.h"
#include "dialogprintchoice.h"
#include "table.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QClipboard>
#include <QCompleter>

productView::productView(database *pdata, QLocale &lang, unsigned char type, QWidget *parent) :
	QWidget(parent), ui(new Ui::productView)
{
	ui->setupUi(this);
	m_data = pdata;
	m_lang = lang;
	ui->comboBoxFiltre->addItem(tr("Codes"));
	ui->comboBoxFiltre->addItem(tr("Nom"));
	ui->comboBoxFiltre->addItem(tr("Catégorie"));
	//Ne pas afficher les produits hors vente
	m_ShowObsoleteProduct = false;
	//afficher la liste des produits
	m_prodPage=1;
	listProducts(m_prodPage);
	// Si cest une vue pour la selection de la facture on cache des elements
	if( type == INVOICE_VIEW){
		ui->toolButton_addProduct->hide();
		ui->toolButton_editProduct->hide();
		ui->toolButton_remove->hide();
		ui->toolButton_print->hide();
		ui->tabWidget->hide();

		delete ui->horizontalLayout_buttons; // ajuste la hauteur sinon perte de place
	}
	//champ recherche
	ui -> lineEdit_searchProduct -> setToolTip(
	tr("Le caractère « % » est un caractère joker qui remplace tous les autres caractères.\nExemple: %Poêle% - cherche le mot <Poêle>") );
}

productView::~productView() {
	delete ui;
}

void productView::changeEvent(QEvent *e) {
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
	Rafraichit la liste des produits
*/
void productView::refreshProductsList() {
	 listProductsToTable(m_prodPage, m_prodfilter, m_prodfield);
}

/**
	Afficher Tous les produits
*/
void productView::listProducts(int page) {
	//show all
	listProductsToTable(page, m_prodfilter, m_prodfield);
}


/**
	Trie les produits selon leur code
	@param string de filtrage
*/
void productView::listProductsFilter(int page, QString val) {
	if(!m_data->isConnected())return;
	//show with filter
	m_prodfilter = val;
	listProductsToTable(page, m_prodfilter, m_prodfield);
}


/**
	Affiche les produits
	@param ...
*/
void productView::listProductsToTable(int page, QString filter, QString field) {
	product::ProductList plist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_products->clear();
	for (int i=ui->tableWidget_products->rowCount()-1; i >= 0; --i)
		ui->tableWidget_products->removeRow(i);
	for (int i=ui->tableWidget_products->columnCount()-1; i>=0; --i)
		ui->tableWidget_products->removeColumn(i);

	//DEsactivation des boutons car aucun produit nest selectionne
	ui->toolButton_editProduct->setEnabled(false);
	ui->toolButton_remove->setEnabled(false);
	ui->toolButton_print->setEnabled(false);
	ui->tabWidget->setEnabled(false);

	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	ui->tableWidget_products->setSortingEnabled(false);
	//Style de la table produit
	ui->tableWidget_products->setColumnCount(7);
	ui->tableWidget_products->setColumnWidth(2,200);
#ifdef QT_NO_DEBUG
	ui->tableWidget_products->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget_products->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_products->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_products->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget_products->setAlternatingRowColors(true);
	QStringList titles;
	titles.clear();
	titles << tr("Id") << tr("Codes") << tr("Noms") << tr("Catégorie") << tr("Prix de vente") << tr("Tva") << tr("Stock") ;
	ui->tableWidget_products->setHorizontalHeaderLabels( titles );

	if( !m_data->getIsTax() ){
		ui->tableWidget_products->setColumnHidden(TAX_ROW , true); //cache la colonne TVA
	}
	
	//Cacul en fonction de la page
	int count = m_data->m_product->count(filter, field, m_ShowObsoleteProduct);
	m_prodNbPage = count/PRODUCTSMAX_PAGE +1;
	int first = PRODUCTSMAX_PAGE;
	int skip = (page-1) * PRODUCTSMAX_PAGE;
	ui->lineEdit_page->setText(QString::number(page));
	ui->label_pageMax->setText("/ "+ QString::number(m_prodNbPage));

	//Recuperation des donnees presentent dans la bdd
	m_data->m_product->getProductList(plist, first, skip, filter, field);

	// list all products
	for(int i=0,j=0; i<plist.id.size();i++){
		if((m_ShowObsoleteProduct)||(plist.state.at(i) == 1)){
			ItemOfTable *item_ID       = new ItemOfTable();
			ItemOfTable *item_CODE     = new ItemOfTable();
			ItemOfTable *item_NAME     = new ItemOfTable();
			ItemOfTable *item_STOCK    = new ItemOfTable();
			ItemOfTable *item_PRICE    = new ItemOfTable();
			ItemOfTable *item_TAX    = new ItemOfTable();
			ItemOfTable *item_CATEGORY = new ItemOfTable();

			item_ID->setData(Qt::DisplayRole, plist.id.at(i));
			item_CODE->setData(Qt::DisplayRole, plist.code.at(i) );
			item_NAME->setIcon( QIcon( QPixmap::fromImage(plist.img.at(i)) ) );
			item_NAME->setData(Qt::DisplayRole, plist.name.at(i));
			item_STOCK->setData(Qt::DisplayRole, plist.stock.at(i));
			item_PRICE->setData(Qt::DisplayRole, plist.selling_price.at(i));
			item_TAX->setData(Qt::DisplayRole, plist.tax.at(i));
			item_CATEGORY->setData(Qt::DisplayRole, plist.category.at(i));

			//couleur de fond pour les produits obsoletes
			if(plist.state.at(i) == 0){
				item_ID->setBackgroundColor(QColor(208,195,195,255));
				item_CODE->setBackgroundColor(QColor(208,195,195,255)); //( int r, int g, int b, int a = 255 )
				item_NAME->setBackgroundColor(QColor(208,195,195,255));
				item_STOCK->setBackgroundColor(QColor(208,195,195,255));
				item_PRICE->setBackgroundColor(QColor(208,195,195,255));
				item_TAX->setBackgroundColor(QColor(208,195,195,255));
				item_CATEGORY->setBackgroundColor(QColor(208,195,195,255));
			}else{
				//couleur de la categorie
				if( (plist.categoryColor.at(i).isValid()) && (plist.categoryColor.at(i)) != QColor(0, 0, 0, 255))
					item_CATEGORY->setBackgroundColor( plist.categoryColor.at(i) );
			}
			//definir le tableau
			ui->tableWidget_products->setRowCount(j+1);
			//remplir les champs
			ui->tableWidget_products->setItem(j, ID_ROW, item_ID);
			ui->tableWidget_products->setItem(j, CODE_ROW, item_CODE);
			ui->tableWidget_products->setItem(j, NAME_ROW, item_NAME);
			ui->tableWidget_products->setItem(j, CATEGORY_ROW, item_CATEGORY);
			ui->tableWidget_products->setItem(j, PRICE_ROW, item_PRICE);
			ui->tableWidget_products->setItem(j, TAX_ROW, item_TAX);
			ui->tableWidget_products->setItem(j, STOCK_ROW, item_STOCK);
			j++;
		}
	}
	ui->tableWidget_products->setSortingEnabled(true);
	ui->tableWidget_products->sortByColumn(NAME_ROW, Qt::AscendingOrder);

	//Nombre de produit
	ui->labelProductsNumber ->setText(tr("Nombre de produits : ")+QString::number(count));
}


/**
	Filtre la liste des clients en fonction du edit
*/
void productView::on_toolButton_searchProduct_clicked()
{
	//filtre avec retour sur la page 1
	m_prodPage=1;
	listProductsFilter( m_prodPage, ui->lineEdit_searchProduct->text() );
}


/**
	Filtre la liste des clients en fonction du edit sur pression du return
  */
void productView::on_lineEdit_searchProduct_returnPressed()
{
	//filtre avec retour sur la page 1
	m_prodPage=1;
	listProductsFilter( m_prodPage, ui->lineEdit_searchProduct->text() );
}

/**
	Ajouter un produit
*/
void productView::on_toolButton_addProduct_clicked()
{
	if(!m_data->isConnected())return;

	DialogProduct *m_DialogProduct = new DialogProduct(m_lang, m_data, PRODUCT_ADD);
	m_DialogProduct->setModal(true);
	m_DialogProduct->setTitle(tr("Ajouter un produit"));
	QObject::connect(m_DialogProduct, SIGNAL(askRefreshList()), this, SLOT(refreshProductsList()));
	m_DialogProduct->exec();

	delete m_DialogProduct;
}

/**
	Editer le produit
*/
void productView::on_toolButton_editProduct_clicked()
{
	if(!m_data->isConnected())return;

	DialogProduct *m_DialogProduct = new DialogProduct(m_lang, m_data, PRODUCT_EDIT);
	m_DialogProduct->setModal(true);
	m_DialogProduct->setTitle(tr("Modifier le produit"));
	m_DialogProduct->loadValuesFormProduct();
	QObject::connect(m_DialogProduct, SIGNAL(askRefreshList()), this, SLOT(refreshProductsList()));
	m_DialogProduct->exec();

	delete m_DialogProduct;
}



/**
	Supprimer le produit
  */
void productView::on_toolButton_remove_clicked()
{
	int ret = QMessageBox::warning(this, tr("Attention"),
								   tr("Voulez-vous vraiment supprimer le produit<br><b>")+
								   m_data->m_product->getCode()+" "+
								   m_data->m_product->getName()+"</b> de la liste ?",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		int stock = m_data->m_product->getStock();
		if(stock == 0) {
			m_data->m_product->remove();
			//list les produits
			listProductsToTable(m_prodPage, "","");
		}
		else{
			QMessageBox::information(this, tr("Information"),
									 tr("D&eacute;sol&eacute; le produit ne peut pas &ecirc;tre supprim&eacute;...<br>Il y en a encore <b>")
									 +QString::number(stock)
									 + tr("</b> en stock!"));
		}
	}
}


/**
	Acquerir toutes les Informations du produit selectionne
*/
QString productView::InfoProduct()
{
	QString Informations;
	Informations = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
				   "<span style=\" font-size:14pt; font-weight:600;\">"
				   +m_data->m_product->getName()+"</span></p>";

	Informations += m_data->m_product->getCode()+"<br>" + tr("Cat&eacute;gorie: ") + m_data->m_product->getCategory();
	Informations += "<br><p>"+ tr("Nombre en stock: ") + QString::number(m_data->m_product->getStock());
	Informations += "&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Seuil d&#146;alerte: ") + QString::number(m_data->m_product->getStockWarning())+"</p>";

	Informations += "<p>"+ tr("Prix d&#146;achat: ") + m_lang.toString(m_data->m_product->getBuyingPrice(),'f',2);
	Informations += "&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Prix de vente: ") + m_lang.toString(m_data->m_product->getSellingPrice(),'f',2);

	/*if(m_data->getIsTax())
		Informations += "<br>"+ tr("Tax: ") + m_lang.toString(m_data->m_product->getTax(),'f',2) +" &#37;</p>";
	else
		Informations += "</p>";
*/
	Informations += "<p>"+ tr("Etat: ");

	if( m_data->m_product->getState() == 1 ) Informations += tr("En vente") +"</p>";
	else Informations += tr("Abandonn&eacute;") +"</p>";

	Informations += "<br>"+ tr("Date de cr&#233;ation: ")+"<font color = blue>";
	Informations += m_data->m_product->getCreationDate().toString(tr("dd-MM-yyyy HH:mm:ss")) + " </font>";

	return Informations;
}

/**
	Acquerir toutes les Informations du fournisseur du produit
*/
QString productView::InfoProvider()
{
	QString Info;
	Info = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
				   "<span style=\" font-size:10pt; font-weight:600;\">Fournisseur</span></p>";

	Info += "<p><b>"+m_data->m_product->m_provider->getName()+"</b><br>";
	Info += m_data->m_product->m_provider->getContact()+"<br>";
	Info += tr("Tel: ")+m_data->m_product->m_provider->getPhoneNumber();
	Info += "&nbsp;&nbsp;&nbsp;&nbsp;"+tr(" Fax: ")+m_data->m_product->m_provider->getFaxNumber()+"<br>";
	Info += m_data->m_product->m_provider->getEmail()+"</p>";

	Info += tr("<p><b>Adresse:</b><br>");
	Info += m_data->m_product->m_provider->getAddress1()+"<br>";
	Info += m_data->m_product->m_provider->getAddress2()+"<br>";
	Info += m_data->m_product->m_provider->getAddress3()+"<br>";
	Info += m_data->m_product->m_provider->getZipCode()+" "+m_data->m_product->m_provider->getCity()+"<br>";
	Info += m_data->m_product->m_provider->getCountry()+"</p>";

	Info += tr("Date de cr&#233;ation: ")+"<font color = blue>";
	Info += m_data->m_product->m_provider->getCreationDate().toString(tr("dd-MM-yyyy HH:mm:ss")) + " </font>";

	return Info;
}

/**
	Event de la selection d'un produit
	Affiche toutes les informations produit dans la tab fiche
*/
void productView::on_tableWidget_products_itemSelectionChanged()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	int m_index = ui->tableWidget_products->currentRow();
	//Si index < 0 on sort
	if(m_index<0)return;

	//affiche les info produits
	bool ret = m_data->m_product->loadFromID( ui->tableWidget_products->item(m_index, ID_ROW)->text().toInt());

	if(ret) {
		QImage logo = m_data->m_product->getPicture();
		if((logo.size().height() > 256)||(logo.size().width() > 256)){
			logo = logo.scaled(QSize(256,256), Qt::KeepAspectRatio);
		}
		ui->label_image->setPixmap(QPixmap::fromImage(logo));
		ui->labelInfoProduct->setText( InfoProduct() );
		int id = m_data->m_product->getProviderID();
		ret = m_data->m_product->m_provider->loadFromID(id);
		if( (ret) && (!m_data->m_product->m_provider->getName().isEmpty()) ) ui->label_provider->setText( InfoProvider() );
		else    ui->label_provider->setText( tr("<font color = red>Fournisseur non disponible<br>Choissisez en un en modifiant le produit.</font>") );
		//Activation des boutons
		ui->toolButton_editProduct->setEnabled(true);
		ui->toolButton_remove->setEnabled(true);
		ui->toolButton_print->setEnabled(true);
		ui->tabWidget->setEnabled(true);
	}//erreur de chargement du produit
	else QMessageBox::critical(this, tr("Erreur"), tr("Impossible de charger le produit..."));
}

/**
	TODO: DEPLACER LE FILTRAGE DU PORDUIT OBSOLETE DANS LE BAS NIV SQL!!
	m_ShowObsoleteProduct
  */
void productView::on_checkBox_viewDiscontinued_clicked(bool checked) {
/**/
	m_ShowObsoleteProduct = checked;
	listProductsFilter( m_prodPage, ui->lineEdit_searchProduct->text() );
}


/**
	Copie les informations produit dans le presse papier
  */
void productView::on_toolButton_clipboard_clicked()
{
	//Si on est pas connecte on sort
   if(!m_data->isConnected())return;

   QTextDocumentFragment mt = QTextDocumentFragment::fromHtml ( InfoProduct() + "<br/><br/>" +InfoProvider() );
   QClipboard *m_clipboard = QApplication::clipboard();
   m_clipboard->setText( mt.toPlainText() );
}

/**
	Imprimer le listing stock des produits
  */
void productView::on_toolButton_print_clicked()
{
	//Si on est pas connecte on sort
	if(!m_data->isConnected())return;

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = "LISTESTOCK-"+ QDateTime::currentDateTime().toString(tr("yyMMdd")) ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterListingStock(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}

/**
	Paint pour l apercu de l impression de l etat du stock
  */
void productView::on_paintPrinterListingStock(QPrinter *printer)
{
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

	product::ProductList plist;
	//Recuperation des donnees presentent dans la bdd
	m_data->m_product->getProductList(plist, m_data->m_product->count("","",true),0, "", "");

	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int itemPerPage;
	if(printer->orientation() == QPrinter::Landscape) itemPerPage = 18;
	else itemPerPage = 33;
	//Defini le nombre a imprimer
	int itemsToPrint = m_data->m_product->count("","",m_ShowObsoleteProduct);
	if(itemsToPrint < itemPerPage )itemPerPage = itemsToPrint;

	int numberOfPage = itemsToPrint/itemPerPage;
	m_DialogWaiting->setProgressBarRange(0, numberOfPage);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	//Logo
	QImage logo = m_data->getLogoTable_informations();
	logo = logo.scaled(128,128,Qt::KeepAspectRatio);
	/// Info societe
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
						   fm.boundingRect( tr("Etat du stock produit") ).height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Etat du stock produit") );

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
		//CODE
		rect = fm.boundingRect(mLeft+5,rect.top(), wUtil/8.0,0, Qt::AlignLeft, tr("Codes") );
		painter.drawText( rect, tr("Codes"));
		//NOM
		rect = fm.boundingRect(mLeft+5+wUtil/8.0,rect.top(), wUtil/3.0,0, Qt::AlignLeft, tr("Noms")  );
		painter.drawText( rect, tr("Noms") );
		//CATEGORY
		rect = fm.boundingRect(mLeft+5+(wUtil/8)+(wUtil/3),rect.top(), wUtil/5.0,0, Qt::AlignLeft, tr("Catégories") );
		painter.drawText( rect, tr("Catégories"));
		//STOCK
		rect = fm.boundingRect(mLeft+5+(wUtil/8.0)+(wUtil/3.0)*2,rect.top(), wUtil/10.0,0, Qt::AlignLeft, tr("Stock") );
		painter.drawText( rect, tr("Stock") );
		//STOCK ALERT
		rect = fm.boundingRect(mLeft+5+(wUtil/8.0)+(wUtil/3.0)*2+(wUtil/10.0),rect.top(), wUtil/10.0,0, Qt::AlignLeft, tr("Stock Alerte") );
		painter.drawText( rect, tr("Stock Alerte") );

		for(int itemOnpage=0; itemOnpage<itemPerPage;){
			//sil ne reste plus a afficher on sort
			if((plist.code.count() - pIndex) <= 0)break;
			//si
			if((m_ShowObsoleteProduct)||(plist.state.at(pIndex) == 1)){
				rect.translate( 0, rect.height()+5);
				//CODE
				rect = fm.boundingRect(mLeft+5,rect.top(), wUtil/8,0, Qt::AlignLeft, plist.code.at(pIndex) );
				rect.setWidth(wUtil/8 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , plist.code.at(pIndex));
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/8,rect.height()+5) );

				//NOM
				rect = fm.boundingRect(mLeft+5+wUtil/8,rect.top(), wUtil/3,0, Qt::AlignLeft, plist.name.at(pIndex) );
				rect.setWidth(wUtil/3 -5); //fixe la largeur
				painter.drawText( rect , Qt::AlignLeft , plist.name.at(pIndex));
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/3,rect.height()+5) );

				//CATEGORY
				rect = fm.boundingRect(mLeft+5+(wUtil/8)+(wUtil/3),rect.top(), wUtil/5,0, Qt::AlignLeft, plist.category.at(pIndex) );
				rect.setWidth(wUtil/5 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , plist.category.at(pIndex));
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/3,rect.height()+5) );

				//STOCK
				rect = fm.boundingRect(mLeft+5+(wUtil/8)+(wUtil/3)*2,rect.top(), wUtil/10,0, Qt::AlignLeft, QString::number(plist.stock.at(pIndex)) );
				rect.setWidth(wUtil/10 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , QString::number(plist.stock.at(pIndex)) );
				painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/10,rect.height()+5) );

				//STOCK ALERT
				rect = fm.boundingRect(mLeft+5+(wUtil/8)+(wUtil/3)*2+(wUtil/10),rect.top(), wUtil/10,0, Qt::AlignLeft, QString::number(plist.stockWarning.at(pIndex)) );
				rect.setWidth(wUtil/10 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignLeft , QString::number(plist.stockWarning.at(pIndex)) );
				//painter.drawRect( QRect(rect.left()-5,rect.top()-5, wUtil/8,rect.height()+5) );

				//Rectangle pour la ligne basse
				painter.drawRect( QRect(mLeft,rect.top()-5,wUtil,rect.height()+5) );

				itemPrinted++;
				itemOnpage++;
			}
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
	Imprimer les informations produit
  */
void productView::on_toolButton_printInfo_clicked(){
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = "PROD-"+ m_data->m_product->getCode();
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterInfoProduct(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}

/**
	Paint pour l apercu de l impression Informations produit
  */
void productView::on_paintPrinterInfoProduct(QPrinter *printer) {
	QPainter painter;
	painter.begin(printer);
   // int res = printer->resolution();
   // QRect paperRect = printer->paperRect();
	QRectF pageRect = printer->pageRect();
   // double coeff = paperRect.width() / 21.0; // A4
	qreal mLeft, mTop, mRight, mBottom;
	printer->getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait
	QFontMetrics fm= painter.fontMetrics();
	QFont font = painter.font();
	qreal wUtil = pageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases

	//Margins Debug
	//painter.drawRect(mLeft, mTop, pageRect.width() - (mLeft+mRight), pageRect.height() - (mTop+mBottom));

	//Titre
	font.setPointSize(24);
	painter.setFont(font);
	fm= painter.fontMetrics();
	QRectF rect = QRectF(mLeft, mTop, pageRect.width() - (mLeft+mRight),
					   fm.boundingRect( tr("Fiche Produit") ).height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Fiche Produit") );

	font.setPointSize(14);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect.translate( 0, 32);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( fm.boundingRect( m_data->m_product->getName() ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, m_data->m_product->getName() );

	//Logo
	QImage logo = m_data->getLogoTable_informations();
	logo = logo.scaled(128,128,Qt::KeepAspectRatio);
	rect = QRect(mLeft+5, mTop, logo.width(), logo.height() );
	painter.drawImage(rect, logo);

	//Info societe
	database::Informations info;
	m_data->getInfo(info);
	font.setPointSize(10);
	painter.setFont(font);
	fm= painter.fontMetrics();
	QString textInfo = info.name + "\n";
	if(!info.phoneNumber.isEmpty()) textInfo += tr("Tel: ") + info.phoneNumber + "\n";
	if(!info.faxNumber.isEmpty())   textInfo += tr("Fax: ") + info.faxNumber + "\n";
	if(!info.email.isEmpty())       textInfo += tr("Email: ") + info.email + "\n";
	if(!info.webSite.isEmpty())     textInfo += tr("Web: ") + info.webSite;

	rect.translate( 0, rect.height()+5);
	rect = fm.boundingRect(mLeft+5,rect.top(), 0,0, Qt::AlignLeft, textInfo );
	painter.drawText( rect, textInfo);

	//Contenu
	QTextDocument textDocument;
	textDocument.setHtml(ui->labelInfoProduct->text()+"<br><br>"+ui->label_provider->text());
	rect.translate( -5 , rect.height()+75 );
	rect.setHeight( textDocument.size().height() );
	rect.setWidth( textDocument.size().width() );
	if( (rect.height()+ rect.top()) < pageRect.height() - mBottom){
		painter.translate( rect.topLeft() );
		textDocument.drawContents( &painter, rect.translated( -rect.topLeft() ) );
		painter.translate( -rect.topLeft() );
	}

	//Information pied de page
	textInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	textInfo += "\n" + info.name + " - " + tr("Capital ") + info.capital + " - " + tr("Siret ") + info.num;
	rect = fm.boundingRect(mLeft, pageRect.height() - mBottom, pageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, textInfo );
	rect.translate( 0, -rect.height());

	painter.drawText( rect, Qt::AlignCenter, textInfo);
	painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + wUtil, rect.top()));


	painter.end();
}

/**
	fonction sur la pression de la touche entree
	de la selection d une page.
  */
void productView::on_lineEdit_page_returnPressed()
{
	int p = ui->lineEdit_page->text().toInt();
	// Test des bornes min
	if(p < 1){
		m_prodPage = 1;
		ui->lineEdit_page->setText(QString::number(m_prodPage));
	} //Max
	else if(p > m_prodNbPage){
		m_prodPage = m_prodNbPage;
		ui->lineEdit_page->setText(QString::number(m_prodPage));
	}
	else m_prodPage = p;

	listProducts(m_prodPage);
}

/**
	Page precedente
  */
void productView::on_toolButton_pagePrev_clicked()
{
	//
	if(m_prodPage>1) listProducts(--m_prodPage);
}

/**
  Page suivante
  */
void productView::on_toolButton_pageNext_clicked()
{
	//
	if(m_prodPage<m_prodNbPage) listProducts(++m_prodPage);
}


/**
  Fonction pour savoir combien de ligne dans le tableau
*/
int productView::getRowCount(){
	return ui->tableWidget_products->rowCount();
}

/**
  avoir le nom du produit selectionne
*/
QString productView::getSelectedProductName(){
	int m_index_tab = ui->tableWidget_products->currentRow();
	 //Si index < 0 on sort
	if(m_index_tab<0)return "";
	else return ui->tableWidget_products->item(m_index_tab, NAME_ROW)->text();
}

/**
  avoir le prix du produit selectionne
*/
qreal productView::getSelectedProductPrice(){
	int m_index_tab = ui->tableWidget_products->currentRow();
	 //Si index < 0 on sort
	if(m_index_tab<0)return 0;
	else return ui->tableWidget_products->item(m_index_tab, PRICE_ROW)->text().toFloat();
}

/**
  avoir le prix du produit selectionne
*/
qreal productView::getSelectedProductTax(){
	int m_index_tab = ui->tableWidget_products->currentRow();
	 //Si index < 0 on sort
	if(m_index_tab<0)return 0;
	else return ui->tableWidget_products->item(m_index_tab, TAX_ROW)->text().toFloat();
}

/**
  avoir l ID du produit selectionne
*/
int productView::getSelectedProductID(){
	int m_index_tab = ui->tableWidget_products->currentRow();
	 //Si index < 0 on sort
	if(m_index_tab<0)return 0;
	else return ui->tableWidget_products->item(m_index_tab, ID_ROW)->text().toInt();
}

/**
	Ouvre la fentre d edition sur un double clique
  */
void productView::on_tableWidget_products_itemDoubleClicked(){
	on_toolButton_editProduct_clicked();
}

/**
	charge l index du combobox de recherche
  */
void productView::setIndexSearchProduct(int index){
	 ui->comboBoxFiltre->setCurrentIndex( index );
}

/**
	retourne l index du combobox de recherche
  */
int productView::getIndexSearchProduct(){
	return ui->comboBoxFiltre->currentIndex();
}


/**
	Selection du champ filtre
  */
void productView::on_comboBoxFiltre_currentIndexChanged(int index) {
	// TODO: VOIR POUR FAIRE PLUS PROPRE
	if(index == 0 )m_prodfield = "TAB_PRODUCTS.CODE ";
	if(index == 1 )m_prodfield = "TAB_PRODUCTS.NAME ";
	if(index == 2 )m_prodfield = "TAB_PRODUCTS_CATEGORIES.NAME ";
}

/**
	Autocompletion pour la recherche produit
  */
void productView::on_lineEdit_searchProduct_textChanged(const QString &arg1) {
	if(m_data->isConnected()) {
		product::ProductList plist;
		//Recuperation des donnees presentent dans la bdd
		m_data->m_product->getProductList(plist, 25, 0, arg1, m_prodfield);
		QStringList wordList;
		if(m_prodfield == "TAB_PRODUCTS.CODE ")wordList = plist.code;
		if(m_prodfield == "TAB_PRODUCTS.NAME ")wordList = plist.name;
		if(m_prodfield == "TAB_PRODUCTS_CATEGORIES.NAME ")wordList = plist.category;
		//Supprimer les doublons
		wordList.removeDuplicates();
		QCompleter *completer = new QCompleter(wordList, this);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		ui->lineEdit_searchProduct->setCompleter(completer);
	}
}

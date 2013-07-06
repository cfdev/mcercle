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

#include "printc.h"
#include "dialogprintchoice.h"
#include "dialogwaiting.h"

#include <QFileDialog>
#include <QDebug>

Printc::Printc(database *pdata, QLocale &lang, QObject *parent) :
	QObject(parent)
{
	m_lang = lang;
	m_data = pdata;
	m_cus  = pdata -> m_customer;
	m_inv  = pdata -> m_customer -> m_invoice;
	m_pro  = pdata -> m_customer -> m_proposal;
	m_serv = pdata -> m_customer -> m_service;	
	mlogo  = m_data -> getLogoTable_informations();
	
	///Info societe
	database::Informations info;
	m_data -> getInfo(info);
	mtextInfo = info.name + "\n";
	if(!info.phoneNumber.isEmpty()) mtextInfo += tr("Tel: ") + info.phoneNumber + "\n";
	if(!info.faxNumber.isEmpty())   mtextInfo += tr("Fax: ") + info.faxNumber + "\n";
	if(!info.email.isEmpty())       mtextInfo += tr("Email: ") + info.email + "\n";
	if(!info.webSite.isEmpty())     mtextInfo += tr("Web: ") + info.webSite;
	
	/// Identite du client
	mtextidentity = m_cus -> getFirstName()+" "+ m_cus -> getLastName()+'\n';
	mtextidentity += m_cus -> getAddress1()+'\n';
	mtextidentity += m_cus -> getAddress2()+'\n';
	mtextidentity += m_cus -> getAddress3()+'\n';
	mtextidentity += m_cus -> getZipCode()+" "+ m_cus -> getCity();
	
	/// Pied de page
	mfooterTextInfo = info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;
	mfooterTextInfo += "\n" + info.name;
	if(!info.capital.isEmpty())	mfooterTextInfo += " - " + tr("Capital ") + info.capital;
	if(!info.num.isEmpty())		mfooterTextInfo += " - " + tr("Siret ") + info.num;
	if(!m_data->getIsTax())		mfooterTextInfo += "\n" + tr("Dispens\351 d'immatriculation au registre du commerce et des soci\351t\351 (RCS) et au r\351pertoire des m\351tiers (RM)");
}

Printc::~Printc(){
}

/**
 * @brief Printc::load_parameters
 * @param p
 */
void Printc::load_parameters(QPrinter *printer, QPainter &painter) {
	printer -> getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	mpageRect = printer -> pageRect();
	
	if(printer->orientation() == QPrinter::Landscape) mBlockHeight = 200;
	else mBlockHeight = 500;
	mRectContent = QRect(mLeft, 0, mwUtil, mBlockHeight);
	
	mFont = painter.font();
	mwUtil = mpageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases
	//painter.setPen(QPen(Qt::black, 0.5)) ; //epaisseur du trait
}


/**
 * @brief Printc::print_Proposal
 * @param id
 */
void Printc::print_Proposal(const int &id) {
	//On charge l objet en fonction de la selection
	m_pro -> loadFromID(id);
	
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = m_pro -> getCode() ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice -> setModal(true);
	m_DialogPrintChoice -> exec();

	if(m_DialogPrintChoice -> result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterProposal(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		m_PreviewDialog.exec();
	}
}

/**
 * @brief Printc::print_header
 * @param painter
 * @return 
 */
QRectF Printc::print_header(QPainter &painter) {
	//Logo
	QRectF rect = QRect(mLeft+5, mTop, mlogo.width(), mlogo.height() );
	painter.drawImage(rect, mlogo);

	//Info societe
	mFont.setPointSize(10);
	painter.setFont(mFont);
	rect.translate( 0, rect.height()+5);
	rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top(), 0,0, Qt::AlignLeft, mtextInfo );
	painter.drawText( rect, mtextInfo);

	//Titre
	mFont.setPointSize(24);
	painter.setFont(mFont);
	rect = QRect(mLeft, mTop, mpageRect.width() - (mLeft+mRight),
						painter.fontMetrics().boundingRect( tr("Devis") ).height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Devis") );

	mFont.setPointSize(14);
	painter.setFont(mFont);
	rect.translate( 0, 32);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( painter.fontMetrics().boundingRect( tr("Code: ") + m_pro -> getCode() ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_pro -> getCode());

	mFont.setPointSize(12);
	painter.setFont(mFont);
	rect.translate( 0, 20);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( painter.fontMetrics().boundingRect( tr("Date: ")+m_pro -> getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
					  tr("Date: ")+
					  m_pro -> getUserDate().toString(tr("dd-MM-yyyy")) );
	rect.translate( 0, rect.height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
					  tr("Date fin validit\351: ")+
					  m_pro -> getValidDate().toString(tr("dd-MM-yyyy")) );

	/// Identite du client
	mFont.setPointSize(10);
	painter.setFont(mFont);
	//rect.translate( wUtil/2, rect.bottom());
	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+15, 0, 0, Qt::AlignLeft, tr("Client: ") );
	painter.drawText(rect, tr("Client: "));

	//rect.translate( 0, rect.height()+5);
	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+5, 0, 0, Qt::AlignLeft, mtextidentity );
	painter.drawText(rect, mtextidentity);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + mwUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	// Contour du contenu, dessine le rectangle avec 5 de radius
	rect.translate( 0, rect.height()+25);
	mRectContent.setTop( rect.top() );
	mRectContent.setHeight( mBlockHeight );
	painter.drawRoundedRect(mRectContent, 5, 5);
	
	return rect;
}

/**
 * @brief Printc::print_footer
 * @param painter
 */
QRectF Printc::print_footer(QPainter &painter) {
	//TODO: make print footer
}

/**
 * @brief Printc::on_paintPrinterProposal
 * @param printer
 */
void Printc::on_paintPrinterProposal(QPrinter *printer) {
	QPainter painter;
	painter.begin(printer);
	
	// charge les parametres d impression
	load_parameters(printer, painter);	
	
	//Recuperation des donnees presentent dans la bdd
	proposal::ProposalListItems plist;
	m_pro->getProposalItemsList(plist, "ITEM_ORDER", "", "");
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int linePerPage;
	if(printer->orientation() == QPrinter::Landscape) linePerPage = 4;
	else linePerPage = 16;
	//Defini le nombre a imprimer
	int itemsToPrint = plist.name.count();
	if(itemsToPrint < linePerPage )linePerPage = itemsToPrint;
	if(linePerPage== 0)linePerPage++;
	int numberOfPage = itemsToPrint/linePerPage;
	
	m_DialogWaiting->setProgressBarRange(0, numberOfPage);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	QRectF rect;
	//VAR Prix total
	qreal totalPrice=0;
	QList<float> ListTotalPrice;
	QString pageText;
	
	// list all products
	for(int pIndex=0, page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		/// Header
		rect = print_header(painter);

		///content header
		rect.translate( 0, 5);
		//DESIGNATION 40%
		rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, tr("D\351signation") );
		painter.drawText( rect, tr("D\351signation"));
		//TVA 12%
		if(m_data->getIsTax()){
			painter.drawLine(QPoint(mLeft+mwUtil*0.40, mRectContent.top()),
							 QPoint(mLeft+mwUtil*0.40, mRectContent.bottom()) );
			rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TVA %") );
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
			painter.drawLine(QPoint(mLeft+mwUtil*0.52, mRectContent.top()),
							 QPoint(mLeft+mwUtil*0.52, mRectContent.bottom()) );
			rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Remise %"));
			painter.drawText( rect, tr("Remise %"));
		}
		//PRIX UNITAIRE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.64, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.64, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Prix U. ")+ QChar(8364) );
		painter.drawText( rect, tr("Prix U. ")+ QChar(8364));
		//QUANTITE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.76, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.76, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Quantit\351")  );
		painter.drawText( rect, tr("Quantit\351") );
		//TOTAL 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.88, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.88, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TOTAL ")+ QChar(8364) );
		painter.drawText( rect, tr("TOTAL ")+ QChar(8364) );
		painter.drawLine(QPoint(mRectContent.left(), rect.bottom()+5),
						 QPoint(mRectContent.right(),rect.bottom()+5));

		rect.translate( 0, 5);
		for(int itemOnpage=0; itemOnpage<linePerPage;){
			//sil ne reste plus a afficher on sort
			if((plist.name.count() - pIndex) <= 0)break;
			rect.translate( 0, rect.height()+5);

			//DESIGNATION 40%
			rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, plist.name.at(pIndex) );
			rect.setWidth(mwUtil*0.50); //fixe la largeur
			painter.drawText( rect,  Qt::AlignLeft , plist.name.at(pIndex));

			//TVA 12%
			if(m_data->getIsTax()){
				rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(plist.tax.at(pIndex),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(plist.tax.at(pIndex),'f',2) );
			}

			//REMISE 12%
			if(discount){
				rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(plist.discount.at(pIndex)) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , QString::number(plist.discount.at(pIndex)) );
			}

			//PRIX UNITAIRE 12%
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(plist.price.at(pIndex),'f',2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , m_lang.toString(plist.price.at(pIndex),'f',2) );

			//QUANTITE 12%
			rect = painter.fontMetrics().boundingRect(mLeft-5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(plist.quantity.at(pIndex)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect , Qt::AlignRight , QString::number(plist.quantity.at(pIndex)));

			//TOTAL 12%
			totalPrice = plist.price.at(pIndex)*plist.quantity.at(pIndex);
			if(plist.discount.at(pIndex)>0) totalPrice -= totalPrice*(plist.discount.at(pIndex)/100.0);
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignRight,
								   m_lang.toString(totalPrice, 'f', 2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight ,
								   m_lang.toString(totalPrice, 'f', 2) );
			ListTotalPrice.push_back(totalPrice);

			itemPrinted++;
			itemOnpage++;

			pIndex++;
			//Nombre ditem max atteind?
			if( (itemOnpage - linePerPage) >= 0) break;
		}

		//Information pied de page
		rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, mfooterTextInfo );
		rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, mfooterTextInfo);

		//Num de page
		pageText = "- " + tr("Page ") + QString::number(page) + " -";
		rect = painter.fontMetrics().boundingRect(mLeft, rect.top(), mpageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		//rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, pageText);

		//Ligne
		//rect.translate( 0, -rect.height());
		painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + mwUtil, rect.top()));
		m_DialogWaiting->setProgressBar(page);

		//New page ?
		if( (itemsToPrint - itemPrinted) > 0) printer->newPage();
	}

	//TOTAL
	totalPrice=0;
	for(int i=0; i<ListTotalPrice.size(); i++)
		totalPrice += ListTotalPrice.at(i);
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),mRectContent.bottom()+15, mwUtil*0.36,0, Qt::AlignLeft, tr("TOTAL : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62),rect.top()-5, mwUtil*0.36 +15, rect.height() +10), 5, 5 );
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("TOTAL : "));

	//mFont = painter.font();
	mFont.setBold(true);
	painter.setFont( mFont );
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),mRectContent.bottom()+15, mwUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice, 'f', 2) );

	mFont.setBold(false);
	painter.setFont( mFont );
	painter.setBrush( Qt::NoBrush );

	//TOTAL TAX
	QString text;
	if(m_data->getIsTax()){
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = painter.fontMetrics().boundingRect(mLeft, mRectContent.bottom()+5 , mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Mode de reglement
	QString typePayment;
	QString typeP = m_pro->getTypePayment();
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == CASH)           typePayment = tr("Espece");
	if(typeP == CHECK)          typePayment = tr("Cheque");
	if(typeP == CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == INTERBANK)      typePayment = tr("TIP");
	if(typeP == TRANSFER)       typePayment = tr("Virement");
	if(typeP == DEBIT)          typePayment = tr("Prelevement");
	if(typeP == OTHER)          typePayment = tr("Autre");
	text = tr("Mode de r\350glement : ")+typePayment;
	rect = painter.fontMetrics().boundingRect(mLeft,rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == INTERBANK)||(typeP == DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de r\350glement.");
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Condition de reglement
	text = tr("Conditions de r\350glement: 30% du montant total lors\nde la signature de cette proposition soit: ");
	text += m_lang.toString(totalPrice * 0.3, 'f', 2) +" "+ QChar(8364);
	rect = painter.fontMetrics().boundingRect(mLeft,rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text);
	painter.drawText( rect, text);

	/// Signature Client
	text = "Signature client:\n\n\n\n\n\n";
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62)+5, rect.bottom()+15, 0, 0, Qt::AlignLeft, text );
	painter.drawText(rect, text);
	painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62),rect.top()-5, mwUtil*0.36 +15, rect.height() +10), 5, 5 );

	/// RIB
	if(typeP == TRANSFER){
		database::Bank mb;
		m_data->getBank(mb);
		text = "Relev\351 d'Itentit\351 Bancaire\n\n\n\n\n\n";
		rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top(), mwUtil*0.36 +15, rect.height() +10, Qt::AlignCenter, text );
		painter.drawText(rect, text);
		painter.setPen( Qt::DashLine );
		painter.drawRoundedRect( QRect(mLeft,rect.top()-5, mwUtil*0.36 +15, rect.height()), 5, 5 );
		painter.setPen( Qt::SolidLine );

		mFont.setPointSize(8);
		painter.setFont(mFont);
		text = tr("Code banque: ")+mb.codeBanque+"  "+tr("Code guichet: ")+mb.codeGuichet+'\n';
		text += tr("Compte: ")+mb.numCompte+"  "+tr("Cl\351 RIB: ")+mb.keyRIB+'\n';
		text += tr("Domiciliation: ");
		text += mb.address+"\n\n";
		text += tr("IBAN: ");
		text += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
		text += tr("BIC: ") + mb.codeBIC+'\n';
		rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top()+15, mwUtil*0.36 +15, rect.height(), Qt::AlignLeft, text );
		rect.setWidth(mwUtil*0.36); //fixe la largeur
		painter.drawText(rect, text);
	}

	delete m_DialogWaiting;
	painter.end();
}


/**
 * @brief Printc::print_Invoice
 * @param id
 */
void Printc::print_Invoice(const int &id) {
	//On charge l objet en fonction de la selection
	m_inv->loadFromID(id);

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = m_inv->getCode() ;
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


/**
 * @brief Printc::on_paintPrinterInvoice
 * @param printer
 */
void Printc::on_paintPrinterInvoice(QPrinter *printer) {
	QPainter painter;
	painter.begin(printer);
	// charge les parametres d impression
	load_parameters(printer, painter);	

	invoice::InvoiceListItems ilist;
	//Recuperation des donnees presentent dans la bdd
	m_inv->getInvoiceItemsList(ilist, "ITEM_ORDER", "", "");
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	//Defini le nombre de produit par page
	int linePerPage;
	if(printer->orientation() == QPrinter::Landscape) linePerPage = 8;
	else linePerPage = 20;
	//Defini le nombre a imprimer
	int itemsToPrint = ilist.name.count();
	if(itemsToPrint < linePerPage )linePerPage = itemsToPrint;
	if(linePerPage== 0)linePerPage++;
	int numberOfPage = itemsToPrint/linePerPage;
	
	m_DialogWaiting->setProgressBarRange(0, numberOfPage);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	QRectF rect;
	qreal totalPrice=0;
	QList<float> ListTotalPrice;
	QString pageText;

	// list all products
	for(int pIndex=0, page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		/// Header
		rect = print_header(painter);
		/// Content
		rect.translate( 0, 5);
		//DESIGNATION 40%
		rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, tr("D\351signation") );
		painter.drawText( rect, tr("D\351signation"));
		//TVA 12%
		if(m_data->getIsTax()){
			painter.drawLine(QPoint(mLeft+mwUtil*0.40, mRectContent.top()),
							 QPoint(mLeft+mwUtil*0.40, mRectContent.bottom()) );
			rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TVA %") );
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
			painter.drawLine(QPoint(mLeft+mwUtil*0.52, mRectContent.top()),
							 QPoint(mLeft+mwUtil*0.52, mRectContent.bottom()) );
			rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Remise %"));
			painter.drawText( rect, tr("Remise %"));
		}
		//PRIX UNITAIRE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.64, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.64, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Prix U. ")+ QChar(8364) );
		painter.drawText( rect, tr("Prix U. ")+ QChar(8364));
		//QUANTITE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.76, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.76, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Quantit\351")  );
		painter.drawText( rect, tr("Quantit\351") );
		//TOTAL 12%
		painter.drawLine(QPoint(mLeft+mwUtil*0.88, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.88, mRectContent.bottom()) );
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TOTAL ")+ QChar(8364) );
		painter.drawText( rect, tr("TOTAL ")+ QChar(8364) );
		painter.drawLine(QPoint(mRectContent.left(), rect.bottom()+5),
						 QPoint(mRectContent.right(),rect.bottom()+5));

		rect.translate( 0, 5);
		for(int itemOnpage=0; itemOnpage<linePerPage;){
			//sil ne reste plus a afficher on sort
			if((ilist.name.count() - pIndex) <= 0)break;
            rect.translate( 0, rect.height()+5);

            if (pIndex>0){
                QStringList lines = ilist.name.at(pIndex-1).split("\n");
                lines.count();
                rect.translate( 0, (lines.count()-1)*rect.height());
            }

			//DESIGNATION 40%
			rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, ilist.name.at(pIndex) );
            rect.setWidth(mwUtil*0.50); //fixe la largeur
			painter.drawText( rect,  Qt::AlignLeft , ilist.name.at(pIndex));

			//TVA 12%
			if(m_data->getIsTax()){
				rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(ilist.tax.at(pIndex),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(ilist.tax.at(pIndex),'f',2) );
            }

			//REMISE 12%
			if(discount){
				rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(ilist.discount.at(pIndex)) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , QString::number(ilist.discount.at(pIndex)) );
			}

			//PRIX UNITAIRE 12%
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(ilist.price.at(pIndex),'f',2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , m_lang.toString(ilist.price.at(pIndex),'f',2) );

			//QUANTITE 12%
			rect = painter.fontMetrics().boundingRect(mLeft-5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(ilist.quantity.at(pIndex)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect , Qt::AlignRight , QString::number(ilist.quantity.at(pIndex)));

			//TOTAL 12%
			totalPrice = ilist.price.at(pIndex)*ilist.quantity.at(pIndex);
			if(ilist.discount.at(pIndex)>0)totalPrice -= totalPrice*(ilist.discount.at(pIndex)/100.0);
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignRight,
								   m_lang.toString(totalPrice, 'f', 2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight ,
								   m_lang.toString(totalPrice, 'f', 2) );
			ListTotalPrice.push_back(totalPrice);

			itemPrinted++;
			itemOnpage++;

			pIndex++;
			//Nombre ditem max atteind?
			if( (itemOnpage - linePerPage) >= 0) break;
		}

		//Information pied de page
		rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, mfooterTextInfo );
		rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, mfooterTextInfo);

		//Num de page
		pageText = "- " + tr("Page ") + QString::number(page) /*+ '/' + QString::number(printer->copyCount())*/ + " -";
		rect = painter.fontMetrics().boundingRect(mLeft, rect.top(), mpageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		//rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, pageText);

		//Ligne
		//rect.translate( 0, -rect.height());
		painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + mwUtil, rect.top()));

		m_DialogWaiting->setProgressBar(page);
		//New page ?
		if( (itemsToPrint - itemPrinted) > 0) printer->newPage();
	}

	//TOTAL
	totalPrice=0;
	for(int i=0; i<ListTotalPrice.size(); i++)
		totalPrice += ListTotalPrice.at(i);
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),mRectContent.bottom()+5, mwUtil*0.36,0, Qt::AlignLeft, tr("TOTAL : ") );
	painter.drawText( rect, tr("TOTAL : "));
	mFont.setBold(true);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),mRectContent.bottom()+5, mwUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice, 'f', 2) );
	mFont.setBold(false);
	painter.setFont(mFont);

	//ACOMPTE
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+5, mwUtil*0.36,0, Qt::AlignLeft, tr("ACOMPTE : ") );
	painter.drawText( rect, tr("ACOMPTE : "));
	mFont.setBold(true);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*0.36,0, Qt::AlignRight, m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
	mFont.setBold(false);
	painter.setFont(mFont);

	//RESTE
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+10, mwUtil*0.36,0, Qt::AlignLeft, tr("RESTE A PAYER : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62),rect.top()-5, mwUtil*0.36 +15,rect.height() +10), 5, 5 );
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("RESTE A PAYER : "));
	mFont.setBold(true);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice-m_inv->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice-m_inv->getPartPayment(), 'f', 2) );
	mFont.setBold(false);
	painter.setFont(mFont);
	painter.setBrush( Qt::NoBrush );

	//TOTAL TAX
	QString text;
	if(m_data->getIsTax()){
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = painter.fontMetrics().boundingRect(mLeft, mRectContent.bottom()+5 , mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Mode de reglement
	QString typePayment;
	QString typeP = m_inv->getTypePayment();
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == CASH)           typePayment = tr("Espece");
	if(typeP == CHECK)          typePayment = tr("Cheque");
	if(typeP == CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == INTERBANK)      typePayment = tr("TIP");
	if(typeP == TRANSFER)       typePayment = tr("Virement");
	if(typeP == DEBIT)          typePayment = tr("Prelevement");
	if(typeP == OTHER)          typePayment = tr("Autre");
	text = tr("Mode de r\350glement : ")+typePayment;
	rect = painter.fontMetrics().boundingRect(mLeft,rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == INTERBANK)||(typeP == DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de r\350glement.");
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	/// RIB
	if(typeP == TRANSFER){
		database::Bank mb;
		m_data->getBank(mb);
		text = "Relev\351 d'Itentit\351 Bancaire\n\n\n\n\n\n";
		rect = painter.fontMetrics().boundingRect(mLeft+5, rect.bottom()+65, mwUtil*0.36 +15, rect.height() +10, Qt::AlignCenter, text );
		painter.drawText(rect, text);
		painter.setPen( Qt::DashLine );
		painter.drawRoundedRect( QRect(mLeft,rect.top()-5, mwUtil*0.36 +15, rect.height()), 5, 5 );
		painter.setPen( Qt::SolidLine );

		mFont.setPointSize(8);
		painter.setFont(mFont);
		text = tr("Code banque: ")+mb.codeBanque+"  "+tr("Code guichet: ")+mb.codeGuichet+'\n';
		text += tr("Compte: ")+mb.numCompte+"  "+tr("Cl\351 RIB: ")+mb.keyRIB+'\n';
		text += tr("Domiciliation: ");
		text += mb.address+"\n\n";
		text += tr("IBAN: ");
		text += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
		text += tr("BIC: ") + mb.codeBIC+'\n';
		rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top()+15, mwUtil*0.36 +15, rect.height(), Qt::AlignLeft, text );
		rect.setWidth(mwUtil*0.36); //fixe la largeur
		painter.drawText(rect, text);
	}

	delete m_DialogWaiting;
	painter.end();
}



/**
 * @brief Printc::print_Proposal
 * @param id
 */
void Printc::print_Service(const int &id) {
	//On charge l objet en fonction de la selection
	m_serv->loadFromID(id);
	
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = "SR"  + m_serv->getDate().toString("yyMMdd") +"-"+ QString::number(m_serv->getId()) ;
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


/**
 * @brief Printc::on_paintPrinterService
 * @param printer
 */
void Printc::on_paintPrinterService(QPrinter *printer) {
	QPainter painter;
	painter.begin(printer);
	// charge les parametres d impression
	load_parameters(printer, painter);	
	
	/// Header
	QRectF rect = print_header(painter);
	/// contenu
	//contour du contenu
	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 450;

	QString serv = tr("Intervention: ") + m_serv->getName()+"\n";
	serv += tr("R\351alis\351 le : ") + m_serv->getDate().toString(tr("dd-MM-yyyy"))+ tr(" \xE0 ")+ m_serv->getDate().toString(tr("HH:mm")) + "\n\n";
	serv += tr("D\351tails:\n");
	serv += m_serv->getDescription();
	serv += " \n";
	// Attention ladaptation avec le Qt::TextWordWrap ne fonctionne qui si il y a des espaces dans la chaine !!
	rect = painter.fontMetrics().boundingRect(mLeft+5,rect.bottom()+40,
						mwUtil-5 ,0,
						Qt::TextWordWrap | Qt::AlignLeft,
						serv );

	// Si la hauteur depasse le rectangle on fixe sa taille et tant pis on perd du text...
	if(rect.height() > blockHeight)rect.setHeight(blockHeight-10);
	painter.drawText( rect, Qt::TextWordWrap, serv);

	rect = QRect(mLeft, rect.top()-10, mwUtil, blockHeight);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	/// Prix
   /* QString price = tr("Prix: ") + QString::number(m_cus->m_service->getPrice())+ QChar(8364);
	if(m_data->getIsTax()){
		price += tr(" HT");
		//TODO:complete...
	}
	else{
		price += tr(" TTC");
	}
	rect.translate( wUtil/2, rect.height()+50);
	rect = painter.fontMetrics().boundingRect(wUtil/2, rect.top(), 0, 0, Qt::AlignLeft, price );
	painter.drawText(rect, price);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + wUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius
	*/

	/// Signature Client
	QString sign = "Signature client:\n\n\n\n\n";
	rect.translate( mwUtil/2, rect.height()+25);
	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.top(), 0, 0, Qt::AlignLeft, sign );
	painter.drawText(rect, sign);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + mwUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius

	//Information pied de page
	mFont.setPointSize(10);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, mfooterTextInfo );
	rect.translate( 0, -rect.height());

	painter.drawText( rect, Qt::AlignCenter, mfooterTextInfo);
	painter.drawLine(QPoint(mLeft, rect.top()) , QPoint(mLeft + mwUtil, rect.top()));

	painter.end();
}

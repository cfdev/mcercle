#include "printc.h"
#include "dialogprintchoice.h"
#include "dialogwaiting.h"

#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>

Printc::Printc(database *pdata, QLocale &lang, QObject *parent) :
	QObject(parent)
{
	m_lang = lang;
	m_data = pdata;
	m_cus = pdata -> m_customer;
	m_inv  = pdata -> m_customer -> m_invoice;
	m_pro  = pdata -> m_customer -> m_proposal;
	m_serv = pdata -> m_customer -> m_service;
}

Printc::~Printc(){
}

/**
 * @brief Printc::print_Proposal
 * @param id
 */
void Printc::print_Proposal(const int &id) {
	//On charge l objet en fonction de la selection
	m_pro->loadFromID(id);
	
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = m_pro->getCode() ;
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

/**
 * @brief Printc::print_head
 * @param painter
 */
void Printc::print_header(const QPainter &painter) {
	//TODO: make print header
}

/**
 * @brief Printc::print_footer
 * @param painter
 */
void Printc::print_footer(const QPainter &painter) {
	//TODO: make print footer
}

/**
 * @brief Printc::on_paintPrinterProposal
 * @param printer
 */
void Printc::on_paintPrinterProposal(QPrinter *printer) {
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
	QString identity = m_cus -> getFirstName()+" "+ m_cus -> getLastName()+'\n';
	identity += m_cus -> getAddress1()+'\n';
	identity += m_cus -> getAddress2()+'\n';
	identity += m_cus -> getAddress3()+'\n';
	identity += m_cus -> getZipCode()+" "+ m_cus -> getCity();

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
		rect.setHeight( fm.boundingRect( tr("Code: ") + m_pro->getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_pro->getCode());

		font.setPointSize(12);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 20);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Date: ")+m_pro->getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_pro->getUserDate().toString(tr("dd-MM-yyyy")) );
		rect.translate( 0, rect.height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date fin validit\351: ")+
						  m_pro->getValidDate().toString(tr("dd-MM-yyyy")) );

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
		for(int itemOnpage=0; itemOnpage<linePerPage;){
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
			if( (itemOnpage - linePerPage) >= 0) break;
		}

		//Information pied de page
		rect = fm.boundingRect(mLeft, pageRect.height() - mBottom, pageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, footerTextInfo );
		rect.translate( 0, -rect.height());
		painter.drawText( rect, Qt::AlignCenter, footerTextInfo);

		//Num de page
		pageText = "- " + tr("Page ") + QString::number(page) + " -";
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
void Printc::on_paintPrinterInvoice(QPrinter *printer)
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
	QString identity = m_cus->getFirstName()+" "+m_cus->getLastName()+'\n';
	identity += m_cus->getAddress1()+'\n';
	identity += m_cus->getAddress2()+'\n';
	identity += m_cus->getAddress3()+'\n';
	identity += m_cus->getZipCode()+" "+m_cus->getCity();

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
		rect.setHeight( fm.boundingRect( tr("Code: ") + m_inv->getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_inv->getCode());

		if(!m_inv->getProposalCode().isEmpty()){
			rect.translate( 0, 20);
			rect.setHeight( fm.boundingRect( tr("Ref: ") + m_inv->getProposalCode() ).height() );
			painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Ref: ") + m_inv->getProposalCode());
		}
		font.setPointSize(12);
		painter.setFont(font);
		fm= painter.fontMetrics();
		rect.translate( 0, 20);
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( fm.boundingRect( tr("Date: ")+m_inv->getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_inv->getUserDate().toString(tr("dd-MM-yyyy")) );

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
			if( (itemOnpage - linePerPage) >= 0) break;
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
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rect.top(), wUtil*0.36,0, Qt::AlignRight, m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
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
	rect = fm.boundingRect(mLeft+(wUtil*0.62),rect.top(), wUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice-m_inv->getPartPayment(), 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice-m_inv->getPartPayment(), 'f', 2) );
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
	QString identity = m_cus->getFirstName()+" "+m_cus->getLastName()+'\n';
	identity += m_cus->getAddress1()+'\n';
	identity += m_cus->getAddress2()+'\n';
	identity += m_cus->getAddress3()+'\n';
	identity += m_cus->getZipCode()+" "+m_cus->getCity();

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
	rect.setHeight( fm.boundingRect( tr("Code: ") + "SR"  + m_serv->getDate().toString("yyMMdd") +"-"+ QString::number(m_serv->getId()) ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + "SR"  + m_serv->getDate().toString("yyMMdd") +"-"+ QString::number(m_serv->getId()));

	font.setPointSize(12);
	painter.setFont(font);
	fm= painter.fontMetrics();
	rect.translate( 0, 20);
	//Ajustement de la hauteur du au changement de px
	rect.setHeight( fm.boundingRect( tr("Date: ")+ m_serv->getDate().toString(tr("dd-MM-yyyy")) ).height() );
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
					  tr("Date: ")+
					  m_serv->getDate().toString(tr("dd-MM-yyyy")) );


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

	QString serv = tr("Intervention: ") + m_serv->getName()+"\n";
	serv += tr("R\351alis\351 le : ") + m_serv->getDate().toString(tr("dd-MM-yyyy"))+ tr(" \xE0 ")+ m_serv->getDate().toString(tr("HH:mm")) + "\n\n";
	serv += tr("D\351tails:\n");
	serv += m_serv->getDescription();
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
   /* QString price = tr("Prix: ") + QString::number(m_cus->m_service->getPrice())+ QChar(8364);
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

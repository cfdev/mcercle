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
#include "mcercle.h"
#include "dialogprintchoice.h"
#include "dialogwaiting.h"
#include "settings.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QDebug>
#include <math.h>

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
	if(!m_data->getIsTax())		mfooterTextInfo += "\n" + QLatin1String("Dispensé d'immatriculation au registre du commerce et des société (RCS) et au répertoire des métiers (RM)");
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
	
	qDebug() << "mpageRect.height: " << mpageRect.height();
	if(printer -> orientation() == QPrinter::Landscape){
		mlinePerLastPage = 8;
		mlinePerPage = mlinePerLastPage +2;
		// Ajuster la hauteur
		mBlockHeight = mpageRect.height() / 4.4;
	}
	else{
		mlinePerLastPage = 25;
		mlinePerPage =  mlinePerLastPage +10;
		// Ajuster la hauteur
		mBlockHeight = mpageRect.height() / 1.4;
	}
	mRectContent = QRect(mLeft, 0, mwUtil, mBlockHeight);
	qDebug() << "LinePerPage:" << mlinePerPage << " LinePerLastPage:" << mlinePerLastPage;
	
	database::Bank mb;
	m_data -> getBank(mb);
	mBankTextID = tr("Code banque: ")+mb.codeBanque+"  "+tr("Code guichet: ")+mb.codeGuichet+'\n';
	mBankTextID += tr("Compte: ")+mb.numCompte+"  "+QLatin1String("Clé RIB: ")+mb.keyRIB+'\n';
	mBankTextID += tr("Domiciliation: ");
	mBankTextID += mb.address+"\n\n";
	mBankTextID += tr("IBAN: ");
	mBankTextID += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
	mBankTextID += tr("BIC: ") + mb.codeBIC+'\n';
	
	// Charge le fichier de configurations
	/// TODO :Print Attention certaines FONTs provoquent des seugfault A voir comment tester ca!
	Settings m_settings;
	painter.setFont( m_settings.getPrintFont() );

	mFont = painter.font();
	mwUtil = mpageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases
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
void Printc::print_header(QPainter &painter, QRectF &rect, int type) {
	///Logo
	rect = QRect(mLeft+5, mTop, mlogo.width(), mlogo.height() );
	painter.drawImage(rect, mlogo);

	///Info societe
	mFont.setPointSize(10);
	painter.setFont(mFont);
	rect.translate( 0, rect.height()+5);
	rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top(), 0,0, Qt::AlignLeft, mtextInfo );
	painter.drawText( rect, mtextInfo);

	///Titre
	QString title;
	if(type == T_PROPOSAL) title = tr("Devis");
	else if(type == T_INVOICE) title = tr("Facture");
	else if(type == T_SERVICE) title = tr("Service");
	mFont.setPointSize(24);
	painter.setFont(mFont);
	rect = QRect(mLeft, mTop, mpageRect.width() - (mLeft+mRight),
						painter.fontMetrics().boundingRect( title ).height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, title );

	///Code en fonction du type
	mFont.setPointSize(14);
	painter.setFont(mFont);
	rect.translate( 0, rect.height());
	if(type == T_PROPOSAL) {
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( painter.fontMetrics().boundingRect( tr("Code: ") + m_pro -> getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_pro -> getCode());
	}
	else if(type == T_INVOICE) {
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( painter.fontMetrics().boundingRect( tr("Code: ") + m_inv -> getCode() ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Code: ") + m_inv -> getCode());
		if(!m_inv -> getProposalCode().isEmpty()){
			rect.translate( 0, rect.height());
			rect.setHeight( painter.fontMetrics().boundingRect( tr("Ref: ") + m_inv -> getProposalCode() ).height() );
			painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Ref: ") + m_inv -> getProposalCode());
		}
	}
	
	///Date en fonction du type
	mFont.setPointSize(12);
	painter.setFont(mFont);
	rect.translate( 0, rect.height());
	if(type == T_PROPOSAL) {
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( painter.fontMetrics().boundingRect( tr("Date: ")+m_pro -> getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_pro -> getUserDate().toString(tr("dd-MM-yyyy")) );
		rect.translate( 0, rect.height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  QLatin1String("Date fin validité: ")+
						  m_pro -> getValidDate().toString(tr("dd-MM-yyyy")) );
	}
	else if(type == T_INVOICE) {
		//Ajustement de la hauteur du au changement de px
		rect.setHeight( painter.fontMetrics().boundingRect( tr("Date: ")+m_inv -> getUserDate().toString(tr("dd-MM-yyyy")) ).height() );
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  tr("Date: ")+
						  m_inv -> getUserDate().toString(tr("dd-MM-yyyy")) );
		rect.translate( 0, rect.height());
		painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter,
						  QLatin1String("Date Echéance: ")+
						  m_inv -> getLimitPayment().toString(tr("dd-MM-yyyy")) );
	}

	/// Identite du client
	mFont.setPointSize(10);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+15, 0, 0, Qt::AlignLeft, tr("Client: ") );
	painter.drawText(rect, tr("Client: "));

	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+5, 0, 0, Qt::AlignLeft, mtextidentity );
	painter.drawText(rect, mtextidentity);
	rect.translate(-5,-5);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + mwUtil/2);
	painter.drawRoundedRect(rect, 5, 5); // dessine le rectangle avec 5 de radius
	// Creation dun espace
	rect.translate( 0, rect.height()+25);
}

/**
  Imprime le contenu de la facture/devis
 */
void Printc::print_content(QPainter &painter, QRectF &rect, itemList Ilist, int &itemPrinted, int page, int NbOfpage) {
	// sauvegarde de la position haute du contenu 
	int rectTop_content = rect.top();
	int rectTop_LineTitre;
	
	/// Content title
	rect.translate( 0, 5);
	//DESIGNATION 40%
	rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, QLatin1String("Désignation") );
	painter.drawText( rect, QLatin1String("Désignation"));
	//TVA 12%
	if(m_data->getIsTax()){
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TVA %") );
		painter.drawText( rect, tr("TVA %") );
	}
	//REMISE 12%
	bool discount=false;
	for(int i=0; i < Ilist.discount.size(); i++)
		if(Ilist.discount.at(i) > 0){
			discount=true;
			break;
		}
	if(discount){
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Remise %"));
		painter.drawText( rect, tr("Remise %"));
	}
	//PRIX UNITAIRE 12%
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("Prix U. ")+ QChar(8364) );
	painter.drawText( rect, tr("Prix U. ")+ QChar(8364));
	//QUANTITE 12%
	rect = painter.fontMetrics().boundingRect(mLeft+5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignLeft, QLatin1String("Quantité")  );
	painter.drawText( rect, QLatin1String("Quantité") );
	//TOTAL 12%
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignLeft, tr("TOTAL ")+ QChar(8364) );
	painter.drawText( rect, tr("TOTAL ")+ QChar(8364) );
	// Sauvegarde de la position pour la line de separation
	rectTop_LineTitre = rect.bottom()+5;
	
	/// Content
	rect.translate( 0, 5);
	QStringList lines_prev, lines;
	qDebug() << "itemPrinted :" << itemPrinted;
	
	//Defini le nombre de line par page
	int maxLine=0;
	if(page >= NbOfpage)
		maxLine = mlinePerLastPage;
	else
		maxLine = mlinePerPage;
	qDebug() << "Page:" << page << " Maxline:" << maxLine;
	
	// Boucle des articles
	for(int linePrinted=0; linePrinted<maxLine; ){
		// sil ne reste plus d item a afficher on sort
		if((Ilist.designation.count() - itemPrinted) <= 0)break;
		rect.translate( 0, rect.height()+5);
		// Adaptation du rect en fonction du nombre de lignes
		// En fonction de l item precedent dou le test >0
		if ((itemPrinted>0) && (linePrinted>0)){
			lines_prev = Ilist.designation.at(itemPrinted-1).split("\n");
			// Si multiligne on ajout un espace a la fin pour la lisibilite de la facture/devis
			if(lines_prev.count() > 1)
				rect.translate( 0, lines_prev.count()*rect.height());
			else
				rect.translate( 0, (lines_prev.count()-1)*rect.height());
		}
		//DESIGNATION 40%
		rect = painter.fontMetrics().boundingRect(mLeft+5,rect.top(), mwUtil*0.40,0, Qt::AlignLeft, Ilist.designation.at(itemPrinted) );
		rect.setWidth(mwUtil*0.50); //fixe la largeur
		painter.drawText( rect,  Qt::AlignLeft , Ilist.designation.at(itemPrinted));
		
		//TVA 12%
		if(m_data->getIsTax()){
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.40),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
		}
		
		//REMISE 12%
		if(discount){
			rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.52),rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(Ilist.discount.at(itemPrinted)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , QString::number(Ilist.discount.at(itemPrinted)) );
		}
		
		//PRIX UNITAIRE 12%
		rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.64),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(Ilist.price.at(itemPrinted),'f',2) );
		//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
		painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.price.at(itemPrinted),'f',2) );

		//QUANTITE 12%
		rect = painter.fontMetrics().boundingRect(mLeft-5+mwUtil*0.76,rect.top(), mwUtil*0.12,0, Qt::AlignRight, QString::number(Ilist.quantity.at(itemPrinted)) );
		//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
		painter.drawText( rect , Qt::AlignRight , QString::number(Ilist.quantity.at(itemPrinted)));

		//TOTAL 12%
		rect = painter.fontMetrics().boundingRect(mLeft-5+(mwUtil*0.88),rect.top(), mwUtil*0.12,0, Qt::AlignRight, m_lang.toString(Ilist.totalPrice.at(itemPrinted), 'f', 2) );
		//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
		painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.totalPrice.at(itemPrinted), 'f', 2) );
		//ListTotalPrice.push_back( printList.totalPrice.at(pIndex) );
		
		// Incrementer le nombre de ligne imprimee
		// Si multiligne on ajoute une ligne espace dans l increment
		lines = Ilist.designation.at(itemPrinted).split("\n");
		if(lines.count() > 1)
			linePrinted += lines.count()+1;
		else
			linePrinted += lines.count();
		
		// Incrementer le nombre d item imprime
		itemPrinted++;
		
		// Si le nombre de lignes par page et atteind  -> ON SORT
		if((linePrinted - maxLine) >= 0)
			break; 
		
		// sil reste des items a afficher et
		// Si le nombre de lignes de l item suivant depasse le max de ligne -> ON SORT
		// Important: cela evite un item multilignes d etre a cheval sur 2 pages !!!
		if((Ilist.designation.count() - itemPrinted) > 0){
			lines = Ilist.designation.at(itemPrinted).split("\n");
			if(lines.count() > 1) {
				if((linePrinted + lines.count()+1) > maxLine)
					break;
			}
			else{
				if((linePrinted + lines.count()) > maxLine)
					break;
			}
		}
	}
	/// Dessine les contours du contenu.
	mRectContent = QRect();
	qreal heightContent = mBlockHeight;
	// Si ce n'est pas la derniere page on adapte la hauteur
	if(page < NbOfpage) {
		qDebug() << "Ajuste hauteur RectContent page " << page;
		// Si multiligne on adapte la hauteur du rect
		if(lines.count() > 1) rect.setHeight( (lines.count()+1)*rect.height() );
		if(heightContent < rect.bottom()) heightContent = rect.bottom();
		//Bornage si depassement du pied de page
		if( ((rectTop_content+heightContent) >= get_RecFooter(painter).top())){
			qDebug() << "TopFooter: " << get_RecFooter(painter).top() << " rectTop_content+heightContent" << rectTop_content+heightContent;
			heightContent = get_RecFooter(painter).top() - 25;
			qDebug() << "Bornage -> depassement du pied de page: " << heightContent;
		}
	}
	mRectContent.adjust(mLeft, rectTop_content, mLeft+mwUtil, heightContent);
	painter.drawRoundedRect(mRectContent, 5, 5);
	
	// LIGNE de separation des TITRES
	painter.drawLine(QPoint(mRectContent.left(), rectTop_LineTitre),
					 QPoint(mRectContent.right(),rectTop_LineTitre));
	// LIGNE de separation TVA 12%
	if(m_data -> getIsTax()){
		painter.drawLine(QPoint(mLeft+mwUtil*0.40, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.40, mRectContent.bottom()) );
	}
	// LIGNE de separation DISCOUNT
	if(discount){
		painter.drawLine(QPoint(mLeft+mwUtil*0.52, mRectContent.top()),
						 QPoint(mLeft+mwUtil*0.52, mRectContent.bottom()) );
	}
	// LIGNE de separation PRIX UNITAIRE 12%
	painter.drawLine(QPoint(mLeft+mwUtil*0.64, mRectContent.top()),
					 QPoint(mLeft+mwUtil*0.64, mRectContent.bottom()) );
	// LIGNE de separation QUANTITE 12%
	painter.drawLine(QPoint(mLeft+mwUtil*0.76, mRectContent.top()),
					 QPoint(mLeft+mwUtil*0.76, mRectContent.bottom()) );
	// LIGNE de separation TOTAL 12%
	painter.drawLine(QPoint(mLeft+mwUtil*0.88, mRectContent.top()),
					 QPoint(mLeft+mwUtil*0.88, mRectContent.bottom()) );
}

/**
 * @brief Printc::get_RecFooter
 * @return 
 */
QRectF Printc::get_RecFooter(QPainter &painter) {
	QRectF rect;
	rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, mfooterTextInfo );
	rect.translate( 0, -rect.height());
	return rect;
}

/**
 * @brief Printc::print_footer
 * @param painter
 */
void Printc::print_footer(QPainter &painter, QRectF &rect, QString page, QString NbOfpage) {
	// Ligne
	painter.drawLine(QPoint(mLeft, get_RecFooter(painter).top()) , QPoint(mLeft + mwUtil, get_RecFooter(painter).top()));

	// Num de page
	if(!page.isEmpty()) {
		QString pageText = tr("Page ") + page + " / " + NbOfpage;
		rect = painter.fontMetrics().boundingRect(mLeft, get_RecFooter(painter).top(), mpageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		rect.translate( 0, +rect.height());
		painter.drawText( rect, Qt::AlignVCenter |Qt::AlignRight, pageText);
	}
	//Information pied de page
	rect = get_RecFooter(painter);
	rect.translate( 0, 10);
	painter.drawText(rect , Qt::AlignHCenter, mfooterTextInfo);
}

/**
 * @brief Printc::print_total
 * @param painter
 * @param rect
 * @param totalPrice
 */
void Printc::print_total(QPainter &painter, QRectF &rect, itemList Ilist, qreal &totalPrice ,int type) {
	//TOTAL
	totalPrice=0;
	for(int i=0; i<Ilist.totalPrice.size(); i++)
		totalPrice += Ilist.totalPrice.at(i);
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),/*mRectContent.bottom()+15*/ mpageRect.height() - mBottom - OFFSET_BOT_TOTAL, mwUtil*0.36,0, Qt::AlignLeft, tr("TOTAL : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	QRect rectTotal = QRect(mLeft+(mwUtil*0.62),rect.top()-5, mwUtil*0.36 +15, rect.height() +10);
	painter.drawRoundedRect(rectTotal, 5, 5 );
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("TOTAL : "));

	mFont.setBold(true);
	painter.setFont( mFont );
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),/*mRectContent.bottom()+15*/ mpageRect.height() - mBottom - OFFSET_BOT_TOTAL, mwUtil*0.36,0, Qt::AlignRight, m_lang.toString(totalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(totalPrice, 'f', 2) );

	mFont.setBold(false);
	painter.setFont( mFont );
	painter.setBrush( Qt::NoBrush );

	if(type == T_INVOICE){
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
	}
	
	//TOTAL TAX
	QString text;
	if(m_data -> getIsTax()) {
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = painter.fontMetrics().boundingRect(mLeft, /*mRectContent.bottom()+5*/ mpageRect.height() - mBottom - OFFSET_BOT_TOTAL , mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}
}
	
/**
 * @brief Printc::print_reglement
 * @param painter
 * @param rect
 */
void Printc::print_reglement(QPainter &painter, QRectF &rect, qreal &totalPrice) {
	qreal topRect = mpageRect.height() - mBottom - OFFSET_BOT_TOTAL + 15;
	//Mode de reglement
	QString typePayment;
	QString typeP = m_pro -> getTypePayment();
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == MCERCLE::CASH)           typePayment = tr("Espece");
	if(typeP == MCERCLE::CHECK)          typePayment = tr("Cheque");
	if(typeP == MCERCLE::CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == MCERCLE::INTERBANK)      typePayment = tr("TIP");
	if(typeP == MCERCLE::TRANSFER)       typePayment = tr("Virement");
	if(typeP == MCERCLE::DEBIT)          typePayment = tr("Prelevement");
	if(typeP == MCERCLE::OTHER)          typePayment = tr("Autre");
	qDebug() <<"PAIEMENT: "<< typeP << " "<< MCERCLE::TRANSFER;
	QString text = tr("Mode de r\350glement : ")+typePayment;
	rect = painter.fontMetrics().boundingRect(mLeft, topRect, mwUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == MCERCLE::INTERBANK)||(typeP == MCERCLE::DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de r\350glement.");
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Condition de reglement
	text = tr("Conditions de r\350glement: 30% du montant total lors\nde la signature de cette proposition soit: ");
	text += m_lang.toString(totalPrice * 0.3, 'f', 2) +" "+ QChar(8364);
	rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text);
	painter.drawText( rect, text);
	
	/// RIB
	if(typeP == MCERCLE::TRANSFER){
		text = QLatin1String("Relevé d'Itentité Bancaire\n\n\n\n\n\n");
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom(), mwUtil*0.36 +15,0, Qt::AlignHCenter, text);
		painter.drawText(rect, text);
		
		mFont.setPointSize(8);
		painter.setFont(mFont);
		rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top()+25, mwUtil*0.36 +15, rect.height(), Qt::AlignLeft, mBankTextID );
		rect.setWidth(mwUtil*0.36); //fixe la largeur
		painter.drawText(rect, mBankTextID);
		
		painter.setPen( Qt::DashLine );
		rect = QRect(mLeft,rect.top()-5, mwUtil*0.36 +15, rect.height());
		painter.drawRoundedRect( rect, 5, 5 );
		painter.setPen( Qt::SolidLine );
	}
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
	
	// Recuperation des donnees presentent dans la bdd
	proposal::ProposalListItems plist;
	m_pro -> getProposalItemsList(plist, "ITEM_ORDER", "", "");
	// Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting -> setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting -> setDetail(tr("<i>Preparation En cours...</i>"));

	// Defini le nombre a imprimer
	int itemsToPrint = plist.name.count();
	int lineToPrint = 0;
	//recupere le nombre de lignes.
	for(int i=0; i<plist.name.count(); i++){
		// Si multiligne on ajout un saut de ligne
		if(plist.name.at(i).count("\n")>0) lineToPrint++;
		// ajout le nombre d'item avec le nombre de lignes
		lineToPrint += ( plist.name.at(i).count("\n")+1 );
	}
	int numberOfPage = 0;
	if(lineToPrint > mlinePerLastPage) {
		numberOfPage = ceil((qreal)lineToPrint/mlinePerPage);
		int nbLines = ((numberOfPage-1)*mlinePerPage)+mlinePerLastPage;
		if((lineToPrint - nbLines) >0) numberOfPage++;
	}
	else {
		numberOfPage = 1;
	}
	
	qDebug() << "lineToPrint: " << lineToPrint;
	qDebug() << "numberOfPage: " << numberOfPage;
	
	m_DialogWaiting -> setProgressBarRange(0, numberOfPage);
	m_DialogWaiting -> setModal(true);
	m_DialogWaiting -> show();
	
	// Variables
	QRectF rect;
	qreal totalPrice=0;
	// Charge les items
	itemList printList;
	for(int i=0; i<plist.name.count();i++){
		printList.designation.push_back( plist.name.at(i) );
		printList.tax.push_back( plist.tax.at(i) );
		printList.discount.push_back( plist.discount.at(i) );
		printList.price.push_back( plist.price.at(i) );
		printList.quantity.push_back( plist.quantity.at(i) );
		//total des items
		totalPrice = plist.price.at(i)*plist.quantity.at(i);
		if(plist.discount.at(i)>0) totalPrice -= totalPrice*(plist.discount.at(i)/100.0);
		printList.totalPrice.push_back( totalPrice );
	}
	
	// list all products
	for(int page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		/// Imprime l entete
		print_header(painter, rect, T_PROPOSAL);
		/// Imprime le contenu
		print_content(painter, rect, printList, itemPrinted, page, numberOfPage);
		/// Imprime le pied de page
		print_footer(painter, rect, QString::number(page), QString::number(numberOfPage));
		// Met a jour la progression
		m_DialogWaiting -> setProgressBar(page);
		// Si il reste des items -> Nouvelle page
		if( (itemsToPrint - itemPrinted) > 0){
			qDebug() << "-> Nouvelle page";
			printer -> newPage();
		}
		// Sinon on creer une nouvelle page avec le total uniquement
		else if( page < numberOfPage){
			page++;
			qDebug() << "-> Nouvelle page avec total";
			printer -> newPage();
			/// Imprime l entete
			print_header(painter, rect, T_PROPOSAL);
			/// Imprime le pied de page
			print_footer(painter, rect, QString::number(page), QString::number(numberOfPage));
		}
	}
	// Imprime le total
	print_total(painter, rect, printList, totalPrice, T_PROPOSAL);
	// Imprime le mode de reglement
	print_reglement(painter, rect, totalPrice);

	/// Signature Client
	QString text = "Signature client:\n(Suivi de la mention \"bon pour accord\")\n\n\n\n\n";
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62)+5, /*rect.top()*/ mpageRect.height() - mBottom - OFFSET_BOT_TOTAL + 45, 0, 0, Qt::AlignLeft, text );
	painter.drawText(rect, text);
	painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*0.36 +15, rect.height()), 5, 5 );
	
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
 * @brief Printc::print_InvoicesList
 * @param listofId
 */
void Printc::print_InvoicesList(QList<int> listofId) {
	QPrinter printer;
	QString name;
	QWidget fenetre;
	QPrintDialog pDialog(&printer, &fenetre);
	// Dialog de choix dimpression
	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice -> setModal(true);
	m_DialogPrintChoice -> exec();
	
	if(m_DialogPrintChoice -> result() == QDialog::Accepted){
		if(m_DialogPrintChoice->typePrint() == DialogPrintChoice::PRINT_FILE)
			if(pDialog.exec() == QDialog::Rejected) {
			 return;
			}
		///TODO:Definition des marges en dur pour le moment
		printer.setPageMargins(5.0,5.0,5.0,10.0, QPrinter::Millimeter);
		// Boucle pour l impression multiple
		foreach(int id, listofId) {
			m_inv -> loadFromID(id);
			name = m_inv -> getCode();
			// Si impression en fichier ou normale
			if(m_DialogPrintChoice->typePrint() == DialogPrintChoice::PRINT_FILE){
				printer.setOutputFormat(QPrinter::NativeFormat);
				printer.setOutputFileName( m_DialogPrintChoice -> pathFile() );
			}
			else{
				printer.setOutputFormat(QPrinter::PdfFormat);
				printer.setOutputFileName( m_DialogPrintChoice ->path() + name + ".pdf");
			}
			printer.setDocName( name );
			printer.setCreator("mcercle");
			// Impression avec le painter
			on_paintPrinterInvoice(&printer);
		}
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

	invoice::InvoiceListItems plist;
	//Recuperation des donnees presentent dans la bdd
	m_inv -> getInvoiceItemsList(plist, "ITEM_ORDER", "", "");
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>GESTION D IMPRESSION</b>"));
	m_DialogWaiting->setDetail(tr("<i>Preparation En cours...</i>"));

	// Defini le nombre a imprimer
	int itemsToPrint = plist.name.count();
	int lineToPrint = itemsToPrint;
	//recupere le nombre de lignes.
	for(int i=0; i<plist.name.count(); i++){
		lineToPrint += plist.name.at(i).count("\n");
	}
	int numberOfPage = ceil((qreal)lineToPrint/mlinePerLastPage);
	if(numberOfPage > 1) {
		int nbLines = ((numberOfPage-1)*mlinePerPage)+mlinePerLastPage;
		if((lineToPrint - nbLines) >0) numberOfPage++;
	}
	
	qDebug() << "lineToPrint: " << lineToPrint;
	qDebug() << "numberOfPage: " << numberOfPage;
	
	m_DialogWaiting -> setProgressBarRange(0, numberOfPage);
	m_DialogWaiting -> setModal(true);
	m_DialogWaiting -> show();

	// Variables
	QRectF rect;
	qreal totalPrice=0;
	// Charge les items
	itemList printList;
	for(int i=0; i<plist.name.count();i++){
		printList.designation.push_back( plist.name.at(i) );
		printList.tax.push_back( plist.tax.at(i) );
		printList.discount.push_back( plist.discount.at(i) );
		printList.price.push_back( plist.price.at(i) );
		printList.quantity.push_back( plist.quantity.at(i) );
		//total des items
		totalPrice = plist.price.at(i)*plist.quantity.at(i);
		if(plist.discount.at(i)>0) totalPrice -= totalPrice*(plist.discount.at(i)/100.0);
		printList.totalPrice.push_back( totalPrice );
	}
	
	// list all products
	for(int page=1, itemPrinted=0; itemPrinted<itemsToPrint ;page++){
		/// Imprime l entete
		print_header(painter, rect, T_INVOICE);
		/// Imprime le contenu
		print_content(painter, rect, printList, itemPrinted, page, numberOfPage);
		/// Imprime le pied de page
		print_footer(painter, rect, QString::number(page), QString::number(numberOfPage));
		// Met a jour la progression
		m_DialogWaiting -> setProgressBar(page);
		// Nouvelle page ?
		if( (itemsToPrint - itemPrinted) > 0){
			qDebug() << "-> Nouvelle page";
			printer -> newPage();
		}
	}
	// Imprime le total
	print_total(painter, rect, printList, totalPrice, T_INVOICE);
	// Imprime le mode de reglement
	print_reglement(painter, rect, totalPrice);
	delete m_DialogWaiting;
	painter.end();
}



/**
 * @brief Printc::print_Service
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
	QRectF rect;
	print_header(painter, rect, T_SERVICE);
	/// contenu
	//contour du contenu
	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 450;

	QString serv = tr("Intervention: ") + m_serv->getName()+"\n";
	serv += QLatin1String("Réalisé le : ") + m_serv->getDate().toString(tr("dd-MM-yyyy"))+ tr(" \xE0 ")+ m_serv->getDate().toString(tr("HH:mm")) + "\n\n";
	serv += QLatin1String("Détails:\n");
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

	// Imprime le pied de page
	print_footer(painter, rect, "1","1");
	// Fin du painter
	painter.end();
}

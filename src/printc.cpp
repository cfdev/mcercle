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

#include "printc.h"
#include "mcercle.h"
#include "dialogprintchoice.h"
#include "dialogwaiting.h"
#include "settings.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QPrintPreviewWidget>
#include <QToolBar>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/qmath.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif

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
	
	/// Pied de page
	mfooterTextInfo = info.name;
	if(!info.capital.isEmpty())	mfooterTextInfo += " - " + tr("Capital ") + info.capital;
	if(!info.num.isEmpty())		mfooterTextInfo += " - " + tr("Siret ") + info.num;
	if((m_data->getIsTax()) &&(!info.numTax.isEmpty()) )	mfooterTextInfo += " - " + tr("N° TVA ") + info.numTax;

	mfooterTextInfo += "\n" + info.address1 + " " + info.address2 + " " +  info.address3 + " - " + info.zipCode + " " + info.city;

	if(!info.line1.isEmpty())	mfooterTextInfo += "\n" + info.line1;
	if(!info.line2.isEmpty())	mfooterTextInfo += "\n" + info.line2;
	if(!info.line3.isEmpty())	mfooterTextInfo += "\n" + info.line3;
	if(!info.line4.isEmpty())	mfooterTextInfo += "\n" + info.line4;

	///Coins carre ou rond
	setRoundedRect(info.borderRadius);
	setDrawLine(info.drawLine);
}

Printc::~Printc(){
}

/**
 * @brief setRoundedRect permet davoir les coins arrondis ou pas
 * @param state
 */
void Printc::setRoundedRect(bool state){
	if(state){
		mRoundedRect = 25;
	}
	else{
		mRoundedRect = 0;
	}
}
/**
 * @brief setRoundedRect dessine les lignes ou pas
 * @param state
 */
void Printc::setDrawLine(bool state){
	mDrawLine = state;
}

/**
 * @brief Printc::load_parameters
 * @param p
 */
void Printc::load_parameters(QPrinter *printer, QPainter &painter) {

	///TODO:Definition des marges en dur pour le moment
	printer -> setPageMargins(5.0,5.0,5.0,15.0, QPrinter::Millimeter);
	// ATTENTION: l'impression physique et PDF n'on pas le même maximum DPI
	// Mettre 300 en max est largement suffisant
	// ATTENTION l'appel a cette fonction doit se faire avant le "painter.begin"
	printer -> setResolution(DPI);
	qDebug() << "Printc::load_parameters: " << printer -> resolution();

	printer -> getPageMargins(&mLeft, &mTop, &mRight, &mBottom, QPrinter::DevicePixel);
	mpageRect = printer -> pageRect();

	qDebug() << "marges px: left(" << mLeft << ") top(" << mTop << ") right(" << mRight << ") bot(" << mBottom << ")";
	qDebug() << "mpageRect.height: " << mpageRect.height();
	if(printer -> orientation() == QPrinter::Portrait){
		mlinePerLastPage = 30;//25
		mlinePerPage =  mlinePerLastPage +10;
		// Ajuster la hauteur
		mBlockHeight = mpageRect.height() / 1.35;
	}
	mRectContent = QRect(mLeft, 0, mwUtil, mBlockHeight);
	qDebug() << "LinePerPage:" << mlinePerPage << " LinePerLastPage:" << mlinePerLastPage;
	
	database::Bank mb;
	m_data -> getBank(mb);
	mBankTextID += tr("Titulaire: ") + mb.name+'\n';
	mBankTextID += tr("IBAN: ");
	mBankTextID += mb.IBAN1+' '+mb.IBAN2+' '+mb.IBAN3+' '+mb.IBAN4+' '+mb.IBAN5+' '+mb.IBAN6+' '+mb.IBAN7+' '+mb.IBAN8+' '+mb.IBAN9+'\n';
	mBankTextID += tr("BIC: ") + mb.codeBIC+'\n';

	// Begin !
	painter.begin(printer);

	// Charge le fichier de configurations
	/// TODO :Print Attention certaines FONTs provoquent des seugfault A voir comment tester ca!
	Settings m_settings;
	painter.setFont( m_settings.getPrintFont() );
	mFontSize = m_settings.getPrintFont().pointSize();
	QPen pen;// cree un pinceau
	pen.setWidthF(PEN_WIDTH);
	painter.setPen(pen);
	painter.setBrush( Qt::NoBrush );
	mFont = painter.font();
	mwUtil = mpageRect.width() - (mLeft+mRight); // Largeur utile. pour la repartition des cases
}

/**
 * @brief Charge les informations client
 */
void Printc::load_customerInfos() {
	/// Identite du client
	mtextidentity = "";
	if(!m_cus ->getGender().isEmpty())mtextidentity +=  m_cus -> getGender() +" ";
	mtextidentity += m_cus -> getFirstName()+" "+ m_cus -> getLastName()+'\n';
	if((!m_cus ->getMobileNumber().isEmpty())||(!m_cus ->getPhoneNumber().isEmpty())){
		if((!m_cus ->getMobileNumber().isEmpty())&&(!m_cus ->getPhoneNumber().isEmpty()))
			mtextidentity += "Tel: " +m_cus ->getMobileNumber()+ " - " + m_cus ->getPhoneNumber()+'\n';
		else
			mtextidentity += "Tel: " + m_cus ->getMobileNumber()+ m_cus ->getPhoneNumber()+'\n';
	}
	if(!m_cus -> getAddress1().isEmpty()) mtextidentity += m_cus -> getAddress1()+'\n';
	if(!m_cus -> getAddress2().isEmpty()) mtextidentity += m_cus -> getAddress2()+'\n';
	if(!m_cus -> getAddress3().isEmpty()) mtextidentity += m_cus -> getAddress3()+'\n';
	mtextidentity += m_cus -> getZipCode()+" "+ m_cus -> getCity();
}

/**
 * @brief Printc::print_Proposal
 * @param id
 */
void Printc::print_Proposal(const int &id) {
	//On charge l objet en fonction de la selection
	m_pro -> loadFromID(id);
	
	QPrinter printer/*( QPrinter::HighResolution )*/;
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
		// Ajuste la taille
		QList<QPrintPreviewWidget *> list = m_PreviewDialog.findChildren<QPrintPreviewWidget *>();
		if(!list.isEmpty()) { // paranoiac safety check
			list.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
		}
		//Cache les boutons inutile de l orientation
		QToolBar *toolbar = m_PreviewDialog.findChild<QToolBar*>();
		if(toolbar) { // paranoiac safety check
			toolbar->removeAction( toolbar->actions().at(7) );
			toolbar->removeAction( toolbar->actions().at(7) );
		}
		m_PreviewDialog.exec();
	}
}

/**
 * @brief Printc::quickPrint_Proposal
 * @param id
 */
void Printc::quickPrint_Proposal(const int &id) {
	//On charge l objet en fonction de la selection
	m_pro -> loadFromID(id);

	QPrinter printer;
	QString name = m_pro->getCode() ;
	QWidget fenetre;
	QPrintDialog pDialog(&printer, &fenetre);
	if(pDialog.exec() == QDialog::Rejected) {
		return;
	}

	printer.setPageSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::NativeFormat);
	printer.setDocName( name );
	printer.setCreator("mcercle");
	printer.setResolution(DPI);
	printer.setDocName( name );
	printer.setCreator("mcercle");

	// Impression avec le painter
	on_paintPrinterProposal(&printer);
}

/**
 * @brief Printc::quickPrintPDF_Proposal
 * @param id
 */
void Printc::quickPrintPDF_Proposal(const int &id) {
	//On charge l objet en fonction de la selection
	m_pro -> loadFromID(id);
	QPrinter printer;
	QString name = m_pro->getCode() ;

	printer.setPageSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setDocName( name );
	printer.setCreator("mcercle");
	printer.setResolution(DPI);
	printer.setDocName( name );
	printer.setCreator("mcercle");

	QString path_DataLocation;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	path_DataLocation = QDesktopServices::storageLocation ( QDesktopServices::HomeLocation );

#else
	// Qt5 gere un dosssier du nom de lapplication
	path_DataLocation = QStandardPaths::writableLocation ( QStandardPaths::HomeLocation );
#endif

	QString filename = QFileDialog::getSaveFileName(0, "Enregister sous... ", path_DataLocation+"/"+name, "*.pdf");
	if( !filename.isEmpty() ) {
		if(QFile::exists ( filename )){
			// Si le fichier existe deja on supprime
			if(!QFile::remove ( filename ) ){
				QMessageBox::critical(0, tr("Erreur"),
									 tr("Impossible de remplacer le fichier!\n\n")+
									 "Destination: "+name+"\n");
				return;
			}
		}
		printer.setOutputFileName( filename );
		// Impression avec le painter
		on_paintPrinterProposal(&printer);
	}

}

/**
 * @brief Printc::print_header
 * @param painter
 * @return
 */
void Printc::print_header(QPainter &painter, QRectF &rect, int type) {
	///Logo
	qreal ratio= (qreal)mlogo.width()/mlogo.height();
	rect = QRect(mLeft+5, mTop, (mpageRect.height()/8)*ratio, mpageRect.height()/8 );
	painter.drawImage(rect, mlogo);

	///Info societe
	//mFont.setPointSize(10);
	mFont.setPointSizeF(mFontSize);
	painter.setFont(mFont);
	rect.translate( 0, rect.height()+5);
	rect = painter.fontMetrics().boundingRect(mLeft+5, rect.top(), 0,0, Qt::AlignLeft, mtextInfo );
	painter.drawText( rect, mtextInfo);

	///Titre
	QString title;
	if(type == T_PROPOSAL) title = tr("Devis");
	else if(type == T_INVOICE){
		title = tr("Facture");
		if(m_inv ->getType() == MCERCLE::TYPE_PART) title = tr("Facture d'acompte");
		if(m_inv ->getType() == MCERCLE::TYPE_CREDIT) title = tr("Facture d'avoir");
	}
	else if(type == T_SERVICE) title = tr("Service");
	//mFont.setPointSize(24);
	mFont.setPointSizeF(mFontSize*2.4);
	painter.setFont(mFont);
	rect = QRect(mLeft, mTop, mpageRect.width() - (mLeft+mRight),
				 painter.fontMetrics().boundingRect( title ).height());
	painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, title );

	///Code en fonction du type
	mFont.setPointSizeF(mFontSize*1.4);
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
		else if(m_inv ->getType() == MCERCLE::TYPE_PART){
			rect.translate( 0, rect.height());
			rect.setHeight( painter.fontMetrics().boundingRect( tr("Ref: ") + m_inv -> getCodeInvoice_Ref() ).height() );
			painter.drawText( rect, Qt::AlignRight|Qt::AlignVCenter, tr("Ref: ") + m_inv -> getCodeInvoice_Ref());

		}
	}
	
	///Date en fonction du type
	mFont.setPointSizeF(mFontSize*1.2);
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
						  tr("Date fin validité: ")+
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
						  tr("Date Echéance: ")+
						  m_inv -> getLimitPayment().toString(tr("dd-MM-yyyy")) );
	}

	mFont.setPointSizeF(mFontSize);
	painter.setFont(mFont);

	/// Identite du client si le type n'est pas un fichier à entête
	if(type != T_FILE) {
		rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+15, 0, 0, Qt::AlignLeft, tr("Client: ") );
		painter.drawText(rect, tr("Client: "));

		rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.bottom()+SPACE_BORDER, 0, 0, Qt::AlignLeft, mtextidentity );
		painter.drawText(rect, mtextidentity);
		rect.translate(-SPACE_BORDER,-SPACE_BORDER);
		rect.setHeight(rect.height()+SPACE_BORDER);
		rect.setWidth(6 + mLeft + mwUtil/2);
		if(mDrawLine) painter.drawRoundedRect(rect, mRoundedRect, mRoundedRect); // dessine le rectangle
		// Creation dun espace
		rect.translate( 0, rect.height()+SPACE_BORDER*5);
	}

}

/**
  Imprime le contenu de la facture/devis
 */
void Printc::print_content(QPainter &painter, QRectF &rect, itemList Ilist, int &itemPrinted, int page, int NbOfpage) {
	// sauvegarde de la position haute du contenu
	int rectTop_content = rect.top();
	int rectTop_LineTitre;
	
	/// Content title
	// color of header
	//painter.setBrush( Qt::lightGray );
	rect.translate( 0, SPACE_BORDER);
	qreal pos = WIDTH_DES;
	//DESIGNATION 50%
	rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER,rect.top(), mwUtil*pos,0, Qt::AlignLeft, tr("Désignation") );
	painter.drawText( rect, tr("Désignation"));

	//REMISE 12%
	bool discount=false;
	for(int i=0; i < Ilist.discount.size(); i++)
		if(Ilist.discount.at(i) > 0){
			discount=true;
			break;
		}
	if(discount){
		//Si il y a une remise
		//TVA 12%
		if(m_data->getIsTax()){
			rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TAX,0, Qt::AlignLeft, tr("TVA %") );
			painter.drawText( rect, tr("TVA %") );
		}
		pos+=WIDTH_TAX;
		rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_DIS,0, Qt::AlignLeft, tr("Rem.%"));
		painter.drawText( rect, tr("Rem.%"));
	}
	else {
		//Sinon on decale de la largeur colonne TVA pour remplacer la colonne remise
		//TVA 12%
		pos+=WIDTH_TAX;
		if(m_data->getIsTax()){
			rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TAX,0, Qt::AlignLeft, tr("TVA %") );
			painter.drawText( rect, tr("TVA %") );
		}
	}
	pos+=WIDTH_DIS;
	//PRIX UNITAIRE 12%
	rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_PRI,0, Qt::AlignLeft, tr("Prix U. ")+  m_data->getCurrency() );
	pos+=WIDTH_PRI;
	painter.drawText( rect, tr("Prix U. ")+ m_data->getCurrency());
	//QUANTITE 8%
	rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+mwUtil*pos,rect.top(), mwUtil*WIDTH_QTE,0, Qt::AlignLeft, tr("Qte")  );
	pos+=WIDTH_QTE;
	painter.drawText( rect, tr("Qte") );
	//TOTAL 12%
	QString stt;
	if(m_data->getIsTax())
		stt = tr("TOTAL HT ")+  m_data->getCurrency();
	else
		stt = tr("TOTAL ")+  m_data->getCurrency();
	rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TOT,0, Qt::AlignLeft, stt );
	painter.drawText( rect, stt );
	
	// Sauvegarde de la position pour la line de separation
	rectTop_LineTitre = rect.bottom()+SPACE_BORDER;
	
	/// Content
	rect.translate( 0, SPACE_BORDER);
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
		// redef de la position 50%
		pos=WIDTH_DES;
		//DESIGNATION 40%
		/// TODO: TEST IMAGE PRINT DE PODUIT LIEES
		/*QSize sizeImage;
		if( !Ilist.picture.at(itemPrinted).isNull() ) {
			sizeImage = Ilist.picture.at(itemPrinted).size();
			Ilist.designation.replace(itemPrinted, Ilist.designation.at(itemPrinted)+"\n");
			painter.drawImage(mLeft+SPACE_BORDER,rect.top(), Ilist.picture.at(itemPrinted));
			rect.setHeight(sizeImage.height());
			rect.setWidth(sizeImage.width());
		}*/
		//TEXTE

		rect = painter.fontMetrics().boundingRect(mLeft+SPACE_BORDER,rect.top(), mwUtil*WIDTH_DES,0, Qt::AlignLeft/*Qt::TextWordWrap*/, Ilist.designation.at(itemPrinted) );
		rect.setWidth(mwUtil*WIDTH_DES); //fixe la largeur
		painter.drawText( rect,  Qt::AlignLeft/*Qt::TextWordWrap*/ , Ilist.designation.at(itemPrinted));
		
		//REMISE 8%
		if(discount){
			//TVA 8%
			if(m_data->getIsTax()){
				rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TAX,0, Qt::AlignRight, m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
			}
			pos += WIDTH_TAX;
			rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_DIS,0, Qt::AlignRight, QString::number(Ilist.discount.at(itemPrinted)) );
			//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
			painter.drawText( rect,  Qt::AlignRight , QString::number(Ilist.discount.at(itemPrinted)) );
		}
		else{
			pos += WIDTH_TAX;
			//TVA 8%
			if(m_data->getIsTax()){
				rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TAX,0, Qt::AlignRight, m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
				//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.tax.at(itemPrinted),'f',2) );
			}
		}
		pos += WIDTH_DIS;
		
		//PRIX UNITAIRE 12%
		rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_PRI,0, Qt::AlignRight, m_lang.toString(Ilist.price.at(itemPrinted),'f',2) );
		pos += WIDTH_PRI;
		//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
		painter.drawText( rect,  Qt::AlignRight , m_lang.toString(Ilist.price.at(itemPrinted),'f',2) );

		//QUANTITE 8%
		rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+mwUtil*pos,rect.top(), mwUtil*WIDTH_QTE,0, Qt::AlignRight, QString::number(Ilist.quantity.at(itemPrinted)) );
		pos += WIDTH_QTE;
		//rect.setWidth(wUtil*0.12 -5); //fixe la largeur
		painter.drawText( rect , Qt::AlignRight , QString::number(Ilist.quantity.at(itemPrinted)));

		//TOTAL 12%
		rect = painter.fontMetrics().boundingRect(mLeft-SPACE_BORDER+(mwUtil*pos),rect.top(), mwUtil*WIDTH_TOT,0, Qt::AlignRight, m_lang.toString(Ilist.totalPrice.at(itemPrinted), 'f', 2) );
		pos += WIDTH_TOT;
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

	// LIGNE de separation des TITRES
	painter.drawLine(QPoint(mRectContent.left(), rectTop_LineTitre),
					 QPoint(mRectContent.right(),rectTop_LineTitre));
	if(mDrawLine) {
		// RECTANGLE GENERAL
		painter.drawRoundedRect(mRectContent, mRoundedRect, mRoundedRect);
		// LIGNE de separation DISCOUNT
		if(discount){
			// LIGNE de separation TVA 12%
			if(m_data -> getIsTax()){
				painter.drawLine(QPoint(mLeft+mwUtil*WIDTH_DES, mRectContent.top()),
								 QPoint(mLeft+mwUtil*WIDTH_DES, mRectContent.bottom()) );
			}
			painter.drawLine(QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX), mRectContent.top()),
							 QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX), mRectContent.bottom()) );
		}
		else{
			// LIGNE de separation TVA 12%
			if(m_data -> getIsTax()){
				painter.drawLine(QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX), mRectContent.top()),
								 QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX), mRectContent.bottom()) );
			}
		}
		// LIGNE de separation PRIX UNITAIRE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS), mRectContent.top()),
						 QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS), mRectContent.bottom()) );
		// LIGNE de separation QUANTITE 12%
		painter.drawLine(QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS+WIDTH_PRI), mRectContent.top()),
						 QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS+WIDTH_PRI), mRectContent.bottom()) );
		// LIGNE de separation TOTAL 12%
		painter.drawLine(QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS+WIDTH_PRI+WIDTH_QTE), mRectContent.top()),
						 QPoint(mLeft+mwUtil*(WIDTH_DES+WIDTH_TAX+WIDTH_DIS+WIDTH_PRI+WIDTH_QTE), mRectContent.bottom()) );
	}
}

/**
 * @brief Printc::get_RecFooter
 * @return
 */
QRectF Printc::get_RecFooter(QPainter &painter) {
	QRectF rect;
	rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom/2, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignHCenter, mfooterTextInfo );
	rect.translate( 0, -rect.height());
	return rect;
}

/**
 * @brief Printc::print_footer
 * @param painter
 */
void Printc::print_footer(QPainter &painter, QRectF &rect, QString page, QString NbOfpage) {
	qreal ptSize = mFont.pointSizeF();
	mFont.setPointSizeF(ptSize-2);
	painter.setFont(mFont);

	/*QPen pen;// cree un pinceau
	pen.setWidthF(2.5);
	pen.setColor( Qt::darkGreen );
	painter.setPen(pen);*/
	// Ligne
	painter.drawLine(QPoint(mLeft, get_RecFooter(painter).top()) , QPoint(mLeft + mwUtil, get_RecFooter(painter).top()));
//	pen.setWidthF(PEN_WIDTH);
//	pen.setColor( Qt::black );
//	painter.setPen(pen);

	// Num de page
	if(!page.isEmpty()) {
		QString pageText = tr("Page ") + page + " / " + NbOfpage;
		rect = painter.fontMetrics().boundingRect(mLeft, get_RecFooter(painter).bottom()-SPACE_BORDER, mpageRect.width() - (mLeft+mRight), 0, Qt::AlignVCenter |Qt::AlignRight, pageText );
		rect.translate( 0, +rect.height());
		painter.drawText( rect, Qt::AlignVCenter |Qt::AlignRight, pageText);
	}
	//Information pied de page
	rect = get_RecFooter(painter);
	rect.translate( 0, 10);
	painter.drawText(rect , Qt::AlignHCenter, mfooterTextInfo);

	mFont.setPointSizeF(ptSize); painter.setFont(mFont);
}

/**
 * @brief Printc::print_total
 * @param painter
 * @param rect
 * @param totalPrice
 */
void Printc::print_total(QPainter &painter, QRectF &rect, itemList Ilist, int type) {
	qreal topRect = mpageRect.height() - mBottom - (mpageRect.height()*0.18);
	qreal ptSize = mFont.pointSizeF();
	qreal reste;
	mFont.setPointSizeF(ptSize-2);
	painter.setFont(mFont);
	//TOTAL
	mtotalPrice = mtotalTaxPrice = 0;
	for(int i=0; i<Ilist.totalPrice.size(); i++) {
		mtotalPrice += Ilist.totalPrice.at(i);
		mtotalTaxPrice += Ilist.totalPrice.at(i) + (Ilist.totalPrice.at(i)*Ilist.tax.at(i)/100.0);
	}
	
	qDebug() << "HT: " << mtotalPrice;
	qDebug() << "TTC: " << mtotalTaxPrice;
	
	QString total;
	if(m_data -> getIsTax()) {
		//Assujetti A TVA
		total = tr("TOTAL HT: ");
	}
	else {
		//Non assujetti A TVA
		if(m_data->m_customer->getType() == customer::COMPAGNY_TYPE)
			total = tr("TOTAL HT : ");
		else
			total = tr("TOTAL : ");
	}
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62), topRect, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, total );
	painter.drawText( rect, total);

	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62), topRect, mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(mtotalPrice, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(mtotalPrice, 'f', 2) );
	painter.setBrush( Qt::NoBrush );

	if(m_data -> getIsTax()) {
		//TOTAL TVA
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+10, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, tr("TOTAL TVA : ") );
		painter.drawText( rect, tr("TOTAL TVA : "));
		rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(mtotalTaxPrice-mtotalPrice, 'f', 2) );
		rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
		painter.drawText( rect,  Qt::AlignRight , m_lang.toString(mtotalTaxPrice-mtotalPrice, 'f', 2) );

		//TOTAL TTC
		rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+10, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, tr("TOTAL TTC : ") );
		painter.drawText( rect, tr("TOTAL TTC : "));
		rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(mtotalTaxPrice, 'f', 2) );
		rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
		painter.drawText( rect,  Qt::AlignRight , m_lang.toString(mtotalTaxPrice, 'f', 2) );
		//CALCUL DU RESTE
		if(type == T_INVOICE) {
			reste = mtotalTaxPrice-m_inv->getPartPaymentTax();
			//ACOMPTE
			if(m_inv->getPartPaymentTax()>0.0) {
				rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+10, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, tr("ACOMPTE : ") );
				painter.drawText( rect, tr("ACOMPTE : "));
				rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(m_inv->getPartPaymentTax(), 'f', 2) );
				rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(m_inv->getPartPaymentTax(), 'f', 2) );
			}
		}
		else{
			reste = mtotalTaxPrice;
		}
	}
	else{
		//CALCUL DU RESTE
		if(type == T_INVOICE) {
			reste = mtotalPrice-m_inv->getPartPayment();
			//ACOMPTE
			if(m_inv->getPartPayment()>0.0) {
				rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+10, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, tr("ACOMPTE : ") );
				painter.drawText( rect, tr("ACOMPTE : "));
				rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
				rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
				painter.drawText( rect,  Qt::AlignRight , m_lang.toString(m_inv->getPartPayment(), 'f', 2) );
			}
		}
		else{
			reste = mtotalPrice;
		}
	}
	
	mFont.setPointSizeF(ptSize);
	painter.setFont(mFont);
	
	//RESTE
	rect = painter.fontMetrics().boundingRect(mLeft+5+(mwUtil*0.62),rect.bottom()+SPACE_BORDER, mwUtil*WIDTH_TOTAUX,0, Qt::AlignLeft, tr("NET A PAYER : ") );
	//dessine le fond
	painter.setBrush( Qt::lightGray );
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62)-SPACE_BORDER,rect.top()-SPACE_BORDER, mwUtil*WIDTH_TOTAUX +SPACE_BORDER*2,rect.height()+SPACE_BORDER*2),  mRoundedRect, mRoundedRect);
	painter.setPen(Qt::black);
	painter.drawText( rect, tr("NET A PAYER : "));
	mFont.setBold(true);
	painter.setFont(mFont);
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX,0, Qt::AlignRight, m_lang.toString(reste, 'f', 2) );
	rect.setWidth(rect.width()+10); //Ajustement car le boundingRect ne prend pas en compte le font BOLD!!
	painter.drawText( rect,  Qt::AlignRight , m_lang.toString(reste, 'f', 2) );
	mFont.setBold(false);
	painter.setFont(mFont);
	painter.setBrush( Qt::NoBrush );
	
	//TOTAL TAX
	QString text;
	if(m_data -> getIsTax()) {
	}
	else{
		text = tr("TVA non applicable - Article 293 B du CGI");
		rect = painter.fontMetrics().boundingRect(mLeft, mpageRect.height() - mBottom - (mpageRect.height()*0.22) , mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}
}

/**
 * @brief Printc::print_reglement
 * @param painter
 * @param rect
 */
void Printc::print_reglement(QPainter &painter, QRectF &rect, const QString &typeP, const int &type) {
	qreal topRect = mpageRect.height() - mBottom - (mpageRect.height()*0.18);
	//Mode de reglement
	QString typePayment;
	if(typeP.isEmpty() || typeP.isNull()) typePayment="";
	if(typeP == MCERCLE::CASH)           typePayment = tr("Espece");
	if(typeP == MCERCLE::CHECK)          typePayment = tr("Cheque");
	if(typeP == MCERCLE::CREDIT_CARD)    typePayment = tr("Carte Bancaire");
	if(typeP == MCERCLE::INTERBANK)      typePayment = tr("TIP");
	if(typeP == MCERCLE::TRANSFER)       typePayment = tr("Virement");
	if(typeP == MCERCLE::DEBIT)          typePayment = tr("Prelevement");
	if(typeP == MCERCLE::OTHER)          typePayment = tr("Autre");
	qDebug() <<"PAIEMENT: "<< typeP << " "<< MCERCLE::TRANSFER;
	QString text = tr("Mode de règlement : ")+typePayment;
	rect = painter.fontMetrics().boundingRect(mLeft, topRect, mwUtil*0.50,0, Qt::AlignLeft, text );
	painter.drawText( rect, text );

	if((typeP == MCERCLE::INTERBANK)||(typeP == MCERCLE::DEBIT)){
		text = tr("Merci de nous fournir un RIB pour ce mode de règlement.");
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text );
		painter.drawText( rect, text );
	}

	//Condition de reglement
	if(type == T_PROPOSAL){
		text = tr("Conditions de règlement: 30% du montant total lors\nde la signature de cette proposition soit: ");
		text += m_lang.toString(mtotalTaxPrice * 0.3, 'f', 2) +" "+  m_data->getCurrency();
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.50,0, Qt::AlignLeft, text);
		painter.drawText( rect, text);
	}
	

	if(typeP == MCERCLE::TRANSFER){
		rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom() + SPACE_BORDER, mwUtil*0.5 +15,0, Qt::AlignLeft, mBankTextID );
		rect.setWidth(mwUtil*0.5); //fixe la largeur
		painter.drawText(rect, mBankTextID);
	}

	//Liste des accomptes
	qreal ptSize = mFont.pointSizeF();
	mFont.setPointSizeF(ptSize-1);
	painter.setFont(mFont);

	if(type == T_INVOICE) {
		// Y a til un acompte ?
		if((m_inv->getPartPayment() >0.0)||(m_inv->getPartPaymentTax() >0.0)) {
			text = tr("TOTAL des acomptes");
			if(m_data->getIsTax()) {
				text += tr("\nHT: ") + m_lang.toString(m_inv->getPartPayment(),'f',2);
				text += tr("\nTTC: ") + m_lang.toString(m_inv->getPartPaymentTax(),'f',2);
			}
			else {
				text += " : " + m_lang.toString(m_inv->getPartPayment(),'f',2);
			}
			rect = painter.fontMetrics().boundingRect(mLeft, rect.bottom()+5, mwUtil*0.36 +15,0, Qt::AlignLeft, text);
			painter.drawText(rect, text);
		}
	}

	mFont.setPointSizeF(ptSize); painter.setFont(mFont);
}

/**
 * @brief Printc::on_paintPrinterProposal
 * @param printer
 */
void Printc::on_paintPrinterProposal(QPrinter *printer) {
	QPainter painter;

	//Charge les parametres d'impression
	load_parameters(printer, painter);
	load_customerInfos();

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
		numberOfPage = qCeil((qreal)lineToPrint/mlinePerPage);
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
	print_total(painter, rect, printList, T_PROPOSAL);
	int bot = rect.bottom();
	// Imprime le mode de reglement
	print_reglement(painter, rect, m_pro -> getTypePayment(), T_PROPOSAL);

	/// Signature Client
	QString text = "Signature client:\n(Suivi de la mention \"bon pour accord\")\n\n\n\n";
	rect = painter.fontMetrics().boundingRect(mLeft+(mwUtil*0.62)+SPACE_BORDER, bot+SPACE_BORDER*2 /*mpageRect.height() - mBottom - mpageRect.height()*0.19 + SPACE_BORDER*16*/, 0, 0, Qt::AlignLeft, text );
	painter.drawText(rect, text);
	if(mDrawLine) painter.drawRoundedRect( QRect(mLeft+(mwUtil*0.62),rect.top(), mwUtil*WIDTH_TOTAUX +15, rect.height()), mRoundedRect, mRoundedRect);
	
	delete m_DialogWaiting;
	painter.end();
}


/**
 * @brief Printc::print_Invoice
 * @param id
 */
void Printc::print_Invoice(const int &id) {
	//On charge l objet en fonction de la selection
	m_inv -> loadFromID(id);

	QPrinter printer/*( QPrinter::HighResolution )*/;
	printer.setPageSize(QPrinter::A4);
	QString name = m_inv->getCode() ;
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");
	///TODO:Definition des marges en dur pour le moment
	//printer.setPageMargins(5.0,5.0,5.0,15.0, QPrinter::Millimeter);
	printer.setResolution(DPI);

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice->setModal(true);
	m_DialogPrintChoice->exec();

	if(m_DialogPrintChoice->result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinterInvoice(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		// Ajuste la taille
		QList<QPrintPreviewWidget *> list = m_PreviewDialog.findChildren<QPrintPreviewWidget *>();
		if(!list.isEmpty()) // paranoiac safety check
			list.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
		//Cache les boutons inutile de l orientation
		QToolBar *toolbar = m_PreviewDialog.findChild<QToolBar*>();
		if(toolbar) { // paranoiac safety check
			toolbar->removeAction( toolbar->actions().at(7) );
			toolbar->removeAction( toolbar->actions().at(7) );
		}
		m_PreviewDialog.exec();
	}
}

/**
 * @brief Printc::quickPrint_Invoice
 * @param id
 */
void Printc::quickPrint_Invoice(const int &id) {
	//On charge l objet en fonction de la selection
	m_inv -> loadFromID(id);

	QPrinter printer;
	QString name = m_inv->getCode() ;
	QWidget fenetre;
	QPrintDialog pDialog(&printer, &fenetre);
	if(pDialog.exec() == QDialog::Rejected) {
		return;
	}

	printer.setPageSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::NativeFormat);
	printer.setDocName( name );
	printer.setCreator("mcercle");
	printer.setResolution(DPI);
	printer.setDocName( name );
	printer.setCreator("mcercle");

	// Impression avec le painter
	on_paintPrinterInvoice(&printer);
}

/**
 * @brief Printc::quickPrintPDF_Invoice
 * @param id
 */
void Printc::quickPrintPDF_Invoice(const int &id) {
	//On charge l objet en fonction de la selection
	m_inv -> loadFromID(id);
	QPrinter printer;
	QString name = m_inv->getCode() ;

	printer.setPageSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setDocName( name );
	printer.setCreator("mcercle");
	printer.setResolution(DPI);
	printer.setDocName( name );
	printer.setCreator("mcercle");

	QString path_DataLocation;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	path_DataLocation = QDesktopServices::storageLocation ( QDesktopServices::HomeLocation );

#else
	// Qt5 gere un dosssier du nom de lapplication
	path_DataLocation = QStandardPaths::writableLocation ( QStandardPaths::HomeLocation );
#endif

	QString filename = QFileDialog::getSaveFileName(0, "Enregister sous... ",  path_DataLocation+"/"+name, "*.pdf");
	if( !filename.isEmpty() ) {
		if(QFile::exists ( filename )){
			// Si le fichier existe deja on supprime
			if(!QFile::remove ( filename ) ){
				QMessageBox::critical(0, tr("Erreur"),
									 tr("Impossible de remplacer le fichier!\n\n")+
									 "Destination: "+name+"\n");
				return;
			}
		}
		printer.setOutputFileName( filename );
		// Impression avec le painter
		on_paintPrinterInvoice(&printer);
	}

}

/**
 * @brief Printc::print_InvoicesList
 * @param listofId
 */
void Printc::print_InvoicesList(QList<int> listofId) {
	QPrinter printer/*( QPrinter::HighResolution )*/;
	printer.setResolution(DPI);
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
		printer.setPageMargins(5.0,5.0,5.0,15.0, QPrinter::Millimeter);
		// Boucle pour l impression multiple
		foreach(int id, listofId) {
			m_inv -> loadFromID(id);
			m_cus -> loadFromID( m_inv->getIdCustomer() );
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

	// charge les parametres d impression
	load_parameters(printer, painter);
	load_customerInfos();

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
	int numberOfPage = qCeil((qreal)lineToPrint/mlinePerLastPage);
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
		/// TODO: TEST IMAGE PRINT si produit liee on charge son image
		/*if(plist.idProduct.at(i)>0) {
			QImage pictureProd;
			m_data->m_product->loadFromID(plist.idProduct.at(i));
			if(m_data->m_product->getPicture().height() < 500){
				pictureProd = m_data->m_product->getPicture().scaled(500,500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
			printList.picture.push_back( pictureProd );
		}
		else{
			printList.picture.push_back(QImage());
		}*/
	}
	
	// Si facture Annulee
	if(m_inv -> getState() == MCERCLE::INV_CANCEL){
		painter.save();
		QPen pen;
		pen.setColor( QColor(201,106,22,255) );
		painter.setPen(pen);
		mFont.setPointSize(60);
		painter.setFont(mFont);
		rect = painter.fontMetrics().boundingRect(mRectContent.left(), mRectContent.bottom(), 0, 0, Qt::AlignLeft, tr("ANNULEE") );
		painter.rotate(-20);
		painter.drawText(rect, tr("ANNULEE"));
		painter.restore();

	}
	else if(m_inv -> getState() == MCERCLE::INV_OVERDUE){
		painter.save();
		QPen pen;
		pen.setColor( QColor(184,92,92,255));
		painter.setPen(pen);
		mFont.setPointSize(60);
		painter.setFont(mFont);
		rect = painter.fontMetrics().boundingRect(mRectContent.left(), mRectContent.bottom(), 0, 0, Qt::AlignLeft, tr("IMPAYEE") );
		painter.rotate(-20);
		painter.drawText(rect, tr("IMPAYEE"));
		painter.restore();

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
	print_total(painter, rect, printList, T_INVOICE);
	// Imprime le mode de reglement
	print_reglement(painter, rect, m_inv -> getTypePayment(), T_INVOICE);
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

	// charge les parametres d impression
	load_parameters(printer, painter);
	load_customerInfos();

	/// Header
	QRectF rect;
	print_header(painter, rect, T_SERVICE);
	/// contenu
	//contour du contenu
	int blockHeight;
	if(printer->orientation() == QPrinter::Landscape) blockHeight = 200;
	else blockHeight = 450;

	QString serv = tr("Intervention: ") + m_serv->getName()+"\n";
	serv += tr("Réalisé le : ") + m_serv->getDate().toString(tr("dd-MM-yyyy"))+ tr(" \xE0 ")+ m_serv->getDate().toString(tr("HH:mm")) + "\n\n";
	serv += tr("Détails:\n");
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
	if(mDrawLine) painter.drawRoundedRect(rect, mRoundedRect, mRoundedRect);

	/// Signature Client
	QString sign = "Signature client:\n\n\n\n\n";
	rect.translate( mwUtil/2, rect.height()+25);
	rect = painter.fontMetrics().boundingRect(mwUtil/2, rect.top(), 0, 0, Qt::AlignLeft, sign );
	painter.drawText(rect, sign);
	rect.translate(-SPACE_BORDER,-SPACE_BORDER);
	rect.setHeight(rect.height()+10);
	rect.setWidth(6 + mLeft + mwUtil/2);
	if(mDrawLine) painter.drawRoundedRect(rect, mRoundedRect, mRoundedRect);

	// Imprime le pied de page
	print_footer(painter, rect, "1","1");
	// Fin du painter
	painter.end();
}




/**
 * @brief print_fileEmpty: imprime un fiche à entête
 * @param id
 */
void Printc::print_fileEmpty() {
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	QString name = tr("Fiche");
	printer.setOutputFileName( name + ".pdf");
	printer.setDocName( name );
	printer.setCreator("mcercle");

	DialogPrintChoice *m_DialogPrintChoice = new DialogPrintChoice(&printer);
	m_DialogPrintChoice -> setModal(true);
	m_DialogPrintChoice -> exec();

	if(m_DialogPrintChoice -> result() == QDialog::Accepted) {
		QWidget fenetre;
		QPrintPreviewDialog m_PreviewDialog(&printer,  &fenetre, Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		connect(&m_PreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(on_paintPrinter_fileEmpty(QPrinter *)));
		m_PreviewDialog.setWindowState(Qt::WindowMaximized);
		// Ajuste la taille
		QList<QPrintPreviewWidget *> list = m_PreviewDialog.findChildren<QPrintPreviewWidget *>();
		if(!list.isEmpty()) { // paranoiac safety check
			list.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
		}
		//Cache les boutons inutile de l orientation
		QToolBar *toolbar = m_PreviewDialog.findChild<QToolBar*>();
		if(toolbar) { // paranoiac safety check
			toolbar->removeAction( toolbar->actions().at(7) );
			toolbar->removeAction( toolbar->actions().at(7) );
		}
		m_PreviewDialog.exec();
	}
}


/**
 * @brief Printc::on_paintPrinter_fileEmpty
 * @param printer
 */
void Printc::on_paintPrinter_fileEmpty(QPrinter *printer) {
	QPainter painter;

	//Charge les parametres d'impression
	load_parameters(printer, painter);
	/// Header
	QRectF rect;
	/// Imprime l entete
	print_header(painter, rect, T_FILE);
	/// Imprime le pied de page
	print_footer(painter, rect, QString::number(1), QString::number(1));

	painter.end();
}


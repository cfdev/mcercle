/**
  This file is a part of mcercle
  Copyright (C) 2010-2015 Cyril FRAUSTI

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

#include "invoice.h"
#include "dbase.h"
#include "mcercle.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QVariant>
#include <QDebug>

///TODO: IMPORTANT=rendre la class proposal derivable afin de reduire les redondances entre les 2 class
invoice::invoice(QSqlDatabase db, QWidget *parent): m_parent(parent) {
	m_db = db;

	m_id = m_idCustomer = m_state = m_type = m_idRef =0;
}

invoice::~invoice() {
}

/**
	Recevoir les icons des differents etats
*/
QIcon invoice::getIconState(int state) {
	switch(state){
		case MCERCLE::INV_UNPAID:return QIcon(":/app/warning");break;
		case MCERCLE::INV_PAID:return QIcon(":/app/ok");break;
		case MCERCLE::INV_OVERDUE:return  QIcon(":/app/Off");break;
		case MCERCLE::INV_CANCEL:return  QIcon(":/app/quit");break;
		default: break;
	}
	return QIcon("");
}

/**
	Recevoir les text des differents etats
*/
QString invoice::getTextState(int state) {
	switch(state){
		case MCERCLE::INV_UNPAID:return tr("Non Réglée");break;
		case MCERCLE::INV_PAID:return tr("Réglée");break;
		case MCERCLE::INV_OVERDUE:return tr("Impayée");break;
		case MCERCLE::INV_CANCEL:return tr("Annulée");break;
		default: break;
	}
	return "";
}

/**
	  Met les valeurs de defauts pour les variables de la class
	  @return true si ok
  */
void invoice::setDefault() {
	m_type = MCERCLE::TYPE_INV;
	m_state = 0;
	m_price = 0;
	m_partPayment = 0;
	m_partPaymentTax = 0;
	m_typePayment = "";

	m_state = m_type = m_idRef = 0;
	m_price = m_priceTax = m_partPayment = m_partPaymentTax =0;
	m_creationDate = QDateTime::currentDateTime();
	m_userDate = m_limitPayment = m_paymentDate = QDate::currentDate();
	m_code = m_proposalCode = m_description = m_typePayment = "";
}

/**
	  Ajoute un facture dans la base de donnee
	  @return true si ok
  */
bool invoice::create() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "INSERT INTO TAB_INVOICES(CREATIONDATE, ID_CUSTOMER, CODE, DATE, LIMIT_PAYMENTDATE, PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PART_PAYMENT_TAX, PRICE, PRICE_TAX, STATE, TYPE, ID_REFERENCE, DESCRIPTION) ";
	req += "VALUES(";
	req += "'" + QDateTime::currentDateTime().toString(tr("yyyy-MM-dd HH:mm:ss")) + "',";
	req += "'" + QString::number(m_idCustomer) + "',";
	req += "'" + m_code.replace("\'","''") + "',";
	req += "'" + m_userDate.toString(tr("yyyy-MM-dd")) + "',";
	req += "'" + m_paymentDate.toString(tr("yyyy-MM-dd")) + "',";
	req += "'" + m_limitPayment.toString(tr("yyyy-MM-dd")) + "',";
	req += "'" + m_typePayment + "',";
	req += "'" + f.setNum(m_partPayment,'f',2) + "',";
	req += "'" + f.setNum(m_partPaymentTax,'f',2) + "',";
	req += "'" + f.setNum(m_price,'f',2) + "',";
	req += "'" + f.setNum(m_priceTax,'f',2) + "',";
	req += "'" + QString::number(m_state)  + "',";
	req += "'" + QString::number(m_type)  + "',";
	req += "'" + QString::number(m_idRef)  + "',";
	req += "'" + m_description.replace("\'","''") + "');";

	qDebug() << req;
	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else{
		//Si facture dacompte on actualise la facture originale si besoin
		if(m_type == MCERCLE::TYPE_PART) {
			check_PartPayment();
		}
		return true;
	}
}

/**
	  Applique les informations courantes dans la base de donnee
	  ATTENTION ICI on update sur ID car le code est modifiable !!
	  @return true si ok
  */
bool invoice::update() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "UPDATE TAB_INVOICES SET ";
	req += "CODE='" + m_code.replace("\'","''") + "',";
	req += "DATE='" + m_userDate.toString(tr("yyyy-MM-dd")) + "',";
	req += "LIMIT_PAYMENTDATE='" + m_limitPayment.toString(tr("yyyy-MM-dd")) + "',";
	req += "PAYMENTDATE='" + m_paymentDate.toString(tr("yyyy-MM-dd")) + "',";
	req += "TYPE_PAYMENT='" + m_typePayment + "',";
	req += "PART_PAYMENT='" + f.setNum(m_partPayment,'f',2) + "',";
	req += "PART_PAYMENT_TAX='" + f.setNum(m_partPaymentTax,'f',2) + "',";
	req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
	req += "PRICE_TAX='" + f.setNum(m_priceTax,'f',2) + "',";
	req += "STATE='" + QString::number(m_state)  + "',";
	req += "DESCRIPTION='" + m_description.replace("\'","''") + "', ";
	req += "TYPE='" + QString::number(m_type)  + "',";
	req += "ID_REFERENCE='" + QString::number(m_idRef)  + "' ";
	req += "WHERE ID='"+ QString::number(m_id) +"';";

	qDebug() << req;
	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else{
		//Si facture dacompte on actualise la facture originale si besoin
		if(m_type == MCERCLE::TYPE_PART) {
			check_PartPayment();
		}
		return true;
	}
}

/**
	  Actualise la somme des factures d acompte
  */
void invoice::check_PartPayment() {

	//Sauvegarde de l id facture acompte
	int id_inv = m_id;
	int id_invRef = m_idRef;

	//Test si besoin d'actualiser le prix
	QString req = "SELECT TAB_INVOICES.PART_PAYMENT "
			"FROM TAB_INVOICES "
			"WHERE TAB_INVOICES.ID= '" + QString::number(id_invRef)+"' ";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		qreal acompte = query.value(query.record().indexOf("PART_PAYMENT")).toFloat();

		//Si acompte calculer diff de l'acompte facture on met a jour
		if(acompte != calcul_partPayment(id_invRef)) {
			//Charge la facture original
			loadFromID( id_invRef );
			//Calcul et met à jour le prix des acomptes
			m_partPayment = calcul_partPayment(id_invRef);
			m_partPaymentTax = calcul_partPaymentTax(id_invRef);
			update();
			//recharge la facture d'acompte
			loadFromID( id_inv );
		}
	}
}

/**
	  Suppression du facture de la bdd
	  ATTENTION ICI on supprimer le facture de la bdd avec les details associes
	  @return true si ok
  */
bool invoice::remove(){
	// Construction de la requette pour supprimer la facture
	QString req = "DELETE FROM TAB_INVOICES";
	req += " WHERE ID='"+ QString::number(m_id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), req+'\n'+query.lastError().text());
		return false;
	}
	else{
		// Si cest la derniere facture on rafraichit l'index
		if(getLastId() == m_id){
			req = "UPDATE SQLITE_SEQUENCE SET SEQ='"+QString::number(m_id-1)+"' WHERE name='TAB_INVOICES';";
			query.prepare(req);
			if(!query.exec()) {
				QMessageBox::critical(this->m_parent, tr("Erreur"), req+'\n'+query.lastError().text());
				return false;
			}
		}
		//Si facture dacompte on actualise la facture originale si besoin
		if(m_type == MCERCLE::TYPE_PART) {
			check_PartPayment();
		}
		return true;
	}
}

/**
	Charge la facture depuis son ID
	@param id invoice
	@return true si ok
  */
bool invoice::loadFromID(const int& id)
{
	QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.ID_CUSTOMER, TAB_INVOICES.CREATIONDATE, TAB_INVOICES.TYPE_PAYMENT, TAB_INVOICES.PART_PAYMENT, PART_PAYMENT_TAX, TAB_INVOICES.DATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.PAYMENTDATE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE, TAB_INVOICES.PRICE_TAX, TAB_INVOICES.STATE,TAB_INVOICES.TYPE,TAB_INVOICES.ID_REFERENCE "
			"FROM TAB_INVOICES "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"LEFT OUTER JOIN TAB_PROPOSALS "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"WHERE TAB_INVOICES.ID= '" + QString::number(id)+"' ";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id = id;
		m_idCustomer = query.value(query.record().indexOf("ID_CUSTOMER")).toInt();
		m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
		m_userDate = query.value(query.record().indexOf("DATE")).toDate();
		m_limitPayment = query.value(query.record().indexOf("LIMIT_PAYMENTDATE")).toDate();
		m_paymentDate = query.value(query.record().indexOf("PAYMENTDATE")).toDate();
		m_code = query.value(query.record().indexOf("ICODE")).toString();
		m_proposalCode = query.value(query.record().indexOf("PCODE")).toString();
		m_typePayment = query.value(query.record().indexOf("TYPE_PAYMENT")).toString();
		m_partPayment = query.value(query.record().indexOf("PART_PAYMENT")).toFloat();
		m_partPaymentTax = query.value(query.record().indexOf("PART_PAYMENT_TAX")).toFloat();
		m_price = query.value(query.record().indexOf("PRICE")).toFloat();
		m_priceTax = query.value(query.record().indexOf("PRICE_TAX")).toFloat();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		m_state = query.value(query.record().indexOf("STATE")).toInt();
		m_type = query.value(query.record().indexOf("TYPE")).toInt();
		m_idRef = query.value(query.record().indexOf("ID_REFERENCE")).toInt();
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	Charge la facture depuis son code
	@param code
	@return true si ok
  */
bool invoice::loadFromCode(const QString& code)
{
	QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.ID_CUSTOMER, TAB_INVOICES.CREATIONDATE, TAB_INVOICES.TYPE_PAYMENT, TAB_INVOICES.PART_PAYMENT, PART_PAYMENT_TAX, TAB_INVOICES.DATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.PAYMENTDATE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE, TAB_INVOICES.PRICE_TAX, TAB_INVOICES.STATE, TAB_INVOICES.TYPE,TAB_INVOICES.ID_REFERENCE "
			"FROM TAB_INVOICES "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"LEFT OUTER JOIN TAB_PROPOSALS "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"WHERE TAB_INVOICES.CODE = '" + code +"' ";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id= query.value(query.record().indexOf("ID")).toInt();
		m_idCustomer = query.value(query.record().indexOf("ID_CUSTOMER")).toInt();
		m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
		m_userDate = query.value(query.record().indexOf("DATE")).toDate();
		m_limitPayment = query.value(query.record().indexOf("LIMIT_PAYMENTDATE")).toDate();
		m_paymentDate = query.value(query.record().indexOf("PAYMENTDATE")).toDate();
		m_code = query.value(query.record().indexOf("ICODE")).toString();
		m_proposalCode = query.value(query.record().indexOf("PCODE")).toString();
		m_typePayment = query.value(query.record().indexOf("TYPE_PAYMENT")).toString();
		m_partPayment = query.value(query.record().indexOf("PART_PAYMENT")).toFloat();
		m_partPaymentTax = query.value(query.record().indexOf("PART_PAYMENT_TAX")).toFloat();
		m_price = query.value(query.record().indexOf("PRICE")).toFloat();
		m_priceTax = query.value(query.record().indexOf("PRICE_TAX")).toFloat();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		m_state = query.value(query.record().indexOf("STATE")).toInt();
		m_type = query.value(query.record().indexOf("TYPE")).toInt();
		m_idRef = query.value(query.record().indexOf("ID_REFERENCE")).toInt();
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}

/**
	 Permet de savoir si un facture est deja present dans la base
	 @param code facture
	 @return vrai si le facture existe
  */
bool invoice::isHere(const QString& code) {
	int count;
	QString req = "SELECT COUNT(*) FROM TAB_INVOICES AS PCOUNT WHERE UPPER(CODE)=UPPER('";
	req += code + "' );";
	if(code.isEmpty())return false;

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		count = query.value(query.record().indexOf("COUNT(*)")).toInt();
		if(count>0)return true;
		else return false;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Permet de d obtenir le dernier id genere
	 @return l id
  */
int invoice::getLastId(){
	int iVal=-1;
	QString req;
	if(m_db.driverName() == "QSQLITE")
			req = "SELECT seq FROM sqlite_sequence WHERE name=\"TAB_INVOICES\";";
	else	req = "SHOW TABLE STATUS LIKE 'TAB_INVOICES\';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		if(m_db.driverName() == "QSQLITE")
			iVal = query.value(query.record().indexOf("seq")).toInt();
		else
			iVal = query.value(query.record().indexOf("Auto_increment")).toInt()-1;
		return iVal;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return -1;
	}
}

/**
	 Liste les donnees du champ en fonction du filtre
	 @param InvoiceList liste de retour, ID du client
	 @return valeur de retour sous forme de liste de chaine de type InvoiceList
  */
bool invoice::getInvoiceList(InvoiceList& list, int id_customer, QString order, QString filter, QString field) {

	QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.DATE, TAB_INVOICES.LIMIT_PAYMENTDATE, PAYMENTDATE, TAB_INVOICES.DESCRIPTION, "
			"TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.PRICE,TAB_INVOICES.PRICE_TAX, TAB_INVOICES.PART_PAYMENT, TAB_INVOICES.PART_PAYMENT_TAX, TAB_INVOICES.STATE "
			"FROM TAB_INVOICES "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"LEFT OUTER JOIN TAB_PROPOSALS "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"WHERE TAB_INVOICES.ID_CUSTOMER = '" + QString::number(id_customer)+"' ";

	if(!field.isEmpty()){
		req += " WHERE UPPER(";
		req += field;
		req += " ) LIKE UPPER('";
		req += filter;
		req += "%' )";
	}
	req += " ORDER BY UPPER("+order+") DESC;";

	qDebug() << "invoice::getInvoiceList\n" << req;

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.code << query.value(query.record().indexOf("ICODE")).toString();
			list.codeProposal << query.value(query.record().indexOf("PCODE")).toString();
			list.userDate.push_back( query.value(query.record().indexOf("DATE")).toDate() );
			list.limitPayment.push_back( query.value(query.record().indexOf("LIMIT_PAYMENTDATE")).toDate());
			list.paymentDate.push_back( query.value(query.record().indexOf("PAYMENTDATE")).toDate());
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
			list.priceTax.push_back( query.value(query.record().indexOf("PRICE_TAX")).toFloat() );
			list.part_payment.push_back( query.value(query.record().indexOf("PART_PAYMENT")).toFloat() );
			list.part_paymentTax.push_back( query.value(query.record().indexOf("PART_PAYMENT_TAX")).toFloat() );
			list.state.push_back( query.value(query.record().indexOf("STATE")).toInt() );
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}

/**
	 Liste les factures reglees! pour le livre des recettes en fonction du mois
	 @param InvoicesBook liste de retour
	 @return valeur de retour sous forme de liste de chaine de type InvoiceList
  */
bool invoice::getInvoices(InvoicesBook& list, QString year, QString month) {

	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";

	if(month.size()<2)month = '0'+month;
	QString req =   "SELECT TAB_INVOICES.ID, TAB_INVOICES.CODE, TAB_CUSTOMERS.FIRSTNAME, TAB_CUSTOMERS.LASTNAME, TAB_INVOICES.DATE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE,TAB_INVOICES.PRICE_TAX, TAB_INVOICES.PART_PAYMENT, PART_PAYMENT_TAX, TAB_INVOICES.TYPE_PAYMENT "
					"FROM TAB_INVOICES "
					"LEFT OUTER JOIN TAB_CUSTOMERS "
					"ON TAB_INVOICES.ID_CUSTOMER = TAB_CUSTOMERS.ID "
					"WHERE TAB_INVOICES.STATE == '1' AND "
					"strftime('%m',TAB_INVOICES."+effective_date+")='"+month+"'AND "
					"strftime('%Y',TAB_INVOICES."+effective_date+")='"+year+"' "
					"ORDER BY TAB_INVOICES."+effective_date+" ASC; ";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.code << query.value(query.record().indexOf("CODE")).toString();
			list.customerFirstName << query.value(query.record().indexOf("FIRSTNAME")).toString();
			list.customerLastName << query.value(query.record().indexOf("LASTNAME")).toString();
			list.userDate.push_back( query.value(query.record().indexOf("DATE")).toDate() );
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
			list.priceTax.push_back( query.value(query.record().indexOf("PRICE_TAX")).toFloat() );
			list.part_payment.push_back( query.value(query.record().indexOf("PART_PAYMENT")).toFloat() );
			list.part_paymentTax.push_back( query.value(query.record().indexOf("PART_PAYMENT_TAX")).toFloat() );
			list.typePayment << query.value(query.record().indexOf("TYPE_PAYMENT")).toString();
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Retour la liste des factures en alert
	 @param InvoiceList liste de retour
	 @return valeur de retour sous forme de liste de chaine de type InvoiceList
  */
bool invoice::getInvoiceListAlert(InvoiceListAlert& list) {

	QString req =   "SELECT TAB_CUSTOMERS.ID AS C_ID, TAB_CUSTOMERS.FIRSTNAME, TAB_CUSTOMERS.LASTNAME, TAB_INVOICES.ID, TAB_INVOICES.DATE, TAB_INVOICES.LIMIT_PAYMENTDATE, PAYMENTDATE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.CODE, TAB_INVOICES.PRICE,TAB_INVOICES.PRICE_TAX, TAB_INVOICES.PART_PAYMENT, TAB_INVOICES.STATE "
					"FROM TAB_INVOICES "
					"LEFT OUTER JOIN TAB_CUSTOMERS "
					"ON TAB_INVOICES.ID_CUSTOMER = TAB_CUSTOMERS.ID "
					"WHERE TAB_INVOICES.STATE = '0' "
					"ORDER BY UPPER(TAB_INVOICES.DATE) ASC; ";
	//Liste les factures en alert avec les plus vielles en premier
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.customerId << query.value(query.record().indexOf("C_ID")).toInt();
			list.customerFirstName << query.value(query.record().indexOf("FIRSTNAME")).toString();
			list.customerLastName << query.value(query.record().indexOf("LASTNAME")).toString();
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.userDate.push_back( query.value(query.record().indexOf("DATE")).toDate() );
			list.limitPayment.push_back( query.value(query.record().indexOf("LIMIT_PAYMENTDATE")).toDate() );
			list.paymentDate.push_back( query.value(query.record().indexOf("PAYMENTDATE")).toDate() );
			list.code.push_back( query.value(query.record().indexOf("CODE")).toString());
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
			list.priceTax.push_back( query.value(query.record().indexOf("PRICE_TAX")).toFloat() );
			list.part_payment.push_back( query.value(query.record().indexOf("PART_PAYMENT")).toFloat() );
			list.state.push_back( query.value(query.record().indexOf("STATE")).toInt() );

		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Listes des articles de la facture
	 @return valeur de retour sous forme de liste de chaine de type InvoiceItem
  */
bool invoice::getInvoiceItemsList(InvoiceListItems& list, QString order, QString filter, QString field) {

	QString req = "SELECT * FROM TAB_INVOICES_DETAILS WHERE ID_INVOICE=" + QString::number(this->m_id)+" ";
	if(!field.isEmpty()){
		req += " WHERE UPPER(";
		req += field;
		req += " ) LIKE UPPER('";
		req += filter;
		req += "%' )";
	}
	req += " ORDER BY CAST("+order+" AS int) ASC;";

	/* Clear les vals */
	list.id.clear();
	list.name.clear();
	list.discount.clear();
	list.quantity.clear();
	list.tax.clear();
	list.price.clear();
	list.order.clear();
	
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.idProduct.push_back( query.value(query.record().indexOf("ID_PRODUCT")).toInt() );
			list.name << query.value(query.record().indexOf("NAME")).toString();
			list.discount.push_back( query.value(query.record().indexOf("DISCOUNT")).toInt() );
			list.quantity.push_back( query.value(query.record().indexOf("QUANTITY")).toInt());
			list.tax.push_back( query.value(query.record().indexOf("TAX")).toFloat() );
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
			list.order.push_back(  query.value(query.record().indexOf("ITEM_ORDER")).toInt() );
			list.type.push_back(  query.value(query.record().indexOf("TYPE")).toInt() );
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
  Obtenir le detail dun article

  @param id de l article
  @return valeur de retour sous forme de liste de chaine de type InvoiceItem
  */
bool invoice::getInvoiceItem(InvoiceItem& item){

	QString req =   "SELECT * FROM TAB_INVOICES_DETAILS WHERE ID_INVOICE="
					+ QString::number(this->m_id)+" AND ID="
					+ QString::number(item.id)+" ;";

	/* Clear les vals */
	item.id = -1;
	item.name="";
	item.discount=0;
	item.quantity=0;
	item.tax=0;
	item.price=0;
	item.order=0;
	
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		item.id = query.value(query.record().indexOf("ID")).toInt();
		item.idProduct = query.value(query.record().indexOf("ID_PRODUCT")).toInt();
		item.name = query.value(query.record().indexOf("NAME")).toString();
		item.discount = query.value(query.record().indexOf("DISCOUNT")).toInt();
		item.quantity = query.value(query.record().indexOf("QUANTITY")).toInt();
		item.tax = query.value(query.record().indexOf("TAX")).toFloat();
		item.price = query.value(query.record().indexOf("PRICE")).toFloat();
		item.order = query.value(query.record().indexOf("ITEM_ORDER")).toInt();
		item.type = query.value(query.record().indexOf("TYPE")).toInt();
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
  Ajoute un article
  @return true si ok
  */
bool invoice::addInvoiceItem(InvoiceItem& item){
	QString f;
	// Construction de la requette
	QString req = "INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, ID_PRODUCT, TYPE, NAME, QUANTITY, DISCOUNT, PRICE, TAX, ITEM_ORDER) ";
	req += "VALUES(";
	req += "'" + QString::number(this->m_id) + "',";
	req += "'" + QString::number(item.idProduct) + "',";
	req += "'" + QString::number(item.type) + "',";
	req += "'" + item.name.replace("\'","''") + "',";
	req += "'" + QString::number(item.quantity)  + "',";
	req += "'" + QString::number(item.discount)  + "',";
	req += "'" + f.setNum(item.price,'f',2) + "',";
	req += "'" + f.setNum(item.tax,'f',2) + "',";
	req += "'" + QString::number(item.order) + "');";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}

/**
  Suppression de l article
  @return true si ok
  */
bool invoice::removeInvoiceItem(int Itemid){
	// Construction de la requette
	QString req = "DELETE FROM TAB_INVOICES_DETAILS WHERE ID_INVOICE="
			+ QString::number(this->m_id)+" AND ID="
			+ QString::number(Itemid)+" ;";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}

/**
	Modifie les valeurs de l article
	@return true si ok
  */
bool invoice::updateInvoiceItem(InvoiceItem& item) {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "UPDATE TAB_INVOICES_DETAILS SET ";
	req += "NAME='" + item.name.replace("\'","''") + "',";
	req += "ID_PRODUCT='" + QString::number(item.idProduct)  + "',";
	req += "TYPE='" + QString::number(item.type) + "', ";
	req += "DISCOUNT='" + QString::number(item.discount)  + "',";
	req += "QUANTITY='" + QString::number(item.quantity)  + "',";
	req += "TAX='" + f.setNum(item.tax,'f',2) + "',";
	req += "PRICE='" + f.setNum(item.price,'f',2) + "', ";
	req += "ITEM_ORDER='" + QString::number(item.order) + "' ";
	req += "WHERE ID="+ QString::number(item.id) +";";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}

/**
  Ajoute un article
  @return true si ok
  */
bool invoice::addInvoiceItems(InvoiceListItems& items){
	InvoiceItem item;

	for(int i = 0; i<items.id.count(); i++) {
		item.id         = items.id.at(i);
		item.idProduct  = items.idProduct.at(i);
		item.type       = items.type.at(i);
		item.name       = items.name.at(i);
		item.quantity   = items.quantity.at(i);
		item.discount   = items.discount.at(i);
		item.price      = items.price.at(i);
		item.tax        = items.tax.at(i);
		item.order      = items.order.at(i);

		if( !addInvoiceItem( item ) ) {
			return false;
		}
	}
	return true;
}

/**
	 Retour la liste des annees de facture
	 @return une liste d annees
	 ex: 2011,2012...
  */
QStringList invoice::getYearsList() {
	QStringList ylist;
	QString req;
	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";
	
	if(m_db.driverName() == "QSQLITE")
		req =	"SELECT DISTINCT YEARS FROM (SELECT strftime('%Y',TAB_INVOICES."+effective_date+") AS YEARS FROM TAB_INVOICES) "
				"ORDER BY UPPER(YEARS) DESC;";
	else
		req =	"SELECT DISTINCT(EXTRACT(YEAR FROM TAB_INVOICES."+effective_date+")) AS YEARS "
				"FROM TAB_INVOICES ORDER BY UPPER("+effective_date+") DESC;";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			ylist.push_back( query.value(query.record().indexOf("YEARS")).toString() );
		}
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}
	return ylist;
}

/**
	 Retourne le CA d une annee
	 @return CA de l annee desiree
  */
qreal invoice::getYearRevenue(QString year) {
	qreal total=0;
	if(year.isEmpty())return 0;
	QString req;
	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";

	if(m_db.driverName() == "QSQLITE")
		req =	"SELECT SUM( round(price,2) - round(part_payment,2) ) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"strftime('%Y',TAB_INVOICES."+effective_date+")='"+year+"';";
	else
		req =	"SELECT SUM( round(price,2) - round(part_payment,2) ) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"extract(YEAR FROM TAB_INVOICES."+effective_date+")='"+year+"';";

	qDebug() << "invoice::getYearRevenue: " << req;
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		total = query.value(query.record().indexOf("TOTAL")).toDouble();

	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}

	if(total < 0)total = 0;
	return total;
}

/**
	 Retourne le CA d une annee en TTC
	 @return CA de l annee desiree en TTC
  */
qreal invoice::getYearRevenueTAX(QString year) {
	qreal total=0;
	if(year.isEmpty())return 0;
	QString req;
	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";

	if(m_db.driverName() == "QSQLITE")
		req =	"SELECT SUM( round(price_tax,2) - round(part_payment_tax,2) ) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"strftime('%Y',TAB_INVOICES."+effective_date+")='"+year+"';";
	else
		req =	"SELECT SUM( round(price_tax,2) - round(part_payment_tax,2) ) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"extract(YEAR FROM TAB_INVOICES."+effective_date+")='"+year+"';";

	qDebug() << "invoice::getYearRevenueTAX: " << req;
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		total = query.value(query.record().indexOf("TOTAL")).toDouble();

	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}

	if(total < 0)total = 0;
	return total;
}

/**
	 Retourne le CA d un mois
	 @return CA du mois desiree
  */
qreal invoice::getMonthRevenue(QString year, QString month) {
	qreal total=0; // important init pour le nonReturnValue
	if(year.isEmpty() | month.isEmpty())return 0;
	QString req;
	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";
	if(m_db.driverName() == "QSQLITE"){
		if(month.size()<2)month = '0'+month;
		req =	"SELECT round(SUM( price - part_payment), 2) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"strftime('%m',TAB_INVOICES."+effective_date+")='"+month+"'and "
				"strftime('%Y',TAB_INVOICES."+effective_date+")='"+year+"';";
	}
	else
		req =	"SELECT round(SUM( price - part_payment), 2) AS TOTAL FROM TAB_INVOICES "
				"WHERE TAB_INVOICES.STATE = '1' and "
				"extract(MONTH FROM TAB_INVOICES."+effective_date+")='"+month+"'and "
				"extract(YEAR FROM TAB_INVOICES."+effective_date+")='"+year+"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		total = query.value(query.record().indexOf("TOTAL")).toFloat();

	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}

	return total;
}

/**
	 Retourne le CA service d un mois
	 @return CA service du mois desiree
  */
qreal invoice::getMonthServiceRevenue(QString year, QString month) {
	qreal total=0; // important init pour le nonReturnValue
	if(year.isEmpty() | month.isEmpty())return 0;

	//recupère la liste des ID factures pour le mois
	QList <int> idList = listofID_Invoices( year, month);

	// On parcours la liste des ID
	for( int i=0; i<idList.count(); i++ ) {
		total += calcul_services( idList[i] ) - calcul_partPaymentService( idList[i] );
	}

	return total;
}

/**
	 Retourne le CA Produit d un mois
	 @return CA Produit du mois desiree
  */
qreal invoice::getMonthProductRevenue(QString year, QString month) {
	qreal total=0; // important init pour le nonReturnValue
	if(year.isEmpty() | month.isEmpty())return 0;

	//recupère la liste des ID factures pour le mois
	QList <int> idList = listofID_Invoices( year, month);

	// On parcours la liste des ID
	for( int i=0; i<idList.count(); i++ ) {
		total += calcul_products( idList[i] ) - calcul_partPaymentProduct( idList[i] );
	}

	return total;
}

/**
    Obtenir la methode de calcul du CA
    @return catype
  */
int invoice::getCaType() {
	int ret=-1;
	QSqlQuery query;
	query.prepare("SELECT CA_TYPE FROM TAB_INFORMATIONS;");

	if(query.exec()){
		query.next();
		ret = query.value(0).toInt();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}
	return ret;
}

/**
	 Permet de savoir combien de facture il y a dans la bdd
	 @param id du clients
	 @return le nombre de facture client
  */
int invoice::count(int id_customer) {
	int count=0;
	//Requete 1 nombre de valeur
	QString req = "SELECT COUNT(*) FROM (SELECT * FROM TAB_INVOICES";
				"WHERE TAB_INVOICES.ID_CUSTOMER = '" + QString::number(id_customer)+"' ";

	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		count = query.value(query.record().indexOf("COUNT(*)")).toInt();
		return count;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return 0;
	}
}

/**
 * @brief récupère la liste des factures réglés dun mois/annee desire
 * @param year
 * @param month
 * @return list des ID facture
 */
QList<int> invoice::listofID_Invoices(QString year, QString month) {
	QList<int> idList;
	QString effective_date;
	if(getCaType() == 1)
		effective_date = "PAYMENTDATE";
	else
		effective_date = "DATE";

	if(month.size()<2)month = '0'+month;
	//récupère tous les ID des factures d'acompte
	QString req =	"SELECT ID FROM TAB_INVOICES"
					" WHERE TAB_INVOICES.STATE = '1' "
					" AND strftime('%m',TAB_INVOICES."+effective_date+")='"+month+"' "
					" AND strftime('%Y',TAB_INVOICES."+effective_date+")='"+year+"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			idList.push_back( query.value(query.record().indexOf("ID")).toInt() );
		}
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());

	}
	return idList;
}


/**
 * @brief invoice::calcul_price
 * @param id_invoice
 * @return calcul la valeur de la facture en HT en fonction des artilces, tax, remises, qte
 */
qreal invoice::calcul_price(int id){

	QString req =	"select SUM("
			" round("
			" (tab_invoices_details.quantity*tab_invoices_details.price)-"
			" (tab_invoices_details.quantity*tab_invoices_details.price*tab_invoices_details.discount/100.0)"
			" ,2)"
			" )"
			" AS TTC"
			" from tab_invoices_details"
			" where tab_invoices_details.id_invoice="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);

	qreal price_ttc=0;
	if(query.exec()){
		query.next();
		price_ttc = query.value(query.record().indexOf("TTC")).toDouble();
		return price_ttc;
	}
	else{
		return 0;
	}
}


/**
 * @brief invoice::calcul_priceTax
 * @param id_invoice
 * @return calcul la valeur de la facture en TTC en fonction des artilces, tax, remises, qte
 */
qreal invoice::calcul_priceTax(int id){

	QString req =	"select SUM("
			" round("
			" tab_invoices_details.quantity*(tab_invoices_details.price+(tab_invoices_details.price*tab_invoices_details.tax/100.0))-"
			" tab_invoices_details.quantity*(tab_invoices_details.price+(tab_invoices_details.price*tab_invoices_details.tax/100.0))*tab_invoices_details.discount/100.0"
			" ,2)"
			" )"
			" AS TTC"
			" from tab_invoices_details"
			" where tab_invoices_details.id_invoice="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);

	qreal price_ttc=0;
	if(query.exec()){
		query.next();
		price_ttc = query.value(query.record().indexOf("TTC")).toDouble();
		return price_ttc;
	}
	else{
		return 0;
	}
}

/**
 * @brief Calcul le montant total des services pour l'acompte
 * @param id facture
 * @return montant total des services acompte
 */
qreal invoice::calcul_partPaymentService(int id) {
	QList<int> idList;
	qreal serv=0;
	//récupère tous les ID des factures d'acompte
	QString req =	"select ID from tab_invoices"
					" where tab_invoices.type = 1"
					" and tab_invoices.id_reference="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			idList.push_back( query.value(query.record().indexOf("ID")).toInt() );
		}
		// On parcours la liste des ID
		for( int i=0; i<idList.count(); i++ ) {
			serv += calcul_services( idList[i] );
		}
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());

	}
	return serv;
}

/**
 * @brief Calcul le montant total des produits pour l'acompte
 * @param id facture
 * @return montant total des produits acompte
 */
qreal invoice::calcul_partPaymentProduct(int id){
	QList<int> idList;
	qreal prod=0;
	//récupère tous les ID des factures d'acompte
	QString req =	"select ID from tab_invoices"
					" where tab_invoices.type = 1"
					" and tab_invoices.id_reference="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			idList.push_back( query.value(query.record().indexOf("ID")).toInt() );
		}
		// On parcours la liste des ID
		for( int i=0; i<idList.count(); i++ ) {
			prod += calcul_products( idList[i] );
		}
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());

	}
	return prod;
}

/**
 * @brief invoice::calcul_partPayment
 * @param id_invoice
 * @return calcul la valeur de l'acompte sur facture
 */
qreal invoice::calcul_partPayment(int id){

	QString req =	"select SUM("
			" round("
			" tab_invoices.price"
			" ,2)"
			" )"
			" AS ACOMPTE"
			" from tab_invoices"
			" where tab_invoices.type=1 AND tab_invoices.id_reference="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		return query.value(query.record().indexOf("ACOMPTE")).toDouble();;
	}
	else{
		return 0;
	}
}

/**
 * @brief invoice::calcul_partPaymentTax
 * @param id_invoice
 * @return calcul la valeur de l'acompte ttc sur facture
 */
qreal invoice::calcul_partPaymentTax(int id){

	QString req =	"select SUM("
			" round("
			" tab_invoices.price_tax"
			" ,2)"
			" )"
			" AS ACOMPTE"
			" from tab_invoices"
			" where tab_invoices.type=1 AND tab_invoices.id_reference="+QString::number(id);

	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		return query.value(query.record().indexOf("ACOMPTE")).toDouble();;
	}
	else{
		return 0;
	}
}

/**
 * @brief invoice::calcul_services - calcul el montant total HT des services
 * @param id facture
 * @return montant total HT des services
 */
qreal invoice::calcul_services(int id){

	QString req = "SELECT round( SUM(( TAB_INVOICES_DETAILS.PRICE-(TAB_INVOICES_DETAILS.PRICE*TAB_INVOICES_DETAILS.DISCOUNT/100.0)) * TAB_INVOICES_DETAILS.QUANTITY), 2)"
			" AS SERVICE"
			" FROM TAB_INVOICES_DETAILS"
			" WHERE TAB_INVOICES_DETAILS.ID_INVOICE = "+QString::number(id)+
			" AND TAB_INVOICES_DETAILS.TYPE = 0;";

	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		return query.value(query.record().indexOf("SERVICE")).toDouble();
	}
	else{
		return 0;
	}
}

/**
 * @brief calcul_products -  calcul el montant total HT des produits
 * @param id facture
 * @return montant total HT des produits
 */
qreal invoice::calcul_products(int id){

	QString req = "SELECT round( SUM(( TAB_INVOICES_DETAILS.PRICE-(TAB_INVOICES_DETAILS.PRICE*TAB_INVOICES_DETAILS.DISCOUNT/100.0)) * TAB_INVOICES_DETAILS.QUANTITY), 2)"
			" AS PRODUCT"
			" FROM TAB_INVOICES_DETAILS"
			" WHERE TAB_INVOICES_DETAILS.ID_INVOICE = "+QString::number(id)+
			" AND TAB_INVOICES_DETAILS.TYPE = 1;";

	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		return query.value(query.record().indexOf("PRODUCT")).toDouble();;
	}
	else{
		return 0;
	}
}










/**
 * @brief invoice::isTypeExiste
 * @return true if type already exist
 */
bool invoice::isTypeExiste(const int& type) {
	int count=0;
	QString req =	"SELECT COUNT(*) FROM tab_invoices AS ICOUNT "
					"WHERE type = "+QString::number(type)+" "
					"AND id_reference = "+QString::number(m_id)+";";

	qDebug()<<req;
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		count = query.value(query.record().indexOf("COUNT(*)")).toInt();
		if(count>0)return true;
		else return false;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}

/**
 * @brief recupere le code de la facture associee
 * @return Code de la facture
 */
QString invoice::getCodeInvoice_Ref(){

	QString req =	"select tab_invoices.code from tab_invoices "
					"where tab_invoices.id ="+QString::number(m_idRef);

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		return query.value(query.record().indexOf("CODE")).toString();
	}
	else{
		return "";
	}
}

/**
 * @brief genere un nouveau code de devis l'applique à la class et retourn la valeur
 * @return code
 */
QString invoice::generateNewCode() {

	//Generation du code
	// TYPE + DATE + ID
	QString typp = tr("FA");
	setCode( typp + QDateTime::currentDateTime().toString("yyMM")
			 + QString::number(getLastId()+1) );
	return m_code;
}



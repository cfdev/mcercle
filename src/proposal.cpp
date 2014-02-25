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

#include "proposal.h"
#include "dbase.h"
#include "mcercle.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QVariant>

proposal::proposal(QSqlDatabase db, QWidget *parent): m_parent(parent) {
	m_db = db;

	m_id = 0;
	m_idCustomer = 0;
	m_state = 0;
	m_delayDeliveryDate = 0;
	m_price = 0;
}

proposal::~proposal() {
}

/**
	Recevoir les icons des differents etats
*/
QIcon proposal::getIconState(int state)
{
	switch(state){
		case MCERCLE::PROPOSAL_WRITING:return QIcon(":/app/process");break;
		case MCERCLE::PROPOSAL_PROPOSED:return QIcon(":/app/wait");break;
		case MCERCLE::PROPOSAL_VALIDATED:return QIcon(":/app/ok");break;
		default: break;
	}
	return QIcon("");
}

/**
	Recevoir les text des differents etats
*/
QString proposal::getTextState(int state)
{
	switch(state){
		case MCERCLE::PROPOSAL_WRITING:return tr("En ecriture");break;
		case MCERCLE::PROPOSAL_PROPOSED:return tr("Propose");break;
		case MCERCLE::PROPOSAL_VALIDATED:return tr("Signe");break;
		default: break;
	}
	return "";
}


/**
	  Ajoute un facture dans la base de donnee
	  @return true si ok
  */
bool proposal::create() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "INSERT INTO TAB_PROPOSALS(CREATIONDATE, ID_CUSTOMER, CODE, DATE, VALIDDATE, DELIVERYDATE, DELAY_DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION) ";
	req += "VALUES(";
	req += "'" + QDateTime::currentDateTime().toString(tr("yyyy/MM/dd-HH:mm:ss")) + "',";
	req += "'" + QString::number(m_idCustomer) + "',";
	req += "'" + m_code.replace("\'","''") + "',";
	req += "'" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "'" + m_validDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "'" + m_deliveryDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "'" + QString::number(m_delayDeliveryDate)  + "',";
	req += "'" + m_typePayment + "',";
	req += "'" + f.setNum(m_price,'f',2) + "',";
	req += "'" + QString::number(m_state)  + "',";
	req += "'" + m_description.replace("\'","''") + "');";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}


/**
	  Applique les informations courantes dans la base de donnee
	  ATTENTION ICI on update sur ID car le code est modifiable !!
	  @return true si ok
  */
bool proposal::update() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "UPDATE TAB_PROPOSALS SET ";
	req += "CODE='" + m_code.replace("\'","''") + "',";
	req += "DATE='" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "VALIDDATE='" + m_validDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "DELIVERYDATE='" + m_deliveryDate.toString(tr("yyyy/MM/dd")) + "',";
	req += "DELAY_DELIVERYDATE='" + QString::number(m_delayDeliveryDate)  + "',";
	req += "TYPE_PAYMENT='" + m_typePayment + "',";
	req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
	req += "STATE='" + QString::number(m_state)  + "',";
	req += "DESCRIPTION='" + m_description.replace("\'","''") + "' ";
	req += "WHERE ID='"+ QString::number(m_id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}


/**
	  Suppression du facture de la bdd
	  ATTENTION ICI on supprimer le facture de la bdd avec les details associes
	  @return true si ok
  */
bool proposal::remove(){
	// Construction de la requette pour supprimer la proposition
	QString req = "DELETE FROM TAB_PROPOSALS";
	req += " WHERE ID='"+ QString::number(m_id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}

/**
	Charge la facture depuis son ID
	@param id proposal
	@return true si ok
  */
bool proposal::loadFromID(const int& id)
{
	QString req = "SELECT TAB_PROPOSALS.ID, TAB_PROPOSALS.ID_CUSTOMER, TAB_PROPOSALS.CREATIONDATE, TAB_PROPOSALS.DATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
			"TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.TYPE_PAYMENT, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
			"FROM TAB_PROPOSALS "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"LEFT OUTER JOIN TAB_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"WHERE TAB_PROPOSALS.ID = '" + QString::number(id)+"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id = id;
		m_idCustomer = query.value(query.record().indexOf("ID_CUSTOMER")).toInt();
		m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
		m_userDate = query.value(query.record().indexOf("DATE")).toDate();
		m_validDate = query.value(query.record().indexOf("VALIDDATE")).toDate();
		m_deliveryDate = query.value(query.record().indexOf("DELIVERYDATE")).toDate();
		m_delayDeliveryDate = query.value(query.record().indexOf("DELAY_DELIVERYDATE")).toInt();
		m_code = query.value(query.record().indexOf("PCODE")).toString();
		m_InvoiceCode = query.value(query.record().indexOf("ICODE")).toString();
		m_typePayment = query.value(query.record().indexOf("TYPE_PAYMENT")).toString();
		m_price = query.value(query.record().indexOf("PRICE")).toFloat();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		m_state = query.value(query.record().indexOf("STATE")).toInt();
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
bool proposal::loadFromCode(const QString& code)
{
  /*  std::string sVal;
	IBPP::Timestamp dateCrea;
	IBPP::Date udate;*/
	QString req = "SELECT TAB_PROPOSALS.ID, TAB_PROPOSALS.ID_CUSTOMER, TAB_PROPOSALS.CREATIONDATE, TAB_PROPOSALS.DATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
			"TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.TYPE_PAYMENT, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
			"FROM TAB_PROPOSALS "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"LEFT OUTER JOIN TAB_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"WHERE TAB_PROPOSALS.CODE = '" +code+"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id = query.value(query.record().indexOf("ID")).toInt();
		m_idCustomer = query.value(query.record().indexOf("ID_CUSTOMER")).toInt();
		m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
		m_userDate = query.value(query.record().indexOf("DATE")).toDate();
		m_validDate = query.value(query.record().indexOf("VALIDDATE")).toDate();
		m_deliveryDate = query.value(query.record().indexOf("DELIVERYDATE")).toDate();
		m_delayDeliveryDate = query.value(query.record().indexOf("DELAY_DELIVERYDATE")).toInt();
		m_code = query.value(query.record().indexOf("PCODE")).toString();
		m_InvoiceCode = query.value(query.record().indexOf("ICODE")).toString();
		m_typePayment = query.value(query.record().indexOf("TYPE_PAYMENT")).toString();
		m_price = query.value(query.record().indexOf("PRICE")).toFloat();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		m_state = query.value(query.record().indexOf("STATE")).toInt();
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
bool proposal::isHere(const QString& code) {
	int count;
	QString req = "SELECT COUNT(*) FROM TAB_PROPOSALS AS PCOUNT WHERE UPPER(CODE)=UPPER('";
	req += code + "');";
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
int proposal::getLastId(){
	int iVal=-1;
	QString req;
	if(m_db.driverName() == "QSQLITE")
			req = "SELECT seq FROM sqlite_sequence WHERE name=\"TAB_PROPOSALS\";";
	else    req = "SHOW TABLE STATUS LIKE 'TAB_PROPOSALS';";

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
	 @param ProposalList liste de retour
	 @return valeur de retour sous forme de liste de chaine de type ProposalList
  */
bool proposal::getProposalList(ProposalList& list, int id_customer, QString order, QString filter, QString field) {
	QString req = "select TAB_PROPOSALS.ID, TAB_PROPOSALS.DATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
			"TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
			"FROM TAB_PROPOSALS "
			"LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
			"ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
			"LEFT OUTER JOIN TAB_INVOICES "
			"ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
			"WHERE TAB_PROPOSALS.ID_CUSTOMER = '" + QString::number(id_customer)+"' ";

	if(!field.isEmpty()){
		req += " WHERE UPPER(";
		req += field.replace("\'","''");
		req += ") LIKE UPPER('";
		req += filter.replace("\'","''");
		req += "%')";
	}
	req += " ORDER BY UPPER("+order.replace("\'","''")+") DESC;";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.code << query.value(query.record().indexOf("PCODE")).toString();
			list.codeInvoice << query.value(query.record().indexOf("ICODE")).toString();
			list.userDate.push_back( query.value(query.record().indexOf("DATE")).toDate() );
			list.validDate.push_back( query.value(query.record().indexOf("VALIDDATE")).toDate());
			list.deliveryDate.push_back( query.value(query.record().indexOf("DELIVERYDATE")).toDate() );
			list.delayDeliveryDate.push_back( query.value(query.record().indexOf("DELAY_DELIVERYDATE")).toInt() );
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
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
	 Retour la liste des propositions en cours decritures
	 @param ProposalListAlert liste de retour
	 @return valeur de retour sous forme de liste de chaine de type ProposalListAlert
  */
bool proposal::getProposalListAlert(ProposalListAlert& list) {

	QString req =   "SELECT TAB_CUSTOMERS.ID AS C_ID, TAB_CUSTOMERS.FIRSTNAME, TAB_CUSTOMERS.LASTNAME, TAB_PROPOSALS.ID, TAB_PROPOSALS.DATE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.CODE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE "
					"FROM TAB_PROPOSALS "
					"LEFT OUTER JOIN TAB_CUSTOMERS "
					"ON TAB_PROPOSALS.ID_CUSTOMER = TAB_CUSTOMERS.ID "
					"WHERE TAB_PROPOSALS.STATE = '0' "
					"ORDER BY UPPER(TAB_PROPOSALS.DATE) ASC; ";
	//Liste les factures en alert avec les plus vielles en premier
	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.customerId << query.value(query.record().indexOf("C_ID")).toInt();
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.customerFirstName << query.value(query.record().indexOf("FIRSTNAME")).toString();
			list.customerLastName << query.value(query.record().indexOf("LASTNAME")).toString();
			list.userDate.push_back( query.value(query.record().indexOf("DATE")).toDate() );
			list.code.push_back( query.value(query.record().indexOf("CODE")).toString());
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );

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
	 @return valeur de retour sous forme de liste de chaine de type ProposalItem
  */
bool proposal::getProposalItemsList(ProposalListItems& list, QString order, QString filter, QString field) {

	QString req = "SELECT * FROM TAB_PROPOSALS_DETAILS WHERE ID_PROPOSAL=" + QString::number(this->m_id)+" ";
	if(!field.isEmpty()){
		req += " WHERE UPPER(";
		req += field.replace("\'","''");
		req += ") LIKE UPPER('";
		req += filter.replace("\'","''");
		req += "%')";
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
  @return valeur de retour sous forme de liste de chaine de type ProposalItem
  */
bool proposal::getProposalItem(ProposalItem& item){

	QString req =   "SELECT * FROM TAB_PROPOSALS_DETAILS WHERE ID_PROPOSAL="
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
bool proposal::addProposalItem(ProposalItem& item){
	QString f;
	// Construction de la requette
	QString req = "INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE, TAX, ITEM_ORDER) ";
	req += "VALUES(";
	req += "'" + QString::number(this->m_id) + "',";
	req += "'" + QString::number(item.idProduct) + "',";
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
bool proposal::removeProposalItem(int Itemid){
	// Construction de la requette
	QString req = "DELETE FROM TAB_PROPOSALS_DETAILS WHERE ID_PROPOSAL="
			+ QString::number(this->m_id)+"  AND ID="
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
bool proposal::updateProposalItem(ProposalItem& item) {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "UPDATE TAB_PROPOSALS_DETAILS SET ";
	req += "NAME='" + item.name.replace("\'","''") + "',";
	req += "ID_PRODUCT='" + QString::number(item.idProduct)  + "',";
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
	Modifie les valeurs de l article
	@return true si ok
  */
bool proposal::setLink( const int& idProposal, const int& idInvoice ){
	// Construction de la requette
	QString req = "INSERT INTO TAB_LINK_PROPOSALS_INVOICES (ID_PROPOSAL, ID_INVOICE) ";
	req += "VALUES(";
	req += "'" + QString::number(idProposal) + "',";
	req += "'" + QString::number(idInvoice)  + "');";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}


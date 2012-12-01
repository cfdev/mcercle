#include "tax.h"
#include <QMessageBox>
#include <QVariant>

tax::tax(QWidget *parent): m_parent(parent) {
}


/**
	Ajout une categorie dans la base de donnee
	@return true si ok
  */
bool tax::create() {
	// Construction de la requette
	QString f;
	QString req = "INSERT INTO TAB_TAX(TAX, DESCRIPTION) ";
	req += "VALUES(";
	req += "'"  +f.setNum(m_value,'f',2) + "', '" + m_description.replace("\'","''")  +"');";

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
	  ATTENTION ICI on update sur ID !!
  */
bool tax::update() {
	// Construction de la requette
	QString f;
	QString req = "UPDATE TAB_TAX SET ";
	req += "TAX='" + f.setNum(m_value,'f',2) + "', ";
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
	  Suppression de la tax
  */
bool tax::remove() {
	// Construction de la requette
	QString req = "DELETE FROM TAB_TAX WHERE ID='";
	req += QString::number(m_id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	else return true;
}


/**
	 Charge la tax depuis son ID
	 @param ID
	 @return vrai si ok
  */
bool tax::loadFromID(const int& id) {
	m_id = id;
	QString req = "SELECT * FROM TAB_TAX WHERE ID='";
	req += QString::number(id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_value = query.value(query.record().indexOf("TAX")).toFloat();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}




/**
	 Liste les donnees du champ en fonction du filtre
	 @param valeur de retour sous forme de liste de chaine de type taxList
	 @return true si ok
  */
bool tax::getTaxList(taxList& list, QString order, QString filter, QString field) {
	QString req = "SELECT * FROM TAB_TAX";

	if(!field.isEmpty()){
		req += " WHERE UPPER(";
		req += field.replace("\'","''");
		req += ") LIKE UPPER('";
		req += filter.replace("\'","''");
		req += "%')";
	}
	req += " ORDER BY UPPER("+order.replace("\'","''")+") ASC;";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		while (query.next()){
			list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
			list.value.push_back( query.value(query.record().indexOf("TAX")).toFloat() );
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();;
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Permet de savoir si la tax est deja presente dans la base
	 @param val, valeur de la tax
	 @return vrai si la tax exist
  */
bool tax::isHere(const float& val) {
	int count;
	QString f;
	QString req = "SELECT COUNT(*) FROM TAB_TAX AS PCOUNT WHERE TAX='";
	req +=  f.setNum(val,'f',2) + "';";

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


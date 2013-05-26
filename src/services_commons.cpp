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

#include "services_commons.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QVariant>

service_common::service_common(QSqlDatabase db, QWidget *parent): m_parent(parent) {
	m_db = db;
	//default
	m_theDate = QDateTime::fromString("1.30.1", "M.d.s");
	m_tax = 0;
}

service_common::~service_common() {
}


/**
	  Ajout une service_common dans la base de donnee
	  @return true si ok
  */
bool service_common::create() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "INSERT INTO TAB_SERVICES_COMMONS(CREATIONDATE, THEDATE, PRICE, TAX, NAME, DESCRIPTION) ";
	req += "VALUES(";
	req += "'" + QDateTime::currentDateTime().toString(tr("yyyy/MM/dd-HH:mm:ss")) + "',";
	req += "'" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
	req += "'" + f.setNum(m_price,'f',2) + "',";
	req += "'" + f.setNum(m_tax,'f',2) + "',";
	req += "'" + m_name.replace("\'","''") + "',";
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
	  ATTENTION ICI on update sur ID  !!
	  @return true si ok
  */
bool service_common::update() {
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	QString req = "UPDATE TAB_SERVICES_COMMONS SET ";
	req += "THEDATE='" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
	req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
	req += "TAX='" + f.setNum(m_tax,'f',2) + "',";
	req += "NAME='" + m_name.replace("\'","''") + "',";
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
	  Suppression de l service_common de la bdd
	  ATTENTION ICI on supprimer l service_common de la bdd
	  @return true si ok
  */
bool service_common::remove(){
	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	// Construction de la requette
	QString req = "DELETE FROM TAB_SERVICES_COMMONS";
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
	 Charge l service_commons depuis son id
	 @param description
	 @return vrai si ok
  */
bool service_common::loadFromID(const int& id) {

	QString req = "SELECT * FROM TAB_SERVICES_COMMONS WHERE ID='";
	req += QString::number(id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id = id;
		m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
		m_theDate = query.value(query.record().indexOf("THEDATE")).toDateTime();
		m_price = query.value(query.record().indexOf("PRICE")).toFloat();
		m_tax = query.value(query.record().indexOf("TAX")).toFloat();
		m_name = query.value(query.record().indexOf("NAME")).toString();
		m_description = query.value(query.record().indexOf("DESCRIPTION")).toString();
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	return false;
}


/**
	 Permet de savoir si un service_common general est deja present dans la base
	 @param name, nom du service_common
	 @return l id du service
  */
int service_common::isHere(const QString& name) {
	int count;
	QString req = "SELECT COUNT(*) FROM TAB_SERVICES_COMMONS WHERE UPPER(NAME)=UPPER('";
	req +=  QString(name).replace("\'","''") + "');";
	if(name.isEmpty()) return -1;

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
	 Liste les donnees du champ en fonction du filtre
	 @return valeur de retour sous forme de liste de chaine de type service_commonList
  */
bool service_common::getServiceCommList(serviceCommList& list, QString order, QString filter, QString field) {
	QString req = "SELECT *"
				  " FROM TAB_SERVICES_COMMONS ";

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
			list.name << query.value(query.record().indexOf("NAME")).toString();
			list.price.push_back( query.value(query.record().indexOf("PRICE")).toFloat() );
			list.tax.push_back( query.value(query.record().indexOf("TAX")).toFloat() );
			list.date.push_back(  query.value(query.record().indexOf("THEDATE")).toDateTime());
			list.description << query.value(query.record().indexOf("DESCRIPTION")).toString();
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}



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

#include "productcategory.h"

#include <QMessageBox>
#include <QStringList>
#include <QVariant>

category::category(QSqlDatabase db, QWidget *parent): m_parent(parent) {
	m_db = db;
}


category::~category() {
	//
}

/**
	  Ajout une categorie dans la base de donnee
	  @return true si ok
  */
bool category::create() {
	// Construction de la requette
	QString req = "INSERT INTO TAB_PRODUCTS_CATEGORIES(NAME, COLOR) ";
	req += "VALUES(";
	req += "'" + m_name.replace("\'","''") + "', '" + m_color.name() +"');";

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
bool category::update() {
	// Construction de la requette
	QString req = "UPDATE TAB_PRODUCTS_CATEGORIES SET ";
	req += "NAME='" + m_name.replace("\'","''") + "', ";
	req += "COLOR='" + m_color.name() + "' ";
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
	  Suppression de la categorie
  */
bool category::remove() {
	// Construction de la requette
	QString req = "DELETE FROM TAB_PRODUCTS_CATEGORIES  WHERE ID='";
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
	 Charge la categorie depuis son nom
	 @param nom categorie
	 @return vrai si ok
  */
bool category::loadFromName(const QString& name) {
	QString req = "SELECT * FROM TAB_PRODUCTS_CATEGORIES WHERE UPPER(NAME)= UPPER('";
	req += QString(name).replace("\'","''") +"');";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_id = query.value(query.record().indexOf("ID")).toInt();
		m_name = query.value(query.record().indexOf("NAME")).toString();
		m_color.setNamedColor( query.value(query.record().indexOf("COLOR")).toString() );
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}

/**
	 Charge la categorie depuis son ID
	 @param ID
	 @return vrai si ok
  */
bool category::loadFromID(const int& id) {
	m_id = id;
	QString req = "SELECT * FROM TAB_PRODUCTS_CATEGORIES WHERE ID='";
	req += QString::number(id) +"';";

	QSqlQuery query;
	query.prepare(req);
	if(query.exec()){
		query.next();
		m_name = query.value(query.record().indexOf("NAME")).toString();
		m_color.setNamedColor( query.value(query.record().indexOf("COLOR")).toString() );
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Liste les donnees du champ en fonction du filtre
	 @param valeur de retour sous forme de liste de chaine de type categoryList
	 @return true si ok
  */
bool category::getcategoryList(categoryList& list, QString order, QString filter, QString field) {
	QString req = "SELECT * FROM TAB_PRODUCTS_CATEGORIES";

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
			list.name << query.value(query.record().indexOf("NAME")).toString();
			list.color.push_back( QColor(  query.value(query.record().indexOf("COLOR")).toString() ) );
		}
		return true;
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
}


/**
	 Permet de savoir si la categorie est deja presente dans la base
	 @param name, nom de la categorie
	 @return vrai si la categorie exist
  */
bool category::isHere(const QString& name) {
	int count;
	QString req = "SELECT COUNT(*) FROM TAB_PRODUCTS_CATEGORIES AS PCOUNT WHERE UPPER(NAME)= UPPER('";
	req +=  QString(name).replace("\'","''") + "');";
	if(name.isEmpty())return false;

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

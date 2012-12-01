/**
  This file is a part of mcercle
  Copyright (C) 2010-2012 Cyril FRAUSTI

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

#include "provider.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QVariant>

provider::provider(QSqlDatabase db, QWidget *parent): m_parent(parent) {
    m_db = db;
}


provider::~provider() {
    //
}

/**
      Ajout un Fournisseur dans la base de donnee
      @return true si ok
  */
bool provider::create() {
    // Construction de la requette
    QString req = "INSERT INTO TAB_PROVIDERS(CREATIONDATE, NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME) ";
    req += "VALUES(";
    req += "'" + QDateTime::currentDateTime().toString(tr("yyyy/MM/dd-HH:mm:ss")) + "',";
    req += "'" + m_name.replace("\'","''") + "',";
    req += "'" + m_address1.replace("\'","''") + "',";
    req += "'" + m_address2.replace("\'","''") + "',";
    req += "'" + m_address3.replace("\'","''") + "',";
    req += "'" + m_zipCode.replace("\'","''") + "',";
    req += "'" + m_city.replace("\'","''") + "',";
    req += "'" + m_country.replace("\'","''") + "',";
    req += "'" + m_phoneNumber.replace("\'","''") + "',";
    req += "'" + m_fax.replace("\'","''") + "',";
    req += "'" + m_email.replace("\'","''")  + "',";
    req += "'" + m_contact.replace("\'","''") + "');";

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
bool provider::update() {
    // Construction de la requette
    QString req = "UPDATE TAB_PROVIDERS SET ";
    req += "NAME='" +  m_name.replace("\'","''") + "',";
    req += "ADDRESS1='" + m_address1.replace("\'","''") + "',";
    req += "ADDRESS2='" + m_address2.replace("\'","''") + "',";
    req += "ADDRESS3='" + m_address3.replace("\'","''") + "',";
    req += "ZIPCODE='" + m_zipCode.replace("\'","''") + "',";
    req += "CITY='" + m_city.replace("\'","''") + "',";
    req += "COUNTRY='" + m_country.replace("\'","''") + "',";
    req += "PHONENUMBER='" + m_phoneNumber.replace("\'","''") + "',";
    req += "FAXNUMBER='" + m_fax.replace("\'","''") + "',";
    req += "EMAIL='" + m_email.replace("\'","''") + "',";
    req += "CONTACT_NAME='" + m_contact.replace("\'","''") + "' ";
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
      Suppression du fournisseur
  */
bool provider::remove() {
    // Construction de la requette
    QString req = "DELETE FROM TAB_PROVIDERS WHERE ID='";
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
     Charge le fournisseur depuis son nom
     @param nom fournisseur
     @return vrai si ok
  */
bool provider::loadFromName(const QString& name) {
    QString req = "SELECT * FROM TAB_PROVIDERS WHERE UPPER(NAME)= UPPER('";
    req += QString(name).replace("\'","''") +"');";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        query.next();
        m_id = query.value(query.record().indexOf("ID")).toInt();
        m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
        m_name = query.value(query.record().indexOf("NAME")).toString();
        m_address1 = query.value(query.record().indexOf("ADDRESS1")).toString();
        m_address2 = query.value(query.record().indexOf("ADDRESS2")).toString();
        m_address3 = query.value(query.record().indexOf("ADDRESS3")).toString();
        m_zipCode = query.value(query.record().indexOf("ZIPCODE")).toString();
        m_city = query.value(query.record().indexOf("CITY")).toString();
        m_country = query.value(query.record().indexOf("COUNTRY")).toString();
        m_phoneNumber = query.value(query.record().indexOf("PHONENUMBER")).toString();
        m_fax = query.value(query.record().indexOf("FAXNUMBER")).toString();
        m_email = query.value(query.record().indexOf("EMAIL")).toString();
        m_contact = query.value(query.record().indexOf("CONTACT_NAME")).toString();
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}

/**
     Charge le fournisseur depuis son ID
     @param ID
     @return vrai si ok
  */
bool provider::loadFromID(const int& id) {
    m_id = id;
    QString req = "SELECT * FROM TAB_PROVIDERS WHERE UPPER(ID)= UPPER('";
    req += QString::number(id) +"');";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        query.next();
        m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
        m_name = query.value(query.record().indexOf("NAME")).toString();
        m_address1 = query.value(query.record().indexOf("ADDRESS1")).toString();
        m_address2 = query.value(query.record().indexOf("ADDRESS2")).toString();
        m_address3 = query.value(query.record().indexOf("ADDRESS3")).toString();
        m_zipCode = query.value(query.record().indexOf("ZIPCODE")).toString();
        m_city = query.value(query.record().indexOf("CITY")).toString();
        m_country = query.value(query.record().indexOf("COUNTRY")).toString();
        m_phoneNumber = query.value(query.record().indexOf("PHONENUMBER")).toString();
        m_fax = query.value(query.record().indexOf("FAXNUMBER")).toString();
        m_email = query.value(query.record().indexOf("EMAIL")).toString();
        m_contact = query.value(query.record().indexOf("CONTACT_NAME")).toString();
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Liste les donnees du champ en fonction du filtre
     Jointure avec LEFT OUTER JOIN car avec les select les conditions ne
     sont pas remplis il ne retour pas le produit
     @param valeur de retour sous forme de liste de chaine de type ProductList
     @return true si ok
  */
bool provider::getProviderList(ProviderList& list, QString order, QString filter, QString field) {
    QString req = "SELECT * FROM TAB_PROVIDERS";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field;
        req += ") LIKE UPPER('";
        req += filter;
        req += "%')";
    }
    req += " ORDER BY UPPER("+order+") ASC;";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        while (query.next()){
            list.name << query.value(query.record().indexOf("NAME")).toString();
            list.phoneNumber << query.value(query.record().indexOf("PHONENUMBER")).toString();
            list.faxNumber << query.value(query.record().indexOf("FAXNUMBER")).toString();
            list.email.push_back( query.value(query.record().indexOf("EMAIL")).toString() );
        }
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Permet de savoir si un fournisseur est deja present dans la base
     @param name, nom du fournisseur
     @return l id du fournisseur
  */
int provider::isHere(const QString& name) {
    int iVal;
    QString req = "SELECT ID FROM TAB_PROVIDERS WHERE UPPER(NAME)= UPPER('";
    req +=  QString(name).replace("\'","''") + "');";
    if(name.isEmpty())return -1;

    QSqlQuery query;
    query.prepare(req);

    if(query.exec()){
        query.next();
        iVal = query.value(query.record().indexOf("ID")).toInt();
        return iVal;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return -1;
    }
}

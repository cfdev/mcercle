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

#include "customer.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QVariant>

customer::customer(QSqlDatabase db, QWidget *parent)
: m_parent(parent) {
    m_db = db;

    m_type = PEOPLE_TYPE;

    //Class des propositions commerciales / devis
    m_proposal = new proposal(db, m_parent);
    //Class des factures
    m_invoice = new invoice(db, m_parent);
    //Class des prestations
    m_service = new service(db, m_parent);
    m_serviceComm = new service_common(db, m_parent);
}


customer::~customer() {
    delete m_proposal;
    delete m_invoice;
    delete m_service;
    delete m_serviceComm;
}

/**
      Ajout un utilisateur dans la base de donnee
      @return true si ok
  */
bool customer::create() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString req = "INSERT INTO TAB_CUSTOMERS(TYPE, CREATIONDATE, DESC_COMPAGNY, LASTNAME, FIRSTNAME, GENDER, BIRTHDAY, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER, MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL) ";
    req += "VALUES('" + QString::number(m_type)  + "',";
    req += "'" + QDateTime::currentDateTime().toString(tr("yyyy/MM/dd-HH:mm:ss")) + "',";
    req += "'" + m_desCompagny.replace("\'","''") + "',";
    req += "'" + m_lastName.replace("\'","''") + "',";
    req += "'" + m_firstName.replace("\'","''") + "',";
    req += "'" + m_gender.replace("\'","''") + "',";
    req += "'" + m_birthday.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_address1.replace("\'","''") + "',";
    req += "'" + m_address2.replace("\'","''") + "',";
    req += "'" + m_address3.replace("\'","''") + "',";
    req += "'" + m_zipCode.replace("\'","''") + "',";
    req += "'" + m_city.replace("\'","''") + "',";
    req += "'" + m_country.replace("\'","''") + "',";
    req += "'" + m_phoneNumber.replace("\'","''") + "',";
    req += "'" + m_mobileNumber.replace("\'","''") + "',";
    req += "'" + m_email.replace("\'","''") + "',";
    req += "'" + m_profession.replace("\'","''") + "',";
    req += "'" + m_comments.replace("\'","''") + "',";
    req += "'" + QString::number(m_sending_pubEmail) + "');";

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
    @return true si ok
  */
bool customer::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString req = "UPDATE TAB_CUSTOMERS SET ";
    req += "TYPE='" + QString::number(m_type) + "', ";
    req += "DESC_COMPAGNY='" + m_desCompagny.replace("\'","''") + "',";
    req += "LASTNAME='" + m_lastName.replace("\'","''") + "',";
    req += "FIRSTNAME='" + m_firstName.replace("\'","''") + "',";
    req += "GENDER='" + m_gender.replace("\'","''") + "',";
    req += "BIRTHDAY='" + m_birthday.toString(tr("yyyy/MM/dd")) + "',";
    req += "ADDRESS1='" + m_address1.replace("\'","''") + "',";
    req += "ADDRESS2='" + m_address2.replace("\'","''") + "',";
    req += "ADDRESS3='" + m_address3.replace("\'","''") + "',";
    req += "ZIPCODE='" + m_zipCode.replace("\'","''") + "',";
    req += "CITY='" + m_city.replace("\'","''") + "',";
    req += "COUNTRY='" + m_country.replace("\'","''") + "',";
    req += "PHONENUMBER='" + m_phoneNumber.replace("\'","''") + "',";
    req += "MOBILENUMBER='" + m_mobileNumber.replace("\'","''") + "',";
    req += "EMAIL='" + m_email.replace("\'","''").toUtf8() + "',";
    req += "PROFESSION='" + m_profession.replace("\'","''") + "',";
    req += "COMMENTS='" + m_comments.replace("\'","''") + "',";
    req += "SENDING_PUB_EMAIL='" + QString::number(m_sending_pubEmail) + "' ";
    req += "WHERE ID="+ QString::number(m_id) +";";

    QSqlQuery query;
    query.prepare(req);
    if(!query.exec()) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
    else return true;
}


/**
      Suppression du client
      ATTENTION ICI on supprime le client de la bdd avec ses donnees associes
      @return true si ok
  */
bool customer::remove(){
    // Construction de la requette pour supprimer la facture
    QString req = "DELETE FROM TAB_CUSTOMERS";
    req += " WHERE ID="+ QString::number(m_id) +";";

    QSqlQuery query;
    query.prepare(req);
    if(!query.exec()) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
    else return true;
}

/**
    Charge le client depuis son ID
    @param code client
    @return true si ok
  */
bool customer::loadFromID(const int& id)
{
    m_id = id;
    QString req = "SELECT * FROM TAB_CUSTOMERS WHERE ID=";
    req += QString::number(m_id) +";";

    QSqlQuery query;
    query.prepare(req);

    if(query.exec()){
        query.next();
        m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
        m_type = query.value(query.record().indexOf("TYPE")).toInt();
        m_firstName = query.value(query.record().indexOf("FIRSTNAME")).toString();
        m_lastName = query.value(query.record().indexOf("LASTNAME")).toString();
        m_desCompagny = query.value(query.record().indexOf("DESC_COMPAGNY")).toString();
        m_gender = query.value(query.record().indexOf("GENDER")).toString();
        m_birthday = query.value(query.record().indexOf("BIRTHDAY")).toDate();
        m_address1 = query.value(query.record().indexOf("ADDRESS1")).toString();
        m_address2 = query.value(query.record().indexOf("ADDRESS2")).toString();
        m_address3 = query.value(query.record().indexOf("ADDRESS3")).toString();
        m_zipCode = query.value(query.record().indexOf("ZIPCODE")).toString();
        m_city = query.value(query.record().indexOf("CITY")).toString();
        m_country = query.value(query.record().indexOf("COUNTRY")).toString();
        m_phoneNumber = query.value(query.record().indexOf("PHONENUMBER")).toString();
        m_mobileNumber = query.value(query.record().indexOf("MOBILENUMBER")).toString();
        m_email = query.value(query.record().indexOf("EMAIL")).toString();
        m_profession = query.value(query.record().indexOf("PROFESSION")).toString();
        m_comments = query.value(query.record().indexOf("COMMENTS")).toString();
        m_sending_pubEmail = query.value(query.record().indexOf("SENDING_PUB_EMAIL")).toBool();
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Avoir une liste des clients existants en fonction des parametres d entrees
     @param list, liste des clients sous forme CustomerList
     @param first, skip ,permettent de fractionner la liste
     @param filter, field permettent de filtrer la liste
     @return true si ok
  */
bool customer::getCustomerList(CustomerList& list, int first, int skip, QString filter, QString field) {
     //Requete obtenir les valeurs
    QString req = "SELECT * FROM TAB_CUSTOMERS";
    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''");
        req += ") LIKE UPPER('";
        req += filter.replace("\'","''");
        req += "%')";
    }
    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += " ORDER BY UPPER(TAB_CUSTOMERS.LASTNAME) ASC LIMIT "+QString::number(skip)+","+QString::number(first)+"; ";

    QSqlQuery query;
    query.prepare(req);

    if(query.exec()){
        while (query.next()){
            list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
            list.firstName << query.value(query.record().indexOf("FIRSTNAME")).toString();
            list.lastName << query.value(query.record().indexOf("LASTNAME")).toString();
        }
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Permet de savoir combien de client il y a dans la bdd
     @param filter, field, filtre le nombre de clients
     @return le nombre de client
  */
int customer::count(QString filter, QString field) {
    int count;
    //Requete 1 nombre de valeur
    QString req = "SELECT COUNT(*) FROM (SELECT * FROM TAB_CUSTOMERS";
    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''");
        req += ") LIKE UPPER('";
        req += filter.replace("\'","''");
        req += "%')";
    }
    req += ")  AS CCOUNT;";

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


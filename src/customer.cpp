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

customer::customer(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent)
: m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;

    m_type = PEOPLE_TYPE;

    //Class des propositions commerciales / devis
    m_proposal = new proposal(db, m_tr, m_st, m_parent);
    //Class des factures
    m_invoice = new invoice(db, m_tr, m_st, m_parent);
    //Class des prestations
    m_service = new service(db, m_tr, m_st, m_parent);
    m_serviceComm = new service_common(db, m_tr, m_st, m_parent);
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
    QString req = "INSERT INTO tab_customers(TYPE, DESC_COMPAGNY, LASTNAME, FIRSTNAME, GENDER, BIRTHDAY, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER, MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL) ";
    req += "VALUES('" + QString::number(m_type)  + "',";
    req += "'" + m_desCompagny.replace("\'","''").toUtf8() + "',";
    req += "'" + m_lastName.replace("\'","''").toUtf8() + "',";
    req += "'" + m_firstName.replace("\'","''").toUtf8() + "',";
    req += "'" + m_gender.replace("\'","''").toUtf8() + "',";
    req += "'" + m_birthday.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_address1.replace("\'","''").toUtf8() + "',";
    req += "'" + m_address2.replace("\'","''").toUtf8() + "',";
    req += "'" + m_address3.replace("\'","''").toUtf8() + "',";
    req += "'" + m_zipCode.replace("\'","''").toUtf8() + "',";
    req += "'" + m_city.replace("\'","''").toUtf8() + "',";
    req += "'" + m_country.replace("\'","''").toUtf8() + "',";
    req += "'" + m_phoneNumber.replace("\'","''").toUtf8() + "',";
    req += "'" + m_mobileNumber.replace("\'","''").toUtf8() + "',";
    req += "'" + m_email.replace("\'","''").toUtf8() + "',";
    req += "'" + m_profession.replace("\'","''").toUtf8() + "',";
    req += "'" + m_comments.replace("\'","''").toUtf8() + "',";
    req += "'" + QString::number(m_sending_pubEmail) + "');";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
    Applique les informations courantes dans la base de donnee
    @return true si ok
  */
bool customer::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString req = "UPDATE tab_customers SET ";
    req += "TYPE='" + QString::number(m_type) + "', ";
    req += "DESC_COMPAGNY='" + m_desCompagny.replace("\'","''").toUtf8() + "',";
    req += "LASTNAME='" + m_lastName.replace("\'","''").toUtf8() + "',";
    req += "FIRSTNAME='" + m_firstName.replace("\'","''").toUtf8() + "',";
    req += "GENDER='" + m_gender.replace("\'","''").toUtf8() + "',";
    req += "BIRTHDAY='" + m_birthday.toString(tr("yyyy/MM/dd")) + "',";
    req += "ADDRESS1='" + m_address1.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS2='" + m_address2.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS3='" + m_address3.replace("\'","''").toUtf8() + "',";
    req += "ZIPCODE='" + m_zipCode.replace("\'","''").toUtf8() + "',";
    req += "CITY='" + m_city.replace("\'","''").toUtf8() + "',";
    req += "COUNTRY='" + m_country.replace("\'","''").toUtf8() + "',";
    req += "PHONENUMBER='" + m_phoneNumber.replace("\'","''").toUtf8() + "',";
    req += "MOBILENUMBER='" + m_mobileNumber.replace("\'","''").toUtf8() + "',";
    req += "EMAIL='" + m_email.replace("\'","''").toUtf8() + "',";
    req += "PROFESSION='" + m_profession.replace("\'","''").toUtf8() + "',";
    req += "COMMENTS='" + m_comments.replace("\'","''").toUtf8() + "',";
    req += "SENDING_PUB_EMAIL='" + QString::number(m_sending_pubEmail) + "' ";
    req += "WHERE ID='"+ QString::number(m_id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
      Suppression du client
      ATTENTION ICI on supprime le client de la bdd avec ses donnees associes
      @return true si ok
  */
bool customer::remove(){
    // Construction de la requette pour supprimer la facture
    QString req = "DELETE FROM tab_customers";
    req += " WHERE ID='"+ QString::number(m_id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e ) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
}

/**
    Charge le client depuis son ID
    @param code client
    @return true si ok
  */
bool customer::loadFromID(const int& id)
{
    std::string val;
    IBPP::Timestamp dateCrea;
    IBPP::Date date;
    bool valBool;
    QString req = "SELECT * FROM tab_customers WHERE ID='";
    req += QString::number(id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_creationDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("TYPE", m_type);
            m_st->Get("DESC_COMPAGNY", val);
            m_desCompagny= QString::fromUtf8(val.c_str());val="";
            m_st->Get("LASTNAME", val);
            m_lastName = QString::fromUtf8(val.c_str());val="";
            m_st->Get("FIRSTNAME", val);
            m_firstName = QString::fromUtf8(val.c_str());val="";
            m_st->Get("GENDER", val);
            m_gender = QString::fromUtf8(val.c_str());val="";
            if(!m_st->IsNull("BIRTHDAY")) {
                m_st->Get("BIRTHDAY", date);
                m_birthday = database::fromIBPPDate(date);
            }
            m_st->Get("ADDRESS1", val);
            m_address1 = QString::fromUtf8(val.c_str());val="";
            m_st->Get("ADDRESS2", val);
            m_address2 = QString::fromUtf8(val.c_str());val="";
            m_st->Get("ADDRESS3", val);
            m_address3 = QString::fromUtf8(val.c_str());val="";
            m_st->Get("ZIPCODE", val);
            m_zipCode = QString::fromUtf8(val.c_str());val="";
            m_st->Get("CITY", val);
            m_city = QString::fromUtf8(val.c_str());val="";
            m_st->Get("COUNTRY", val);
            m_country = QString::fromUtf8(val.c_str());val="";
            m_st->Get("PHONENUMBER", val);
            m_phoneNumber = QString::fromUtf8(val.c_str());val="";
            m_st->Get("MOBILENUMBER", val);
            m_mobileNumber = QString::fromUtf8(val.c_str());val="";
            m_st->Get("EMAIL", val);
            m_email = QString::fromUtf8(val.c_str());val="";
            m_st->Get("PROFESSION", val);
            m_profession = QString::fromUtf8(val.c_str());val="";
            m_st->Get("COMMENTS", val);
            m_comments = QString::fromUtf8(val.c_str());val="";

            m_st->Get("SENDING_PUB_EMAIL", valBool);
            m_sending_pubEmail = valBool;
        }
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
     Avoir une liste des clients existants en fonction des parametres d entrees
     @param list, liste des clients sous forme CustomerList
     @param first, skip ,permettent de fractionner la liste
     @param filter, field permettent de filtrer la liste
     @return true si ok
  */
bool customer::getCustomerList(CustomerList& list, int first, int skip, QString filter, QString field) {
    std::string sVal;
    int iVal;

     //Requete obtenir les valeurs
    QString req = "SELECT FIRST "+QString::number(first)+" SKIP "+QString::number(skip)+" * FROM tab_customers";
    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }
    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += " ORDER BY UPPER(tab_customers.LASTNAME COLLATE UTF8) ASC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);
            m_st->Get("LASTNAME", sVal);
            list.lastName << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("FIRSTNAME", sVal);
            list.firstName << QString::fromUtf8(sVal.c_str());sVal="";
        }
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
     Permet de savoir combien de client il y a dans la bdd
     @param filter, field, filtre le nombre de clients
     @return le nombre de client
  */
int customer::count(QString filter, QString field) {
    int count;

    //Requete 1 nombre de valeur
    QString req = "SELECT COUNT(*) FROM (SELECT * FROM tab_customers";
    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }
    req += ");";


    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        m_st->Fetch();
        m_st->Get(1, count);
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return 0;
    }
    return count;
}


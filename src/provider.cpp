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

provider::provider(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;
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
    QString req = "INSERT INTO tab_providers(NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME) ";
    req += "VALUES(";
    req += "'" + m_name.replace("\'","''").toUtf8() + "',";
    req += "'" + m_address1.replace("\'","''").toUtf8() + "',";
    req += "'" + m_address2.replace("\'","''").toUtf8() + "',";
    req += "'" + m_address3.replace("\'","''").toUtf8() + "',";
    req += "'" + m_zipCode.replace("\'","''").toUtf8() + "',";
    req += "'" + m_city.replace("\'","''").toUtf8() + "',";
    req += "'" + m_phoneNumber.replace("\'","''").toUtf8() + "',";
    req += "'" + m_fax.replace("\'","''").toUtf8() + "',";
    req += "'" + m_email.replace("\'","''").toUtf8()  + "',";
    req += "'" + m_contact.replace("\'","''").toUtf8() + "');";

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
      ATTENTION ICI on update sur ID !!
  */
bool provider::update() {
    // Construction de la requette
    QString req = "UPDATE tab_providers SET ";
    req += "NAME='" + m_name.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS1='" + m_address1.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS2='" + m_address2.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS3='" + m_address3.replace("\'","''").toUtf8() + "',";
    req += "ZIPCODE='" + m_zipCode.replace("\'","''").toUtf8() + "',";
    req += "CITY='" + m_city.replace("\'","''").toUtf8() + "',";
    req += "PHONENUMBER='" + m_phoneNumber.replace("\'","''").toUtf8() + "',";
    req += "FAXNUMBER='" + m_fax.replace("\'","''").toUtf8() + "',";
    req += "EMAIL='" + m_email.replace("\'","''").toUtf8() + "',";
    req += "CONTACT_NAME='" + m_contact.replace("\'","''").toUtf8() + "' ";
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
      Suppression du fournisseur
  */
bool provider::remove() {
    // Construction de la requette
    QString req = "DELETE FROM tab_providers WHERE ID='";
    req += QString::number(m_id) +"';";

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
     Charge le fournisseur depuis son nom
     @param nom fournisseur
     @return vrai si ok
  */
bool provider::loadFromName(const QString& name) {
    std::string val;
    IBPP::Timestamp dateCrea;
    QString req = "SELECT * FROM tab_providers WHERE UPPER(NAME COLLATE UTF8)= UPPER('";
    req += name +"' COLLATE UTF8);";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_creationDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS1", val);
            m_address1 = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS2", val);
            m_address2 = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS3", val);
            m_address3 = QString::fromUtf8(val.c_str());
            m_st->Get("ZIPCODE", val);
            m_zipCode = QString::fromUtf8(val.c_str());
            m_st->Get("CITY", val);
            m_city = QString::fromUtf8(val.c_str());
            m_st->Get("PHONENUMBER", val);
            m_phoneNumber = QString::fromUtf8(val.c_str());
            m_st->Get("FAXNUMBER", val);
            m_fax = QString::fromUtf8(val.c_str());
            m_st->Get("EMAIL", val);
            m_email = QString::fromUtf8(val.c_str());
            m_st->Get("CONTACT_NAME", val);
            m_contact = QString::fromUtf8(val.c_str());
        }
        m_tr->Commit();
        if(m_id>0)
            return true;
        else return false;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}

/**
     Charge le fournisseur depuis son ID
     @param ID
     @return vrai si ok
  */
bool provider::loadFromID(const int& id) {
    std::string val;
    IBPP::Timestamp dateCrea;
    QString req = "SELECT * FROM tab_providers WHERE UPPER(ID)= UPPER('";
    req += QString::number(id) +"');";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_creationDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS1", val);
            m_address1 = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS2", val);
            m_address2 = QString::fromUtf8(val.c_str());
            m_st->Get("ADDRESS3", val);
            m_address3 = QString::fromUtf8(val.c_str());
            m_st->Get("ZIPCODE", val);
            m_zipCode = QString::fromUtf8(val.c_str());
            m_st->Get("CITY", val);
            m_city = QString::fromUtf8(val.c_str());
            m_st->Get("PHONENUMBER", val);
            m_phoneNumber = QString::fromUtf8(val.c_str());
            m_st->Get("FAXNUMBER", val);
            m_fax = QString::fromUtf8(val.c_str());
            m_st->Get("EMAIL", val);
            m_email = QString::fromUtf8(val.c_str());
            m_st->Get("CONTACT_NAME", val);
            m_contact = QString::fromUtf8(val.c_str());
        }
        m_tr->Commit();
        if(m_id>0)
            return true;
        else return false;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
     Liste les donnees du champ en fonction du filtre
     Jointure avec LEFT OUTER JOIN car avec les select les conditions ne
     sont pas remplis il ne retour pas le produit
     @return valeur de retour sous forme de liste de chaine de type ProductList
  */
void provider::getProviderList(ProviderList& list, QString order, QString filter, QString field) {
    std::string sVal;
    QString req = "SELECT * FROM tab_providers";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field;
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter;
        req += "%' COLLATE UTF8)";
    }
    req += " ORDER BY UPPER("+order+" COLLATE UTF8) ASC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());
            m_st->Get("PHONENUMBER", sVal);
            list.phoneNumber <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("FAXNUMBER", sVal);
            list.faxNumber <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("EMAIL", sVal);
            list.email <<  QString::fromUtf8(sVal.c_str());
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}


/**
     Permet de savoir si un fournisseur est deja present dans la base
     @param name, nom du fournisseur
     @return l id du fournisseur
  */
int provider::isHere(const QString& name) {
    int iVal;
    QString req = "SELECT ID FROM tab_providers WHERE UPPER(NAME COLLATE UTF8)= UPPER('";
    req +=  name + "' COLLATE UTF8);";
    if(name.isEmpty())return -1;

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("ID", iVal);
        m_tr->Commit();
        return iVal;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return -1;
    }
}

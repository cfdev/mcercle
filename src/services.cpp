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

#include "services.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>

service::service(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;
    //default
    m_theDate = QDateTime::fromString("1.30.1", "M.d.s");
}

service::~service() {
}


/**
      Ajout une service dans la base de donnee
      @return true si ok
  */
bool service::create() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "INSERT INTO tab_services(THEDATE, PRICE, NAME, DESCRIPTION, ID_CUSTOMER) ";
    req += "VALUES(";
    req += "'" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
    req += "'" + f.setNum(m_price,'f',2) + "',";
    req += "'" + m_name.replace("\'","''").toUtf8() + "',";
    req += "'" + m_description.replace("\'","''").toUtf8() + "',";
    req += "'" + QString::number(m_idCustomer)  + "');";
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
      ATTENTION ICI on update sur ID  !!
      @return true si ok
  */
bool service::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_services SET ";
    req += "THEDATE='" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
    req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
    req += "NAME='" + m_name.replace("\'","''").toUtf8() + "',";
    req += "DESCRIPTION='" + m_description.replace("\'","''").toUtf8() + "',";
    req += "ID_CUSTOMER='" + QString::number(this->m_idCustomer) + "' ";
    req += "WHERE ID='"+ QString::number(m_id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e ) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}


/**
      Suppression de l service de la bdd
      ATTENTION ICI on supprimer l service de la bdd
      @return true si ok
  */
bool service::remove(){
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    // Construction de la requette
    QString req = "DELETE FROM tab_services";
    req += " WHERE ID='"+ QString::number(m_id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e ) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}



/**
     Charge l services depuis son id
     @param description
     @return vrai si ok
  */
bool service::loadFromID(const int& id) {
    std::string val;
    float fVal;
    IBPP::Timestamp dateCrea, theDate;
    QString req = "SELECT * FROM tab_services WHERE ID='";
    req += QString::number(id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_theDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("THEDATE", theDate);
            m_theDate = database::fromIBPPTimeStamp(theDate);
            m_st->Get("PRICE", fVal);
            m_price = fVal;
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            val="";
            m_st->Get("DESCRIPTION", val);
            m_description = QString::fromUtf8(val.c_str());
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
     @return valeur de retour sous forme de liste de chaine de type serviceList
  */
void service::getServiceList(serviceList& list, int id_customer, QString order, QString filter, QString field) {
    std::string sVal;
    float fVal;
    int iVal;
    IBPP::Timestamp dateCrea, theDate;
    QString req = "SELECT tab_services.ID, tab_services.CREATIONDATE, tab_services.THEDATE, tab_services.NAME, tab_services.DESCRIPTION, tab_services.PRICE"
                  " FROM tab_services WHERE ID_CUSTOMER='" + QString::number(id_customer)+"'";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }

    req += " ORDER BY UPPER("+order.replace("\'","''").toUtf8()+") ASC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal );
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( database::fromIBPPTimeStamp(dateCrea) );
            m_st->Get("THEDATE", theDate);
            list.date.push_back( database::fromIBPPTimeStamp(theDate) );
            m_st->Get("NAME", sVal);
            list.name <<  QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("DESCRIPTION", sVal);
            list.description <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("PRICE", fVal);
            list.price.push_back(fVal);
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}



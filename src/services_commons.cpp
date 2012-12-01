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

#include "services_commons.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>

service_common::service_common(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;
    //default
    m_theDate = QDateTime::fromString("1.30.1", "M.d.s");
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
    QString req = "INSERT INTO tab_services_commons(THEDATE, PRICE, NAME, DESCRIPTION) ";
    req += "VALUES(";
    req += "'" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
    req += "'" + f.setNum(m_price,'f',2) + "',";
    req += "'" + m_name.replace("\'","''").toUtf8() + "',";
    req += "'" + m_description.replace("\'","''").toUtf8() + "');";
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
bool service_common::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_services_commons SET ";
    req += "THEDATE='" + m_theDate.toString(tr("yyyy/MM/dd hh:mm:ss")) + "',";
    req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
    req += "NAME='" + m_name.replace("\'","''").toUtf8() + "',";
    req += "DESCRIPTION='" + m_description.replace("\'","''").toUtf8() + "' ";
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
      Suppression de l service_common de la bdd
      ATTENTION ICI on supprimer l service_common de la bdd
      @return true si ok
  */
bool service_common::remove(){
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    // Construction de la requette
    QString req = "DELETE FROM tab_services_commons";
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
     Charge l service_commons depuis son id
     @param description
     @return vrai si ok
  */
bool service_common::loadFromID(const int& id) {
    std::string val;
    float fVal;
    IBPP::Timestamp dateCrea, theDate;
    QString req = "SELECT * FROM tab_services_commons WHERE ID='";
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
     Permet de savoir si un service_common general est deja present dans la base
     @param name, nom du service_common
     @return l id du service
  */
int service_common::isHere(const QString& name) {
    int iVal;
    QString req = "SELECT ID FROM tab_services_commons WHERE UPPER(NAME COLLATE UTF8)=UPPER('";
    req +=  QString(name).replace("\'","''").toUtf8() + "' COLLATE UTF8);";
    if(name.isEmpty()) return -1;

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


/**
     Liste les donnees du champ en fonction du filtre
     @return valeur de retour sous forme de liste de chaine de type service_commonList
  */
void service_common::getServiceCommList(serviceCommList& list, QString order, QString filter, QString field) {
    std::string sVal;
    float fVal;
    int iVal;
    IBPP::Timestamp dateCrea, theDate;
    QString req = "SELECT *"
                  " FROM tab_services_commons ";

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



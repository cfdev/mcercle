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

#include "productcategory.h"

#include <QMessageBox>
#include <QStringList>

category::category(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;
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
    QString req = "INSERT INTO tab_products_categories(NAME, COLOR) ";
    req += "VALUES(";
    req += "'" + m_name.replace("\'","''").toUtf8() + "', '" + m_color.name() +"');";

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
bool category::update() {
    // Construction de la requette
    QString req = "UPDATE tab_products_categories SET ";
    req += "NAME='" + m_name.replace("\'","''").toUtf8() + "', ";
    req += "COLOR='" + m_color.name() + "' ";
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
      Suppression de la categorie
  */
bool category::remove() {
    // Construction de la requette
    QString req = "DELETE FROM tab_products_categories  WHERE ID='";
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
     Charge la categorie depuis son nom
     @param nom categorie
     @return vrai si ok
  */
bool category::loadFromName(const QString& name) {
    std::string val;
    QString req = "SELECT * FROM tab_products_categories WHERE UPPER(NAME COLLATE UTF8)= UPPER('";
    req += QString(name).replace("\'","''").toUtf8() +"' COLLATE UTF8);";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            val="";
            m_st->Get("COLOR", val);
            m_color.setNamedColor( QString::fromUtf8(val.c_str()) );
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
     Charge la categorie depuis son ID
     @param ID
     @return vrai si ok
  */
bool category::loadFromID(const int& id) {
    std::string val;
    QString req = "SELECT * FROM tab_products_categories WHERE UPPER(ID)= UPPER('";
    req += QString::number(id) +"');";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            val="";
            m_st->Get("COLOR", val);
            m_color.setNamedColor( QString::fromUtf8(val.c_str()) );
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
void category::getcategoryList(categoryList& list, QString order, QString filter, QString field) {
    std::string sVal;
    QString req = "SELECT * FROM tab_products_categories";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }
    req += " ORDER BY UPPER("+order.replace("\'","''").toUtf8()+" COLLATE UTF8) ASC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("NAME", sVal);
            list.name.push_back( QString::fromUtf8(sVal.c_str()) );
            sVal="";
            m_st->Get("COLOR", sVal);
            list.color.push_back( QColor( QString::fromUtf8(sVal.c_str()) ) );
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}


/**
     Permet de savoir si la categorie est deja presente dans la base
     @param name, nom de la categorie
     @return vrai si la categorie exist
  */
bool category::isHere(const QString& name) {
    int iVal;
    QString req = "SELECT COUNT(*) AS PCOUNT FROM tab_products_categories WHERE UPPER(NAME COLLATE UTF8)= UPPER('";
    req +=  QString(name).replace("\'","''").toUtf8() + "' COLLATE UTF8);";
    if(name.isEmpty())return false;

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("PCOUNT", iVal);
        m_tr->Commit();
        if(iVal>0)return true;
        else return false;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
}

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

#include "product.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>

product::product(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QLocale &lang, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;

    //langage
    m_lang = lang;

    //Class des fournisseurs
    m_provider = new provider(m_db, m_tr, m_st, m_parent);
    //Class des categories
    m_category = new category(m_db, m_tr, m_st, m_parent);

}

product::~product() {
    delete m_provider;
    delete m_category;
}

/**
    Recevoir les icons des differents etats
*/
QIcon product::getIconState(int state)
{
    switch(state){
        case 0:return QIcon(":/app/warning");break;
        case 1:return QIcon(":/app/quit");break;
        default: break;
    }
    return QIcon("");
}

/**
    Recevoir les text des differents etats
*/
QString product::getTextState(int state)
{
    switch(state){
        case 0:return tr("Stock faible");break;
        case 1:return tr("Stock Alerte");break;
        default: break;
    }
    return "";
}

/**
      Ajout un produit dans la base de donnee
      @return true si ok
  */
bool product::create() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "INSERT INTO tab_products(CODE, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY) ";
    req += "VALUES(";
    req += "'" + m_code.replace("\'","''").toUtf8() + "',";
    req += "'" + f.setNum(m_selling_price,'f',2) + "',";
    req += "'" + f.setNum(m_buying_price,'f',2) + "',";
    req += "'" + f.setNum(m_tax,'f',2) + "',";
    req += "'" + m_name.replace("\'","''").toUtf8() + "',";
    req += "'" + QString::number(m_stock) + "',";
    req += "'" + QString::number(m_stock_warning) + "',";
    req += "'" + QString::number(m_state)  + "',";
    req += "'" + QString::number(m_idProvider/*m_provider->getId()*/)  + "',";
    req += "'" + QString::number(m_idCategory/*m_category->getId()*/)   + "');";
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
      ATTENTION ICI on update sur ID car le code est modifiable !!
      @return true si ok
  */
bool product::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_products SET ";
    req += "CODE='" + m_code.replace("\'","''").toUtf8() + "',";
    req += "SELLING_PRICE='" + f.setNum(m_selling_price,'f',2) + "',";
    req += "BUYING_PRICE='" + f.setNum(m_buying_price,'f',2) + "',";
    req += "TAX='" + f.setNum(m_tax,'f',2) + "',";
    req += "NAME='" + m_name.replace("\'","''").toUtf8() + "',";
    req += "STOCK='" + QString::number(m_stock) + "',";
    req += "STOCK_WARNING='" + QString::number(m_stock_warning) + "',";
    req += "STATE='" + QString::number(m_state) + "',";
    req += "ID_PROVIDER='" + QString::number(m_idProvider/*m_provider->getId()*/) + "',";
    req += "ID_CATEGORY='" + QString::number(m_idCategory/*m_category->getId()*/) + "' ";
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
      Suppression du produit de la bdd
      ATTENTION ICI on supprimer le produit de la bdd
      @return true si ok
  */
bool product::remove(){
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    // Construction de la requette
    QString req = "DELETE FROM tab_products";
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
     Charge le produit depuis son code
     @param code produit
     @return true si ok
  */
bool product::loadFromCode(const QString& code) {
    std::string val;
    IBPP::Timestamp dateCrea;
    QString req = "SELECT * FROM tab_products WHERE UPPER(CODE COLLATE UTF8)=UPPER('";
    req += code +"' COLLATE UTF8);";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_creationDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("CODE", val);
            m_code = QString::fromUtf8(val.c_str());
            val=""; //raz
            m_st->Get("SELLING_PRICE", m_selling_price);
            m_st->Get("BUYING_PRICE", m_buying_price);
            m_st->Get("TAX", m_tax);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            m_st->Get("STOCK", m_stock);
            m_st->Get("STOCK_WARNING", m_stock_warning);
            m_st->Get("STATE", m_state);
            m_st->Get("ID_PROVIDER", m_idProvider);
            m_st->Get("ID_CATEGORY",m_idCategory);
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
     Charge le produit depuis son ID
     @param id produit
     @return true si ok
  */
bool product::loadFromID(const int& id) {
    std::string val;
    IBPP::Timestamp dateCrea;
    QString req = "SELECT * FROM tab_products WHERE ID='";
    req += QString::number(id) +"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        if(m_st->Fetch()){
            m_st->Get("ID", m_id);
            m_st->Get("CREATIONDATE", dateCrea);
            m_creationDate = database::fromIBPPTimeStamp(dateCrea);
            m_st->Get("CODE", val);
            m_code = QString::fromUtf8(val.c_str());
            val=""; //raz
            m_st->Get("SELLING_PRICE", m_selling_price);
            m_st->Get("BUYING_PRICE", m_buying_price);
            m_st->Get("TAX", m_tax);
            m_st->Get("NAME", val);
            m_name = QString::fromUtf8(val.c_str());
            m_st->Get("STOCK", m_stock);
            m_st->Get("STOCK_WARNING", m_stock_warning);
            m_st->Get("STATE", m_state);
            m_st->Get("ID_PROVIDER", m_idProvider);
            m_st->Get("ID_CATEGORY",m_idCategory); 
        }
        else{
            m_tr->Commit();
            return false;
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
     Permet de savoir si un produit est deja present dans la base
     @param code, code produit
     @return vrai si le produit exist
  */
bool product::isHere(const QString& code) {
    int iVal=0;
    QString req = "SELECT COUNT(*) AS PCOUNT FROM tab_products WHERE UPPER(CODE COLLATE UTF8)=UPPER('";
    req += code + "' COLLATE UTF8);";
    if(code.isEmpty())return false;

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

/**
     Permet de savoir combien de produit il y a dans la bdd
     @return le nombre de produit
  */
int product::count(QString filter, QString field, bool showObsoleteProduct) {
    int count=0;
    //Requete 1 nombre de valeur
    QString req = "SELECT COUNT(*) FROM (SELECT tab_products.CODE FROM tab_products"
            " LEFT OUTER JOIN tab_products_categories"
            " ON tab_products.ID_CATEGORY = tab_products_categories.ID";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
        if(!showObsoleteProduct)
            req += " AND STATE='1'";
        req += " );";
    }
    else{
        if(!showObsoleteProduct)
            req += " WHERE STATE='1'";
        req += ");";
    }

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


/**
     Avoir une liste des produits existants en fonction des parametres d entrees
     Jointure avec LEFT OUTER JOIN car avec les select, les conditions ne
     sont pas remplis il ny a pas de retour produit
     @param list, liste des produits sous forme ProductList
     @param first, skip ,permettent de fractionner la liste
     @param filter, field permettent de filtrer la liste
     @return true si ok
  */
bool product::getProductList(ProductList& list, int first, int skip, QString filter, QString field) {
    std::string sVal;
    int iVal;
    float fVal;
    QString req = "SELECT FIRST "+QString::number(first)+" SKIP "+QString::number(skip)+" tab_products.ID, tab_products.CODE, tab_products.NAME, tab_products.SELLING_PRICE, tab_products_categories.NAME AS CATEGORY, tab_products_categories.COLOR AS CATEGORYCOLOR, tab_providers.NAME AS PROVIDER, tab_products.STOCK, tab_products.STOCK_WARNING, tab_products.STATE"
                  " FROM tab_products"
                  " LEFT OUTER JOIN tab_products_categories"
                  "  ON tab_products.ID_CATEGORY = tab_products_categories.ID"
                  " LEFT OUTER JOIN tab_providers"
                  "  ON tab_products.ID_PROVIDER = tab_providers.ID";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }
    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += " ORDER BY UPPER(TAB_PRODUCTS.NAME COLLATE UTF8) ASC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back(iVal);iVal=0;
            m_st->Get("CODE", sVal);            
            list.code << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("NAME", sVal);
            list.name <<  QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("SELLING_PRICE", fVal);
            list.selling_price.push_back(fVal);fVal=0;
            sVal=""; //raz
            m_st->Get("CATEGORY", sVal);
            list.category <<  QString::fromUtf8(sVal.c_str());sVal="";
            sVal=""; //raz
            m_st->Get("CATEGORYCOLOR", sVal);
            list.categoryColor.push_back( QColor( QString::fromUtf8(sVal.c_str()) ) );sVal="";
            sVal=""; //raz
            m_st->Get("PROVIDER", sVal);
            list.provider <<  QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("STOCK", iVal);
            list.stock.push_back(iVal);iVal=0;
            m_st->Get("STOCK_WARNING", iVal);
            list.stockWarning.push_back(iVal);iVal=0;
            m_st->Get("STATE", iVal);
            list.state.push_back(iVal);iVal=0;
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
     Avoir le fournisseur associe
     Jointures de table pour en sortir le champ voulue !
     @return valeur de retour sous forme de chaine
  */
QString product::getProvider() {
    QString data;
    std::string val;
    QString req =   "SELECT tab_providers.NAME"
                    " FROM tab_providers, tab_products"
                    " WHERE tab_products.ID_PROVIDER = tab_providers.ID"
                    " AND tab_products.ID='"+QString::number(m_id)+"'"
                    " ORDER BY NAME ASC;";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("NAME", val);
        data = QString::fromUtf8(val.c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return "";
    }
    return data;
}


/**
     Avoir la categorie du produit
     Jointures de table pour en sortir le champ voulue !
     @return valeur de retour sous forme de chaine
  */
QString product::getCategory() {
    QString data;
    std::string val;
    QString req =   "SELECT tab_products_categories.NAME"
                    " FROM tab_products_categories, tab_products"
                    " WHERE tab_products.ID_CATEGORY = tab_products_categories.ID"
                    " AND tab_products.ID='"+QString::number(m_id)+"'"
                    " ORDER BY NAME ASC;";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("NAME", val);
        data = QString::fromUtf8(val.c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return "";
    }
    return data;
}


/**
     Avoir une liste des produits existants en fonction des parametres d entrees
     Jointure avec LEFT OUTER JOIN car avec les select, les conditions ne
     sont pas remplis il ny a pas de retour produit
     @return true si ok
  */
bool product::getProductListStockAlert(ProductList& list) {
    std::string sVal;
    int iVal;
    float fVal;
    QString req =   "SELECT tab_products.ID, tab_products.CODE, tab_products.NAME, tab_products.SELLING_PRICE, tab_products_categories.NAME AS CATEGORY, tab_products_categories.COLOR AS CATEGORYCOLOR, tab_providers.NAME AS PROVIDER, tab_products.STOCK, tab_products.STOCK_WARNING, tab_products.STATE "
                    "FROM TAB_PRODUCTS "
                    "LEFT OUTER JOIN tab_products_categories ON tab_products.ID_CATEGORY = tab_products_categories.ID "
                    "LEFT OUTER JOIN tab_providers ON tab_products.ID_PROVIDER = tab_providers.ID "
                    "WHERE TAB_PRODUCTS.STATE='1' AND TAB_PRODUCTS.STOCK <= (TAB_PRODUCTS.STOCK_WARNING*2) ";

    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += "ORDER BY UPPER(TAB_PRODUCTS.NAME COLLATE UTF8) ASC; ";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back(iVal);iVal=0;
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("NAME", sVal);
            list.name <<  QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("SELLING_PRICE", fVal);
            list.selling_price.push_back(fVal);fVal=0;
            sVal=""; //raz
            m_st->Get("CATEGORY", sVal);
            list.category <<  QString::fromUtf8(sVal.c_str());sVal="";
            sVal=""; //raz
            m_st->Get("CATEGORYCOLOR", sVal);
            list.categoryColor.push_back( QColor( QString::fromUtf8(sVal.c_str()) ) );sVal="";
            sVal=""; //raz
            m_st->Get("PROVIDER", sVal);
            list.provider <<  QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("STOCK", iVal);
            list.stock.push_back(iVal);iVal=0;
            m_st->Get("STOCK_WARNING", iVal);
            list.stockWarning.push_back(iVal);iVal=0;
            m_st->Get("STATE", iVal);
            list.state.push_back(iVal);iVal=0;
        }
        m_tr->Commit();
        return true;
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return false;
}

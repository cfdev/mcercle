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
#include <QVariant>
#include <QBuffer>

product::product(QSqlDatabase db, QLocale &lang, QWidget *parent): m_parent(parent) {
    m_db = db;

    //langage
    m_lang = lang;

    //Class des fournisseurs
    m_provider = new provider(m_db, m_parent);
    //Class des categories
    m_category = new category(m_db, m_parent);

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
   /* QByteArray data;
    QBuffer buf(&data);
    writes image into ba in PNG format
    m_image.save(&buf,"PNG");*/

    QString f;
    QString req = "INSERT INTO TAB_PRODUCTS(CREATIONDATE, CODE, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE,ID_PROVIDER, ID_CATEGORY) "; //  IMAGE,
    req += "VALUES(";
    req += "'" + QDateTime::currentDateTime().toString(tr("yyyy/MM/dd-HH:mm:ss")) + "',";
    req += "'" + m_code.replace("\'","''") + "',";
    req += "'" + f.setNum(m_selling_price,'f',2) + "',";
    req += "'" + f.setNum(m_buying_price,'f',2) + "',";
    req += "'" + f.setNum(m_tax,'f',2) + "',";
    req += "'" + m_name.replace("\'","''") + "',";
    req += "'" + QString::number(m_stock) + "',";
    req += "'" + QString::number(m_stock_warning) + "',";
    req += "'" + QString::number(m_state)  + "',";
  //  req += " :data,";
    req += "'" + QString::number(m_idProvider)  + "',";
    req += "'" + QString::number(m_idCategory)   + "');";

    QSqlQuery query;
    query.prepare(req);
    //query.bindValue( ":data", buf.data() );
    if(!query.exec()) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
    else return true;
}


/**
      Applique les informations courantes dans la base de donnee
      ATTENTION ICI on update sur ID car le code est modifiable !!
      @return true si ok
  */
bool product::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
   /* QByteArray data;
    QBuffer buf(&data);
     writes image into ba in PNG format
    m_image.save(&buf,"PNG");*/

    QString f;
    QString req = "UPDATE TAB_PRODUCTS SET ";
    req += "CODE='" + m_code.replace("\'","''") + "',";
    req += "SELLING_PRICE='" + f.setNum(m_selling_price,'f',2) + "',";
    req += "BUYING_PRICE='" + f.setNum(m_buying_price,'f',2) + "',";
    req += "TAX='" + f.setNum(m_tax,'f',2) + "',";
    req += "NAME='" + m_name.replace("\'","''") + "',";
    req += "STOCK='" + QString::number(m_stock) + "',";
    req += "STOCK_WARNING='" + QString::number(m_stock_warning) + "',";
    req += "STATE='" + QString::number(m_state) + "',";
  //  req += "IMAGE = :data,";
    req += "ID_PROVIDER='" + QString::number(m_idProvider) + "',";
    req += "ID_CATEGORY='" + QString::number(m_idCategory) + "' ";
    req += "WHERE ID="+ QString::number(m_id) +";";

    QSqlQuery query;
    query.prepare(req);
    //query.bindValue( ":data", buf.data() );
    if(!query.exec()) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
    else return true;
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
    QString req = "DELETE FROM TAB_PRODUCTS";
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
     Charge le produit depuis son code
     @param code produit
     @return true si ok
  */
bool product::loadFromCode(const QString& code) {
    QString req = "SELECT * FROM TAB_PRODUCTS WHERE UPPER(CODE)=UPPER('";
    req += code +"');";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        query.next();
        m_id = query.value(query.record().indexOf("ID")).toInt();
        m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
        m_code = query.value(query.record().indexOf("CODE")).toString();
        m_selling_price = query.value(query.record().indexOf("SELLING_PRICE")).toFloat();
        m_buying_price = query.value(query.record().indexOf("BUYING_PRICE")).toFloat();
        m_tax = query.value(query.record().indexOf("TAX")).toFloat();
        m_name = query.value(query.record().indexOf("NAME")).toString();
        m_stock = query.value(query.record().indexOf("STOCK")).toInt();
        m_stock_warning = query.value(query.record().indexOf("STOCK_WARNING")).toInt();
        m_state = query.value(query.record().indexOf("STATE")).toInt();
        m_idProvider = query.value(query.record().indexOf("ID_PROVIDER")).toInt();
        m_idCategory = query.value(query.record().indexOf("ID_CATEGORY")).toInt();
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}

/**
     Charge le produit depuis son ID
     @param id produit
     @return true si ok
  */
bool product::loadFromID(const int& id) {
    QByteArray data;
    m_id = id;
    QString req = "SELECT * FROM TAB_PRODUCTS WHERE ID=";
    req += QString::number(id) +";";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        query.next();
        m_creationDate = query.value(query.record().indexOf("CREATIONDATE")).toDateTime();
        m_code = query.value(query.record().indexOf("CODE")).toString();
        m_selling_price = query.value(query.record().indexOf("SELLING_PRICE")).toFloat();
        m_buying_price = query.value(query.record().indexOf("BUYING_PRICE")).toFloat();
        m_tax = query.value(query.record().indexOf("TAX")).toFloat();
        m_name = query.value(query.record().indexOf("NAME")).toString();
        m_stock = query.value(query.record().indexOf("STOCK")).toInt();
        m_stock_warning = query.value(query.record().indexOf("STOCK_WARNING")).toInt();
        m_state = query.value(query.record().indexOf("STATE")).toInt();
        m_idProvider = query.value(query.record().indexOf("ID_PROVIDER")).toInt();
        m_idCategory = query.value(query.record().indexOf("ID_CATEGORY")).toInt();
     /*   QImage img;
        m_image = img;
        data = query.value(query.record().indexOf("IMAGE")).toByteArray();
        m_image.loadFromData(data,"PNG");*/
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Permet de savoir si un produit est deja present dans la base
     @param code, code produit
     @return vrai si le produit exist
  */
bool product::isHere(const QString& code) {
    int count=0;
    QString req = "SELECT COUNT(*) FROM TAB_PRODUCTS AS PCOUNT WHERE UPPER(CODE)=UPPER('";
    req += code + "');";
    if(code.isEmpty())return false;

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
     Permet de savoir combien de produit il y a dans la bdd
     @return le nombre de produit
  */
int product::count(QString filter, QString field, bool showObsoleteProduct) {
    int count=0;
    //Requete 1 nombre de valeur
    QString req = "SELECT COUNT(*) FROM (SELECT TAB_PRODUCTS.CODE FROM TAB_PRODUCTS"
            " LEFT OUTER JOIN TAB_PRODUCTS_CATEGORIES"
            " ON TAB_PRODUCTS.ID_CATEGORY = TAB_PRODUCTS_CATEGORIES.ID";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''");
        req += ") LIKE UPPER('";
        req += filter.replace("\'","''");
        req += "%')";
        if(!showObsoleteProduct)
            req += " AND STATE=1";
    }
    else{
        if(!showObsoleteProduct)
            req += " WHERE STATE=1";
    }
    req += ")  AS PCOUNT;";

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

/**
     Permet de d obtenir le dernier id genere
     @return l id
  */
int product::getLastId(){
    int iVal=-1;
    QString req;
    if(m_db.driverName() == "QSQLITE")
        req = "SELECT seq FROM sqlite_sequence WHERE name=\"TAB_PRODUCTS\";";
    else    req = "SHOW TABLE STATUS LIKE 'TAB_PRODUCTS\';";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        query.next();
        if(m_db.driverName() == "QSQLITE")
            iVal = query.value(query.record().indexOf("seq")).toInt();
        else
            iVal = query.value(query.record().indexOf("Auto_increment")).toInt()-1;
        return iVal;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return -1;
    }
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

    QString req = "SELECT TAB_PRODUCTS.ID, TAB_PRODUCTS.CODE, TAB_PRODUCTS.NAME, TAB_PRODUCTS.SELLING_PRICE, TAB_PRODUCTS_CATEGORIES.NAME AS CATEGORY, TAB_PRODUCTS_CATEGORIES.COLOR AS CATEGORYCOLOR, TAB_PROVIDERS.NAME AS PROVIDER, TAB_PRODUCTS.STOCK, TAB_PRODUCTS.STOCK_WARNING, TAB_PRODUCTS.STATE"
                  " FROM TAB_PRODUCTS"
                  " LEFT OUTER JOIN TAB_PRODUCTS_CATEGORIES"
                  "  ON TAB_PRODUCTS.ID_CATEGORY = TAB_PRODUCTS_CATEGORIES.ID"
                  " LEFT OUTER JOIN TAB_PROVIDERS"
                  "  ON TAB_PRODUCTS.ID_PROVIDER = TAB_PROVIDERS.ID";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''");
        req += ") LIKE UPPER('";
        req += filter.replace("\'","''");
        req += "%')";
    }
    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += " ORDER BY UPPER(TAB_PRODUCTS.NAME) ASC LIMIT "+QString::number(skip)+","+QString::number(first)+"; ";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        while (query.next()){
            list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
            list.code << query.value(query.record().indexOf("CODE")).toString();
            list.name << query.value(query.record().indexOf("NAME")).toString();
            list.selling_price.push_back( query.value(query.record().indexOf("SELLING_PRICE")).toFloat() );
            list.category << query.value(query.record().indexOf("CATEGORY")).toString();
            list.categoryColor.push_back( query.value(query.record().indexOf("CATEGORYCOLOR")).toString() );
            list.provider << query.value(query.record().indexOf("PROVIDER")).toString();
            list.stock.push_back( query.value(query.record().indexOf("STOCK")).toInt() );
            list.stockWarning.push_back( query.value(query.record().indexOf("STOCK_WARNING")).toInt() );
            list.state.push_back( query.value(query.record().indexOf("STATE")).toInt() );
        }
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}


/**
     Avoir le fournisseur associe
     Jointures de table pour en sortir le champ voulue !
     @return valeur de retour sous forme de chaine
  */
QString product::getProvider() {
    QString data;
    QString req =   "SELECT TAB_PROVIDERS.NAME"
                    " FROM TAB_PROVIDERS, TAB_PRODUCTS"
                    " WHERE TAB_PRODUCTS.ID_PROVIDER = TAB_PROVIDERS.ID"
                    " AND TAB_PRODUCTS.ID="+QString::number(m_id)+
                    " ORDER BY TAB_PROVIDERS.NAME ASC;";
    QSqlQuery query;
    query.prepare(req);

    if(query.exec()){
        query.next();
        data = query.value(query.record().indexOf("NAME")).toString();
        return data;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return "";
    }
}


/**
     Avoir la categorie du produit
     Jointures de table pour en sortir le champ voulue !
     @return valeur de retour sous forme de chaine
  */
QString product::getCategory() {
    QString data;
     QString req =   "SELECT TAB_PRODUCTS_CATEGORIES.NAME"
                    " FROM TAB_PRODUCTS_CATEGORIES, TAB_PRODUCTS"
                    " WHERE TAB_PRODUCTS.ID_CATEGORY = TAB_PRODUCTS_CATEGORIES.ID"
                    " AND TAB_PRODUCTS.ID="+QString::number(m_id)+
                    " ORDER BY TAB_PRODUCTS_CATEGORIES.NAME ASC;";
    QSqlQuery query;
    query.prepare(req);

    if(query.exec()){
        query.next();
        data = query.value(query.record().indexOf("NAME")).toString();
        return data;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return "";
    }
}


/**
     Avoir une liste des produits existants en fonction des parametres d entrees
     Jointure avec LEFT OUTER JOIN car avec les select, les conditions ne
     sont pas remplis il ny a pas de retour produit
     @return true si ok
  */
bool product::getProductListStockAlert(ProductList& list) {
    QString req =   "SELECT TAB_PRODUCTS.ID, TAB_PRODUCTS.CODE, TAB_PRODUCTS.NAME, TAB_PRODUCTS.SELLING_PRICE, TAB_PRODUCTS_CATEGORIES.NAME AS CATEGORY, TAB_PRODUCTS_CATEGORIES.COLOR AS CATEGORYCOLOR, TAB_PROVIDERS.NAME AS PROVIDER, TAB_PRODUCTS.STOCK, TAB_PRODUCTS.STOCK_WARNING, TAB_PRODUCTS.STATE "
                    "FROM TAB_PRODUCTS "
                    "LEFT OUTER JOIN TAB_PRODUCTS_CATEGORIES ON TAB_PRODUCTS.ID_CATEGORY = TAB_PRODUCTS_CATEGORIES.ID "
                    "LEFT OUTER JOIN TAB_PROVIDERS ON TAB_PRODUCTS.ID_PROVIDER = TAB_PROVIDERS.ID "
                    "WHERE TAB_PRODUCTS.STATE=1 AND TAB_PRODUCTS.STOCK <= (TAB_PRODUCTS.STOCK_WARNING*2) ";

    // ATTENTION PEUT FAIRE RALENTIR LE TRAITEMENT
    req += "ORDER BY UPPER(TAB_PRODUCTS.NAME) ASC; ";

    QSqlQuery query;
    query.prepare(req);
    if(query.exec()){
        while (query.next()){
            list.id.push_back( query.value(query.record().indexOf("ID")).toInt() );
            list.code << query.value(query.record().indexOf("CODE")).toString();
            list.name << query.value(query.record().indexOf("NAME")).toString();
            list.selling_price.push_back( query.value(query.record().indexOf("SELLING_PRICE")).toFloat() );
            list.category << query.value(query.record().indexOf("CATEGORY")).toString();
            list.categoryColor.push_back( query.value(query.record().indexOf("CATEGORYCOLOR")).toString() );
            list.provider << query.value(query.record().indexOf("PROVIDER")).toString();
            list.stock.push_back( query.value(query.record().indexOf("STOCK")).toInt() );
            list.stockWarning.push_back( query.value(query.record().indexOf("STOCK_WARNING")).toInt() );
            list.state.push_back( query.value(query.record().indexOf("STATE")).toInt() );
        }
        return true;
    }
    else{
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
        return false;
    }
}

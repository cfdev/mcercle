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

#include "invoice.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>


invoice::invoice(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;

    m_id = 0;
    m_idCustomer = 0;
    m_state = 0;
    m_price = 0;
    m_partPayment = 0;
}

invoice::~invoice() {
}

/**
    Recevoir les icons des differents etats
*/
QIcon invoice::getIconState(int state)
{
    switch(state){
        case UNPAID:return QIcon(":/app/warning");break;
        case PAID:return QIcon(":/app/ok");break;
        default: break;
    }
    return QIcon("");
}

/**
    Recevoir les text des differents etats
*/
QString invoice::getTextState(int state)
{
    switch(state){
        case UNPAID:return tr("Non R\351gl\351e");break;
        case PAID:return tr("R\351gl\351e");break;
        default: break;
    }
    return "";
}


/**
      Ajoute un facture dans la base de donnee
      @return true si ok
  */
bool invoice::create() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "INSERT INTO tab_invoices(ID_CUSTOMER, CODE, IDATE, LIMIT_PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION) ";
    req += "VALUES(";
    req += "'" + QString::number(m_idCustomer) + "',";
    req += "'" + m_code.replace("\'","''").toUtf8() + "',";
    req += "'" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_limitPayment.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_typePayment + "',";    
    req += "'" + f.setNum(m_partPayment,'f',2) + "',";
    req += "'" + f.setNum(m_price,'f',2) + "',";
    req += "'" + QString::number(m_state)  + "',";
    req += "'" + m_description.replace("\'","''").toUtf8() + "');";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();        
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    return true;
}


/**
      Applique les informations courantes dans la base de donnee
      ATTENTION ICI on update sur ID car le code est modifiable !!
      @return true si ok
  */
bool invoice::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_invoices SET ";
    req += "CODE='" + m_code.replace("\'","''").toUtf8() + "',";
    req += "IDATE='" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "LIMIT_PAYMENTDATE='" + m_limitPayment.toString(tr("yyyy/MM/dd")) + "',";
    req += "TYPE_PAYMENT='" + m_typePayment + "',";
    req += "PART_PAYMENT='" + f.setNum(m_partPayment,'f',2) + "',";
    req += "PRICE='" + f.setNum(m_price,'f',2) + "',";
    req += "STATE='" + QString::number(m_state)  + "',";
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
      Suppression du facture de la bdd
      ATTENTION ICI on supprimer le facture de la bdd avec les details associes
      @return true si ok
  */
bool invoice::remove(){
    // Construction de la requette pour supprimer la facture
    QString req = "DELETE FROM tab_invoices";
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
    Charge la facture depuis son ID
    @param id invoice
    @return true si ok
  */
bool invoice::loadFromID(const int& id)
{
    std::string sVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date udate;
    QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.ID_CUSTOMER, TAB_INVOICES.CREATIONDATE, TAB_INVOICES.TYPE_PAYMENT, TAB_INVOICES.PART_PAYMENT, TAB_INVOICES.IDATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE, TAB_INVOICES.STATE "
            "FROM TAB_INVOICES "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "LEFT OUTER JOIN TAB_PROPOSALS "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "WHERE TAB_INVOICES.ID= '" + QString::number(id)+"' ";
    m_id = 0;
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        m_st->Fetch();
        m_st->Get("ID", m_id);
        m_st->Get("ID_CUSTOMER", m_idCustomer);
        m_st->Get("CREATIONDATE", dateCrea);
        m_creationDate = database::fromIBPPTimeStamp(dateCrea);
        m_st->Get("IDATE", udate);
        m_userDate = database::fromIBPPDate(udate);
        m_st->Get("LIMIT_PAYMENTDATE", udate);
        m_limitPayment = database::fromIBPPDate(udate);
        m_st->Get("ICODE", sVal);
        m_code = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("PCODE", sVal);
        m_proposalCode = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("TYPE_PAYMENT",sVal);
        m_typePayment = QString::fromUtf8(sVal.c_str());
        m_st->Get("PART_PAYMENT", m_partPayment);
        m_st->Get("PRICE", m_price);
        sVal="";
        m_st->Get("DESCRIPTION", sVal);
        m_description = QString::fromUtf8(sVal.c_str());
        m_st->Get("STATE", m_state);
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
    Charge la facture depuis son code
    @param code
    @return true si ok
  */
bool invoice::loadFromCode(const QString& code)
{
    std::string sVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date udate;
    QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.ID_CUSTOMER, TAB_INVOICES.CREATIONDATE, TAB_INVOICES.TYPE_PAYMENT, TAB_INVOICES.PART_PAYMENT, TAB_INVOICES.IDATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE, TAB_INVOICES.STATE "
            "FROM TAB_INVOICES "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "LEFT OUTER JOIN TAB_PROPOSALS "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "WHERE TAB_INVOICES.CODE = '" + code +"' ";
    m_id = 0;
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        m_st->Fetch();
        m_st->Get("ID", m_id);
        m_st->Get("ID_CUSTOMER", m_idCustomer);
        m_st->Get("CREATIONDATE", dateCrea);
        m_creationDate = database::fromIBPPTimeStamp(dateCrea);
        m_st->Get("IDATE", udate);
        m_userDate = database::fromIBPPDate(udate);
        m_st->Get("LIMIT_PAYMENTDATE", udate);
        m_limitPayment = database::fromIBPPDate(udate);
        m_st->Get("ICODE", sVal);
        m_code = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("PCODE", sVal);
        m_proposalCode = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("TYPE_PAYMENT",sVal);
        m_typePayment = QString::fromUtf8(sVal.c_str());
        m_st->Get("PART_PAYMENT", m_partPayment);
        m_st->Get("PRICE", m_price);
        sVal="";
        m_st->Get("DESCRIPTION", sVal);
        m_description = QString::fromUtf8(sVal.c_str());
        m_st->Get("STATE", m_state);
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
     Permet de savoir si un facture est deja present dans la base
     @param code facture
     @return vrai si le facture existe
  */
bool invoice::isHere(const QString& code) {
    int iVal;
    QString req = "SELECT COUNT(*) AS PCOUNT FROM tab_invoices WHERE UPPER(CODE COLLATE UTF8)=UPPER('";
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
     Permet de d obtenir le dernier id genere
     @return l id
  */
int invoice::getLastId(){
    int iVal;
    QString req = "SELECT GEN_ID(GEN_INVOICES_ID, 0) FROM rdb$database;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("GEN_ID", iVal);
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
     @param InvoiceList liste de retour
     @return valeur de retour sous forme de liste de chaine de type InvoiceList
  */
void invoice::getInvoiceList(InvoiceList& list, int id_customer, QString order, QString filter, QString field) {
    std::string sVal;
    int iVal;
    float fVal;
    IBPP::Date iDate;
    QString req = "SELECT TAB_INVOICES.ID, TAB_INVOICES.IDATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.DESCRIPTION, "
            "TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.PRICE, TAB_INVOICES.STATE "
            "FROM TAB_INVOICES "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "LEFT OUTER JOIN TAB_PROPOSALS "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "WHERE TAB_INVOICES.ID_CUSTOMER = '" + QString::number(id_customer)+"' ";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field;
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter;
        req += "%' COLLATE UTF8)";
    }

    req += " ORDER BY UPPER("+order+") DESC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal );
            m_st->Get("IDATE", iDate);
            list.userDate.push_back( database::fromIBPPDate(iDate) );
            m_st->Get("LIMIT_PAYMENTDATE", iDate);
            list.limitPayment.push_back( database::fromIBPPDate(iDate) );
            m_st->Get("ICODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("PCODE", sVal);
            list.codeProposal << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("DESCRIPTION", sVal);
            list.description <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("PRICE", fVal);
            list.price.push_back(fVal);
            sVal=""; //raz
            m_st->Get("STATE", iVal);
            list.state.push_back(iVal);
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}

/**
     Retour la liste des factures en alert
     @param InvoiceList liste de retour
     @return valeur de retour sous forme de liste de chaine de type InvoiceList
  */
void invoice::getInvoiceListAlert(InvoiceListAlert& list) {
    std::string sVal;
    int iVal;
    float fVal;
    IBPP::Date iDate;
    QString req =   "SELECT TAB_CUSTOMERS.FIRSTNAME, TAB_CUSTOMERS.LASTNAME, TAB_INVOICES.ID, TAB_INVOICES.IDATE, TAB_INVOICES.LIMIT_PAYMENTDATE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.CODE, TAB_INVOICES.DESCRIPTION, TAB_INVOICES.PRICE, TAB_INVOICES.STATE "
                    "FROM TAB_INVOICES "
                    "LEFT OUTER JOIN TAB_CUSTOMERS "
                    "ON TAB_INVOICES.ID_CUSTOMER = TAB_CUSTOMERS.ID "
                    "WHERE TAB_INVOICES.STATE != '1' "
                    "ORDER BY UPPER(IDATE) ASC; ";
    //Liste les factures en alert avec les plus vielles en premier
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("FIRSTNAME", sVal);
            list.customerFirstName << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("LASTNAME", sVal);
            list.customerLastName << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("ID", iVal);
            list.id.push_back( iVal );
            m_st->Get("IDATE", iDate);
            list.userDate.push_back( database::fromIBPPDate(iDate) );
            m_st->Get("LIMIT_PAYMENTDATE", iDate);
            list.limitPayment.push_back( database::fromIBPPDate(iDate) );
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("DESCRIPTION", sVal);
            list.description <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("PRICE", fVal);
            list.price.push_back(fVal);
            sVal=""; //raz
            m_st->Get("STATE", iVal);
            list.state.push_back(iVal);
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}


/**
     Listes des articles de la facture
     @return valeur de retour sous forme de liste de chaine de type InvoiceItem
  */
void invoice::getInvoiceItemsList(InvoiceListItems& list, QString order, QString filter, QString field) {
    std::string sVal;
    int iVal;
    float fVal;
    QString req = "SELECT * FROM tab_invoices_details WHERE ID_INVOICE='" + QString::number(this->m_id)+" '";


    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field;
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter;
        req += "%' COLLATE UTF8)";
    }

    req += " ORDER BY UPPER("+order+" COLLATE UTF8) ASC;";

    /* Clear les vals */
    list.id.clear();
    list.name.clear();
    list.discount.clear();
    list.quantity.clear();
    list.tax.clear();
    list.price.clear();

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal );iVal=0;
            m_st->Get("ID_PRODUCT", iVal);
            list.idProduct.push_back( iVal );iVal=0;
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("DISCOUNT", iVal);
            list.discount.push_back(iVal);iVal=0;
            m_st->Get("QUANTITY", iVal);
            list.quantity.push_back(iVal);iVal=0;
            m_st->Get("TAX", fVal);
            list.tax.push_back(fVal);fVal=0;
            m_st->Get("PRICE", fVal);
            list.price.push_back(fVal);fVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}


/**
  Obtenir le detail dun article

  @param id de l article
  @return valeur de retour sous forme de liste de chaine de type InvoiceItem
  */
void invoice::getInvoiceItem(InvoiceItem& item){
    std::string sVal;
    int iVal;
    float fVal;
    QString req =   "SELECT * FROM tab_invoices_details WHERE ID_INVOICE='"
                    + QString::number(this->m_id)+" ' AND ID='"
                    + QString::number(item.id)+" ';";

    /* Clear les vals */
    item.id = -1;
    item.name="";
    item.discount=0;
    item.quantity=0;
    item.tax=0;
    item.price=0;

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            item.id = iVal; iVal=0;
            m_st->Get("ID_PRODUCT", iVal);
            item.idProduct= iVal ;iVal=0;
            m_st->Get("NAME", sVal);
            item.name = QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("DISCOUNT", iVal);
            item.discount = iVal ;iVal=0;
            m_st->Get("QUANTITY", iVal);
            item.quantity = iVal ;iVal=0;
            m_st->Get("TAX", fVal);
            item.tax = fVal; fVal=0;
            m_st->Get("PRICE", fVal);
            item.price = fVal; fVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}


/**
  Ajoute un article
  @return true si ok
  */
bool invoice::addInvoiceItem(InvoiceItem& item){
    QString f;
    // Construction de la requette
    QString req = "INSERT INTO TAB_invoices_details(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE, TAX) ";
    req += "VALUES(";
    req += "'" + QString::number(this->m_id) + "',";
    req += "'" + QString::number(item.idProduct) + "',";
    req += "'" + item.name.replace("\'","''").mid(0, 128).toUtf8() + "',";
    req += "'" + QString::number(item.quantity)  + "',";
    req += "'" + QString::number(item.discount)  + "',";
    req += "'" + f.setNum(item.price,'f',2) + "',";
    req += "'" + f.setNum(item.tax,'f',2) + "');";

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
  Suppression de l article
  @return true si ok
  */
bool invoice::removeInvoiceItem(int Itemid){
    // Construction de la requette
    QString req = "DELETE FROM tab_invoices_details WHERE ID_INVOICE='"
            + QString::number(this->m_id)+" ' AND ID='"
            + QString::number(Itemid)+" ';";

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
    Modifie les valeurs de l article
    @return true si ok
  */
bool invoice::updateInvoiceItem(InvoiceItem& item) {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_invoices_details SET ";
    req += "NAME='" + item.name.replace("\'","''").mid(0, 128).toUtf8() + "',";
    req += "ID_PRODUCT='" + QString::number(item.idProduct)  + "',";
    req += "DISCOUNT='" + QString::number(item.discount)  + "',";
    req += "QUANTITY='" + QString::number(item.quantity)  + "',";
    req += "TAX='" + f.setNum(item.tax,'f',2) + "',";
    req += "PRICE='" + f.setNum(item.price,'f',2) + "' ";

    req += "WHERE ID='"+ QString::number(item.id) +"';";

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
     Retour la liste des annees de facture
     @return une liste d annees
     ex: 2011,2012...
  */
QStringList invoice::getYearsList() {
    QStringList ylist;
    int iDate;
    QString req =   "SELECT DISTINCT(EXTRACT(YEAR FROM TAB_INVOICES.IDATE)) AS YEARS "
                    "FROM TAB_INVOICES ORDER BY UPPER(IDATE) DESC;";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("YEARS", iDate);
            ylist.push_back( QString::number(iDate) );
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }

    return ylist;
}

/**
     Retourne le CA d une annee
     @return CA de l annee desiree
  */
float invoice::getYearRevenue(QString year) {
    float total;
    if(year.isEmpty())return 0;
    QString req =   "SELECT SUM( price ) AS TOTAL FROM TAB_INVOICES "
                    "WHERE TAB_INVOICES.STATE = '1' and "
                    "extract(YEAR FROM TAB_INVOICES.IDATE)='"+year+"';";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        m_st->Fetch();
        m_st->Get("TOTAL", total);
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }

    if(total < 0)total = 0;
    return total;
}

/**
     Retourne le CA d un mois
     @return CA du mois desiree
  */
float invoice::getMonthRevenue(QString year, QString month) {
    float total=0; // important init pour le nonReturnValue
    if(year.isEmpty() | month.isEmpty())return 0;

    QString req =   "SELECT SUM( price ) AS TOTAL FROM TAB_INVOICES "
                    "WHERE TAB_INVOICES.STATE = '1' and "
                    "extract(MONTH FROM TAB_INVOICES.IDATE)='"+month+"'and "
                    "extract(YEAR FROM TAB_INVOICES.IDATE)='"+year+"';";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        m_st->Fetch();
        m_st->Get("TOTAL", total);
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }

    return total;
}

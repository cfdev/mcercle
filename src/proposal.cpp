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

#include "proposal.h"
#include "dbase.h"

#include <QMessageBox>
#include <QStringList>
#include <QWidget>


proposal::proposal(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent): m_parent(parent) {
    m_db = db;
    m_tr = tr;
    m_st = st;

    m_id = 0;
    m_idCustomer = 0;
    m_state = 0;
    m_delayDeliveryDate = 0;
    m_price = 0;
}

proposal::~proposal() {
}

/**
    Recevoir les icons des differents etats
*/
QIcon proposal::getIconState(int state)
{
    switch(state){
        case WRITING:return QIcon(":/app/process");break;
        case PROPOSED:return QIcon(":/app/wait");break;
        case VALIDATED:return QIcon(":/app/ok");break;
        default: break;
    }
    return QIcon("");
}

/**
    Recevoir les text des differents etats
*/
QString proposal::getTextState(int state)
{
    switch(state){
        case WRITING:return tr("En \351criture");break;
        case PROPOSED:return tr("Propos\351e");break;
        case VALIDATED:return tr("Sign\351e");break;
        default: break;
    }
    return "";
}


/**
      Ajoute un facture dans la base de donnee
      @return true si ok
  */
bool proposal::create() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "INSERT INTO tab_proposals(ID_CUSTOMER, CODE, PDATE, VALIDDATE, DELIVERYDATE, DELAY_DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION) ";
    req += "VALUES(";
    req += "'" + QString::number(m_idCustomer) + "',";
    req += "'" + m_code.replace("\'","''").toUtf8() + "',";
    req += "'" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_validDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + m_deliveryDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "'" + QString::number(m_delayDeliveryDate)  + "',";
    req += "'" + m_typePayment + "',";    
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
bool proposal::update() {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_proposals SET ";
    req += "CODE='" + m_code.replace("\'","''").toUtf8() + "',";
    req += "PDATE='" + m_userDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "VALIDDATE='" + m_validDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "DELIVERYDATE='" + m_deliveryDate.toString(tr("yyyy/MM/dd")) + "',";
    req += "DELAY_DELIVERYDATE='" + QString::number(m_delayDeliveryDate)  + "',";
    req += "TYPE_PAYMENT='" + m_typePayment + "',";
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
bool proposal::remove(){
    // Construction de la requette pour supprimer la proposition
    QString req = "DELETE FROM tab_proposals";
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
    @param id proposal
    @return true si ok
  */
bool proposal::loadFromID(const int& id)
{
    std::string sVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date udate;
    QString req = "SELECT TAB_PROPOSALS.ID, TAB_PROPOSALS.ID_CUSTOMER, TAB_PROPOSALS.CREATIONDATE, TAB_PROPOSALS.PDATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
            "TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.TYPE_PAYMENT, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
            "FROM TAB_PROPOSALS "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "LEFT OUTER JOIN TAB_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "WHERE TAB_PROPOSALS.ID = '" + QString::number(id)+"';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        m_st->Fetch();
        m_st->Get("ID", m_id);
        m_st->Get("ID_CUSTOMER", m_idCustomer);
        m_st->Get("CREATIONDATE", dateCrea);
        m_creationDate = database::fromIBPPTimeStamp(dateCrea);
        m_st->Get("PDATE", udate);
        m_userDate = database::fromIBPPDate(udate);
        m_st->Get("VALIDDATE", udate);
        m_validDate = database::fromIBPPDate(udate);udate.Clear();
        m_st->Get("DELIVERYDATE", udate);
        m_deliveryDate = database::fromIBPPDate(udate);udate.Clear();
        m_st->Get("DELAY_DELIVERYDATE", m_delayDeliveryDate);
        m_st->Get("PCODE", sVal);
        m_code = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("ICODE", sVal);
        m_InvoiceCode = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("TYPE_PAYMENT",sVal);
        m_typePayment = QString::fromUtf8(sVal.c_str());
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
bool proposal::loadFromCode(const QString& code)
{
    std::string sVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date udate;
    QString req = "SELECT TAB_PROPOSALS.ID, TAB_PROPOSALS.ID_CUSTOMER, TAB_PROPOSALS.CREATIONDATE, TAB_PROPOSALS.PDATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
            "TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.TYPE_PAYMENT, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
            "FROM TAB_PROPOSALS "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "LEFT OUTER JOIN TAB_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "WHERE TAB_PROPOSALS.CODE = '" +code+"';";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_id = 0;
        // list all info
        m_st->Fetch();
        m_st->Get("ID", m_id);
        m_st->Get("ID_CUSTOMER", m_idCustomer);
        m_st->Get("CREATIONDATE", dateCrea);
        m_creationDate = database::fromIBPPTimeStamp(dateCrea);
        m_st->Get("PDATE", udate);
        m_userDate = database::fromIBPPDate(udate);udate.Clear();
        m_st->Get("VALIDDATE", udate);
        m_validDate = database::fromIBPPDate(udate);udate.Clear();
        m_st->Get("DELIVERYDATE", udate);
        m_deliveryDate = database::fromIBPPDate(udate);udate.Clear();
        m_st->Get("DELAY_DELIVERYDATE", m_delayDeliveryDate);
        m_st->Get("PCODE", sVal);
        m_code = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("ICODE", sVal);
        m_InvoiceCode = QString::fromUtf8(sVal.c_str());
        sVal="";
        m_st->Get("TYPE_PAYMENT",sVal);
        m_typePayment = QString::fromUtf8(sVal.c_str());
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
bool proposal::isHere(const QString& code) {
    int iVal;
    QString req = "SELECT COUNT(*) AS PCOUNT FROM tab_proposals WHERE UPPER(CODE COLLATE UTF8)=UPPER('";
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
int proposal::getLastId(){
    int iVal;
    QString req = "SELECT GEN_ID(GEN_PROPOSALS_ID, 0) FROM rdb$database;";

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
     @param ProposalList liste de retour
     @return valeur de retour sous forme de liste de chaine de type ProposalList
  */
void proposal::getProposalList(ProposalList& list, int id_customer, QString order, QString filter, QString field) {
    std::string sVal;
    int iVal;
    float fVal;
    IBPP::Date iDate;
    QString req = "select TAB_PROPOSALS.ID, TAB_PROPOSALS.PDATE, TAB_PROPOSALS.VALIDDATE, TAB_PROPOSALS.DELIVERYDATE, TAB_PROPOSALS.DELAY_DELIVERYDATE, "
            "TAB_PROPOSALS.CODE AS PCODE, TAB_INVOICES.CODE AS ICODE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE, TAB_PROPOSALS.STATE "
            "FROM TAB_PROPOSALS "
            "LEFT OUTER JOIN TAB_LINK_PROPOSALS_INVOICES "
            "ON TAB_PROPOSALS.ID = TAB_LINK_PROPOSALS_INVOICES.ID_PROPOSAL "
            "LEFT OUTER JOIN TAB_INVOICES "
            "ON TAB_INVOICES.ID = TAB_LINK_PROPOSALS_INVOICES.ID_INVOICE "
            "WHERE TAB_PROPOSALS.ID_CUSTOMER = '" + QString::number(id_customer)+"' ";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }

    req += " ORDER BY UPPER("+order.replace("\'","''").toUtf8()+") DESC;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info

        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal );
            m_st->Get("PDATE", iDate);
            list.userDate.push_back( database::fromIBPPDate(iDate) );iDate.Clear();
            m_st->Get("VALIDDATE", iDate);
            list.validDate.push_back( database::fromIBPPDate(iDate) );iDate.Clear();
            m_st->Get("DELIVERYDATE", iDate);
            list.deliveryDate.push_back( database::fromIBPPDate(iDate) );iDate.Clear();
            iVal = 0;
            m_st->Get("DELAY_DELIVERYDATE", iVal);
            list.delayDeliveryDate.push_back(iVal);
            m_st->Get("PCODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("ICODE", sVal);
            list.codeInvoice << QString::fromUtf8(sVal.c_str());
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
     Retour la liste des propositions en cours decritures
     @param ProposalListAlert liste de retour
     @return valeur de retour sous forme de liste de chaine de type ProposalListAlert
  */
void proposal::getProposalListAlert(ProposalListAlert& list) {
    std::string sVal;
    int iVal;
    float fVal;
    IBPP::Date iDate;
    QString req =   "SELECT TAB_CUSTOMERS.FIRSTNAME, TAB_CUSTOMERS.LASTNAME, TAB_PROPOSALS.ID, TAB_PROPOSALS.PDATE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.CODE, TAB_PROPOSALS.DESCRIPTION, TAB_PROPOSALS.PRICE "
                    "FROM TAB_PROPOSALS "
                    "LEFT OUTER JOIN TAB_CUSTOMERS "
                    "ON TAB_PROPOSALS.ID_CUSTOMER = TAB_CUSTOMERS.ID "
                    "WHERE TAB_PROPOSALS.STATE = '0' "
                    "ORDER BY UPPER(PDATE) ASC; ";
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
            m_st->Get("PDATE", iDate);
            list.userDate.push_back( database::fromIBPPDate(iDate) );
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());
            sVal="";
            m_st->Get("DESCRIPTION", sVal);
            list.description <<  QString::fromUtf8(sVal.c_str());
            m_st->Get("PRICE", fVal);
            list.price.push_back(fVal);
            sVal=""; //raz
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
}



/**
     Listes des articles de la facture
     @return valeur de retour sous forme de liste de chaine de type ProposalItem
  */
void proposal::getProposalItemsList(ProposalListItems& list, QString order, QString filter, QString field) {
    std::string sVal;
    int iVal;
    float fVal;
    IBPP::Date iDate;
    QString req = "SELECT * FROM tab_proposals_details WHERE ID_PROPOSAL='" + QString::number(this->m_id)+" '";

    if(!field.isEmpty()){
        req += " WHERE UPPER(";
        req += field.replace("\'","''").toUtf8();
        req += " COLLATE UTF8) LIKE UPPER('";
        req += filter.replace("\'","''").toUtf8();
        req += "%' COLLATE UTF8)";
    }
    req += " ORDER BY UPPER("+order.replace("\'","''").toUtf8()+" COLLATE UTF8) ASC;";

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
  @return valeur de retour sous forme de liste de chaine de type ProposalItem
  */
void proposal::getProposalItem(ProposalItem& item){
    std::string sVal;
    int iVal;
    float fVal;
    QString req =   "SELECT * FROM tab_proposals_details WHERE ID_PROPOSAL='"
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
bool proposal::addProposalItem(ProposalItem& item){
    QString f;
    // Construction de la requette
    QString req = "INSERT INTO TAB_proposals_details(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE, TAX) ";
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
bool proposal::removeProposalItem(int Itemid){
    // Construction de la requette
    QString req = "DELETE FROM tab_proposals_details WHERE ID_PROPOSAL='"
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
bool proposal::updateProposalItem(ProposalItem& item) {
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE tab_proposals_details SET ";
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
    Modifie les valeurs de l article
    @return true si ok
  */
bool proposal::setLink( const int& idProposal, const int& idInvoice ){
    // Construction de la requette
    QString req = "INSERT INTO TAB_LINK_PROPOSALS_INVOICES (ID_PROPOSAL, ID_INVOICE) ";
    req += "VALUES(";
    req += "'" + QString::number(idProposal) + "',";
    req += "'" + QString::number(idInvoice)  + "');";
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


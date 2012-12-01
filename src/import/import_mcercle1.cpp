#include "import_mcercle1.h"
#include "dialogwaiting.h"

#include <QMessageBox>

import_mcercle1::import_mcercle1(QString hostName, int port, QString name, QString login, QString password, QWidget *parent)
{
    m_parent = parent;
    m_hostName = hostName+ "/" + QString::number(port);
    m_name = name;
    m_login = login;
    m_password = password;
}

import_mcercle1::~import_mcercle1() {
}

bool import_mcercle1::run() {
    //Affichage de la fenetre d attente
    DialogWaiting m_DialogWaiting(m_parent);
    m_DialogWaiting.setTitle(tr("<b>Migration de la base de donn&#233;es</b>"));
    m_DialogWaiting.setDetail(tr("Connexion firebird..."));
    m_DialogWaiting.setProgressBarRange(0,14);
    m_DialogWaiting.setModal(true);
    m_DialogWaiting.show();


    //Connexion a firebird
    try  {
        db = IBPP::DatabaseFactory( m_hostName.toStdString().c_str(),
                                    m_name.toStdString().c_str(),
                                    m_login.toStdString().c_str(),
                                    m_password.toStdString().c_str(),
                                    ROLE, CHARSET, OPTIONS );
        db->Connect();
        // Transaction et statement sur une base valide connectee
        m_tr = IBPP::TransactionFactory(db, IBPP::amWrite, IBPP::ilReadCommitted, IBPP::lrWait /*Attente que les autres transaction soient fini*/);
        m_st = IBPP::StatementFactory(db, m_tr);
    }
    catch ( IBPP::Exception& e ) {
        QMessageBox mBox(QMessageBox::Critical, tr("Erreur"), tr("<b>La connexion avec la base de donn&#233;es n&#146;a pas pu &#234;tre &#233;tablie!</b>"),QMessageBox::Ok);
        mBox.setDetailedText ( e.ErrorMessage() );
        mBox.exec();
        return false;
    }


    m_DialogWaiting.setProgressBar(1);
    m_DialogWaiting.setDetail(tr("<i>table_informations...</i>"));
    bool ret = table_informations();

    m_DialogWaiting.setProgressBar(2);
    m_DialogWaiting.setDetail(tr("<i>table_customers...</i>"));
    if(ret)ret = table_customers();

    m_DialogWaiting.setProgressBar(3);
    m_DialogWaiting.setDetail(tr("<i>table_products.....</i>"));
    if(ret)ret = table_products();

    m_DialogWaiting.setProgressBar(4);
    m_DialogWaiting.setDetail(tr("<i>table_products_categories.....</i>"));
    if(ret)ret = table_products_categories();

    m_DialogWaiting.setProgressBar(5);
    m_DialogWaiting.setDetail(tr("<i>table_providers.....</i>"));
    if(ret)ret = table_providers();

    m_DialogWaiting.setProgressBar(6);
    m_DialogWaiting.setDetail(tr("<i>table_services.....</i>"));
    if(ret)ret = table_services();

    m_DialogWaiting.setProgressBar(7);
    m_DialogWaiting.setDetail(tr("<i>table_services_common.....</i>"));
    if(ret)ret = table_services_commons();

    m_DialogWaiting.setProgressBar(8);
    m_DialogWaiting.setDetail(tr("<i>table_proposals.....</i>"));
    if(ret)ret = table_proposals();

    m_DialogWaiting.setProgressBar(9);
    m_DialogWaiting.setDetail(tr("<i>table_proposals_details.....</i>"));
    if(ret)ret = table_proposals_details();

    m_DialogWaiting.setProgressBar(10);
    m_DialogWaiting.setDetail(tr("<i>table_invoices.....</i>"));
    if(ret)ret = table_invoices();

    m_DialogWaiting.setProgressBar(11);
    m_DialogWaiting.setDetail(tr("<i>table_invoices_details.....</i>"));
    if(ret)ret = table_invoices_details();

    m_DialogWaiting.setProgressBar(12);
    m_DialogWaiting.setDetail(tr("<i>table_link_proposals_invoices.....</i>"));
    if(ret)ret = table_link_proposals_invoices();

    return ret;
}

/**
    Permet de passer dune IBPP::Date a une QDate
    @param IBPP::Date
    @return QDate
  */
QDate import_mcercle1::fromIBPPDate(IBPP::Date &IBPPdate)
{
    int y,m,d;
    IBPPdate.GetDate(y,m,d);
    return QDate(y,m,d);
}
/**
    Permet de passer dune IBPPTimeStamp a une QDateTime
    @param IBPPTimeStamp
    @return QDateTime
  */
QDateTime import_mcercle1::fromIBPPTimeStamp(IBPP::Timestamp &dateTime)
{
    int y,m,d,h,min,s,ms;
    dateTime.GetDate(y,m,d);
    dateTime.GetTime(h,min,s,ms);
    return QDateTime(QDate(y,m,d), QTime(h,min,s/*,ms*/), Qt::LocalTime);
}

/**
    TABLE INFORMATIONS
  */
bool import_mcercle1::table_informations() {
    std::string val;
    int iVal;
    Informations info;
    QString req =   "SELECT * from TAB_informations;";
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        m_st->Get("NAME", val);
        info.name = QString::fromUtf8(val.c_str());
        m_st->Get("NUM", val);
        info.num = QString::fromUtf8(val.c_str());
        m_st->Get("CAPITAL", val);
        info.capital = QString::fromUtf8(val.c_str());
        m_st->Get("ADDRESS1", val);
        info.address1 = QString::fromUtf8(val.c_str());
        m_st->Get("ADDRESS2", val);
        info.address2 = QString::fromUtf8(val.c_str());
        m_st->Get("ADDRESS3", val);
        info.address3 = QString::fromUtf8(val.c_str());
        m_st->Get("ZIPCODE", val);
        info.zipCode = QString::fromUtf8(val.c_str());
        m_st->Get("CITY", val);
        info.city = QString::fromUtf8(val.c_str());
        m_st->Get("PHONENUMBER", val);
        info.phoneNumber = QString::fromUtf8(val.c_str());
        m_st->Get("FAXNUMBER", val);
        info.faxNumber = QString::fromUtf8(val.c_str());
        m_st->Get("EMAIL", val);
        info.email = QString::fromUtf8(val.c_str());
        m_st->Get("WEBSITE", val);
        info.webSite = QString::fromUtf8(val.c_str());
        m_st->Get("TAX", iVal);
        info.tax = iVal;
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    req = "INSERT INTO TAB_INFORMATIONS(DBASE_VERSION, TAX, NAME, NUM, CAPITAL, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, WEBSITE)"
          " VALUES('1', '0', '"+info.name+"','"+info.num+"', '"+info.capital+"', '"+info.address1+"', '"+info.address2+"', '"+info.address3+"', '"+info.zipCode+"', '"+info.city+"', '"+info.phoneNumber+"', '"+info.faxNumber+"', '"+info.email+"', '"+info.webSite+"');";
    query.prepare(req);
    if(!query.exec()) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
        return false;
    }

    return true;
}

/**
    TABLE CUSTOMERS
  */
bool import_mcercle1::table_customers(){
    std::string sVal;
    int iVal;
    CustomerList list;
    IBPP::Date date;
    IBPP::Timestamp dateCrea;
    QString req =   "SELECT * from TAB_customers;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;            
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("TYPE", iVal);
            list.type.push_back( iVal);iVal=0;
            m_st->Get("DESC_COMPAGNY", sVal);
            list.desCompagny << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("FIRSTNAME", sVal);
            list.firstName << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("LASTNAME", sVal);
            list.lastName << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("GENDER", sVal);
            list.gender << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("BIRTHDAY", date);
            list.birthday.push_back( fromIBPPDate(date) );
            m_st->Get("ADDRESS1", sVal);
            list.address1 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ADDRESS2", sVal);
            list.address2 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ADDRESS3", sVal);
            list.address3 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ZIPCODE", sVal);
            list.zipCode << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("CITY", sVal);
            list.city << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("COUNTRY", sVal);
            list.country << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PHONENUMBER", sVal);
            list.phoneNum << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("MOBILENUMBER", sVal);
            list.mobileNum << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("EMAIL", sVal);
            list.email << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PROFESSION", sVal);
            list.profession << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("COMMENTS", sVal);
            list.comments << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("SENDING_PUB_EMAIL", iVal);
            list.sending_pubEmail.push_back( iVal);iVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_customers(ID, CREATIONDATE, TYPE, DESC_COMPAGNY, FIRSTNAME, LASTNAME, GENDER, BIRTHDAY, ADDRESS1,ADDRESS2,ADDRESS3,ZIPCODE,CITY,COUNTRY,PHONENUMBER,MOBILENUMBER,EMAIL,PROFESSION,COMMENTS, SENDING_PUB_EMAIL)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+QString::number(list.type.at(i))+"', '"+list.desCompagny.at(i)+"', '"+list.firstName.at(i)+"', '"+list.lastName.at(i)
              +"', '"+list.gender.at(i)+"', '"+list.birthday.at(i).toString("yyyy-MM-dd")+"', '"+list.address1.at(i)+"', '"+list.address2.at(i)+"', '"+list.address3.at(i)
              +"', '"+list.zipCode.at(i)+"', '"+list.city.at(i)+"', '"+list.country.at(i)+"', '"+list.phoneNum.at(i)+"', '"+list.mobileNum.at(i)+"', '"+list.email.at(i)+"', '"+list.profession.at(i)
              +"', '"+list.comments.at(i)+"', '"+QString::number(list.sending_pubEmail.at(i))
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}

/**
    TABLE PRODUCTS
  */
bool import_mcercle1::table_products(){
    std::string sVal;
    int iVal;
    float fVal; QString fs,fb;
    ProductList list;
    IBPP::Timestamp dateCrea;
    QString req =   "SELECT * from TAB_products;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("BUYING_PRICE", fVal);
            list.buying_price.push_back( fVal);fVal=0;
            m_st->Get("SELLING_PRICE", fVal);
            list.selling_price.push_back( fVal);fVal=0;
            m_st->Get("STOCK", iVal);
            list.stock.push_back( iVal);iVal=0;
            m_st->Get("STOCK_WARNING", iVal);
            list.stockWarning.push_back( iVal);iVal=0;
            m_st->Get("STATE", iVal);
            list.state.push_back( iVal);iVal=0;
            m_st->Get("ID_PROVIDER", iVal);
            list.idProvider.push_back( iVal);iVal=0;
            m_st->Get("ID_CATEGORY", iVal);
            list.idCategory.push_back( iVal);iVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_products(ID, CREATIONDATE ,NAME, CODE, SELLING_PRICE, BUYING_PRICE ,STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+list.name.at(i)+"', '"+list.code.at(i)
              +"', '"+fs.setNum(list.selling_price.at(i),'f',2)+"', '"+fb.setNum(list.buying_price.at(i),'f',2)+"', '"+QString::number(list.stock.at(i))+"', '"+QString::number(list.stockWarning.at(i))+"', '"+QString::number(list.state.at(i))
              +"', '"+QString::number(list.idProvider.at(i))+"', '"+QString::number(list.idCategory.at(i))
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}


/**
    TABLE PRODUCTS CATEGORIES
*/
bool import_mcercle1::table_products_categories(){
    std::string sVal;
    int iVal;
    QColor col;
    categoryList list;
    QString req =   "SELECT * from TAB_products_categories;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("COLOR", sVal);
            col.setNamedColor(QString::fromUtf8(sVal.c_str()));
            list.color.push_back(col);
            sVal="";col=QColor::QColor();
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_products_categories(ID, NAME, COLOR)"
                " VALUES('"+QString::number(list.id.at(i))+"', '"+list.name.at(i)+"', '"+list.color.at(i).name()
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}


/**
    TABLE PROVIDES
*/
bool import_mcercle1::table_providers(){
    std::string sVal;
    int iVal;
    ProviderList list;
    QString req =   "SELECT * from TAB_providers;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ADDRESS1", sVal);
            list.address1 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ADDRESS2", sVal);
            list.address2 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ADDRESS3", sVal);
            list.address3 << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ZIPCODE", sVal);
            list.zipCode << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("CITY", sVal);
            list.city << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PHONENUMBER", sVal);
            list.phoneNum << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("FAXNUMBER", sVal);
            list.faxNumber << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("EMAIL", sVal);
            list.email << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("CONTACT_NAME", sVal);
            list.contactName << QString::fromUtf8(sVal.c_str());sVal="";
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_providers(ID, NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+list.name.at(i)+"', '"+list.address1.at(i)+"', '"+list.address2.at(i)+"', '"+list.address3.at(i)
              +"', '"+list.zipCode.at(i)+"', '"+list.city.at(i)+"', '"+list.phoneNum.at(i)+"', '"+list.faxNumber.at(i)+"', '"+list.email.at(i)+"', '"+list.contactName.at(i)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}


/**
    TABLE SERVICES
*/
bool import_mcercle1::table_services(){
    std::string sVal;
    float fVal; QString fp;
    int iVal;
    IBPP::Timestamp dateCrea,tDate;

    ServiceList list;
    QString req =   "SELECT * from TAB_services;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("THEDATE", tDate);
            list.date.push_back( fromIBPPTimeStamp(tDate) );
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
            m_st->Get("DESCRIPTION", sVal);
            list.description << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("ID_CUSTOMER", iVal);
            list.idCust.push_back( iVal);iVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_services(ID, CREATIONDATE, THEDATE, NAME, PRICE, DESCRIPTION, ID_CUSTOMER)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+list.date.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+list.name.at(i)+"', '"+fp.setNum(list.price.at(i),'f',2)
                +"', '"+list.description.at(i)+"', '"+QString::number(list.idCust.at(i))
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}

/**
    TABLE SERVICES COMMUN
*/
bool import_mcercle1::table_services_commons(){
    std::string sVal;
    float fVal; QString fp;
    int iVal;
    IBPP::Timestamp dateCrea,tDate;

    ServiceList list;
    QString req =   "SELECT * from TAB_services_commons;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("THEDATE", tDate);
            list.date.push_back( fromIBPPTimeStamp(tDate) );
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
            m_st->Get("DESCRIPTION", sVal);
            list.description << QString::fromUtf8(sVal.c_str());sVal="";
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_services_commons(ID, CREATIONDATE, THEDATE, NAME, PRICE, DESCRIPTION)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+list.date.at(i).toString("yyyy-MM-dd HH:MM:ss")+"', '"+list.name.at(i)+"', '"+fp.setNum(list.price.at(i),'f',2)
                +"', '"+list.description.at(i)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}



/**
    TABLE PROPOSAL
*/
bool import_mcercle1::table_proposals(){
    std::string sVal;
    float fVal; QString fp;
    int iVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date tDate;

    ProposalList list;
    QString req =   "SELECT * from tab_proposals;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("ID_CUSTOMER", iVal);
            list.idCust.push_back( iVal);iVal=0;
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("PDATE", tDate);
            list.userDate.push_back( fromIBPPDate(tDate) );
            m_st->Get("VALIDDATE", tDate);
            list.validDate.push_back( fromIBPPDate(tDate) );
            m_st->Get("DELIVERYDATE", tDate);
            list.deliveryDate.push_back( fromIBPPDate(tDate) );
            m_st->Get("DELAY_DELIVERYDATE", iVal);
            list.delayDeliveryDate.push_back( iVal);iVal=0;
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
            m_st->Get("TYPE_PAYMENT", sVal);
            list.typePayment << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("STATE", iVal);
            list.state.push_back( iVal);iVal=0;
            m_st->Get("DESCRIPTION", sVal);
            list.description << QString::fromUtf8(sVal.c_str());sVal="";
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO tab_proposals(ID ,ID_CUSTOMER, CREATIONDATE, CODE, DATE, VALIDDATE, DELIVERYDATE, DELAY_DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+QString::number(list.idCust.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")
                +"', '"+list.code.at(i)+"', '"+list.userDate.at(i).toString("yyyy-MM-dd")+"', '"+list.validDate.at(i).toString("yyyy-MM-dd")+"', '"+list.deliveryDate.at(i).toString("yyyy-MM-dd")+"', '"+QString::number(list.delayDeliveryDate.at(i))
                +"', '"+list.typePayment.at(i)+"', '"+fp.setNum(list.price.at(i),'f',2)+"', '"+QString::number(list.state.at(i))
                +"', '"+list.description.at(i)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}


/**
    TABLE PROPOSAL DETAILS
*/
bool import_mcercle1::table_proposals_details(){
    std::string sVal;
    float fVal; QString fp;
    int iVal;

    ProposalListItems list;
    QString req =   "SELECT * from TAB_proposals_details;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("ID_PROPOSAL", iVal);
            list.idProposal.push_back( iVal);iVal=0;
            m_st->Get("ID_PRODUCT", iVal);
            list.idProduct.push_back( iVal);iVal=0;
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("QUANTITY", iVal);
            list.quantity.push_back( iVal);iVal=0;
            m_st->Get("DISCOUNT", iVal);
            list.discount.push_back( iVal);iVal=0;
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_proposals_details(ID ,ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+QString::number(list.idProposal.at(i))+"', '"+QString::number(list.idProduct.at(i))+"', '"+list.name.at(i)
                +"', '"+QString::number(list.quantity.at(i))+"', '"+QString::number(list.discount.at(i))
                +"', '"+fp.setNum(list.price.at(i),'f',2)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}



/**
    TABLE INVOICES
*/
bool import_mcercle1::table_invoices(){
    std::string sVal;
    float fVal; QString fp, fpx;
    int iVal;
    IBPP::Timestamp dateCrea;
    IBPP::Date tDate;

    InvoiceList list;
    QString req =   "SELECT * from tab_invoices;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("ID_CUSTOMER", iVal);
            list.idCust.push_back( iVal);iVal=0;
            m_st->Get("CREATIONDATE", dateCrea);
            list.creationDate.push_back( fromIBPPTimeStamp(dateCrea) );
            m_st->Get("IDATE", tDate);
            list.userDate.push_back( fromIBPPDate(tDate) );
            m_st->Get("CODE", sVal);
            list.code << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
            m_st->Get("LIMIT_PAYMENTDATE", tDate);
            list.limitPayment.push_back( fromIBPPDate(tDate) );
            m_st->Get("PART_PAYMENT", fVal);
            list.partPayment.push_back( fVal);fVal=0;
            m_st->Get("TYPE_PAYMENT", sVal);
            list.typePayment << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("STATE", iVal);
            list.state.push_back( iVal);iVal=0;
            m_st->Get("DESCRIPTION", sVal);
            list.description << QString::fromUtf8(sVal.c_str());sVal="";
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO tab_invoices(ID ,ID_CUSTOMER, CREATIONDATE, CODE, DATE, LIMIT_PAYMENTDATE, PART_PAYMENT, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+QString::number(list.idCust.at(i))+"', '"+list.creationDate.at(i).toString("yyyy-MM-dd HH:MM:ss")
                +"', '"+list.code.at(i)+"', '"+list.userDate.at(i).toString("yyyy-MM-dd")+"', '"+list.limitPayment.at(i).toString("yyyy-MM-dd")+"', '"+fp.setNum(list.partPayment.at(i),'f',2)
                +"', '"+list.typePayment.at(i)+"', '"+fpx.setNum(list.price.at(i),'f',2)+"', '"+QString::number(list.state.at(i))
                +"', '"+list.description.at(i)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}


/**
    TABLE INVOICE DETAILS
*/
bool import_mcercle1::table_invoices_details(){
    std::string sVal;
    float fVal; QString fp;
    int iVal;

    InvoiceListItems list;
    QString req =   "SELECT * from TAB_invoices_details;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("ID_INVOICE", iVal);
            list.idInvoice.push_back( iVal);iVal=0;
            m_st->Get("ID_PRODUCT", iVal);
            list.idProduct.push_back( iVal);iVal=0;
            m_st->Get("NAME", sVal);
            list.name << QString::fromUtf8(sVal.c_str());sVal="";
            m_st->Get("QUANTITY", iVal);
            list.quantity.push_back( iVal);iVal=0;
            m_st->Get("DISCOUNT", iVal);
            list.discount.push_back( iVal);iVal=0;
            m_st->Get("PRICE", fVal);
            list.price.push_back( fVal);fVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_invoices_details(ID ,ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+QString::number(list.idInvoice.at(i))+"', '"+QString::number(list.idProduct.at(i))+"', '"+list.name.at(i)
                +"', '"+QString::number(list.quantity.at(i))+"', '"+QString::number(list.discount.at(i))
                +"', '"+fp.setNum(list.price.at(i),'f',2)
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}



/**
    TABLE LINK PROPOSAL INVOICE
*/
bool import_mcercle1::table_link_proposals_invoices(){
    int iVal;

    LinkList list;
    QString req =   "SELECT * from TAB_LINK_PROPOSALS_INVOICES;";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        // list all info
        while (m_st->Fetch()) {
            m_st->Get("ID", iVal);
            list.id.push_back( iVal);iVal=0;
            m_st->Get("ID_INVOICE", iVal);
            list.idInvoice.push_back( iVal);iVal=0;
            m_st->Get("ID_PROPOSAL", iVal);
            list.idProposal.push_back( iVal);iVal=0;
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //TRANSFERT
    QSqlQuery query;
    for(unsigned int i=0; i<list.id.size(); i++){
        req = "INSERT INTO TAB_LINK_PROPOSALS_INVOICES(ID ,ID_PROPOSAL, ID_INVOICE)"
              " VALUES('"+QString::number(list.id.at(i))+"', '"+QString::number(list.idProposal.at(i))+"', '"+QString::number(list.idInvoice.at(i))
              +"');" ;
        query.prepare(req);
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text()+ "\n" + req);
            return false;
        }
    }
    return true;
}

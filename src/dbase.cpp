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

#include "dbase.h"
#include "dialogwaiting.h"

#include <QMessageBox>
#include <QWidget>
#include <QByteArray>
#include <QBuffer>

database::database(QLocale &lang, QWidget *parent): m_parent(parent)
{
    m_name = "db_mcercle";
    m_port = 3050;
    m_hostName = "localhost/" + QString::number(m_port);
    m_login = "SYSDBA";
    m_password = "masterkey";
    addSample = true;
    m_connected = false;

    m_lang = lang;
    //Base de donnee default
    db = IBPP::DatabaseFactory( m_hostName.toStdString().c_str(),
                                m_name.toStdString().c_str(),
                                m_login.toStdString().c_str(),
                                m_password.toStdString().c_str(),
                                ROLE, CHARSET, OPTIONS );
    /* Valeur entreprise */
    m_tax = 0;

    /* Si Mode release on n ajout  pas d exemples a la creation d une base*/
#ifdef QT_NO_DEBUG
    addSample = false;
#endif

}

database::~database(){
    delete m_customer;
    delete m_product;
}


/**
    connexion a la base de donnees
  */
char database::connect(char type){
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
        m_connected = true;

        //Recuperation de la version de firebird
        m_FDBversion = FireBirdVersion();
        //Recuperation de la version de la base de donnees
        if(type != DB_CREATE)  m_databaseVersion = databaseVersion();
        //Creation des sous class
        m_customer = new customer(db, m_tr, m_st, m_parent);
        //instanciation de la class product
        m_product = new product(db, m_tr, m_st, m_lang, m_parent);
    }
    catch ( IBPP::Exception& e )    {
        m_connected = false;
        QString mess;
        mess += e.ErrorMessage();
        int pos = mess.indexOf("335544344"); // Engine Code
        //mess.replace("\n","<br>");
        if(pos>=0) {
            QMessageBox mBox(QMessageBox::Warning, tr("Attention"), tr("<b>La base de donn&#233;es n&#146;existe pas!</b>"),QMessageBox::Ok);
            mBox.setDetailedText ( mess );
            mBox.exec();
            return DB_NOTEXIST_ERR;
        }
        else{
            QMessageBox mBox(QMessageBox::Critical, tr("Erreur"), tr("<b>La connexion avec la base de donn&#233;es n&#146;a pas pu &#234;tre &#233;tablie!</b>"),QMessageBox::Ok);
            mBox.setDetailedText ( mess );
            mBox.exec();
            return DB_CON_ERR;
        }
    }
    //Test de la version de firebird compatible
    QString versFB = m_FDBversion.replace(".","");
    if( versFB.toInt() > FIREBIRD_SUPPORTED){
        QString mess = tr("Version de mcercle: ") + MCERCLE_VERSION;
        mess += tr("\nVersion de FireBird: ") + m_FDBversion;
        mess += tr("\n\nVersions de FireBird compatibles: <= ") + QString::number(FIREBIRD_SUPPORTED);
        QMessageBox mBox(QMessageBox::Warning, tr("Attention"), tr("mcercle ne support pas cette version de FireBird..."),QMessageBox::Ok);
        mBox.setDetailedText ( mess );
        mBox.exec();
        this->close();
        return DB_CON_ERR;
    }
    //Test de la version de la base de donnees... !!
    if((type != DB_CREATE) && (m_databaseVersion > DBASE_SUPPORTED)){
        QString mess = tr("Version de mcercle: ") + MCERCLE_VERSION;
        mess += tr("\nVersion de la base de donn\351es: ") + QString::number(m_databaseVersion);
        mess += tr("\n\nVersions des bases de donn\351es compatibles: <= ") + QString::number(DBASE_SUPPORTED);
        QMessageBox mBox(QMessageBox::Warning, tr("Attention"), tr("mcercle ne support pas cette version de base de donn\351es...\nMerci de faire \351voluer mcercle."),QMessageBox::Ok);
        mBox.setDetailedText ( mess );
        mBox.exec();
        this->close();
        return DB_CON_ERR;
    }
    return DB_CON_OK;
}


/**
    Creation de la base de donnees
  */
bool database::create(){
    int bar=0;
    //Affichage de la fenetre d attente
    DialogWaiting* m_DialogWaiting = new DialogWaiting();
    m_DialogWaiting->setTitle(tr("<b>Cr&#233;ation de la base de donn&#233;es</b>"));
    m_DialogWaiting->setDetail(tr("<i>Cr&#233;ation de la base de donn&#233;es...</i>"));
    m_DialogWaiting->setProgressBarRange(0,12);
    m_DialogWaiting->setModal(true);
    m_DialogWaiting->show();
    //On Creation de la base
    try {
        m_DialogWaiting->setProgressBar(bar++);
        db->Create(3);
    }
    catch ( IBPP::Exception& e )    {
        QString mess = e.ErrorMessage();
        QMessageBox mBox(QMessageBox::Critical, tr("Erreur"), tr("Impossible de cr&#233;er la base de donn&#233;es<br>V&#233;rifier les droits du r&#233;pertoire cible..."));
        mBox.setDetailedText ( mess );
        mBox.exec();
        m_DialogWaiting->setDetail(tr("<b>Erreur</b>... veuillez contacter votre administrateur r&#233;seau."));
        return false;
    }
    //Connexion et creation des tables
    if (connect(DB_CREATE) == DB_CON_OK) {
        m_DialogWaiting->setProgressBar(bar++);
        m_DialogWaiting->setDetail(tr("<i>Cr&#233;ation des tables...</i>"));
         //Creation des tables
        bool ret = createTable_informations();
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_customers();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_tax();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_products();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_products_categories();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_providers();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_services();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_services_common();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_proposals();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_proposals_details();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_invoices();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_invoices_details();
        }
        if(ret){
            m_DialogWaiting->setProgressBar(bar++);
            ret = createTable_link_proposals_invoices();
        }

        if(ret) {
            //creation OK
            m_DialogWaiting->setDetail(tr("<i>Cr&#233;ation termin&#233;e avec succ&#232;s !</i>"));
            m_connected = true;
            return true;
        }else {
            // on supprime la database
            try {
                db->Drop();
            }
            catch ( IBPP::Exception& e )    {
                QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            }
            m_DialogWaiting->setDetail(tr("<i>Erreur... veuillez contacter votre administrateur r&#233;seau.</i>"));
            m_connected = false;
            return false;

        }
    }
    else{
        //Erreur de connexion
        m_DialogWaiting->setDetail(tr("<i>Erreur... veuillez contacter votre administrateur r&#233;seau.</i>"));
        return false;
    }
}


/**
    Creation de la table informations
  */
bool database::createTable_informations(){
    QString req = "CREATE TABLE TAB_informations("
                    "ID             INTEGER NOT NULL,"
                    "DBASE_VERSION  INTEGER NOT NULL,"
                    "TAX            INTEGER,"
                    "NAME           VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "NUM            VARCHAR(64) CHARACTER SET UTF8,"
                    "CAPITAL        VARCHAR(64) CHARACTER SET UTF8,"
                    "ADDRESS1       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS2       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS3       VARCHAR(128) CHARACTER SET UTF8,"
                    "ZIPCODE        VARCHAR(10) CHARACTER SET UTF8,"
                    "CITY           VARCHAR(64) CHARACTER SET UTF8,"
                    "PHONENUMBER    VARCHAR(24) CHARACTER SET UTF8,"
                    "FAXNUMBER      VARCHAR(24) CHARACTER SET UTF8,"
                    "EMAIL          VARCHAR(128) CHARACTER SET UTF8,"
                    "WEBSITE        VARCHAR(256) CHARACTER SET UTF8,"
                    "LOGO           BLOB SUB_TYPE BINARY,"
                    "CG             BLOB SUB_TYPE TEXT," /* condition general de vente*/
                   "PRIMARY KEY (ID)"
                  ");";
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_INFORMATIONS\" ON \"TAB_INFORMATIONS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_INFORMATIONS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_INFORMATIONS FOR TAB_INFORMATIONS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_INFORMATIONS_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout
    try {
        m_tr->Start();
        m_st->Execute( "INSERT INTO TAB_informations(DBASE_VERSION, TAX, NAME, NUM, CAPITAL, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, WEBSITE)"
                    "VALUES('1', '0', 'SARL NOM', '123456', '7500', '12 rue du chateau', 'Chemin de livre', '', '13000', 'MARSEILLE', '(+33) 04 92 89 07 76', '(+33) 04 92 89 07 78', 'info@provvence.com', 'http://www.provvence.com');"
                  );
        m_tr->Commit();    // Or tr->Rollback(
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    return true;
}


/**
    Creation de la table client
  */
bool database::createTable_customers(){
    QString req = "CREATE TABLE TAB_customers ("
                    "ID             INTEGER NOT NULL,"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "TYPE           INTEGER  NOT NULL," /* Particulier ou societe */
                    "DESC_COMPAGNY  VARCHAR(128) CHARACTER SET UTF8,"/* Forme Juridique */
                    "LASTNAME       VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "FIRSTNAME      VARCHAR(64) CHARACTER SET UTF8,"
                    "GENDER         VARCHAR(3) CHARACTER SET UTF8,"
                    "BIRTHDAY       DATE,"
                    "ADDRESS1       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS2       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS3       VARCHAR(128) CHARACTER SET UTF8,"
                    "ZIPCODE        VARCHAR(10) CHARACTER SET UTF8,"
                    "CITY           VARCHAR(64) CHARACTER SET UTF8,"
                    "COUNTRY        VARCHAR(64) CHARACTER SET UTF8,"
                    "PHONENUMBER    VARCHAR(24) CHARACTER SET UTF8,"
                    "MOBILENUMBER   VARCHAR(24) CHARACTER SET UTF8,"
                    "EMAIL          VARCHAR(128) CHARACTER SET UTF8,"
                    "PROFESSION     VARCHAR(128) CHARACTER SET UTF8,"
                    "COMMENTS       VARCHAR(256) CHARACTER SET UTF8,"
                    "SENDING_PUB_EMAIL	INTEGER NOT NULL,"
                   "PRIMARY KEY (ID)"
                  ");";
    try {
        //TABLE
        m_tr->Start();        
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_CUSTOMERS\" ON \"TAB_CUSTOMERS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_CUSTOMERS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_CUSTOMERS FOR TAB_CUSTOMERS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_CUSTOMERS_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_customers(TYPE, LASTNAME, FIRSTNAME, GENDER, BIRTHDAY, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER,"
                    "MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL)"
                    "VALUES('0', 'DUPOND', 'Jean', 'M.', '1993/04/05', 'appt 208, bat B', 'citee des Alpes',"
                           "'rue du centre', '04100', 'MANOSQUE', 'FRANCE','+33492001122', '+33698543221', 'info@free.fr', 'Commercial', 'good customer !', '1');"
                  );
            m_st->Execute( "INSERT INTO TAB_customers(TYPE, DESC_COMPAGNY, LASTNAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER,"
                    "MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL)"
                    "VALUES('1', 'EURL SIRET:123456798', 'Elect Rose', 'BATC rue des rosiers', 'Quartier St Paul',"
                           "'rue du centre', '13011', 'MARSEILLE', 'FRANCE','+3342005122', '+33698456221', 'info@elect-rose.fr', 'Electricite Generale', 'Montage dautomate, video surveillance...', '0');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table des taxes
  */
bool database::createTable_tax(){
    QString req = "CREATE TABLE TAB_tax ("
                    "ID             INTEGER NOT NULL,"
                    "TAX            NUMERIC(5,2) NOT NULL,"
                    "DESCRIPTION    VARCHAR(256) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";

    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_TAX\" ON \"TAB_TAX\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_TAX_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_TAX FOR TAB_TAX "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_TAX_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_tax(TAX, DESCRIPTION)"
                    "VALUES('19.6', 'Standard');"
                  );
            m_st->Execute( "INSERT INTO TAB_tax(TAX, DESCRIPTION)"
                    "VALUES('5.5', 'Restauration, etc...');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table produits
  */
bool database::createTable_products(){
/*
 MANUAL
 Standard SQL requires that DECIMAL(5,2) be able to store any value with five digits and two decimals,
 so values that can be stored in the salary column range from -999.99 to 999.99.
 */
    QString req = "CREATE TABLE TAB_products("
                    "ID             INTEGER NOT NULL,"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "NAME           VARCHAR(256) CHARACTER SET UTF8 NOT NULL,"
                    "CODE           VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "CODE_USER      VARCHAR(64) CHARACTER SET UTF8,"
                    "SELLING_PRICE  NUMERIC(8,2) NOT NULL,"
                    "BUYING_PRICE   NUMERIC(8,2) NOT NULL,"
                    "TAX            NUMERIC(5,2),"
                    "STOCK          INTEGER NOT NULL,"
                    "STOCK_WARNING  INTEGER NOT NULL,"
                    "STATE          INTEGER NOT NULL,"
                    "ID_PROVIDER    INTEGER NOT NULL,"
                    "ID_CATEGORY   INTEGER NOT NULL,"
                   "PRIMARY KEY (ID)"
                  ");";

    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_PRODUCTS\" ON \"TAB_PRODUCTS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"CODE_PRODUCTS\" ON \"TAB_PRODUCTS\"(\"CODE\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"USERCODE_PRODUCTS\" ON \"TAB_PRODUCTS\"(\"CODE_USER\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_PRODUCTS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_PRODUCTS FOR TAB_PRODUCTS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_PRODUCTS_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_products(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
                        "VALUES('X-00000001', 'U0001', '45.99', '38.45','19.6', 'Pot de miel', '0', '1', '0', '1','2');"
                      );
            m_st->Execute( "INSERT INTO TAB_products(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
                        "VALUES('X-00000002', 'U0002', '14.95', '8.75','19.6', 'Huile de Lavande', '2', '1', '1','2','1');"
                      );
            m_st->Execute( "INSERT INTO TAB_products(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
                        "VALUES('X-00000003', 'U0003', '7.74', '3.05','19.6', 'Croquants aux amandes', '50', '10', '1','1','2');"
                      );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table des categories de produits
  */
bool database::createTable_products_categories(){
    QString req = "CREATE TABLE TAB_products_categories("
                    "ID     INTEGER NOT NULL,"
                    "NAME   VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "COLOR  VARCHAR(64) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_PRODUCTS_CATEGORIES\" ON \"TAB_PRODUCTS_CATEGORIES\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_PRODUCTS_CATEGORIES_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_PRODUCTS_CATEGORIES FOR TAB_PRODUCTS_CATEGORIES "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_PRODUCTS_CATEGORIES_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_products_categories(NAME)"
                        "VALUES('Huiles essentielles');"
                      );
            m_st->Execute( "INSERT INTO TAB_products_categories(NAME)"
                        "VALUES('Miels & Amandes');"
                      );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}



/**
    Creation de la table fournisseur
  */
bool database::createTable_providers(){
    QString req = "CREATE TABLE TAB_providers( "
                    "ID             INTEGER NOT NULL,"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "NAME           VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "ADDRESS1       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS2       VARCHAR(128) CHARACTER SET UTF8,"
                    "ADDRESS3       VARCHAR(128) CHARACTER SET UTF8,"
                    "ZIPCODE        VARCHAR(10) CHARACTER SET UTF8,"
                    "CITY           VARCHAR(64) CHARACTER SET UTF8,"
                    "PHONENUMBER    VARCHAR(24) CHARACTER SET UTF8,"
                    "FAXNUMBER      VARCHAR(24) CHARACTER SET UTF8,"
                    "EMAIL          VARCHAR(128) CHARACTER SET UTF8,"
                    "CONTACT_NAME   VARCHAR(64) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_PROVIDERS\" ON \"TAB_PROVIDERS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_PROVIDERS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_PROVIDERS FOR TAB_PROVIDERS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_PROVIDERS_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_providers(NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME)"
                    "VALUES('Miel Plus', '104 chemin du tord', '14 bds micheler', '', '04210', 'VALENSOLE', '0442425687', '0655234857', 'info@mielPlus.fr', 'Mr Jean');"
                  );
            m_st->Execute( "INSERT INTO TAB_providers(NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME)"
                    "VALUES('huilesEssentiellesAix', '2 rue du chateau', '', 'http://www.huilesEssentiellesAix.com', '13100', 'AIX-EN-PROVENCE', '0442456695', '0665554855', '', 'Mme Desjardin');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table des services
  */
bool database::createTable_services(){
    QString req = "CREATE TABLE TAB_services ("
                    "ID             INTEGER NOT NULL,"
                    "ID_CUSTOMER    INTEGER NOT NULL,"
                    "TAX            NUMERIC(5,2),"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "THEDATE        TIMESTAMP NOT NULL,"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                    "NAME           VARCHAR(256) CHARACTER SET UTF8 NOT NULL,"
                    "DESCRIPTION    VARCHAR(1024) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
    //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_SERVICES\" ON \"TAB_SERVICES\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_SERVICES_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_SERVICES FOR TAB_SERVICES "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_SERVICES_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_SERVICES "
                "add constraint FK_TAB_SERVICES "
                "foreign key (ID_CUSTOMER) "
                "references TAB_CUSTOMERS (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_services(ID_CUSTOMER, TAX, THEDATE, PRICE, NAME, DESCRIPTION)"
                            "VALUES('2', '19.6', '1900/01/01 08:01:02', '65.4', 'Creation de site internet', 'CMS:Wordpress\nHebergeur:OVH');"
                  );
            m_st->Execute( "INSERT INTO TAB_services(ID_CUSTOMER, TAX, THEDATE, PRICE, NAME, DESCRIPTION)"
                           "VALUES('1', '19.6', '2011/04/01 08:01:02' ,'15.54', 'Service de mise en marche', '-Installation de la pompe a eau\n-Cablage du tableau electrique\n...');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table des services commun
  */
bool database::createTable_services_common(){
    QString req = "CREATE TABLE TAB_services_commons ("
                    "ID             INTEGER NOT NULL,"
                    "TAX            NUMERIC(5,2),"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "THEDATE        TIMESTAMP NOT NULL,"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                    "NAME           VARCHAR(256) CHARACTER SET UTF8 NOT NULL,"
                    "DESCRIPTION    VARCHAR(1024) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
    //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_SERVICES_COMMONS\" ON \"TAB_SERVICES_COMMONS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_SERVICES_COMMONS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_SERVICES_COMMONS FOR TAB_SERVICES_COMMONS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_SERVICES_COMMONS_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_services_commons(THEDATE, PRICE, NAME)"
                    "VALUES('1900/01/01 08:01:02', '1500.40', 'Forfait mise en service');"
                  );
            m_st->Execute( "INSERT INTO TAB_services_commons(TAX, THEDATE, PRICE, NAME)"
                    "VALUES('19.6','1900/01/01 08:01:02', '20', 'Forfait horaire');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}

/**
    Creation de la table des propositions commerciales / Devis
  */
bool database::createTable_proposals(){
    QString req = "CREATE TABLE TAB_proposals ("
                    "ID             INTEGER NOT NULL,"
                    "ID_CUSTOMER    INTEGER NOT NULL,"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "PDATE          DATE NOT NULL,"
                    "VALIDDATE      DATE NOT NULL,"
                    "DELIVERYDATE         DATE NOT NULL,"
                    "DELAY_DELIVERYDATE   INTEGER," /* DELAY IN DAYS */
                    "CODE           VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "TYPE_PAYMENT   VARCHAR(2) CHARACTER SET UTF8,"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                    "STATE          INTEGER NOT NULL,"      /* SIGNEE OU PAS */
                    "DESCRIPTION    VARCHAR(256) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
     //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    //Add comments
    try {
        m_tr->Start();
        m_st->Execute("COMMENT ON column TAB_proposals.DELAY_DELIVERYDATE IS 'DELAY IN DAYS';");
        m_st->Execute("COMMENT ON column TAB_proposals.STATE IS 'VALIDATED or UNVALIDATED';");
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_PROPOSALS\" ON \"TAB_PROPOSALS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //UNIQUE CODE_INVOICE
        req =   "CREATE UNIQUE INDEX \"CODE_PROPOSALS\" ON \"TAB_PROPOSALS\"(\"CODE\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_PROPOSALS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_PROPOSALS FOR TAB_PROPOSALS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_PROPOSALS_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_PROPOSALS "
                "add constraint FK_TAB_PROPOSALS "
                "foreign key (ID_CUSTOMER) "
                "references TAB_CUSTOMERS (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_proposals(ID_CUSTOMER, CODE, PDATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
                    "VALUES('1', 'PR110802-1', '2011/08/02', '2011/08/02', '2011/08/02', 'CC', '98.95', '0', 'Produits provence');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals(ID_CUSTOMER, CODE, PDATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
                    "VALUES('1', 'PR110802-2', '2011/08/02', '2011/08/02', '2011/08/02', 'CA', '490.99', '1', 'Installation web');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals(ID_CUSTOMER, CODE, PDATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
                           "VALUES('1', 'PR110802-3', '2011/08/02', '2011/08/02', '2011/08/02', 'CC', '1589.10', '2', 'Montage d une pompe hydraulique');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals(ID_CUSTOMER, CODE, PDATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE)"
                    "VALUES('2', 'PR110802-4', '2011/08/02', '2011/08/02', '2011/08/02', 'CH', '20', '0');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}


/**
    Creation de la table de details des propositions commerciales / Devis
  */
bool database::createTable_proposals_details(){
    QString req = "CREATE TABLE TAB_proposals_details ("
                    "ID             INTEGER NOT NULL,"
                    "ID_PROPOSAL     INTEGER NOT NULL,"
                    "ID_PRODUCT     INTEGER," /* Gestion du sotck automatique */
                    "NAME           VARCHAR(128) CHARACTER SET UTF8 NOT NULL,"
                    "DISCOUNT       INTEGER NOT NULL,"
                    "QUANTITY       INTEGER NOT NULL,"
                    "TAX            NUMERIC(5,2),"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                   "PRIMARY KEY (ID)"
                  ");";
        //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Add comments
    try {
        m_tr->Start();
        m_st->Execute("COMMENT ON column TAB_proposals_details.TAX IS 'value of tax table, Null if not subject to tax ';");
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_PROPOSALS_DETAILS\" ON \"TAB_PROPOSALS_DETAILS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_PROPOSALS_DETAILS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_PROPOSALS_DETAILS FOR TAB_PROPOSALS_DETAILS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_PROPOSALS_DETAILS_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_PROPOSALS_DETAILS "
                "add constraint FK_TAB_PROPOSALS_DETAILS "
                "foreign key (ID_PROPOSAL) "
                "references TAB_PROPOSALS (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('1','1','Pot de miel','2','0','45.99');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('1','3','Croquants aux amandes','1','10','7.74');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('2','installation CMS wordpress','1','0','450.99');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT,PRICE)"
                    "VALUES('2','Forfait horaire','2','0','20');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('3','Pompe hydraulique','1','5','1400.95');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('3','raccords','10','0','5.56');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('3','tuyaux au metre','10','0','4.26');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('3','Forfait horaire','8','0','20');"
                  );
            m_st->Execute( "INSERT INTO TAB_proposals_details(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('4','Forfait horaire','1','0','20');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}


/**
    Creation de la table des factures
  */
bool database::createTable_invoices(){
    QString req = "CREATE TABLE TAB_invoices ("
                    "ID             INTEGER NOT NULL,"
                    "ID_CUSTOMER    INTEGER NOT NULL,"
                    "CREATIONDATE   TIMESTAMP NOT NULL,"
                    "IDATE          DATE NOT NULL,"
                    "LIMIT_PAYMENTDATE    DATE NOT NULL,"
                    "CODE           VARCHAR(64) CHARACTER SET UTF8 NOT NULL,"
                    "TYPE_PAYMENT   VARCHAR(2) CHARACTER SET UTF8," /* Null si devis */
                    "PART_PAYMENT   NUMERIC(8,2) NOT NULL,"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                    "STATE          INTEGER NOT NULL,"      /* REGLEE OU PAS */
                    "DESCRIPTION    VARCHAR(256) CHARACTER SET UTF8,"
                   "PRIMARY KEY (ID)"
                  ");";
     //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Add comments
    try {
        m_tr->Start();
	m_st->Execute("COMMENT ON column TAB_invoices.STATE IS 'UNPAID or PAID';");
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_INVOICES\" ON \"TAB_INVOICES\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //UNIQUE CODE_INVOICE
        req =   "CREATE UNIQUE INDEX \"CODE_INVOICES\" ON \"TAB_INVOICES\"(\"CODE\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_INVOICES_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_INVOICES FOR TAB_INVOICES "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_INVOICES_ID, 1); "
                "if (NEW.CREATIONDATE is NULL) then NEW.CREATIONDATE = 'now'; "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_INVOICES "
                "add constraint FK_TAB_INVOICES "
                "foreign key (ID_CUSTOMER) "
                "references TAB_CUSTOMERS (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_invoices(ID_CUSTOMER, CODE, IDATE, LIMIT_PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                           "VALUES('1', 'FA110802-1', '2011/08/02', '2011/08/02', 'CC', '0.00', '361.02', '0', 'Produits');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices(ID_CUSTOMER, CODE, IDATE, LIMIT_PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                           "VALUES('1', 'FA110804-2', '2011/08/04', '2011/08/04', 'CC', '0.00', '1589.10', '1', 'Montage d une pompe hydraulique');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices(ID_CUSTOMER, CODE, IDATE, LIMIT_PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                           "VALUES('1', 'FA110820-3', '2011/08/20', '2011/09/02', 'CA', '0.00', '65.99', '1', 'Produits et Services');"
                  );            
            m_st->Execute( "INSERT INTO TAB_invoices(ID_CUSTOMER, CODE, IDATE, LIMIT_PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                           "VALUES('2', 'FA110815-4', '2011/08/15', '2011/08/15', 'CH', '11.55', '20', '0', 'test');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}


/**
    Creation de la table de details des factures
  */
bool database::createTable_invoices_details(){
    QString req = "CREATE TABLE TAB_invoices_details ("
                    "ID             INTEGER NOT NULL,"
                    "ID_INVOICE     INTEGER NOT NULL,"
                    "ID_PRODUCT     INTEGER," /* Gestion du sotck automatique */
                    "NAME           VARCHAR(128) CHARACTER SET UTF8 NOT NULL,"
                    "DISCOUNT       INTEGER NOT NULL,"
                    "QUANTITY       INTEGER NOT NULL,"
                    "TAX            NUMERIC(5,2),"
                    "PRICE          NUMERIC(8,2) NOT NULL,"
                   "PRIMARY KEY (ID)"
                  ");";
        //TABLE
    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Add comments
    try {
        m_tr->Start();
        m_st->Execute("COMMENT ON column TAB_invoices_details.TAX IS 'value of tax table, Null if not subject to tax ';");
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID_INVOICES_DETAILS\" ON \"TAB_INVOICES_DETAILS\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_INVOICES_DETAILS_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_INVOICES_DETAILS FOR TAB_INVOICES_DETAILS "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_INVOICES_DETAILS_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_INVOICES_DETAILS "
                "add constraint FK_TAB_INVOICES_DETAILS "
                "foreign key (ID_INVOICE) "
                "references TAB_INVOICES (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('1','1','Pot de miel','2','0','45.99');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('1','3','Croquants aux amandes','1','10','7.74');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('1','2','Huile de Lavande','1','5','14.95');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('1','amandes','5','0','3.56');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('1','Figues','20','0','4.26');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('1','Savons Marseille','10','5','15.25');"
                  );

            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('2','Pompe hydraulique','1','5','1400.95');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('2','raccords','10','0','5.56');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('2','tuyaux au metre','10','0','4.26');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('2','Forfait horaire','8','0','20');"
                  );

            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
                           "VALUES('3','1','Pot de miel','1','0','45.99');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT,PRICE)"
                    "VALUES('3','Forfait horaire','1','0','20');"
                  );
            m_st->Execute( "INSERT INTO TAB_invoices_details(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
                    "VALUES('4','Forfait horaire','1','0','20');"
                  );
            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}




/**
    Creation de la table de Lien entre propositions commerciales / Devis et Factures
  */
bool database::createTable_link_proposals_invoices(){
    //TABLE LINK
    QString req = "CREATE TABLE TAB_link_proposals_invoices ("
                        "ID             INTEGER NOT NULL,"
                        "ID_PROPOSAL    INTEGER NOT NULL,"
                        "ID_INVOICE     INTEGER NOT NULL,"
                         "PRIMARY KEY (ID)"
                      ");";
        try {
            m_tr->Start();
            m_st->Execute(req.toStdString().c_str());
            m_tr->Commit();
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    // Contrainte sur la table
    // ATTENTION Une seule requette par execute....
    try {
        //UNIQUE ID
        req =   "CREATE UNIQUE INDEX \"ID\" ON \"TAB_LINK_PROPOSALS_INVOICES\"(\"ID\");";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //GENERATOR
        req =   "CREATE GENERATOR GEN_LINK_PROPOSALS_INVOICES_ID;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //TRIGGER
        req =   "CREATE TRIGGER TR_LINK_PROPOSALS_INVOICES FOR TAB_LINK_PROPOSALS_INVOICES "
                "ACTIVE BEFORE INSERT POSITION 0 "
                "AS "
                "BEGIN "
                "if (NEW.ID is NULL) then NEW.ID = GEN_ID(GEN_LINK_PROPOSALS_INVOICES_ID, 1); "
                "END ";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        //FOREIGN KEY
        req =   "alter table TAB_LINK_PROPOSALS_INVOICES "
                "add constraint FK_TAB_LINK_PR_INV "
                "foreign key (ID_PROPOSAL) "
                "references TAB_PROPOSALS (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();

        req =   "alter table TAB_LINK_PROPOSALS_INVOICES "
                "add constraint FK_TAB_LINK_PR_INV2 "
                "foreign key (ID_INVOICE) "
                "references TAB_INVOICES (ID) "
                "on update CASCADE "
                "on delete CASCADE;";
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }

    //Ajout d exemples
    if(addSample){
        try {
            m_tr->Start();
            m_st->Execute( "INSERT INTO TAB_link_proposals_invoices(ID_PROPOSAL, ID_INVOICE)"
                    "VALUES('3', '2');"
                  );

            m_tr->Commit();    // Or tr->Rollback(
        }
        catch ( IBPP::Exception& e )    {
            QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
            return false;
        }
    }
    return true;
}




/**
    Permet de passer dune IBPPTimeStamp a une QDateTime
    @param IBPPTimeStamp
    @return QDateTime
  */
QDateTime database::fromIBPPTimeStamp(IBPP::Timestamp &dateTime)
{
    int y,m,d,h,min,s,ms;
    dateTime.GetDate(y,m,d);
    dateTime.GetTime(h,min,s,ms);
    return QDateTime(QDate(y,m,d), QTime(h,min,s/*,ms*/), Qt::LocalTime);
}


/**
    Permet de passer dune QDateTime a une IBPPTimeStamp
    @param QDateTime
    @return IBPPTimeStamp
  */
IBPP::Timestamp database::toIBPPTimeStamp(const QDateTime &dateTime)
{
    IBPP::Timestamp timeStamp;
    if (dateTime.isValid())
        timeStamp = IBPP::Timestamp(dateTime.date().year(), dateTime.date().month(), dateTime.date().day(),
                             dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second(),
                             dateTime.time().msec());
    return timeStamp;
}


/**
    Permet de passer dune IBPP::Date a une QDate
    @param IBPP::Date
    @return QDate
  */
QDate database::fromIBPPDate(IBPP::Date &IBPPdate)
{
    int y,m,d;
    IBPPdate.GetDate(y,m,d);
    return QDate(y,m,d);
}


/**
    Permet de passer dune QDate a une IBPP::Date
    @param QDate
    @return IBPP::Date
  */
IBPP::Date database::toIBPPDate(const QDate &date)
{
    IBPP::Date IBPPdate;
    if (date.isValid())
        IBPPdate.SetDate(date.year(), date.month(), date.day());
    return IBPPdate;
}

/**
    Obtenir la version de IBPP utilise
   @return version du drv ibpp
  */
QString database::IBPPversion() {
    QString ver;
    ver += QString::number((IBPP::Version & 0xFF000000) >> 24);
    ver += '.';
    ver += QString::number((IBPP::Version & 0xFF0000) >> 16);
    ver += '.';
    ver += QString::number((IBPP::Version & 0xFF00) >> 8);
    ver += '.';
    ver += QString::number(IBPP::Version & 0xFF);

    return ver;
}

/**
    Obtenir la version de la base de donnees
    @return version
  */
int database::databaseVersion() {
    if(!this->m_connected)return -1;
    int ver=-1;
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute("SELECT DBASE_VERSION FROM TAB_informations;");
        m_st->Fetch();
        m_st->Get("DBASE_VERSION", ver);
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return -1;
    }

    return ver;
}

/**
    Obtenir la version de FireBird
    @return version
  */
QString database::FireBirdVersion() {
    if(!this->m_connected)return "";
    QString ver;
    std::string val;
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute("SELECT rdb$get_context('SYSTEM','ENGINE_VERSION') AS version FROM rdb$database;");
        m_st->Fetch();
        m_st->Get("version", val);
        ver = QString::fromUtf8(val.c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return "";
    }

    return ver;
}

/**
    Met a jour le logo dans la base de donnees
  */
bool database::updateLogoTable_informations(const QImage& image){
    if(!this->m_connected)return false;
    IBPP::Blob blob;    
    QByteArray data;
    QBuffer buf(&data);
    // writes image into ba in PNG format
    image.save(&buf,"PNG");
    int dataSize = data.size();
    char* pData = data.data() ;
    QString req = "UPDATE TAB_informations SET "
                    "LOGO=?"
                  ";";
    try {
        //TABLE
        m_tr->Start();
        blob  = IBPP::BlobFactory(db, m_tr);
        blob->Create();
        for(int i=0; i<dataSize; i++, pData++)
            blob->Write(pData, 1);
        blob->Close();

        m_st->Prepare(req.toStdString().c_str());
        m_st->Set(1, blob);
        m_st->Execute();
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    return true;
}

/**
    Recupere le logo dans la base de donnees
  */
QImage database::getLogoTable_informations(){
    QImage image;
    IBPP::Blob blob;
    QByteArray data;
    int size, largest, segments;
    if(!this->m_connected)return image;
    QString req = "SELECT LOGO FROM TAB_informations;";
    try {
        //TABLE
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_st->Fetch();
        blob  = IBPP::BlobFactory(db, m_tr);
        bool ret = m_st->Get("LOGO", blob);
        //returns true to signal the NULL.
        if(!ret){
            blob->Open();
            blob->Info(&size, &largest, &segments);
            data.resize(size);
            char* pData = data.data() ;

            for (int s=0,offset = 0; s<segments; s++, pData++)
                offset += blob->Read(pData, largest);

            blob->Close();
            image.loadFromData(data,"PNG");
        }
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
    }
    return image;
}

/**
      Applique les informations dans la base de donnee
      @return true si ok
  */
bool database::updateInfo(Informations &info) {
    if(!this->m_connected)return false;
    // Construction de la requette
    // Si le charactere speciaux "\'" existe on l'adapte pour la requette
    QString f;
    QString req = "UPDATE TAB_informations SET ";
    req += "NAME='" + info.name.replace("\'","''").toUtf8() + "',";
    req += "NUM='" + info.num.replace("\'","''").toUtf8() + "',";
    req += "CAPITAL='" + info.capital.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS1='" + info.address1.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS2='" + info.address2.replace("\'","''").toUtf8() + "',";
    req += "ADDRESS3='" + info.address3.replace("\'","''").toUtf8() + "',";
    req += "ZIPCODE='" + info.zipCode.replace("\'","''").toUtf8() + "',";
    req += "CITY='" + info.city.replace("\'","''").toUtf8() + "',";
    req += "PHONENUMBER='" + info.phoneNumber.replace("\'","''").toUtf8() + "',";
    req += "FAXNUMBER='" + info.faxNumber.replace("\'","''").toUtf8() + "',";
    req += "EMAIL='" + info.email.replace("\'","''").toUtf8() + "',";
    req += "WEBSITE='" + info.webSite.replace("\'","''").toUtf8() + "', ";
    req += "TAX='" + QString::number(info.tax) + "' ";
    req += "WHERE ID='1';";

    try {
        m_tr->Start();
        m_st->Execute(req.toStdString().c_str());
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e ) {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return false;
    }
    /* update cache */
    m_tax = info.tax;
    return true;
}

/**
      Applique les informations dans la base de donnee
      @return Informations
  */
bool database::getInfo(Informations &info) {
    if(!this->m_connected)return false;
    std::string val;
    int iVal;
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
    /* update cache */
    m_tax = info.tax;
    return true;
}

/**
   Retourne la date courante
  */
QDate database::getCurrentDate() {
    IBPP::Date date;
    QDate mdate;
    if(!this->m_connected)return mdate;

    try {
        //TABLE
        m_tr->Start();
        m_st->Execute("SELECT CURRENT_DATE AS DATE_NOW FROM rdb$database");
        m_st->Fetch();
        m_st->Get("DATE_NOW", date);
        mdate = database::fromIBPPDate(date);
        m_tr->Commit();
    }
    catch ( IBPP::Exception& e )    {
        QMessageBox::critical(this->m_parent, tr("Erreur"), e.ErrorMessage());
        return mdate;
    }

    return mdate;
}

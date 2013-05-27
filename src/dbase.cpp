/**
  This file is a part of mcercle
  Copyright (C) 2010-2013 Cyril FRAUSTI

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
#include <QDesktopServices>
#include <QFile>
#include <QVariant>
#include <QTextStream>
#include <QDir>
#include <QDebug>


database::database(QLocale &lang, QWidget *parent): m_parent(parent)
{
	m_name = "mcercle.db";
	m_port = 3306;
	m_hostName = "localhost";
	m_login = "root";
	m_password = "";
	addSample = true;
	m_connected = false;
	m_lang = lang;
	/* Valeur entreprise */
	m_isTax = 0;

	/* Si Mode release on n ajout  pas d exemples a la creation d une base*/
#ifdef QT_NO_DEBUG
	addSample = false;
#endif

}

database::~database(){
	if(m_customer>0) delete m_customer;
	if(m_product>0) delete m_product;
}


/**
	connexion a la base de donnees
  */
char database::connect(){
	//test de la version 1.0
	if( m_name.contains(".fdb")){
		QMessageBox mBox(QMessageBox::Warning, tr("Attention"), tr("<b>mcercle ne supporte plus firebird!</b><br>Veuillez installer mcercle v1.9 pour la migration."),QMessageBox::Ok);
		mBox.exec();
		return DB_CON_ERR;
	}

	db = QSqlDatabase::addDatabase("Q"+m_bdd);
	db.setHostName( m_hostName );
	db.setPort( m_port );
	db.setDatabaseName( m_name);
	db.setUserName( m_login );
	db.setPassword( m_password );

	if (!db.open()) {
		QMessageBox mBox(QMessageBox::Critical, tr("Erreur"), tr("<b>La connexion avec la base de donn&#233;es n&#146;a pas pu &#234;tre &#233;tablie!</b>"),QMessageBox::Ok);
		mBox.setDetailedText ( db.lastError().text() );
		mBox.exec();
		m_connected = false;
		return DB_CON_ERR;
	}
	else m_connected = true;

	// Si pas de tables on les crees
	QStringList tList = db.tables();
	if((!tList.contains("TAB_INFORMATIONS"))||(tList.count()< 14)){
		 // Demande si on creer une nouvelle base de donnees
		QString mess;
		if(db.driverName() == "QSQLITE")mess = tr("Voulez-vous cr\351er une nouvelle base de donn\351es ?\n\n")+ db.databaseName();
		else mess = tr("Voulez-vous cr\351er de nouvelles tables dans la base de donn\351es ?\n\n") + db.databaseName();
		QMessageBox mBox(QMessageBox::Question, tr("Question"), mess ,QMessageBox::Yes | QMessageBox::No);
		mBox.setDefaultButton(QMessageBox::No);
		int ret = mBox.exec();

		if(ret == QMessageBox::Yes){
			if( !create() ){
				m_connected = false;
				return DB_CON_ERR;
			}
		}else{
			m_connected = false;
			return DB_CON_ERR;
		}
	}
	else m_connected = true;

	//DI SQLITE activation des foreign Keys
	if(db.driverName() == "QSQLITE"){

		QSqlQuery query;
		query.prepare("PRAGMA foreign_keys = ON;");

		if(!query.exec()) {
			QMessageBox::critical(m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}

	// AVOIR: SI DEFAUT LES RETURN PRECEDENT QUI ZAPP LA CREATION DES CLASS NE POSE PAS SOUCIS!?
	m_databaseVersion = databaseVersion();
	m_isTax = isTax();
	//Creation des sous class
	m_customer = new customer(db, m_parent);
	//instanciation de la class product
	m_product = new product(db, m_lang, m_parent);
	//class de la gestion des tax
	m_tax = new tax(m_parent);

	//Test de la version de la base de donnees... !!
	if(m_databaseVersion > DBASE_SUPPORTED){
		QString mess = tr("Version de mcercle: ") + MCERCLE_VERSION;
		mess += tr("\nVersion de la base de donn\351es: ") + QString::number(m_databaseVersion);
		mess += tr("\n\nVersions des bases de donn\351es compatibles: <= ") + QString::number(DBASE_SUPPORTED);
		QMessageBox mBox(QMessageBox::Warning, tr("Attention"), tr("mcercle ne support pas cette version de base de donn\351es...\nMerci de faire \351voluer mcercle."),QMessageBox::Ok);
		mBox.setDetailedText ( mess );
		mBox.exec();
		this->close();
		return DB_CON_ERR;
	}
	qDebug() << "version base:" <<QString::number(m_databaseVersion);
	
	//Mise a jour de la base de donnees
	if(m_databaseVersion <= 1) {
		if(upgradeToV2()){
			QMessageBox mBox(QMessageBox::Information, tr("Information"), tr("Mise à jour de la base de donnees reussie !"),QMessageBox::Ok);
			mBox.exec();
		}
		else{
			this->close();
			return DB_CON_ERR;
		}
	}
	else if(m_databaseVersion <= 2 ) {
		if(upgradeToV3()){
			QMessageBox mBox(QMessageBox::Information, tr("Information"), tr("Mise à jour de la base de donnees reussie !"),QMessageBox::Ok);
			mBox.exec();
		}
		else{
			this->close();
			return DB_CON_ERR;
		}
	}
	return DB_CON_OK;
}


/**
	Creation de la base de donnees
  */
bool database::create(){
	int bar=0;
	bool ret = true;
	//Affichage de la fenetre d attente
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle(tr("<b>Cr&#233;ation de la base de donn&#233;es</b>"));
	m_DialogWaiting->setDetail(tr("<i>Cr&#233;ation de la base de donn&#233;es...</i>"));
	m_DialogWaiting->setProgressBarRange(0,13);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	//Connexion et creation des tables
	if( m_connected ) {
		m_DialogWaiting->setProgressBar(bar++);
		m_DialogWaiting->setDetail(tr("<i>Cr&#233;ation des tables...</i>"));
		 //Creation des tables
		if(!db.tables().contains("TAB_INFORMATIONS")) ret = createTable_informations();
		if((!db.tables().contains("TAB_BANK"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_bank();
		}
		if((!db.tables().contains("TAB_CUSTOMERS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_customers();
		}
		if((!db.tables().contains("TAB_TAX"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_tax();
		}
		if((!db.tables().contains("TAB_PRODUCTS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_products();
		}
		if((!db.tables().contains("TAB_PRODUCTS_CATEGORIES"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_products_categories();
		}
		if((!db.tables().contains("TAB_PROVIDERS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_providers();
		}
		if((!db.tables().contains("TAB_SERVICES"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_services();
		}
		if((!db.tables().contains("TAB_SERVICES_COMMON"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_services_common();
		}
		if((!db.tables().contains("TAB_PROPOSALS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_proposals();
		}
		if((!db.tables().contains("TAB_PROPOSALS_DETAILS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_proposals_details();
		}
		if((!db.tables().contains("TAB_INVOICES"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_invoices();
		}
		if((!db.tables().contains("TAB_INVOICES_DETAILS"))&&(ret)){
			m_DialogWaiting->setProgressBar(bar++);
			ret = createTable_invoices_details();
		}
		if((!db.tables().contains("TAB_LINK_PROPOSALS_INVOICES"))&&(ret)){
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
			if(db.driverName() == "QSQLITE") {
				if ( !QFile::remove( m_name ) )    {
					QMessageBox::critical(this->m_parent, tr("Erreur"), tr("Impossible de supprimer le fichier:\n")+m_name);
				}
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
	return false;
}


/**
	Creation de la table informations
  */
bool database::createTable_informations(){
  /*  QString path = QDir::currentPath() +"/scripts/"+db.driverName()+"/create_table_informations.sql";
	QFile sqlFile( path );
	if (!sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::critical(this->m_parent, tr("Erreur"), tr("Impossible d ouvrir le fichier SQL\n")+path);
	  return false;
	}
	QSqlQuery query;
	query.prepare( sqlFile.readAll() );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
*/
	QString req ="CREATE TABLE TAB_INFORMATIONS("
			"ID             INTEGER NOT NULL ,"
			"DBASE_VERSION  INTEGER NOT NULL ,"
			"TAX            INTEGER,"
			"NAME           VARCHAR(64) NOT NULL,"
			"NUM            VARCHAR(64),"
			"CAPITAL        VARCHAR(64),"
			"ADDRESS1       VARCHAR(128),"
			"ADDRESS2       VARCHAR(128),"
			"ADDRESS3       VARCHAR(128),"
			"ZIPCODE        VARCHAR(10),"
			"CITY           VARCHAR(64),"
			"PHONENUMBER    VARCHAR(24),"
			"FAXNUMBER      VARCHAR(24),"
			"EMAIL          VARCHAR(128),"
			"WEBSITE        VARCHAR(256),"
			"LOGO           BLOB,"
			"CG             TEXT,"
            "CA_TYPE        INTEGER NOT NULL ,"
		   "PRIMARY KEY (ID)"
			");";

	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
    query.prepare("INSERT INTO TAB_INFORMATIONS(DBASE_VERSION, TAX, NAME, CA_TYPE)"
                  "VALUES('3', '0', '','1');");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	/* TODO UPDATE POUR LE SAMPLE
	 if(addSample){
	 */

	return true;
}

/**
	Creation de la table banque
  */
bool database::createTable_bank(){
	QString req ="CREATE TABLE TAB_BANK("
			"ID             INTEGER NOT NULL ,"
			"CODE_BANQUE    VARCHAR(5),"
			"CODE_GUICHET   VARCHAR(5),"
			"NUM_COMPTE     VARCHAR(11),"
			"KEY_RIB        VARCHAR(64),"
			"ADDRESS       VARCHAR(128), "
			"IBAN_1        VARCHAR(4),"
			"IBAN_2        VARCHAR(4),"
			"IBAN_3        VARCHAR(4),"
			"IBAN_4        VARCHAR(4),"
			"IBAN_5        VARCHAR(4),"
			"IBAN_6        VARCHAR(4),"
			"IBAN_7        VARCHAR(4),"
			"IBAN_8        VARCHAR(4),"
			"IBAN_9        VARCHAR(2),"
			"CODE_BIC    VARCHAR(11),"
			"PRIMARY KEY (ID)"
			");";

	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
	query.prepare("INSERT INTO TAB_BANK(CODE_BANQUE)"
				  "VALUES('');");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
   /* TODO UPDATE POUR LE SAMPLE
	if(addSample){
		query.prepare("INSERT INTO TAB_BANK(CODE_BANQUE, CODE_GUICHET, NUM_COMPTE, KEY_RIB, ADDRESS, IBAN_1, IBAN_2, IBAN_3, IBAN_4, IBAN_5, IBAN_6, IBAN_7, IBAN_8, IBAN_9, CODE_BIC)"
					  "VALUES('19106', '00000', '12345678911', '12', 'CA PCA', 'FR00', '1234', '1234', '1234', '1234', '1234', '1234', '', '', 'A1234567891');");
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}*/
	return true;

}


/**
	Creation de la table client
  */
bool database::createTable_customers(){
	QString req = "CREATE TABLE TAB_CUSTOMERS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"CREATIONDATE   TIMESTAMP,"
			"TYPE           INTEGER  NOT NULL,"
			"DESC_COMPAGNY  VARCHAR(128) ,"
			"LASTNAME       VARCHAR(64) NOT NULL,"
			"FIRSTNAME      VARCHAR(64) ,"
			"GENDER         VARCHAR(3) ,"
			"BIRTHDAY       DATE,"
			"ADDRESS1       VARCHAR(128) ,"
			"ADDRESS2       VARCHAR(128) ,"
			"ADDRESS3       VARCHAR(128) ,"
			"ZIPCODE        VARCHAR(10) ,"
			"CITY           VARCHAR(64) ,"
			"COUNTRY        VARCHAR(64) ,"
			"PHONENUMBER    VARCHAR(24) ,"
			"MOBILENUMBER   VARCHAR(24) ,"
			"EMAIL          VARCHAR(128) ,"
			"PROFESSION     VARCHAR(128) ,"
			"COMMENTS       VARCHAR(256) ,"
			"SENDING_PUB_EMAIL	INTEGER NOT NULL"
			");";

	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_CUSTOMERS(TYPE, LASTNAME, FIRSTNAME, GENDER, BIRTHDAY, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER,"
					  "MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL)"
					  "VALUES('0', 'DUPOND', 'Jean', 'M.', '1993-04-05', 'appt 208, bat B', 'citee des Alpes',"
							 "'rue du centre', '04100', 'MANOSQUE', 'FRANCE','+33492001122', '+33698543221', 'info@free.fr', 'Commercial', 'good customer !', '1');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}

		query.prepare("INSERT INTO TAB_CUSTOMERS(TYPE, DESC_COMPAGNY, LASTNAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, COUNTRY, PHONENUMBER,"
					  "MOBILENUMBER, EMAIL, PROFESSION, COMMENTS, SENDING_PUB_EMAIL)"
					  "VALUES('1', 'EURL SIRET:123456798', 'Elect Rose', 'BATC rue des rosiers', 'Quartier St Paul',"
							 "'rue du centre', '13011', 'MARSEILLE', 'FRANCE','+3342005122', '+33698456221', 'info@elect-rose.fr', 'Electricite Generale', 'Montage dautomate, video surveillance...', '0');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Creation de la table des taxes
  */
bool database::createTable_tax(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_TAX ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"TAX            NUMERIC(5,2) NOT NULL,"
			"DESCRIPTION    VARCHAR(256)"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_TAX(TAX, DESCRIPTION)"
					  "VALUES('19.6', 'Standard');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_TAX(TAX, DESCRIPTION)"
					  "VALUES('5.5', 'Restauration, etc...');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
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
	QString req = "CREATE TABLE IF NOT EXISTS TAB_PRODUCTS("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"CREATIONDATE   TIMESTAMP,"
			"NAME           VARCHAR(256) NOT NULL,"
			"CODE           VARCHAR(64) NOT NULL,"
			"CODE_USER      VARCHAR(64) ,"
			"SELLING_PRICE  NUMERIC(8,2) NOT NULL,"
			"BUYING_PRICE   NUMERIC(8,2) NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"STOCK          INTEGER NOT NULL,"
			"STOCK_WARNING  INTEGER NOT NULL,"
			"STATE          INTEGER NOT NULL,"
			"IMAGE          BLOB,"
			"ID_PROVIDER    INTEGER NOT NULL,"
			"ID_CATEGORY    INTEGER NOT NULL"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//CONTRAINT
	query.prepare( "CREATE UNIQUE INDEX CODE_PRODUCT ON TAB_PRODUCTS(CODE);");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	query.prepare( "CREATE UNIQUE INDEX USERCODE_PRODUCT ON TAB_PRODUCTS(CODE_USER);");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_PRODUCTS(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
					  "VALUES('X-00000001', 'U0001', '45.99', '38.45','19.6', 'Pot de miel', '0', '1', '0', '1','2');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PRODUCTS(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
					  "VALUES('X-00000002', 'U0002', '14.95', '8.75','19.6', 'Huile de Lavande', '2', '1', '1','2','1');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PRODUCTS(CODE, CODE_USER, SELLING_PRICE, BUYING_PRICE, TAX, NAME, STOCK, STOCK_WARNING, STATE, ID_PROVIDER, ID_CATEGORY)"
					  "VALUES('X-00000003', 'U0003', '7.74', '3.05','19.6', 'Croquants aux amandes', '50', '10', '1','1','2');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Creation de la table des categories de produits
  */
bool database::createTable_products_categories(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_PRODUCTS_CATEGORIES("
				"ID     INTEGER PRIMARY KEY AUTOINCREMENT,"
				"NAME   VARCHAR(64) NOT NULL,"
				"COLOR  VARCHAR(64)"
				");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_PRODUCTS_CATEGORIES(NAME)"
					  "VALUES('Huiles essentielles');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PRODUCTS_CATEGORIES(NAME)"
					  "VALUES('Miels & Amandes');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}



/**
	Creation de la table fournisseur
  */
bool database::createTable_providers(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_PROVIDERS("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"CREATIONDATE   TIMESTAMP,"
			"NAME           VARCHAR(64)NOT NULL,"
			"ADDRESS1       VARCHAR(128),"
			"ADDRESS2       VARCHAR(128),"
			"ADDRESS3       VARCHAR(128),"
			"ZIPCODE        VARCHAR(10),"
			"CITY           VARCHAR(64),"
			"COUNTRY        VARCHAR(64),"
			"PHONENUMBER    VARCHAR(24),"
			"FAXNUMBER      VARCHAR(24),"
			"EMAIL          VARCHAR(128),"
			"CONTACT_NAME   VARCHAR(64)"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_PROVIDERS(NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME)"
					  "VALUES('Miel Plus', '104 chemin du tord', '14 bds micheler', '', '04210', 'VALENSOLE', '0442425687', '0655234857', 'info@mielPlus.fr', 'Mr Jean');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROVIDERS(NAME, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, FAXNUMBER, EMAIL, CONTACT_NAME)"
					  "VALUES('huilesEssentiellesAix', '2 rue du chateau', '', 'http://www.huilesEssentiellesAix.com', '13100', 'AIX-EN-PROVENCE', '0442456695', '0665554855', '', 'Mme Desjardin');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Creation de la table des services
  */
bool database::createTable_services(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_SERVICES ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_CUSTOMER    INTEGER NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"CREATIONDATE   TIMESTAMP,"
			"THEDATE        TIMESTAMP,"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"NAME           VARCHAR(256) NOT NULL,"
			"DESCRIPTION    VARCHAR(1024),"
			"FOREIGN KEY(ID_CUSTOMER) REFERENCES TAB_CUSTOMERS(ID) ON DELETE CASCADE"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_SERVICES(ID_CUSTOMER, TAX, THEDATE, PRICE, NAME, DESCRIPTION)"
					  "VALUES('2', '19.6', '1900-01-01 08:01:02', '65.4', 'Creation de site internet', 'CMS:Wordpress\nHebergeur:OVH');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_SERVICES(ID_CUSTOMER, TAX, THEDATE, PRICE, NAME, DESCRIPTION)"
					  "VALUES('1', '19.6', '201-04-01 08:01:02' ,'15.54', 'Service de mise en marche', '-Installation de la pompe a eau\n-Cablage du tableau electrique\n...');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Creation de la table des services commun
  */
bool database::createTable_services_common(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_SERVICES_COMMONS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"TAX            NUMERIC(5,2),"
			"CREATIONDATE   TIMESTAMP,"
			"THEDATE        TIMESTAMP,"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"NAME           VARCHAR(256) NOT NULL,"
			"DESCRIPTION    VARCHAR(1024)"
			"); ";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_SERVICES_COMMONS(THEDATE, PRICE, NAME)"
					  "VALUES('1900-01-01 08:01:02', '1500.40', 'Forfait mise en service');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_SERVICES_COMMONS(TAX, THEDATE, PRICE, NAME)"
					  "VALUES('19.6','1900-01-01 08:01:02', '20', 'Forfait horaire');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Creation de la table des propositions commerciales / Devis
  */
bool database::createTable_proposals(){
	QString req = "    CREATE TABLE IF NOT EXISTS TAB_PROPOSALS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_CUSTOMER    INTEGER NOT NULL,"
			"CREATIONDATE   TIMESTAMP,"
			"DATE           DATE NOT NULL,"
			"VALIDDATE      DATE NOT NULL,"
			"DELIVERYDATE         DATE NOT NULL,"
			"DELAY_DELIVERYDATE   INTEGER,"
			"CODE           VARCHAR(64) NOT NULL,"
			"TYPE_PAYMENT   VARCHAR(2),"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"STATE          INTEGER NOT NULL,"
			"DESCRIPTION    VARCHAR(256),"
			"FOREIGN KEY(ID_CUSTOMER) REFERENCES TAB_CUSTOMERS(ID) ON DELETE CASCADE"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	//CONTRAINT
	query.prepare( "CREATE UNIQUE INDEX CODE_PROPOSALS ON TAB_PROPOSALS(CODE);");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_PROPOSALS(ID_CUSTOMER, CODE, DATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
					  "VALUES('1', 'PR110802-1', '2011-08-02', '2011-08-02', '2011-08-02', 'CC', '98.95', '0', 'Produits provence');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS(ID_CUSTOMER, CODE, DATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
					  "VALUES('1', 'PR110802-2', '2011-08-02', '2011-08-02', '2011-08-02', 'CA', '490.99', '1', 'Installation web');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS(ID_CUSTOMER, CODE, DATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE, DESCRIPTION)"
					  "VALUES('1', 'PR110802-3', '2011-08-02', '2011-08-02', '2011-08-02', 'CC', '1589.10', '2', 'Montage d une pompe hydraulique');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS(ID_CUSTOMER, CODE, DATE, VALIDDATE, DELIVERYDATE, TYPE_PAYMENT, PRICE, STATE)"
					  "VALUES('2', 'PR110802-4', '2011-08-02', '2011-08-02', '2011-08-02', 'CH', '20', '0');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}


/**
	Creation de la table de details des propositions commerciales / Devis
  */
bool database::createTable_proposals_details(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_PROPOSALS_DETAILS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_PROPOSAL    INTEGER NOT NULL,"
			"ID_PRODUCT     INTEGER,"
			"NAME           VARCHAR(128) NOT NULL,"
			"DISCOUNT       INTEGER NOT NULL,"
			"QUANTITY       INTEGER NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"ITEM_ORDER     INTEGER,"
			"FOREIGN KEY(ID_PROPOSAL) REFERENCES TAB_PROPOSALS(ID) ON DELETE CASCADE"
			"); ";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','1','Pot de miel','2','0','45.99');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','3','Croquants aux amandes','1','10','7.74');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('2','installation CMS wordpress','1','0','450.99');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT,PRICE)"
					  "VALUES('2','Forfait horaire','2','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('3','Pompe hydraulique','1','5','1400.95');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('3','raccords','10','0','5.56');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('3','tuyaux au metre','10','0','4.26');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('3','Forfait horaire','8','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_PROPOSALS_DETAILS(ID_PROPOSAL, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('4','Forfait horaire','1','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}


/**
	Creation de la table des factures
  */
bool database::createTable_invoices(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_INVOICES ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_CUSTOMER    INTEGER NOT NULL,"
			"CREATIONDATE   TIMESTAMP,"
			"DATE           DATE NOT NULL,"
			"LIMIT_PAYMENTDATE    DATE NOT NULL,"
            "PAYMENTDATE    DATE NOT NULL,"
			"CODE           VARCHAR(64) NOT NULL,"
			"TYPE_PAYMENT   VARCHAR(2) ,"
			"PART_PAYMENT   NUMERIC(8,2) NOT NULL,"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"STATE          INTEGER NOT NULL,"
			"DESCRIPTION    VARCHAR(256),"
			"FOREIGN KEY(ID_CUSTOMER) REFERENCES TAB_CUSTOMERS(ID) ON DELETE CASCADE"
			");  ";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//CONTRAINT
	query.prepare( "CREATE UNIQUE INDEX CODE_INVOCES ON TAB_INVOICES(CODE);");
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
    //INSERT
    if(addSample){
        query.prepare("INSERT INTO TAB_INVOICES(ID_CUSTOMER, CODE, DATE, LIMIT_PAYMENTDATE, PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                      "VALUES('1', 'FA110802-1', '2011-08-02', '2011-08-02', '2011-08-02', 'CC', '0.00', '361.02', '0', 'Produits');" );
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
            return false;
        }
        query.prepare("INSERT INTO TAB_INVOICES(ID_CUSTOMER, CODE, DATE, LIMIT_PAYMENTDATE, PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                      "VALUES('1', 'FA110804-2', '2011-08-04', '2011-08-04', '2011-08-04', 'CC', '0.00', '1589.10', '1', 'Montage d une pompe hydraulique');" );
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
            return false;
        }
        query.prepare("INSERT INTO TAB_INVOICES(ID_CUSTOMER, CODE, DATE, LIMIT_PAYMENTDATE, PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                      "VALUES('1', 'FA110820-3', '2011-08-20', '2011-09-02', '2011-09-02', 'CA', '0.00', '65.99', '1', 'Produits et Services');" );
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
            return false;
        }
        query.prepare("INSERT INTO TAB_INVOICES(ID_CUSTOMER, CODE, DATE, LIMIT_PAYMENTDATE, PAYMENTDATE, TYPE_PAYMENT, PART_PAYMENT, PRICE, STATE, DESCRIPTION)"
                      "VALUES('2', 'FA120115-4', '2012-08-15', '2012-01-15', '2012-01-15', 'CH', '11.55', '20', '1', 'test');" );
        if(!query.exec()) {
            QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
            return false;
        }
    }
	return true;
}


/**
	Creation de la table de details des factures
  */
bool database::createTable_invoices_details(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_INVOICES_DETAILS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_INVOICE     INTEGER NOT NULL,"
			"ID_PRODUCT     INTEGER,"
			"NAME           VARCHAR(128) NOT NULL,"
			"DISCOUNT       INTEGER NOT NULL,"
			"QUANTITY       INTEGER NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"ITEM_ORDER     INTEGER,"
			"FOREIGN KEY(ID_INVOICE) REFERENCES TAB_INVOICES(ID) ON DELETE CASCADE"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','1','Pot miel','2','0','45.99');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','3','Croquants aux amandes','1','10','7.74');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','2','Huile de Lavande','1','5','14.95');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','amandes','5','0','3.56');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','Figues','20','0','4.26');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('1','Savons Marseille','10','5','15.25');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('2','Pompe hydraulique','1','5','1400.95');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('2','raccords','10','0','5.56');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('2','tuyaux au metre','10','0','4.26');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('2','Forfait horaire','8','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, ID_PRODUCT, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('3','1','Pot de miel','1','0','45.99');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT,PRICE)"
					  "VALUES('3','Forfait horaire','1','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
		query.prepare("INSERT INTO TAB_INVOICES_DETAILS(ID_INVOICE, NAME, QUANTITY, DISCOUNT, PRICE)"
					  "VALUES('4','Forfait horaire','1','0','20');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}




/**
	Creation de la table de Lien entre propositions commerciales / Devis et Factures
  */
bool database::createTable_link_proposals_invoices(){
	QString req = "CREATE TABLE IF NOT EXISTS TAB_LINK_PROPOSALS_INVOICES ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_PROPOSAL    INTEGER NOT NULL,"
			"ID_INVOICE     INTEGER NOT NULL,"
			"FOREIGN KEY(ID_INVOICE) REFERENCES TAB_INVOICES(ID) ON DELETE CASCADE,"
			"FOREIGN KEY(ID_PROPOSAL) REFERENCES TAB_PROPOSALS(ID) ON DELETE CASCADE"
			");";
	//fuck the diff
	if(db.driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");

	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//INSERT
	if(addSample){
		query.prepare("INSERT INTO TAB_LINK_PROPOSALS_INVOICES(ID_PROPOSAL, ID_INVOICE)"
					  "VALUES('3', '2');" );
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}
	return true;
}

/**
	Obtenir la version de la base de donnees
	@return version
  */
int database::databaseVersion() {
	if(!this->m_connected)return -1;
	int ver=-1;

	QSqlQuery query;
	query.prepare("SELECT DBASE_VERSION FROM TAB_INFORMATIONS;");

	if(query.exec()){
		query.next();
		ver = query.value(0).toInt();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}
	return ver;
}

/**
	Savoir si la societe est assujetti a la TVA
	@return true if TAX
  */
bool database::isTax() {
	if(!this->m_connected)return false;
	bool ret=false;

	QSqlQuery query;
	query.prepare("SELECT TAX FROM TAB_INFORMATIONS;");

	if(query.exec()){
		query.next();
		ret = query.value(0).toBool();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}
	return ret;
}

/**
	  Applique les informations dans la base de donnee
	  @return true si ok
  */
bool database::updateInfo(Informations &info) {
	if(!this->m_connected)return false;
	//Test si l enregistrement existe
	QString req = "SELECT COUNT(*) FROM TAB_INFORMATIONS WHERE ID=1;";
	int count=0;
	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		count = query.value(query.record().indexOf("COUNT(*)")).toInt();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//Si pas d enregistrement on en creer un
	if(count<=0){
        query.prepare("INSERT INTO TAB_INFORMATIONS(DBASE_VERSION, TAX, NAME,CA_TYPE)"
                      "VALUES('1', '0', '', '1');");
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}

	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	QString f;
	req = "UPDATE TAB_INFORMATIONS SET ";
	req += "NAME='" + info.name.replace("\'","''") + "',";
	req += "NUM='" + info.num.replace("\'","''") + "',";
	req += "CAPITAL='" + info.capital.replace("\'","''") + "',";
	req += "ADDRESS1='" + info.address1.replace("\'","''") + "',";
	req += "ADDRESS2='" + info.address2.replace("\'","''") + "',";
	req += "ADDRESS3='" + info.address3.replace("\'","''") + "',";
	req += "ZIPCODE='" + info.zipCode.replace("\'","''") + "',";
	req += "CITY='" + info.city.replace("\'","''") + "',";
	req += "PHONENUMBER='" + info.phoneNumber.replace("\'","''") + "',";
	req += "FAXNUMBER='" + info.faxNumber.replace("\'","''") + "',";
	req += "EMAIL='" + info.email.replace("\'","''") + "',";
	req += "WEBSITE='" + info.webSite.replace("\'","''") + "', ";
    req += "TAX='" + QString::number(info.tax) + "', ";
    req += "CA_TYPE='" + QString::number(info.ca_type) + "' ";
	req += "WHERE ID='1';";

	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	/* update cache */
	m_isTax = info.tax;
	return true;
}

/**
	  Retourne les informations dans la base de donnee
	  @return Informations
  */
bool database::getInfo(Informations &info) {
	if(!this->m_connected)return false;

	QSqlQuery query;
	query.prepare("SELECT * from TAB_INFORMATIONS;");

	if(query.exec()){
		query.next();
		info.name = query.value(query.record().indexOf("NAME")).toString();
		info.num = query.value(query.record().indexOf("NUM")).toString();
		info.capital = query.value(query.record().indexOf("CAPITAL")).toString();
		info.address1 = query.value(query.record().indexOf("ADDRESS1")).toString();
		info.address2 = query.value(query.record().indexOf("ADDRESS2")).toString();
		info.address3 = query.value(query.record().indexOf("ADDRESS3")).toString();
		info.zipCode = query.value(query.record().indexOf("ZIPCODE")).toString();
		info.city = query.value(query.record().indexOf("CITY")).toString();
		info.phoneNumber = query.value(query.record().indexOf("PHONENUMBER")).toString();
		info.faxNumber = query.value(query.record().indexOf("FAXNUMBER")).toString();
		info.email = query.value(query.record().indexOf("EMAIL")).toString();
		info.webSite = query.value(query.record().indexOf("WEBSITE")).toString();
		info.tax = query.value(query.record().indexOf("TAX")).toInt();
        info.ca_type = query.value(query.record().indexOf("CA_TYPE")).toInt();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	/* update cache */
	m_isTax = info.tax;
	return true;
}

/**
	  Applique les informations de la banque dans la base de donnee
	  @return true si ok
  */
bool database::updateBank(Bank &b) {
	if(!this->m_connected)return false;
	//Test si l enregistrement existe
	QString req = "SELECT COUNT(*) FROM TAB_BANK WHERE ID=1;";
	int count=0;
	QSqlQuery query;
	query.prepare(req);

	if(query.exec()){
		query.next();
		count = query.value(query.record().indexOf("COUNT(*)")).toInt();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	//Si pas d enregistrement on en creer un
	if(count<=0){
		query.prepare("INSERT INTO TAB_BANK(CODE_BANQUE)"
					  "VALUES('');");
		if(!query.exec()) {
			QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
			return false;
		}
	}

	// Construction de la requette
	// Si le charactere speciaux "\'" existe on l'adapte pour la requette
	req = "UPDATE TAB_BANK SET ";
	req += "CODE_BANQUE='" + b.codeBanque.replace("\'","''") + "',";
	req += "CODE_GUICHET='" + b.codeGuichet.replace("\'","''") + "',";
	req += "NUM_COMPTE='" + b.numCompte.replace("\'","''") + "',";
	req += "KEY_RIB='" + b.keyRIB.replace("\'","''") + "',";
	req += "ADDRESS='" + b.address.replace("\'","''") + "',";
	req += "IBAN_1='" + b.IBAN1.replace("\'","''") + "',";
	req += "IBAN_2='" + b.IBAN2.replace("\'","''") + "',";
	req += "IBAN_3='" + b.IBAN3.replace("\'","''") + "',";
	req += "IBAN_4='" + b.IBAN4.replace("\'","''") + "',";
	req += "IBAN_5='" + b.IBAN5.replace("\'","''") + "',";
	req += "IBAN_6='" + b.IBAN6.replace("\'","''") + "',";
	req += "IBAN_7='" + b.IBAN7.replace("\'","''") + "', ";
	req += "IBAN_8='" + b.IBAN8.replace("\'","''") + "', ";
	req += "IBAN_9='" + b.IBAN9.replace("\'","''") + "', ";
	req += "CODE_BIC='" + b.codeBIC.replace("\'","''") + "'";
	req += "WHERE ID='1';";

	query.prepare(req);
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	return true;
}

/**
	  Retourne les informations banque de la base de donnee
	  @return bank
  */
bool database::getBank(Bank &b) {
	if(!this->m_connected)return false;

	QSqlQuery query;
	query.prepare("SELECT * from TAB_BANK;");
	if(query.exec()){
		query.next();
		b.codeBanque = query.value(query.record().indexOf("CODE_BANQUE")).toString();
		b.codeGuichet = query.value(query.record().indexOf("CODE_GUICHET")).toString();
		b.numCompte = query.value(query.record().indexOf("NUM_COMPTE")).toString();
		b.keyRIB = query.value(query.record().indexOf("KEY_RIB")).toString();
		b.address = query.value(query.record().indexOf("ADDRESS")).toString();
		b.IBAN1 = query.value(query.record().indexOf("IBAN_1")).toString();
		b.IBAN2 = query.value(query.record().indexOf("IBAN_2")).toString();
		b.IBAN3 = query.value(query.record().indexOf("IBAN_3")).toString();
		b.IBAN4 = query.value(query.record().indexOf("IBAN_4")).toString();
		b.IBAN5 = query.value(query.record().indexOf("IBAN_5")).toString();
		b.IBAN6 = query.value(query.record().indexOf("IBAN_6")).toString();
		b.IBAN7 = query.value(query.record().indexOf("IBAN_7")).toString();
		b.IBAN8 = query.value(query.record().indexOf("IBAN_8")).toString();
		b.IBAN9 = query.value(query.record().indexOf("IBAN_9")).toString();
		b.codeBIC = query.value(query.record().indexOf("CODE_BIC")).toString();
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	return true;
}

/**
	Met a jour le logo dans la base de donnees
  */
bool database::updateLogoTable_informations(const QImage& image){
	if(!this->m_connected)return false;

	QByteArray data;
	QBuffer buf(&data);
	// writes image into ba in PNG format
	image.save(&buf,"PNG");

	QSqlQuery query;
	query.prepare( "UPDATE TAB_INFORMATIONS SET LOGO = :data" );
	query.bindValue( ":data", buf.data() );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	return true;
}

/**
	Recupere le logo dans la base de donnees
  */
QImage database::getLogoTable_informations(){
	QImage image;
	QByteArray data;
	if(!this->m_connected)return image;

	QSqlQuery query;
	query.prepare("SELECT LOGO FROM TAB_INFORMATIONS;");
	if(query.exec()){
		query.next();
		data = query.value(query.record().indexOf("LOGO")).toByteArray();
		image.loadFromData(data,"PNG");
	}
	else{
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
	}
	return image;
}

/**
   Retourne la date courante
  */
QDate database::getCurrentDate() {
	QDate mdate;
/*    IBPP::Date date;

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
	}*/

	return mdate;
}

/**
   Met a jour la base de donnees en version 2
  */
bool database::upgradeToV2() {

	QString req =	"ALTER TABLE TAB_PROPOSALS_DETAILS ADD ITEM_ORDER INTEGER;";
	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	
	req =	"ALTER TABLE TAB_INVOICES_DETAILS ADD ITEM_ORDER INTEGER;";
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=2;";
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}
	
	return true;
}

/**
   Met a jour la base de donnees en version 3
  */
bool database::upgradeToV3() {
	
	QString req =	"ALTER TABLE TAB_INVOICES ADD PAYMENTDATE;";
	QSqlQuery query;
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	// Pour les factures existante on prend la date de creation comme date de reglement
	req =	"UPDATE TAB_INVOICES SET PAYMENTDATE = DATE;";
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	req =	"ALTER TABLE TAB_INFORMATIONS ADD CA_TYPE INT DEFAULT 0;";
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=3;";
	query.prepare( req );
	if(!query.exec()) {
		QMessageBox::critical(this->m_parent, tr("Erreur"), query.lastError().text());
		return false;
	}

	return true;
}

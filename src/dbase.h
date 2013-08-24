#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDateTime>
#include <QLocale>
#include <QImage>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "tax.h"
#include "customer.h"
#include "product.h"

class customer;
class product;

#define MCERCLE_VERSION "13.08 beta"

class database : public QObject
{
private:
	QSqlDatabase db;

	QWidget *m_parent;
	bool m_connected;
	QString m_bdd, m_name, m_hostName, m_login, m_password;
    int m_port, m_databaseVersion;
	bool addSample;
	QString m_FDBversion;
	QLocale m_lang;

	/* Version max supporter par mcercle */
    #define DBASE_SUPPORTED 3


	/* Creation des tables */
	bool createTable_informations();
	bool createTable_bank();
	bool createTable_customers();
	bool createTable_tax();
	bool createTable_products();
	bool createTable_products_categories();
	bool createTable_providers();
	bool createTable_services();
	bool createTable_services_common();
	bool createTable_proposals();
	bool createTable_proposals_details();
	bool createTable_link_proposals_invoices();
	bool createTable_invoices();
	bool createTable_invoices_details();

	/* Cache info entreprise */
	bool m_isTax;
	int databaseVersion();
	bool isTax();
	
    /* fonctions de mise a jour */
	bool upgradeToV2();
    bool upgradeToV3();
	
public:

	typedef struct{
		//Valeur pour le placement dans un tableau
		QString name;
		QString num;
		QString capital;
		QString phoneNumber;QString faxNumber;
		QString email; QString webSite;
		QString zipCode;
		QString city;
		QString address1;QString address2;QString address3;
        int tax; int ca_type;
    }Informations;

	typedef struct{
		//Valeur pour le placement dans un tableau
		QString codeBanque;
		QString codeGuichet;
		QString numCompte;
		QString keyRIB;
		QString address;
		QString IBAN1,IBAN2,IBAN3,IBAN4,IBAN5,IBAN6,IBAN7,IBAN8,IBAN9;
		QString codeBIC;
	}Bank;

	database(QLocale &lang, QWidget *parent);
	~database();

	/* Methode de pilotage de la bdd */
	char connect();
	void close(){db.close();m_connected = false;}
	bool create();
	bool isConnected(){return m_connected;}

	QString getBdd(){return m_bdd;}
	QString getHostName(){return m_hostName;}
	int getPort(){return m_port;}
	QString getDatabaseName(){return m_name;}
	QString getUserName(){return m_login;}
	QString getPassword(){return m_password;}
	QString getFireBirdVersion(){return m_FDBversion;}
	int getDatabaseVersion(){return m_databaseVersion;}
	QDate getCurrentDate();
	QString getDriverName(){return db.driverName();}
	QStringList getDrivers(){return db.drivers();}

	void setBdd(const QString& bdd){if(!bdd.isEmpty())m_bdd = bdd;}
	void setHostName(const QString& hostName){if(!hostName.isEmpty())m_hostName = hostName;}
	void setPort(const int& port){if(port>0)m_port = port;}
	void setDatabaseName(const QString& name){if(!name.isEmpty())m_name = name;}
	void setUserName(const QString& login){if(!login.isEmpty())m_login = login;}
	void setPassword(const QString& password){if(!password.isEmpty())m_password = password;}

	//Informations
	bool updateLogoTable_informations(const QImage& image);
	QImage getLogoTable_informations();
	bool updateInfo(Informations &info);
	bool getInfo(Informations &info);
	bool updateBank(Bank &b);
	bool getBank(Bank &b);
	bool getIsTax(){return m_isTax;}


	enum{DB_CON_OK, DB_CONNECT, DB_CREATE, DB_CON_ERR, DB_NOTEXIST_ERR};

	/* SOUS CLASS */
	customer *m_customer;
	product *m_product;
	tax *m_tax;
};

#endif // DATABASE_H

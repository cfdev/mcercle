#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDateTime>
#include <QLocale>
#include <QImage>

#include "bdd/ibpp.h"
#include "customer.h"
#include "product.h"

class customer;
class product;

#define MCERCLE_VERSION "1.0"

class database : public QObject
{
private:
    IBPP::Database db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

    QWidget *m_parent;
    bool m_connected;
    QString m_name, m_hostName, m_login, m_password;
    int m_port, m_databaseVersion;
    bool addSample;
    QString m_FDBversion;
    QLocale m_lang;

    #define ROLE        ""
    #define CHARSET     "UTF8"
    #define OPTIONS     "PAGE_SIZE = 4096"

    #define FIREBIRD_SUPPORTED 259 // pour 2.5.9
    #define DBASE_SUPPORTED 1


    /* Creation des tables */
    bool createTable_informations();
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

    QString FireBirdVersion();
    int databaseVersion();

    /* Cache info entreprise */
    int m_tax;

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
        int tax;
    }Informations;

    database(QLocale &lang, QWidget *parent);
    ~database();

    /* Methode de pilotage de la bdd */
    char connect(char type);
    void close(){db->Disconnect();m_connected = false;}
    bool create();
    bool isConnected(){return m_connected;}

    QString getHostName(){return m_hostName;}
    int getPort(){return m_port;}
    QString getDatabaseName(){return m_name;}
    QString getUserName(){return m_login;}
    QString getPassword(){return m_password;}
    QString getFireBirdVersion(){return m_FDBversion;}
    int getDatabaseVersion(){return m_databaseVersion;}
    QDate getCurrentDate();

    void setHostName(const QString& hostName){if(!hostName.isEmpty())m_hostName = hostName+ "/" + QString::number(m_port);}
    void setPort(const int& port){if(port>0)m_port = port;}
    void setDatabaseName(const QString& name){if(!name.isEmpty())m_name = name;}
    void setUserName(const QString& login){if(!login.isEmpty())m_login = login;}
    void setPassword(const QString& password){if(!password.isEmpty())m_password = password;}

    //Informations
    bool updateLogoTable_informations(const QImage& image);
    QImage getLogoTable_informations();
    bool updateInfo(Informations &info);
    bool getInfo(Informations &info);
    int getIsTax(){return m_tax;}


    enum{DB_CON_OK, DB_CONNECT, DB_CREATE};
    enum{DB_CON_ERR, DB_NOTEXIST_ERR};

    static QDateTime fromIBPPTimeStamp(IBPP::Timestamp &dateTime);
    static IBPP::Timestamp toIBPPTimeStamp(const QDateTime &dateTime);
    static QDate fromIBPPDate(IBPP::Date &IBPPdate);
    static IBPP::Date toIBPPDate(const QDate &date);
    static QString IBPPversion();

    /* SOUS CLASS */
    customer *m_customer;
    product *m_product;
};

#endif // DATABASE_H

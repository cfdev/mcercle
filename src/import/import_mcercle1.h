#ifndef IMPORT_MCERCLE1_H
#define IMPORT_MCERCLE1_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QString>
#include <QColor>
#include <vector>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ibpp/ibpp.h"

class import_mcercle1  : public QObject
{
public:
    import_mcercle1(QString hostName, int port, QString name, QString login, QString password, QWidget *parent = 0);
    ~import_mcercle1();
    bool run();

private:
    IBPP::Database db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

    QString m_hostName, m_name, m_login, m_password;
    QWidget *m_parent;

#define ROLE        ""
#define CHARSET     "UTF8"
#define OPTIONS     "PAGE_SIZE = 4096"

    bool table_informations();
    bool table_customers();
    bool table_products();
    bool table_products_categories();
    bool table_providers();
    bool table_services();
    bool table_services_commons();

    bool table_proposals();
    bool table_proposals_details();
    bool table_invoices();
    bool table_invoices_details();
    bool table_link_proposals_invoices();


    QDate fromIBPPDate(IBPP::Date &IBPPdate);
    QDateTime fromIBPPTimeStamp(IBPP::Timestamp &dateTime);

    typedef struct{
        //Valeur pour le placement dans un tableau
        QString name;
        QString num;
        QString capital;
        QString phoneNumber,faxNumber;
        QString email,webSite;
        QString zipCode;
        QString city;
        QString address1,address2,address3;
        int tax;
    }Informations;

    typedef struct{
        std::vector<int> id,type,sending_pubEmail;
        std::vector<QDateTime> creationDate;
        QStringList firstName,lastName;
        QStringList desCompagny;
        QStringList gender;
        std::vector<QDate> birthday;
        QStringList address1,address2,address3,zipCode,city,country;
        QStringList phoneNum,mobileNum,email;
        QStringList profession,comments;
    }CustomerList;

    typedef struct{
        std::vector<int> id;
        std::vector<QDateTime> creationDate;
        QStringList code;
        QStringList name;
        std::vector<float> buying_price, selling_price;
        std::vector<int> idCategory, idProvider;
        std::vector<int> stock, stockWarning;
        std::vector<int> state;
    }ProductList;

    typedef struct{
        std::vector<int> id;
        QStringList name;
        std::vector<QColor> color;
    }categoryList;

    typedef struct{
        std::vector<int> id;
        QStringList name, contactName;
        QStringList address1,address2,address3,zipCode,city;
        QStringList phoneNum,faxNumber,email;
    }ProviderList;

    typedef struct{
        std::vector<int> id, idCust;
        std::vector<QDateTime> creationDate;
        std::vector<QDateTime> date;
        QStringList name;
        QStringList description;
        std::vector<float> price;
    }ServiceList;

    typedef struct{
        std::vector<int> id, idCust;
        std::vector<QDateTime> creationDate;
        std::vector<QDate> userDate, deliveryDate, validDate;
        std::vector<int> delayDeliveryDate;
        QStringList code;
        QStringList typePayment, description;
        std::vector<float> price;
        std::vector<int> state;
    }ProposalList;

    //Details de la proposition
    typedef struct{
        std::vector<int> id, idProposal, idProduct;
        QStringList name;
        std::vector<int> discount, quantity;
        std::vector<float> price;
    }ProposalListItems;


    typedef struct{
        std::vector<int> id, idCust;
        std::vector<QDateTime> creationDate;
        std::vector<QDate> userDate, limitPayment;
        QStringList code;
        QStringList typePayment, description;
        std::vector<float> price, partPayment;
        std::vector<int> state;
    }InvoiceList;

    //Details de la facture
    typedef struct{
        std::vector<int> id, idInvoice, idProduct;
        QStringList name;
        std::vector<int> discount, quantity;
        std::vector<float> price;
    }InvoiceListItems;

    typedef struct{
        std::vector<int> id, idProposal, idInvoice;
    }LinkList;

};

#endif // IMPORT_MCERCLE1_H

#ifndef INVOICE_H
#define INVOICE_H

#include <QStringList>
#include <vector>
#include <QLocale>
#include <QDateTime>
#include <QIcon>
#include "bdd/ibpp.h"


// Type paiement: espece, cheque, CB, TIP, virement, prelevement, autre
#define CASH "CA"
#define CHECK "CH"
#define CREDIT_CARD "CC"
#define INTERBANK "IB"
#define TRANSFER "TR"
#define DEBIT "DE"
#define OTHER "OT"

class invoice : public QObject {

private:
    IBPP::Database m_db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

    QWidget *m_parent;
    int m_id, m_idCustomer, m_state ;
    float m_price, m_partPayment;
    QDateTime m_creationDate;
    QDate m_userDate, m_limitPayment;
    QString m_code, m_proposalCode, m_description, m_typePayment;
    QLocale m_lang;


public:
    //Valeur pour le placement dans un tableau.
    //Facture
    typedef struct{
        std::vector<int> id;
        std::vector<QDate> userDate;
        std::vector<QDate> limitPayment;
        QStringList code;
        QStringList codeProposal; //lien avec la proposition commerciale sil existe
        QStringList description;
        std::vector<float> price;
        std::vector<int> state;
    }InvoiceList;

    typedef struct{
        QStringList customerFirstName;
        QStringList customerLastName;
        std::vector<int> id;
        std::vector<QDate> userDate;
        std::vector<QDate> limitPayment;
        QStringList code;
        QStringList codeProposal; //lien avec la proposition commerciale sil existe
        QStringList description;
        std::vector<float> price;
        std::vector<int> state;
    }InvoiceListAlert;


    //Details de la facture
    typedef struct{
        std::vector<int> id;
        std::vector<int> idProduct;
        QStringList name;
        std::vector<int> discount;
        std::vector<int> quantity;
        std::vector<float> tax;
        std::vector<float> price;
    }InvoiceListItems;

    //Un Item de la facture
    typedef struct{
        int id;
        int idProduct;
        QString name;
        int discount;
        int quantity;
        float tax;
        float price;
    }InvoiceItem;

    //State
    enum{UNPAID , PAID};

    invoice(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent = 0);
    ~invoice();

    bool create();
    bool update();
    bool remove();
    bool loadFromID(const int& id);
    bool loadFromCode(const QString& code);
    bool isHere(const QString& code);
    int getLastId();

    //Appliquer les valeurs
    void setId(const int& ident){m_id = ident;}
    void setIdCustomer(const int& cIdent){m_idCustomer = cIdent;}
    void setPrice(const float& price){m_price = price;}
    void setPartPayment(const float& partPayment){m_partPayment = partPayment;}
    void setTypePayment(const QString& type){m_typePayment = type;}
    void setState(const int& state){m_state = state;}
    void setCode(const  QString& code){m_code = code;}
    void setDescription(const  QString& description){m_description = description;}
    void setUserDate(const QDate& date){m_userDate = date;}
    void setLimitPayment(const QDate& date){m_limitPayment = date;}

    //recup les valeurs de la facture
    QIcon getIconState(int state);
    QString getTextState(int state);    
    int getId(){return m_id;}
    int getIdCustomer(){return m_idCustomer;}
    float getPrice(){return m_price;}
    float getPartPayment(){return m_partPayment;}
    QString getTypePayment(){return m_typePayment;}
    int getState(){return m_state;}
    QDateTime getCreationDate(){return m_creationDate;}
    QDate getUserDate(){return m_userDate;}
    QDate getLimitPayment(){return m_limitPayment;}
    QString getCode(){return m_code;}
    QString getProposalCode(){return m_proposalCode;}
    QString getDescription(){return m_description;}

    void getInvoiceList(InvoiceList& list, int id_customer, QString order, QString filter, QString field);
    void getInvoiceListAlert(InvoiceListAlert& list);
    QStringList getYearsList();
    float getMonthRevenue(QString year, QString month);
    float getYearRevenue(QString year);


    //recup des articles de la facture
    void getInvoiceItemsList(InvoiceListItems& list, QString order, QString filter, QString field);

    // Fonction sur un article
    void getInvoiceItem(InvoiceItem& item);
    bool addInvoiceItem(InvoiceItem& item);
    bool removeInvoiceItem(int Itemid);
    bool updateInvoiceItem(InvoiceItem& item);


};



#endif // INVOICE_H

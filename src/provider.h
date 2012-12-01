#ifndef PROVIDER_H
#define PROVIDER_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include "bdd/ibpp.h"


/// TODO: refaire la class provider avec des noms plus explicites dans la stucture - cf class "productcategory"
class provider : public QObject
{

private:
    IBPP::Database m_db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

   QWidget *m_parent;
   int m_id;
   QString  m_name, m_address1, m_address2, m_address3, m_zipCode,
            m_city, m_phoneNumber, m_fax, m_email, m_contact;
   QDateTime m_creationDate;
   QString m_field, m_filter;

public:
   typedef struct{
       //Valeur pour le placement dans un tableau
        QStringList name;
        QStringList phoneNumber;
        QStringList faxNumber;
        QStringList email;
   }ProviderList;


    provider(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent = 0);
    ~provider();

    bool create();
    bool update();
    bool remove();
    int isHere(const QString& name);
    bool loadFromID(const int& id);
    bool loadFromName(const QString& name);

    //Appliquer les valeurs
    void setName(const  QString& Name){m_name = Name;}
    void setAddress(const  QString& address1, const  QString& address2, const  QString& address3){m_address1 = address1;m_address2 = address2;m_address3 = address3;}
    void setZipCode(const  QString& zipCode){m_zipCode = zipCode;}
    void setCity(const  QString& city){m_city = city;}
    void setPhone(const  QString& phoneNumber, const  QString& faxNumber){m_phoneNumber = phoneNumber;m_fax = faxNumber;}
    void setEmail(const  QString& email){m_email = email;}
    void setContact(const  QString& contact){m_contact = contact;}

    //recup les valeurs
    int getId(){return m_id;}
    QDateTime getCreationDate(){return m_creationDate;}
    QString getName(){return m_name;}
    QString getAddress1(){return m_address1;}
    QString getAddress2(){return m_address2;}
    QString getAddress3(){return m_address3;}
    QString getZipCode(){return m_zipCode;}
    QString getCity(){return m_city;}
    QString getPhoneNumber(){return m_phoneNumber;}
    QString getFaxNumber(){return m_fax;}
    QString getEmail(){return m_email;}
    QString getContact(){return m_contact;}

    void getProviderList(ProviderList& list, QString order, QString filter, QString field);

};

#endif // PROVIDER_H

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QString>
#include <vector>
#include "bdd/ibpp.h"

#include "proposal.h"
#include "invoice.h"
#include "services.h"
#include "services_commons.h"

// Monsieur, Madame
#define MISTER "MR"
#define MADAM "MRS"


class invoice;
class intervention;

class customer : public QObject
{
Q_OBJECT

private:
    IBPP::Database m_db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

    QWidget *m_parent;
    int m_id;
    QString m_lastName, m_firstName, m_gender, m_desCompagny,
            m_address1, m_address2, m_address3, m_zipCode,
            m_city, m_country, m_phoneNumber, m_mobileNumber, m_email, m_profession,
            m_comments;
    QDateTime m_creationDate;
    QDate m_birthday;
    bool m_sending_pubEmail;
    int m_type;

public:
    typedef struct{
        //Valeur pour le placement dans un tableau
        std::vector<int> id;
         QStringList lastName;
        QStringList firstName;
    }CustomerList;

    enum{PEOPLE_TYPE, COMPAGNY_TYPE};

    customer(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent = 0);
    ~customer();

    bool create();
    bool update();
    bool remove();
    bool loadFromID(const int& id);
    int count(QString filter, QString field);

    //Appliquer les valeurs
    void setType(const int&type){m_type = type;}
    void setDesCompagny(const  QString& des){m_desCompagny = des;}
    void setName(const  QString& firstName, const  QString& lastName){m_firstName = firstName;m_lastName = lastName;}
    void setGender(const  QString& gender){m_gender = gender;}
    void setBirthday(const  QDate& birthday){m_birthday = birthday;}
    void setAddress(const  QString& address1, const  QString& address2, const  QString& address3){m_address1 = address1;m_address2 = address2;m_address3 = address3;}
    void setZipCode(const  QString& zipCode){m_zipCode = zipCode;}
    void setCity(const  QString& city){m_city = city;}
    void setCountry(const  QString& country){m_country = country;}
    void setPhone(const  QString& phoneNumber, const  QString& mobileNumber){m_phoneNumber = phoneNumber;m_mobileNumber = mobileNumber;}
    void setEmail(const  QString& email){m_email = email;}
    void setProfession(const  QString& profession){m_profession = profession;}
    void setComments(const  QString& comments){m_comments = comments;}
    void setSendingPubEmail(bool state){m_sending_pubEmail = state;}

    //recup les valeurs
    int getId(){return m_id;}
    QDateTime getCreationDate(){return m_creationDate;}
    int getType(){return m_type;}
    QString getDesCompagny(){return m_desCompagny;}
    QString getLastName(){return m_lastName;}
    QString getFirstName(){return m_firstName;}
    QString getGender(){return m_gender;}
    QDate getBirthday(){return m_birthday;}
    QString getAddress1(){return m_address1;}
    QString getAddress2(){return m_address2;}
    QString getAddress3(){return m_address3;}
    QString getZipCode(){return m_zipCode;}
    QString getCity(){return m_city;}
    QString getCountry(){return m_country;}
    QString getPhoneNumber(){return m_phoneNumber;}
    QString getMobileNumber(){return m_mobileNumber;}

    QString getEmail(){return m_email;}
    QString getProfession(){return m_profession;}
    QString getComments(){return m_comments;}
    bool getSendingPubEmail(){return m_sending_pubEmail;}

    bool getCustomerList(CustomerList& list,  int first, int skip, QString filter, QString field);

    // sous class
    proposal *m_proposal;
    invoice *m_invoice;
    service *m_service;
    service_common *m_serviceComm;
};

#endif // CUSTOMER_H

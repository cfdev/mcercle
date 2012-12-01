#ifndef service_common_H
#define service_common_H

#include <QStringList>
#include <vector>
#include <QLocale>
#include <QDateTime>
#include "bdd/ibpp.h"


class service_common : public QObject {

private:
    IBPP::Database m_db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

    QWidget *m_parent;
    int m_id, m_idCustomer;
    float m_price;
    QDateTime m_creationDate, m_theDate;
    QString m_name, m_description;
    QLocale m_lang;


public:

    typedef struct{
        //Valeur pour le placement dans un tableau.
        std::vector<int> id;
        std::vector<QDateTime> creationDate;
        std::vector<QDateTime> date;
        QStringList name;
        QStringList description;
        std::vector<float> price;
    }serviceCommList;

    //State
    enum{DISCONTINUED ,OK};

    service_common(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent = 0);
    ~service_common();

    bool create();
    bool update();
    bool remove();
    int isHere(const QString& name);
    bool loadFromID(const int& id);

    //Appliquer les valeurs
    void setId(const int& id){m_id = id;}
    void setIdCustomer(const int& idCustomer){m_idCustomer = idCustomer;}
    void setPrice(const float& price){m_price = price;}
    void setDate(const QDateTime& theDate){m_theDate = theDate;}
    void setName(const  QString& name){m_name = name;}
    void setDescription(const  QString& description){m_description = description;}

    //recup les valeurs
    int getId(){return m_id;}
    int getIdCustomer(){return m_idCustomer;}
    float getPrice(){return m_price;}
    QDateTime getCreationDate(){return m_creationDate;}
    QDateTime getDate(){return m_theDate;}
    QString getName(){return m_name;}
    QString getDescription(){return m_description;}

    void getServiceCommList(serviceCommList& list, QString order, QString filter, QString field);
};



#endif // service_common_H

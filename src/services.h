#ifndef services_H
#define services_H

#include <QStringList>
#include <vector>
#include <QLocale>
#include <QDateTime>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class service : public QObject {

private:
    QSqlDatabase m_db;

    QWidget *m_parent;
    int m_id, m_idCustomer;
    float m_price, m_tax;
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
    }serviceList;

    //State
    enum{DISCONTINUED ,OK};

    service(QSqlDatabase db, QWidget *parent = 0);
    ~service();

    bool create();
    bool update();
    bool remove();
    bool loadFromID(const int& id);

    //Appliquer les valeurs
    void setId(const int& id){m_id = id;}
    void setIdCustomer(const int& idCustomer){m_idCustomer = idCustomer;}
    void setPrice(const float& price){m_price = price;}
    void setTax(const float& tax){m_tax = tax;}
    void setDate(const QDateTime& theDate){m_theDate = theDate;}
    void setName(const  QString& name){m_name = name;}
    void setDescription(const  QString& description){m_description = description;}

    //recup les valeurs
    int getId(){return m_id;}
    int getIdCustomer(){return m_idCustomer;}
    float getPrice(){return m_price;}
    float getTax(){return m_tax;}
    QDateTime getCreationDate(){return m_creationDate;}
    QDateTime getDate(){return m_theDate;}
    QString getName(){return m_name;}
    QString getDescription(){return m_description;}

    bool getServiceList(serviceList& list, int id_customer, QString order, QString filter, QString field);
};



#endif // services_H

#ifndef PRODUCTCATEGORY_H
#define PRODUCTCATEGORY_H

#include <QWidget>
#include <vector>
#include "bdd/ibpp.h"


class category : public QObject
{

private:
    IBPP::Database m_db;
    IBPP::Transaction m_tr;
    IBPP::Statement m_st;

   QWidget *m_parent;
   int m_id;
   QString  m_name;
   QColor  m_color;

public:
   typedef struct{
       QStringList name;
       std::vector<QColor> color;
   }categoryList;



    category(IBPP::Database db, IBPP::Transaction tr, IBPP::Statement st, QWidget *parent = 0);
    ~category();

    bool create();
    bool update();
    bool remove();
    bool isHere(const QString& name);
    bool loadFromName(const QString& name);
    bool loadFromID(const int& id);

    //Appliquer les valeurs
    void setName(const  QString& Name){m_name = Name;}
    void setColor(const  QColor& Color){m_color = Color;}
    //recup les valeurs
    int getId(){return m_id;}
    QString getName(){return m_name;}
    QColor getColor(){return m_color;}
    void getcategoryList(categoryList& list, QString order, QString filter, QString field);

};

#endif // category_H

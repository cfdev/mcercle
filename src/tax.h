#ifndef TAX_H
#define TAX_H

#include <QObject>
#include <QWidget>
#include <vector>
#include <QStringList>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class tax : public QObject
{
    Q_OBJECT

private:
    QWidget *m_parent;
    int m_id;
    float m_value;
    QString m_description;

public:
    tax(QWidget *parent = 0);

    typedef struct{
        std::vector<int> id;
        std::vector<float> value;
        QStringList description;
    }taxList;

    bool create();
    bool update();
    bool remove();
    bool isHere(const float& val);
    bool loadFromID(const int& id);

    //Appliquer les valeurs
    void setDescription(const  QString& description){m_description = description;}
    void setValue(const  float& val){m_value = val;}
    //recup les valeurs
    int getId(){return m_id;}
    QString getDescription(){return m_description;}
    float getValue(){return m_value;}
    bool getTaxList(taxList& list, QString order, QString filter, QString field);
};

#endif // TAX_H

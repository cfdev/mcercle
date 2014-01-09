#ifndef services_H
#define services_H

#include <QStringList>
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
	qreal m_price, m_tax;
	QDateTime m_creationDate, m_theDate;
	QString m_name, m_description;
	QLocale m_lang;


public:

	typedef struct{
		//Valeur pour le placement dans un tableau.
		QList<int> id;
		QList<QDateTime> creationDate;
		QList<QDateTime> date;
		QStringList name;
		QStringList description;
		QList<qreal> price;
		QList<qreal> tax;
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
	void setPrice(const qreal& price){m_price = price;}
	void setTax(const qreal& tax){m_tax = tax;}
	void setDate(const QDateTime& theDate){m_theDate = theDate;}
	void setName(const  QString& name){m_name = name;}
	void setDescription(const  QString& description){m_description = description;}

	//recup les valeurs
	int getId(){return m_id;}
	int getIdCustomer(){return m_idCustomer;}
	qreal getPrice(){return m_price;}
	qreal getTax(){return m_tax;}
	QDateTime getCreationDate(){return m_creationDate;}
	QDateTime getDate(){return m_theDate;}
	QString getName(){return m_name;}
	QString getDescription(){return m_description;}

	bool getServiceList(serviceList& list, int id_customer, QString order, QString filter, QString field);
};



#endif // services_H

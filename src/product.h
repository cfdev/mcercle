#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QLocale>
#include <QStringList>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "provider.h"
#include "productcategory.h"

class provider;
class category;

class product : public QObject {

private:
	QSqlDatabase m_db;

	QWidget *m_parent;
	int m_id, m_stock, m_stock_warning, m_state, m_idProvider, m_idCategory;
	qreal m_selling_price, m_buying_price, m_tax;
	QDateTime m_creationDate;
	QString m_code, m_name;
	QLocale m_lang;
	QImage m_image;

public:
	//Acces a la class fournisseur
	provider *m_provider;
	//Acces class categorie
	category *m_category;

	typedef struct{
		//Valeur pour le placement dans un tableau
		QList<int> id;
		QStringList code;
		QStringList name;
		QList<qreal> selling_price;
		QList<qreal> tax;
		QStringList category;
		QList<QColor> categoryColor;
		QStringList provider;
		QList<int> stock;
		QList<int> stockWarning;
		QList<int> state;
		QList<QImage> img;
	}ProductList;


	//State
	enum{DISCONTINUED ,OK};

	product(QSqlDatabase db, QLocale &lang, QWidget *parent = 0);
	~product();

	bool create();
	bool update();
	bool remove();
	bool loadFromCode(const QString& code);
	bool loadFromID(const int& id);
	bool isHere(const QString& code);

	//Liste des fournisseurs
	bool providerIsHere(provider* prv);
	bool addProvider(provider* prv, const QString& ref, const qreal& price, const qreal& tax);
	bool updateProvider(provider* prv, const QString& ref, const qreal& price, const qreal& tax);
	bool removeProvider(provider* prv);
	bool removeProviderForAllProducts(provider* prv);

	//Appliquer les valeurs
	void setId(const int& ident){m_id = ident;}
	void setStock(const int& stock){m_stock = stock;}
	void setStockWarning(const int& stock_warning){m_stock_warning = stock_warning;}
	void setSellingPrice(const qreal& price){m_selling_price = price;}
	void setBuyingPrice(const qreal& price){m_buying_price = price;}
	void setTax(const qreal& tax){m_tax = tax;}
	void setState(const int& state){m_state = state;}
	void setCode(const  QString& code){m_code = code;}
	void setName(const  QString& name){m_name = name;}
	void setProviderId(const int& ident){m_idProvider = ident;}
	void setCategoryId(const int& ident){m_idCategory = ident;}
	void setPicture(const QImage& image){m_image = image;}

	//recup les valeurs
	QIcon getIconState(int state);
	QString getTextState(int state);
	int getId(){return m_id;}
	int getStock(){return m_stock;}
	int getStockWarning(){return m_stock_warning;}
	qreal getSellingPrice(){return m_selling_price;}
	qreal getBuyingPrice(){return m_buying_price;}
	qreal getTax(){return m_tax;}
	int getState(){return m_state;}
	QDateTime getCreationDate(){return m_creationDate;}
	QString getCode(){return m_code;}
	QString getName(){return m_name;}
	int getLastId();
	int getProviderID(){return m_idProvider;}
	QImage getPicture(){return m_image;}


	QString getProvider();
	QString getCategory();
	int count(QString filter, QString field, bool showObsoleteProduct);
	bool getProductList(ProductList& list, int first, int skip, QString filter, QString field);
	bool getProductListStockAlert(ProductList& list);
};



#endif // PRODUCT_H

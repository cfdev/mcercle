#ifndef INVOICE_H
#define INVOICE_H

#include <QStringList>
#include <QList>
#include <QLocale>
#include <QDateTime>
#include <QIcon>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
	QSqlDatabase m_db;
	QSqlQuery m_query;

	QWidget *m_parent;
	int m_id, m_idCustomer, m_state ;
	qreal m_price, m_partPayment;
	QDateTime m_creationDate;
	QDate m_userDate, m_limitPayment, m_paymentDate;
	QString m_code, m_proposalCode, m_description, m_typePayment;
	QLocale m_lang;


public:
	//Valeur pour le placement dans un tableau.
	//Facture
	typedef struct{
		QList<int> id;
		QList<QDate> userDate;
		QList<QDate> limitPayment;
		QList<QDate> paymentDate;
		QStringList code;
		QStringList codeProposal; //lien avec la proposition commerciale sil existe
		QStringList description;
		QList<qreal> price;
		QList<int> state;
	}InvoiceList;

	//Structure pour le livre des recettes
	typedef struct{
		QList<int> id;
		QList<QDate> userDate;
		QStringList code;
		QStringList customerFirstName;
		QStringList customerLastName;
		QStringList description;
		QList<qreal> price;
		QStringList typePayment;
	}InvoicesBook;

	typedef struct{
		QList<int> customerId;
		QStringList customerFirstName;
		QStringList customerLastName;
		QList<int> id;
		QList<QDate> userDate;
		QList<QDate> limitPayment;
		QList<QDate> paymentDate;
		QStringList code;
		QStringList codeProposal; //lien avec la proposition commerciale sil existe
		QStringList description;
		QList<qreal> price;
		QList<int> state;
	}InvoiceListAlert;


	//Details de la facture
	typedef struct{
		QList<int> id;
		QList<int> idProduct;
		QStringList name;
		QList<int> discount;
		QList<int> quantity;
		QList<qreal> tax;
		QList<qreal> price;
		QList<int> order;
	}InvoiceListItems;

	//Un Item de la facture
	typedef struct{
		int id;
		int idProduct;
		QString name;
		int discount;
		int quantity;
		qreal tax;
		qreal price;
		int order;
	}InvoiceItem;

	//State
	enum{UNPAID , PAID};
	
	invoice(QSqlDatabase db, QWidget *parent = 0);
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
	void setPrice(const qreal& price){m_price = price;}
	void setPartPayment(const qreal& partPayment){m_partPayment = partPayment;}
	void setTypePayment(const QString& type){m_typePayment = type;}
	void setState(const int& state){m_state = state;}
	void setCode(const  QString& code){m_code = code;}
	void setDescription(const  QString& description){m_description = description;}
	void setUserDate(const QDate& date){m_userDate = date;}
	void setLimitPayment(const QDate& date){m_limitPayment = date;}
	void setPaymentDate(const QDate& date){m_paymentDate = date;}

	//recup les valeurs de la facture
	QIcon getIconState(int state);
	QString getTextState(int state);
	int getId(){return m_id;}
	int getIdCustomer(){return m_idCustomer;}
	qreal getPrice(){return m_price;}
	qreal getPartPayment(){return m_partPayment;}
	QString getTypePayment(){return m_typePayment;}
	int getState(){return m_state;}
	QDateTime getCreationDate(){return m_creationDate;}
	QDate getUserDate(){return m_userDate;}
	QDate getLimitPayment(){return m_limitPayment;}
	QDate getPaymentDate(){return m_paymentDate;}
	QString getCode(){return m_code;}
	QString getProposalCode(){return m_proposalCode;}
	QString getDescription(){return m_description;}

	bool getInvoiceList(InvoiceList& list, int id_customer, QString order, QString filter, QString field);
	bool getInvoices(InvoicesBook& list, QString year, QString month);
	bool getInvoiceListAlert(InvoiceListAlert& list);
	QStringList getYearsList();
	qreal getMonthRevenue(QString year, QString month);
	qreal getMonthServiceRevenue(QString year, QString month);
	qreal getMonthProductRevenue(QString year, QString month);
	qreal getYearRevenue(QString year);


	//recup des articles de la facture
	bool getInvoiceItemsList(InvoiceListItems& list, QString order, QString filter, QString field);

	// Fonction sur un article
	bool getInvoiceItem(InvoiceItem& item);
	bool addInvoiceItem(InvoiceItem& item);
	bool removeInvoiceItem(int Itemid);
	bool updateInvoiceItem(InvoiceItem& item);

	// Parametrage
	int getCaType();

};



#endif // INVOICE_H

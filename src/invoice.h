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

class invoice : public QObject {

private:
	QSqlDatabase m_db;
	QSqlQuery m_query;

	QWidget *m_parent;
	int m_id, m_idCustomer, m_state , m_type, m_idRef;
	qreal m_price, m_priceTax, m_partPayment, m_partPaymentTax;
	QDateTime m_creationDate;
	QDate m_userDate, m_limitPayment, m_paymentDate;
	QString m_code, m_proposalCode, m_description, m_typePayment;
	QLocale m_lang;

	void check_PartPayment();

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
		QList<qreal> price, priceTax;
		QList<qreal> part_payment, part_paymentTax;
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
		QList<qreal> price, priceTax;
		QList<qreal> part_payment, part_paymentTax;
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
		QList<qreal> priceTax;
		QList<qreal> part_payment;
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
		QList<int> type;
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
		int invoice;
		int type;
	}InvoiceItem;
	
	invoice(QSqlDatabase db, QWidget *parent = 0);
	~invoice();

	bool create();
	bool update();
	bool remove();
	bool loadFromID(const int& id);
	bool loadFromCode(const QString& code);
	bool isHere(const QString& code);
	int getLastId();
	int count(int id_customer);
	qreal calcul_price(int id);
	qreal calcul_priceTax(int id);
	qreal calcul_partPayment(int id);
	qreal calcul_partPaymentTax(int id);

	//Appliquer les valeurs
	void setId(const int& ident){m_id = ident;}
	void setIdCustomer(const int& cIdent){m_idCustomer = cIdent;}
	void setPrice(const qreal& price){m_price = price;}
	void setPriceTax(const qreal& priceTax){m_priceTax = priceTax;}
	void setPartPayment(const qreal& partPayment){m_partPayment = partPayment;}
	void setPartPaymentTax(const qreal& partPaymentTax){m_partPaymentTax = partPaymentTax;}
	void setTypePayment(const QString& type){m_typePayment = type;}
	void setState(const int& state){m_state = state;}
	void setCode(const  QString& code){m_code = code;}
	void setDescription(const  QString& description){m_description = description;}
	void setUserDate(const QDate& date){m_userDate = date;}
	void setLimitPayment(const QDate& date){m_limitPayment = date;}
	void setPaymentDate(const QDate& date){m_paymentDate = date;}
	void setType(const int& type){m_type = type;}
	void setIdRef(const int& idref){m_idRef = idref;}

	//recup les valeurs de la facture
	QIcon getIconState(int state);
	QString getTextState(int state);
	int getId(){return m_id;}
	int getIdCustomer(){return m_idCustomer;}
	qreal getPrice(){return m_price;}
	qreal getPriceTax(){return m_priceTax;}
	qreal getPartPayment(){return m_partPayment;}
	qreal getPartPaymentTax(){return m_partPaymentTax;}
	QString getTypePayment(){return m_typePayment;}
	int getState(){return m_state;}
	QDateTime getCreationDate(){return m_creationDate;}
	QDate getUserDate(){return m_userDate;}
	QDate getLimitPayment(){return m_limitPayment;}
	QDate getPaymentDate(){return m_paymentDate;}
	QString getCode(){return m_code;}
	QString getProposalCode(){return m_proposalCode;}
	QString getDescription(){return m_description;}
	int getType(){return m_type;}
	int getIdRef(){return m_idRef;}

	bool getInvoiceList(InvoiceList& list, int id_customer, QString order, QString filter, QString field);
	bool getInvoices(InvoicesBook& list, QString year, QString month);
	bool getInvoiceListAlert(InvoiceListAlert& list);
	QStringList getYearsList();
	qreal getMonthRevenue(QString year, QString month);
	qreal getMonthServiceRevenue(QString year, QString month);
	qreal getMonthProductRevenue(QString year, QString month);
	qreal getYearRevenue(QString year);
	qreal getYearRevenueTAX(QString year);
	bool isTypeExiste(const int& type);
	QString getCodeInvoice_Ref();
	QString generateNewCode();

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

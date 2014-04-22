#ifndef PROPOSAL_H
#define PROPOSAL_H

#include <QStringList>
#include <QList>
#include <QLocale>
#include <QDateTime>
#include <QIcon>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class proposal : public QObject {

private:
	QSqlDatabase m_db;

	QWidget *m_parent;
	int m_id, m_idCustomer, m_state, m_delayDeliveryDate;
	qreal m_price;
	QDateTime m_creationDate;
	QDate m_userDate, m_deliveryDate, m_validDate;
	QString m_code, m_InvoiceCode, m_description, m_typePayment;
	QLocale m_lang;


public:
	//Valeur pour le placement dans un tableau.
	//Proposition commercial /devis
	typedef struct{
		QList<int> id;
		QList<QDate> userDate;
		QList<QDate> deliveryDate;
		QList<int> delayDeliveryDate;
		QList<QDate> validDate;
		QStringList code;
		QStringList codeInvoice; //Facture associee
		QStringList description;
		QList<qreal> price;
		QList<int> state;
	}ProposalList;

	typedef struct{
		QList<int> customerId;
		QStringList customerFirstName;
		QStringList customerLastName;
		QList<int> id;
		QList<QDate> userDate;
		QStringList code;
		QStringList description;
		QList<qreal> price;
	}ProposalListAlert;

	//Details de la proposition
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
	}ProposalListItems;

	//Un Item de la proposition
	typedef struct{
		int id;
		int idProduct;
		QString name;
		int discount;
		int quantity;
		qreal tax;
		qreal price;
		int order;
		int type;
	}ProposalItem;

	proposal(QSqlDatabase db, QWidget *parent = 0);
	~proposal();

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
	void setTypePayment(const QString& type){m_typePayment = type;}
	void setState(const int& state){m_state = state;}
	void setCode(const  QString& code){m_code = code;}
	void setDescription(const  QString& description){m_description = description;}
	void setUserDate(const QDate& date){m_userDate = date;}
	void setDeliveryDate(const QDate& date){m_deliveryDate = date;}
	void setDelayDeliveryDate(const int& days){m_delayDeliveryDate = days;}
	void setValidDate(const QDate& date){m_validDate = date;}

	//recup les valeurs de la facture
	QIcon getIconState(int state);
	QString getTextState(int state);
	QString getCode(){return m_code;}
	QString getInvoiceCode(){return m_InvoiceCode;}
	QString getDescription(){return m_description;}
	QString getTypePayment(){return m_typePayment;}
	int getId(){return m_id;}
	int getIdCustomer(){return m_idCustomer;}
	int getState(){return m_state;}
	qreal getPrice(){return m_price;}
	QDateTime getCreationDate(){return m_creationDate;}
	QDate getUserDate(){return m_userDate;}
	QDate getDeliveryDate(){return m_deliveryDate;}
	int getDelayDeliveryDate(){return m_delayDeliveryDate;}
	QDate getValidDate(){return m_validDate;}


	bool getProposalList(ProposalList& list, int id_customer, QString order, QString filter, QString field);
	bool getProposalListAlert(ProposalListAlert& list);

	//Fonction pour creer le lien entre proposition commerciale et facture
	bool setLink( const int& idProposal, const int& idInvoice );

	//recup des articles de la facture
	bool getProposalItemsList(ProposalListItems& list, QString order, QString filter, QString field);

	// Fonction sur un article
	bool getProposalItem(ProposalItem& item);
	bool addProposalItem(ProposalItem& item);
	bool removeProposalItem(int Itemid);
	bool updateProposalItem(ProposalItem& item);
};



#endif // PROPOSAL_H

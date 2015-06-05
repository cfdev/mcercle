#ifndef INOUT_H
#define INOUT_H

#include <QObject>
#include <QFile>
#include "dbase.h"
#include "customer.h"
#include "product.h"

class inout : public QObject
{
	Q_OBJECT
	private:
		QWidget *m_parent;
		database *m_data;
		customer *m_customer;
		product *m_product;
		void exportData(QString query, QString typeOfExport);

		void importData(int typeOfExport, QString Name);
		QString importCustomer(const QString &line, const QStringList &vals);
		QString importProduct(const QString &line, const QStringList &vals);

	public:
		explicit inout(database *pdata, QWidget *parent = 0);
		~inout();

		void importAllCustomers();
		void importAllProducts();

		void exportAllCustomers();
		void exportAllProducts();
		void exportAllServices();
		void exportAllProviders();

	signals:
	
	public slots:
};

#endif // INOUT_H

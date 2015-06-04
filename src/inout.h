#ifndef INOUT_H
#define INOUT_H

#include <QObject>
#include <QFile>
#include "dbase.h"
#include "customer.h"

class inout : public QObject
{
	Q_OBJECT
	private:
		QWidget *m_parent;
		database *m_data;
		customer *m_customer;
		void exportData(QString query, QString typeOfExport);

	public:
		explicit inout(database *pdata, QWidget *parent = 0);
		~inout();

		void importCustomers();

		void exportAllCustomers();
		void exportAllProducts();
		void exportAllServices();
		void exportAllProviders();

	signals:
	
	public slots:
};

#endif // INOUT_H

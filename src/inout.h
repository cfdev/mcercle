#ifndef INOUT_H
#define INOUT_H

#include <QObject>
#include "dbase.h"

class inout : public QObject
{
	Q_OBJECT
	private:
		QWidget *m_parent;
		database *m_data;
		//Exporter les donnees
		void exportData(QString query, QString typeOfExport);

	public:
		explicit inout(database *pdata, QWidget *parent = 0);
		~inout();

		void exportAllCustomers();
		void exportAllProducts();
		void exportAllServices();
		void exportAllProviders();

	signals:
	
	public slots:
};

#endif // INOUT_H

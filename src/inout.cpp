#include "inout.h"

#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

/**
 * @brief Class pour l'import export
 * @param db
 * @param parent
 */
inout::inout(database *db, QWidget *parent) : m_parent(parent) {
	m_data = db;
}

inout::~inout() {
	
}

/**
 * @brief inout::exportData
 * @param query string contain sql
 * @param typeOfExport string name of table
 */
void inout::exportData(QString query, QString typeOfExport){
	QString fileName = QFileDialog::getSaveFileName(0, tr("Exporter '")+typeOfExport+tr("' vers ..."),
													QDir::homePath(),
													tr("CSV File (*.csv)"));
	if(fileName.isEmpty())
		return;
	QFile::remove(fileName);
	QSqlQuery querExport(m_data->getDb());
	querExport.prepare(query);

	if(!querExport.exec()) {
		qDebug()<<"Query error = " << querExport.lastError();
		QMessageBox::warning(0,"Erreur exportation",querExport.lastError().text());
	}
	else {
		QString toSave;
		while(querExport.next()) {
			QApplication::processEvents();
			const QSqlRecord record = querExport.record();
			QStringList recordexp;
			if(toSave.isEmpty()) {
				for(int p = 0; p < record.count() ; p++)
				recordexp<<record.fieldName(p);
				toSave+=recordexp.join(";");
				toSave+="\n";
			}
			recordexp.clear();
			for(int p = 0; p < record.count() ; p++) {
				recordexp<<record.value(p).toString();
			}
			toSave+=recordexp.join(";");
			toSave+="\n";
		}
	if(!toSave.isEmpty()) {
		if(!fileName.contains(".csv")) {
			fileName+=".csv";
		}
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
			QMessageBox::warning(0,QString("Erreur exportation"),QString("Impossible d'ouvrir le fichier:\n")+QString(file.errorString()));
		}
		else {
			QTextStream out(&file);
			out <<toSave;
		}
		QMessageBox::information(0,"Exportation","Exportation réussie");
	}
	else
		QMessageBox::information(0,"Exportation","Rien à exporter");
	}
	querExport.finish();
}

/**
 * @brief export tous les clients
 */
void inout::exportAllCustomers() {
	QString req = "SELECT * FROM TAB_CUSTOMERS";
	exportData(req,"Clients");
}

/**
 * @brief export tous les produits
 */
void inout::exportAllProducts() {
	QString req = "SELECT TAB_PRODUCTS.ID, TAB_PRODUCTS.CODE, TAB_PRODUCTS.NAME, TAB_PRODUCTS.SELLING_PRICE, TAB_PRODUCTS.TAX, TAB_PRODUCTS_CATEGORIES.NAME AS CATEGORY, TAB_PRODUCTS_CATEGORIES.COLOR AS CATEGORYCOLOR, TAB_PROVIDERS.NAME AS PROVIDER, TAB_PRODUCTS.STOCK, TAB_PRODUCTS.STOCK_WARNING, TAB_PRODUCTS.STATE"
	" FROM TAB_PRODUCTS"
	" LEFT OUTER JOIN TAB_PRODUCTS_CATEGORIES"
	" ON TAB_PRODUCTS.ID_CATEGORY = TAB_PRODUCTS_CATEGORIES.ID"
	" LEFT OUTER JOIN TAB_PROVIDERS"
	" ON TAB_PRODUCTS.ID_PROVIDER = TAB_PROVIDERS.ID";
	exportData(req,"Produits");
}

/**
 * @brief export tous les services
 */
void inout::exportAllServices() {
	QString req = "SELECT *"
	" FROM TAB_SERVICES";
	exportData(req,"Services");
}

/**
 * @brief export tous les fournisseurs
 */
void inout::exportAllProviders() {
	QString req = "SELECT * FROM TAB_PROVIDERS";
	exportData(req,"Fournisseurs");
}


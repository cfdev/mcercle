#include "inout.h"
#include "dialogwaiting.h"
#include "mcercle.h"

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
	m_customer = m_data->m_customer;
	m_product = m_data->m_product;
}

inout::~inout() {
	
}

/**
 * @brief Import All Customers
 */
void inout::importAllCustomers() {
	importData(MCERCLE::TYPE_CUSTOMER, tr("Clients"));
}
/**
 * @brief Import All Products
 */
void inout::importAllProducts() {
	importData(MCERCLE::TYPE_PRODUCT, tr("Produits"));
}



/**
 * @brief import the customers
 */
QString inout::importCustomer(const QString &line, const QStringList &vals) {
	QString log="";
	//Test if empty
	if(  ((vals.value(2).toInt() == 1) && !vals.value(4).isEmpty()) ||
		 ((vals.value(2).toInt() == 0) && !vals.value(4).isEmpty() && !vals.value(5).isEmpty())
	){

		// to fill the class
		m_customer->setType( vals.value(2).toInt() );
		m_customer->setDesCompagny( vals.value(3) );
		m_customer->setName( vals.value(5), vals.value(4) );
		m_customer->setGender( vals.value(6) );
		m_customer->setBirthday( QDate::fromString(vals.value(7)) );
		m_customer->setAddress(vals.value(8),
							   vals.value(9),
							   vals.value(10));

		m_customer->setZipCode( vals.value(11) );
		m_customer->setCity( vals.value(12) );
		m_customer->setCountry( vals.value(13) );

		m_customer->setPhone(vals.value(14), vals.value(15));
		m_customer->setEmail( vals.value(16) );
		m_customer->setProfession( vals.value(17) );
		m_customer->setComments( vals.value(18) );
		m_customer->setSendingPubEmail( vals.value(19).toInt() );

		// Add default line to the log
		if(!m_customer->create()) {
			log += line;
			log += "\n\n";
		}
	}
	else{
		// Add default line to the log
		log += line;
		log += "\n\n";
	}

	return log;
}



/**
 * @brief import the product
 */
QString inout::importProduct(const QString &line, const QStringList &vals) {
	QString log="";
	//Test if empty
	if( (!vals.value(1).isEmpty()) && (!vals.value(2).isEmpty()) ) {

		// to fill the class
		m_product->setCode( vals.value(1) );
		m_product->setName( vals.value(2) );
		m_product->setSellingPrice( vals.value(3).toDouble() );
		m_product->setBuyingPrice( vals.value(4).toDouble() );
		m_product->setTax( vals.value(5).toDouble() );

		//Check if category exist
		if(!m_product->m_category->isHere(vals.value(6)) && !vals.value(6).isEmpty()){
			m_product->m_category->setName(vals.value(6));
			m_product->m_category->setColor(QColor(vals.value(7)));
			m_product->m_category->create();
		}
		m_product->m_category->loadFromName(vals.value(6));
		m_product->setCategoryId(m_product->m_category->getId());

		//Check if supplier exist
		if(!m_product->m_provider->isHere(vals.value(8)) && !vals.value(8).isEmpty()){
			m_product->m_provider->setName(vals.value(8));
			m_product->m_provider->create();
		}
		m_product->m_provider->loadFromName(vals.value(8));
		m_product->setProviderId(m_product->m_provider->getId());

		m_product->setStock( vals.value(9).toInt() );
		m_product->setStockWarning( vals.value(10).toInt() );
		m_product->setState( vals.value(11).toInt() );


		// check is productalready exist
		// Add default line to the log
		if(!m_product->isHere(m_product->getCode())) {
			if(!m_product->create()) {
				log += line;
				log += "\n\n";
			}
		}
		else{
			log += tr("Produit déja présent...: ");
			log += line;
			log += "\n\n";
		}
	}
	else{
		// Add default line to the log
		log += line;
		log += "\n\n";
	}

	return log;
}


/**
 * @brief Import the data
 * @param type of export (customer, product ...)
 * @param String of type name
 */
void inout::importData(int typeOfExport, QString Name) {
	//Warning
	QString st = tr("Faite une sauvegarde de la base de données avant l'import!");
	st += "\n\n" + tr("INFO: Pour voir la structure du fichier cvs, réaliser un export avant.");
	QMessageBox::warning(m_parent,tr("Attention"), st);

	QString fileName = QFileDialog::getOpenFileName(0, tr("Importer des ")+Name,
													QDir::homePath(),
													tr("CSV File (*.csv)"));
	if(fileName.isEmpty())
		return;
	// Open file
	QFile cvsFile(fileName);
	qDebug() << "inout::importCustomers";
	QString log="",mess="";
	if(cvsFile.open(QIODevice::ReadOnly)) {
		//Affichage de la fenetre d attente
		DialogWaiting* m_DialogWaiting = new DialogWaiting();
		m_DialogWaiting->setTitle("<b>"+tr("Importation des ")+Name+"</b>");
		m_DialogWaiting->setDetail("<i>"+tr("En cours... ")+fileName+"</i>");
		m_DialogWaiting->setProgressBarRange(0, 0);
		m_DialogWaiting->setProgressBar(0);
		m_DialogWaiting->setModal(true);
		m_DialogWaiting->show();

		QString line;
		QStringList vals;
		//Read line by line
		line = cvsFile.readLine();
		//first line into trash read second line
		while (!cvsFile.atEnd()) {
			line = cvsFile.readLine();
			// Process line
			vals = line.split(";");
			if(vals.count()<=1) vals = line.split(",");
			if(vals.count()>1) {
				qDebug() << vals;
				//Test of type
				switch(typeOfExport){
					case MCERCLE::TYPE_CUSTOMER:	log += importCustomer(line, vals);
					case MCERCLE::TYPE_PRODUCT:		log += importProduct(line, vals);
				}
				m_DialogWaiting->refresh();
			}
		}
		delete m_DialogWaiting;
	}
	else {
		QMessageBox::warning(m_parent,QString("Erreur d'ouverture"),QString("Impossible d'ouvrir le fichier:\n")+QString(cvsFile.errorString()));
	}

	QMessageBox mBox(QMessageBox::Information, tr("Information"), tr("Rapport d'importation"), QMessageBox::Ok);
	if(!log.isEmpty()) {
		mess = tr("L'import contient des erreurs :-(\nAfficher les détails pour voir ce qui ne va pas.");
		mBox.setIcon( QMessageBox::Critical );
		mBox.setDetailedText(log);
	}
	else{
		mess = tr("L'import a réussi !");
		mBox.setIcon( QMessageBox::Information );

	}
	mBox.setText( mess );
	mBox.setDetailedText(log);
	mBox.exec();
	cvsFile.close();
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
	QString req = "SELECT TAB_PRODUCTS.ID, TAB_PRODUCTS.CODE, TAB_PRODUCTS.NAME, TAB_PRODUCTS.SELLING_PRICE, TAB_PRODUCTS.BUYING_PRICE, TAB_PRODUCTS.TAX, TAB_PRODUCTS_CATEGORIES.NAME AS CATEGORY, TAB_PRODUCTS_CATEGORIES.COLOR AS CATEGORYCOLOR, TAB_PROVIDERS.NAME AS PROVIDER, TAB_PRODUCTS.STOCK, TAB_PRODUCTS.STOCK_WARNING, TAB_PRODUCTS.STATE"
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
	QString req = "SELECT * FROM TAB_SERVICES_COMMONS";
	exportData(req,"Services");
}

/**
 * @brief export tous les fournisseurs
 */
void inout::exportAllProviders() {
	QString req = "SELECT * FROM TAB_PROVIDERS";
	exportData(req,"Fournisseurs");
}


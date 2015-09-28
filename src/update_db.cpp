#include "update_db.h"
#include "dialogwaiting.h"
#include "settings.h"

update_db::update_db(database *pdata, QObject *parent) : QObject(parent) {
	m_data = pdata;
}

/**
   Met a jour la base de donnees en version 2
  */
bool update_db::upgradeToV2(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 2:");

	req =	"ALTER TABLE TAB_PROPOSALS_DETAILS ADD ITEM_ORDER INTEGER;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req =	"ALTER TABLE TAB_INVOICES_DETAILS ADD ITEM_ORDER INTEGER;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=2;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

/**
   Met a jour la base de donnees en version 3
  */
bool update_db::upgradeToV3(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 3:");

	req =	"ALTER TABLE TAB_INVOICES ADD PAYMENTDATE;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// Pour les factures existante on prend la date de creation comme date de reglement
	req =	"UPDATE TAB_INVOICES SET PAYMENTDATE = DATE;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req =	"ALTER TABLE TAB_INFORMATIONS ADD CA_TYPE INT DEFAULT 0;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=3;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

/**
   Met a jour la base de donnees en version 4
  */
bool update_db::upgradeToV4(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 4:");

	// ajout dune colonne
	req =	"ALTER TABLE TAB_INFORMATIONS ADD NUM_TAX VARCHAR(64);";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Creation dune table temporaire DEVIS
	req =	"ALTER TABLE 'TAB_PROPOSALS_DETAILS' RENAME TO 'TAB_PROPOSALS_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//creation de la table modifiee
	req = "CREATE TABLE IF NOT EXISTS TAB_PROPOSALS_DETAILS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_PROPOSAL    INTEGER NOT NULL,"
			"ID_PRODUCT     INTEGER,"
			"NAME           TEXT NOT NULL,"
			"DISCOUNT       INTEGER NOT NULL,"
			"QUANTITY       INTEGER NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"ITEM_ORDER     INTEGER,"
			"FOREIGN KEY(ID_PROPOSAL) REFERENCES TAB_PROPOSALS(ID) ON DELETE CASCADE"
			"); ";
	if(m_data->getDb().driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Copie des valeurs dans la nouvelle table !
	req =  "INSERT INTO 'TAB_PROPOSALS_DETAILS' SELECT * FROM 'TAB_PROPOSALS_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Suppression de la table temporaire
	req =  "DROP TABLE 'TAB_PROPOSALS_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// ajout dune colonne
	req =	"ALTER TABLE TAB_PROPOSALS_DETAILS ADD TYPE INTEGER;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// Modification du type d item
	req =	"UPDATE TAB_PROPOSALS_DETAILS SET TYPE=1 WHERE ID_PRODUCT>0;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Creation dune table temporaire FACTURE
	req =	"ALTER TABLE 'TAB_INVOICES_DETAILS' RENAME TO 'TAB_INVOICES_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req = "CREATE TABLE IF NOT EXISTS TAB_INVOICES_DETAILS ("
			"ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ID_INVOICE     INTEGER NOT NULL,"
			"ID_PRODUCT     INTEGER,"
			"NAME           TEXT NOT NULL,"
			"DISCOUNT       INTEGER NOT NULL,"
			"QUANTITY       INTEGER NOT NULL,"
			"TAX            NUMERIC(5,2),"
			"PRICE          NUMERIC(8,2) NOT NULL,"
			"ITEM_ORDER     INTEGER,"
			"FOREIGN KEY(ID_INVOICE) REFERENCES TAB_INVOICES(ID) ON DELETE CASCADE"
			");";
	if(m_data->getDb().driverName() == "QMYSQL")
		req.replace("AUTOINCREMENT","AUTO_INCREMENT");
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Copie des valeurs dans la nouvelle table !
	req =  "INSERT INTO 'TAB_INVOICES_DETAILS' SELECT * FROM 'TAB_INVOICES_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Suppression de la table temporaire
	req =  "DROP TABLE 'TAB_INVOICES_DETAILS_TEMP';";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// Ajout dune colonne
	req =	"ALTER TABLE TAB_INVOICES_DETAILS ADD TYPE INTEGER;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// Modification du type d item
	req =	"UPDATE TAB_INVOICES_DETAILS SET TYPE=1 WHERE ID_PRODUCT>0;"; // ET UPDATE TAB_INVOICES_DETAILS SET TYPE=0 WHERE TYPE IS NULL;
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout champ pour impression dans bdd
	req =	"ALTER TABLE TAB_INFORMATIONS ADD PRINT_LINE1 VARCHAR(64)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	req =	"ALTER TABLE TAB_INFORMATIONS ADD PRINT_LINE2 VARCHAR(64)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Update numero version bdd
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=4;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

/**
   Met a jour la base de donnees en version 5
  */
bool update_db::upgradeToV5(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 5:");

	// Modification du type d item
	req =	"UPDATE TAB_INVOICES_DETAILS SET TYPE=1 WHERE ID_PRODUCT>0;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// Modification du type d item
	req =	"UPDATE TAB_INVOICES_DETAILS SET TYPE=0 WHERE TYPE IS NULL;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Update numero version bdd
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=5;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

/**
   Met a jour la base de donnees en version 6
  */
bool update_db::upgradeToV6(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 6:");

	//Ajout champ pour impression dans bdd
	req =	"ALTER TABLE TAB_INFORMATIONS ADD BORDER_RADIUS INTERGER";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout champ pour impression dans bdd
	req =	"ALTER TABLE TAB_INFORMATIONS ADD MANAGE_STOCK INTERGER";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Update numero version bdd
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=6;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

/**
   Met a jour la base de donnees en version 6
  */
bool update_db::upgradeToV7(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise à jour de la base de données en version 7:");

	//Affichage de la fenetre d attente
	char bar=0;
	DialogWaiting* m_DialogWaiting = new DialogWaiting();
	m_DialogWaiting->setTitle( tr("Mise à jour de la base de données en version 7:") );
	m_DialogWaiting->setProgressBarRange(0,2);
	m_DialogWaiting->setModal(true);
	m_DialogWaiting->show();

	//Ajoute une colonne pour limpression
	req =	"ALTER TABLE TAB_INFORMATIONS ADD PRINT_LINE3 VARCHAR(256)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajoute une colonne pour limpression
	req =	"ALTER TABLE TAB_INFORMATIONS ADD DRAW_LINE INTEGER";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout une colonne prix ttc dans la table devis
	req =	"ALTER TABLE TAB_PROPOSALS ADD PRICE_TAX NUMERIC(8.2)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout une colonne acompte ttc dans la table facture
	req =	"ALTER TABLE TAB_INVOICES ADD PART_PAYMENT_TAX NUMERIC(8.2)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout une colonne prix ttc dans la table facture
	req =	"ALTER TABLE TAB_INVOICES ADD PRICE_TAX NUMERIC(8.2)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout une colonne type de facture
	req =	"ALTER TABLE TAB_INVOICES ADD TYPE INTERGER";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajout une colonne reference ID
	req =	"ALTER TABLE TAB_INVOICES ADD ID_REFERENCE INTERGER";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Update le type de facture si on trouve un titre avec acompte
	m_DialogWaiting->setProgressBar(bar++);
	req =	"UPDATE tab_invoices SET type=1 WHERE (tab_invoices.description LIKE '%acompt%') or (tab_invoices.description LIKE '%accompt%');";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Mise a jour des prix de la table proposals
	m_DialogWaiting->setDetail( tr("Mise à jour des prix de la table des devis") );
	m_DialogWaiting->setProgressBar(bar++);
	int iLastId = m_data -> m_customer -> m_proposal-> getLastId();
	qreal price, priceTTc, partpayment;
	for(int id=1; id<=iLastId; id++) {
		if( m_data -> m_customer -> m_proposal->loadFromID(id) ) {
			//recupere les prix dun devis
			price = m_data->m_customer -> m_proposal -> calcul_price(id);
			priceTTc = m_data->m_customer -> m_proposal -> calcul_priceTax(id);
			//modifi dans le devis
			m_data->m_customer -> m_proposal -> setPrice(price);
			m_data->m_customer -> m_proposal -> setPriceTax( priceTTc );
			m_data->m_customer -> m_proposal -> update();
		}
	}

	//Mise a jour des prix de la table invoice
	m_DialogWaiting->setDetail( tr("Mise à jour des prix de la table des factures") );
	m_DialogWaiting->setProgressBar(bar++);
	iLastId = m_data -> m_customer -> m_invoice-> getLastId();
	price = priceTTc = 0;
	for(int id=1; id<=iLastId; id++) {
		if( m_data -> m_customer -> m_invoice->loadFromID(id) ) {
			//recupere les prix dune facture
			price = m_data -> m_customer -> m_invoice -> calcul_price(id);
			priceTTc = m_data->  m_customer -> m_invoice -> calcul_priceTax(id);
			//modifi dans la facture
			m_data->m_customer -> m_invoice -> setPrice(price);
			m_data->m_customer -> m_invoice -> setPriceTax( priceTTc );
			//recupere acompte
			partpayment = m_data->m_customer -> m_invoice ->getPartPayment();
			m_data->m_customer -> m_invoice -> setPartPaymentTax(partpayment);
			//On met a jour
			m_data->m_customer -> m_invoice -> update();
		}
	}

	//Update numero version bdd
	m_DialogWaiting->setProgressBar(bar++);
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=7;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";
	return done;
}

/**
   Met a jour la base de donnees en version 8
  */
bool update_db::upgradeToV8(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise à jour de la base de données en version 8:");

	//Ajoute une colonne pour limpression
	req =	"ALTER TABLE TAB_INFORMATIONS ADD PRINT_LINE4 VARCHAR(256)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Ajoute une colonne pour la devise
	req =	"ALTER TABLE TAB_INFORMATIONS ADD CURRENCY VARCHAR(3)";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	//Update numero version bdd
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=8;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	// update repository
	// https://raw.githubusercontent.com/cfdev/mcercle/master/version.txt
	Settings mset;
	mset.setUrl("https://raw.githubusercontent.com/cfdev/mcercle/master/version.txt");

	return done;
}


/**
   Met a jour la base de donnees en version 9
  */
bool update_db::upgradeToV9(QString *log) {
	QString req;
	bool done=true;
	QSqlQuery query;
	*log = tr("Mise a jour de la base de données en version 9:");

	//Ajout champ pour impression dans bdd
	req =	"ALTER TABLE TAB_BANK ADD NAME VARCHAR(24);";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";


	//Update numero version bdd
	req =	"UPDATE TAB_INFORMATIONS SET DBASE_VERSION=9;";
	*log += "\n\n"+ req;
	query.prepare( req );
	if(!query.exec()) {
		*log += "\n->" + query.lastError().text();
		done = false;
	}
	else
		*log += "\n-> FAIT";

	return done;
}

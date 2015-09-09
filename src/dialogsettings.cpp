/**
  This file is a part of mcercle
  Copyright (C) 2010-2013 Cyril FRAUSTI

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "mcercle.h"

#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif
#include <qdebug.h>

DialogSettings::DialogSettings(Settings *s, database *pdata, QLocale &lang, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogSettings)
{
	ui->setupUi(this);
	setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	m_Settings = s;
	m_data = pdata;
	m_lang = lang;
	m_DialogTax = NULL;

	//charge les settings
	loadInfoSettings();
	//Active ou desactive selon letat de la connexion
	setDbaseEditState(!m_data->isConnected());
	//charge les info de la base de donnees
	loadInfoDatabase();
	//Selectionne la tab 0
	ui->tabWidget->setCurrentIndex(0);
}

void DialogSettings::setDbaseEditState( bool state) {
	ui->comboBox_dbase->setEnabled(state);
	ui->lineEdit_hostName->setEnabled(state);
	ui->lineEdit_port->setEnabled(state);
	ui->lineEdit_databaseName->setEnabled(state);
	ui->lineEdit_login->setEnabled(state);
	ui->lineEdit_password->setEnabled(state);
	ui->toolButton_BaseSelect->setEnabled(state);
}

DialogSettings::~DialogSettings()
{
	delete ui;
}

/**
	Sauvegarde des valeurs
  */
void DialogSettings::on_buttonBox_accepted() {
	//sauvegarde les donnees de la societe dans la bdd
	database::Informations inf;
	inf.name = ui->lineEdit_sName->text();
	inf.num = ui->lineEdit_sNum->text();
	inf.numTax = ui->lineEdit_numTVA->text();
	inf.capital = ui->lineEdit_sCapital->text();
	inf.phoneNumber = ui->lineEdit_sPhone->text();
	inf.faxNumber = ui->lineEdit_sFax->text();
	inf.email = ui->lineEdit_sEmail->text();
	inf.webSite = ui->lineEdit_sWebSite->text();
	inf.zipCode = ui->lineEdit_sZipCode->text();
	inf.city = ui->lineEdit_sCity->text();
	inf.address1 = ui->lineEdit_sAdd1->text();
	inf.address2 = ui->lineEdit_sAdd2->text();
	inf.address3 = ui->lineEdit_sAdd3->text();
	inf.tax = ui->checkBox_TAX->checkState();
	inf.ca_type = ui->comboBox_CA->currentIndex();
	inf.line1 = ui->lineEdit_print1->text();
	inf.line2 = ui->lineEdit_print2->text();
	inf.line3 = ui->lineEdit_print3->text();
	inf.line4 = ui->lineEdit_print4->text();
	inf.borderRadius = ui->checkBox_border_radius->checkState();
	inf.drawLine = ui->checkBox_drawLine->checkState();
	inf.manageStock = ui->checkBox_manageStock->checkState();
	inf.currency = ui->lineEdit_currency->text();
	m_data -> updateInfo(inf);

	//sauvegarde les donnees de la banque dans la bdd
	database::Bank b;
	b.codeBanque = ui->lineEdit_codeBanque->text();
	b.codeGuichet = ui->lineEdit_codeGuichet->text();
	b.numCompte = ui->lineEdit_numCompte->text();
	b.keyRIB = ui->lineEdit_keyRIB->text();
	b.address = ui->lineEdit_Address->text();
	b.IBAN1 = ui->lineEdit_IBAN1->text();
	b.IBAN2 = ui->lineEdit_IBAN2->text();
	b.IBAN3 = ui->lineEdit_IBAN3->text();
	b.IBAN4 = ui->lineEdit_IBAN4->text();
	b.IBAN5 = ui->lineEdit_IBAN5->text();
	b.IBAN6 = ui->lineEdit_IBAN6->text();
	b.IBAN7 = ui->lineEdit_IBAN7->text();
	b.IBAN8 = ui->lineEdit_IBAN8->text();
	b.IBAN9 = ui->lineEdit_IBAN9->text();
	b.codeBIC = ui->lineEdit_BIC->text();
	m_data -> updateBank(b);

	//settings
	m_Settings -> setDatabase_bdd( ui->comboBox_dbase->currentText() );
	m_Settings -> setDatabase_hostName( ui->lineEdit_hostName->text() );
	m_Settings -> setDatabase_port( ui->lineEdit_port->text().toInt() );
	m_Settings -> setDatabase_databaseName( ui->lineEdit_databaseName->text() );
	m_Settings -> setDatabase_userName( ui->lineEdit_login->text());
	m_Settings -> setDatabase_userPassword( ui->lineEdit_password->text() );
	m_Settings -> setPrintFont( ui->comboBox_printFont->currentText() );
	m_Settings -> setCheckVersion( ui->checkBox_checkVersion->checkState() );
	m_Settings -> setTheme ( ui->comboBox_theme->currentText() );
	m_Settings -> setUrl( ui->lineEdit_url->text() );
	m_Settings -> setSettingState(true);
}

/**
	Ouverture d'un dialog pour le chargement d'un image
  */
void DialogSettings::on_pushButton_Logo_clicked() {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QString pathPictures = QDesktopServices::storageLocation ( QDesktopServices::PicturesLocation );
#else
	QString pathPictures = QStandardPaths::writableLocation( QStandardPaths::PicturesLocation );
#endif
	QString fileName = QFileDialog::getOpenFileName(this, tr("Selectionner une image..."), pathPictures.toStdString().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
	if(fileName.isEmpty())return;

	//verifie la taille
	QImage logo;
	int ret = logo.load(fileName);
	if(!ret){
		QMessageBox::critical(this, tr("Erreur"), tr("Impossible de charger l'image...Désoler :("));
		return;
	}
	if((logo.height()>1200) ||(logo.width()>1200)) {
		logo = logo.scaled(1200,1200, Qt::KeepAspectRatio);
	}
	//ajout a la base de donnees
	m_data->updateLogoTable_informations(logo);

	//relecture de limage et affichage du logo
	QImage lo = m_data->getLogoTable_informations();
	lo = lo.scaled(128,128, Qt::KeepAspectRatio);
	ui->label_logo->setPixmap(QPixmap::fromImage(lo));
}

/**
	Efface l'image du logo
  */
void DialogSettings::on_pushButton_ClearImage_clicked() {
	QImage logo;
	//ajout a la base de donnees
	m_data -> updateLogoTable_informations(logo);
	//relecture de limage et affichage du logo
	QImage lo = m_data -> getLogoTable_informations();
	ui->label_logo->setPixmap(QPixmap::fromImage(lo));
}

/**
	Selectionnez la base de donnees
  */
void DialogSettings::on_toolButton_BaseSelect_clicked() {
	//charge le chemin s il existe
	QString pathFile = ui->lineEdit_databaseName->text();
	if(!pathFile.isEmpty()) {
		QFileInfo file( pathFile );
		pathFile = file.dir().absolutePath()+ '/';
	}
	// ouvre le dialogue du choix
	QString filename = QFileDialog::getOpenFileName(this, "Selectionnez un fichier *.db",  pathFile.toStdString().c_str(), "*.db");
	if( !filename.isEmpty() ) {
		ui->lineEdit_databaseName->setText(filename);
	}
}

/**
	Gerer la connexion avec la base de donnees
  */
void DialogSettings::on_pushButton_connect_clicked() {
	if(!m_data->isConnected()){
		m_data->setBdd( ui->comboBox_dbase->currentText() );
		m_data->setHostName( ui->lineEdit_hostName->text() );
		m_data->setPort( ui->lineEdit_port->text().toInt() );
		m_data->setDatabaseName( ui->lineEdit_databaseName->text() );
		m_data->setUserName( ui->lineEdit_login->text() );
		m_data->setPassword( ui->lineEdit_password->text() );

		if(m_data->connect() == database::DB_NOTEXIST_ERR){
			// Demande si on creer une nouvelle base de donnees
			QMessageBox mBox(QMessageBox::Question, tr("Question"), tr("Voulez-vous créer une nouvelle base de données ?"),QMessageBox::Yes | QMessageBox::No);
			mBox.setDefaultButton(QMessageBox::No);
			int ret = mBox.exec();
			if(ret == QMessageBox::Yes) m_data -> create();
		}
	}
	else m_data -> close();

	//Active ou desactive selon letat de la connexion
	setDbaseEditState(!m_data->isConnected());

	//charge les info de la base de donnees
	loadInfoDatabase();

	//Demande le rafraichissement des listes
	emit askRefreshList();
}

/**
	Gerer les infos du fichier setting
  */
void DialogSettings::loadInfoSettings() {
	//Onglet Database
	ui->lineEdit_hostName->setText( m_Settings->getDatabase_hostName() );
	ui->lineEdit_port->setText( QString::number(m_Settings->getDatabase_port()) );
	ui->lineEdit_databaseName->setText( m_Settings->getDatabase_databaseName() );
	ui->lineEdit_login->setText( m_Settings->getDatabase_userName() );
	ui->lineEdit_password->setText( m_Settings->getDatabase_userPassword() );

	//Dbase
	QString bdd = QString("Q")+m_Settings->getDatabase_bdd();
	QStringList dlist = m_data->getDrivers();
	int select=0;
	for(int i=0; i<dlist.count(); i++){
		if(dlist[i] == "QSQLITE"){
			ui->comboBox_dbase->addItem( "SQLITE" );
			if(bdd == dlist[i]) select = 0;
		}
		else if(dlist[i] == "QMYSQL"){
			ui->comboBox_dbase->addItem( "MYSQL" );
			if(bdd == dlist[i]) select = 1;
		}
	}
	ui->comboBox_dbase->setCurrentIndex(select);
	//Impression
	ui->comboBox_printFont->setCurrentFont( m_Settings->getPrintFont() );
	//checkversion
	ui->checkBox_checkVersion->setCheckState( Qt::CheckState(m_Settings->getCheckVersion()) );
	//theme	
	for(int i=0; i<ui->comboBox_theme->count(); i++){
		if(m_Settings->getTheme() == ui->comboBox_theme->itemText(i)){
			ui->comboBox_theme->setCurrentIndex(i);
		}
	}
	//Url
	ui->lineEdit_url->setText( m_Settings->getUrl() );
}

/**
	Gerer la connexion avec la base de donnees
  */
void DialogSettings::loadInfoDatabase() {
	//Etat de la connexion
	 if(m_data->isConnected()){
		 ui->label_state->setPixmap(QPixmap::fromImage(QImage(":/app/On").scaled(24,24)));
		 ui->pushButton_connect->setText( tr("Se déconnecter") );
		 //TAX
		 //Setting
		 if(m_data->getIsTax()){
			 if(m_DialogTax){
				delete m_DialogTax;
			 }
			 m_DialogTax = new DialogTax(m_data->m_tax, MCERCLE::Widget);
			 ui->verticalLayout_tax->addWidget(m_DialogTax);
		}
		else
			 ui->tabWidget->setTabEnabled(3, false);
	 }
	 else{
		 ui->label_state->setPixmap(QPixmap::fromImage(QImage(":/app/Off").scaled(24,24)));
		 ui->pushButton_connect->setText( tr("Se connecter") );
	 }
	// Informations dbase
	 QString info = tr("Version: v");
	 info += QString::number( m_data->getDatabaseVersion() );
	 ui -> label_db_info -> setText(info);
	 
	 //Onglet societe
	 QImage lo = m_data->getLogoTable_informations();
	 lo = lo.scaled(128,128, Qt::KeepAspectRatio);
	 ui->label_logo->setPixmap(QPixmap::fromImage(lo));

	 database::Informations inf;
	 m_data->getInfo( inf );
	 ui->lineEdit_sName->setText(inf.name);
	 ui->lineEdit_sNum->setText(inf.num);
	 ui->lineEdit_numTVA->setText(inf.numTax);
	 ui->lineEdit_sCapital->setText(inf.capital);
	 ui->lineEdit_sPhone->setText(inf.phoneNumber);
	 ui->lineEdit_sFax->setText(inf.faxNumber);
	 ui->lineEdit_sEmail->setText(inf.email);
	 ui->lineEdit_sWebSite->setText(inf.webSite);
	 ui->lineEdit_sZipCode->setText(inf.zipCode);
	 ui->lineEdit_sCity->setText(inf.city);
	 ui->lineEdit_sAdd1->setText(inf.address1);
	 ui->lineEdit_sAdd2->setText(inf.address2);
	 ui->lineEdit_sAdd3->setText(inf.address3);
	 ui->checkBox_TAX->setCheckState( Qt::CheckState(inf.tax) );
	 ui->lineEdit_numTVA->setEnabled( Qt::CheckState(inf.tax) );
	 ui->comboBox_CA->setCurrentIndex(inf.ca_type);
	 ui->lineEdit_print1->setText(inf.line1);
	 ui->lineEdit_print2->setText(inf.line2);
	 ui->lineEdit_print3->setText(inf.line3);
	 ui->lineEdit_print4->setText(inf.line4);
	 ui->checkBox_drawLine->setCheckState( Qt::CheckState(inf.drawLine) );
	 ui->checkBox_border_radius->setCheckState( Qt::CheckState(inf.borderRadius) );
	 ui->checkBox_manageStock->setCheckState( Qt::CheckState(inf.manageStock) );
	 ui->lineEdit_currency->setText(inf.currency);

	 database::Bank b;
	 m_data->getBank( b );
	 ui->lineEdit_codeBanque->setText( b.codeBanque );
	 ui->lineEdit_codeGuichet->setText( b.codeGuichet );
	 ui->lineEdit_numCompte->setText( b.numCompte );
	 ui->lineEdit_keyRIB->setText( b.keyRIB );
	 ui->lineEdit_Address->setText( b.address );
	 ui->lineEdit_IBAN1->setText( b.IBAN1 );
	 ui->lineEdit_IBAN2->setText( b.IBAN2 );
	 ui->lineEdit_IBAN3->setText( b.IBAN3 );
	 ui->lineEdit_IBAN4->setText( b.IBAN4 );
	 ui->lineEdit_IBAN5->setText( b.IBAN5 );
	 ui->lineEdit_IBAN6->setText( b.IBAN6 );
	 ui->lineEdit_IBAN7->setText( b.IBAN7 );
	 ui->lineEdit_IBAN8->setText( b.IBAN8 );
	 ui->lineEdit_IBAN9->setText( b.IBAN9 );
	 ui->lineEdit_BIC->setText( b.codeBIC );
}

/**
 * @brief DialogSettings::on_checkBox_TAX_toggled
 * @param checked
 */
void DialogSettings::on_checkBox_TAX_toggled(bool checked) {
	ui->lineEdit_numTVA->setEnabled( checked );
}

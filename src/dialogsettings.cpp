/**
  This file is a part of mcercle
  Copyright (C) 2010-2012 Cyril FRAUSTI

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

#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QDesktopServices>

DialogSettings::DialogSettings(Settings *s, database *pdata, QLocale &lang, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    m_Settings = s;
    m_data = pdata;
    m_lang = lang;

    //Onglet Database
    ui->lineEdit_hostName->setText( m_Settings->getDatabase_hostName() );
    ui->lineEdit_port->setText( QString::number(m_Settings->getDatabase_port()) );
    ui->lineEdit_databaseName->setText( m_Settings->getDatabase_databaseName() );
    ui->lineEdit_login->setText( m_Settings->getDatabase_userName() );
    ui->lineEdit_password->setText( m_Settings->getDatabase_userPassword() );

    //charge les info de la base de donnees
    loadInfoDatabase();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

/**
    Sauvegarde des valeurs
  */
void DialogSettings::on_buttonBox_accepted()
{
    //sauvegarde les donnees de la societe dans la bdd
    database::Informations inf;
    inf.name = ui->lineEdit_sName->text();
    inf.num = ui->lineEdit_sNum->text();
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
    m_data->updateInfo(inf);    

    //settings
    m_Settings->setDatabase_hostName( ui->lineEdit_hostName->text() );
    m_Settings->setDatabase_port( ui->lineEdit_port->text().toInt() );
    m_Settings->setDatabase_databaseName( ui->lineEdit_databaseName->text() );
    m_Settings->setDatabase_userName( ui->lineEdit_login->text());
    m_Settings->setDatabase_userPassword( ui->lineEdit_password->text() );
}

/**
    Ouverture d'un dialog pour le chargement d'un image
  */
void DialogSettings::on_pushButton_Logo_clicked()
{
    QString pathPictures = QDesktopServices::storageLocation ( QDesktopServices::PicturesLocation );
    QString fileName = QFileDialog::getOpenFileName(this, tr("Selectionner une image..."), pathPictures.toStdString().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));

    //verifie la taille
    QImage logo;
    int ret = logo.load(fileName);
    if(!ret){
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de charger l'image..."));
        return;
    }
    if(logo.size().height() > 128) logo = logo.scaled(QSize(logo.width(),128));
    if(logo.size().width() > 128)  logo = logo.scaled(QSize(128,logo.height()));


    //ajout a la base de donnees
    m_data->updateLogoTable_informations(logo);
    //relecture de limage et affichage du logo
    QImage lo = m_data->getLogoTable_informations();
    ui->label_logo->setPixmap(QPixmap::fromImage(lo));
}

/**
    Efface l'image du logo
  */
void DialogSettings::on_pushButton_ClearImage_clicked()
{
    QImage logo;
    //ajout a la base de donnees
    m_data->updateLogoTable_informations(logo);
    //relecture de limage et affichage du logo
    QImage lo = m_data->getLogoTable_informations();
    ui->label_logo->setPixmap(QPixmap::fromImage(lo));
}

/**
    Selectionnez la base de donnees
  */
void DialogSettings::on_toolButton_BaseSelect_clicked()
{
    QFileDialog dialog(this);
    QString filename = dialog.getOpenFileName(this, "Selectionnez un fichier *.fdb", "*.fdb", "*.fdb");
    if( !filename.isEmpty() ) {
        ui->lineEdit_databaseName->setText(filename);
    }
}

/**
    Gerer la connexion avec la base de donnees
  */
void DialogSettings::on_pushButton_connect_clicked()
{
    if(!m_data->isConnected()){
        m_data->setHostName( ui->lineEdit_hostName->text() );
        m_data->setPort( ui->lineEdit_port->text().toInt() );
        m_data->setDatabaseName( ui->lineEdit_databaseName->text() );
        m_data->setUserName( ui->lineEdit_login->text() );
        m_data->setPassword( ui->lineEdit_password->text() );

        if(m_data->connect(database::DB_CONNECT) == database::DB_NOTEXIST_ERR){
            // Demande si on creer une nouvelle base de donnees
            QMessageBox mBox(QMessageBox::Question, tr("Question"), tr("Voulez-vous cr\351er une nouvelle base de donn\351es ?\n"),QMessageBox::Yes | QMessageBox::No);
            mBox.setDefaultButton(QMessageBox::No);
            int ret = mBox.exec();
            if(ret == QMessageBox::Yes)m_data->create();
        }
    }
    else m_data->close();

    //charge les info de la base de donnees
    loadInfoDatabase();

    //Demande le rafraichissement des listes
    emit askRefreshList();
}

/**
    Gerer la connexion avec la base de donnees
  */
void DialogSettings::loadInfoDatabase() {
    //Etat de la connexion
     if(m_data->isConnected()){
         ui->label_state->setPixmap(QPixmap::fromImage(QImage(":/app/On").scaled(24,24)));
         ui->pushButton_connect->setText( tr("Se d\351connecter") );
     }
     else{
         ui->label_state->setPixmap(QPixmap::fromImage(QImage(":/app/Off").scaled(24,24)));
         ui->pushButton_connect->setText( tr("Se connecter") );
     }

     //Onglet societe
     QImage lo = m_data->getLogoTable_informations();
     ui->label_logo->setPixmap(QPixmap::fromImage(lo));

     database::Informations inf;
     m_data->getInfo( inf );
     ui->lineEdit_sName->setText(inf.name);
     ui->lineEdit_sNum->setText(inf.num);
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
}

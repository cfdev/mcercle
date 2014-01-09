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

#include "settings.h"
#include <QStandardPaths>
#include <QDate>

Settings::Settings(QObject *parent) :
	QObject(parent)
{
	m_fileName = "/.mcercle";
	path_DataLocation = QStandardPaths::writableLocation ( QStandardPaths::DataLocation );
	QString path = path_DataLocation+m_fileName;
	m_settings = new QSettings(path,QSettings::IniFormat,this);
}

/**
	Recuperation des parametres de connexion pour la bdd
  */
QString Settings::getDatabase_bdd(){
	m_settings->beginGroup("connection");
	QString val = m_settings->value("bdd").toString();
	m_settings->endGroup();
	return val;
}

QString Settings::getDatabase_hostName(){
	m_settings->beginGroup("connection");
	QString val = m_settings->value("hostName").toString();
	m_settings->endGroup();
	return val;
}

int Settings::getDatabase_port(){
	m_settings->beginGroup("connection");
	int val = m_settings->value("port").toInt();
	m_settings->endGroup();
	return val;
}

QString Settings::getDatabase_databaseName(){
	m_settings->beginGroup("connection");
	QString val = m_settings->value("databaseName").toString();
	m_settings->endGroup();
	return val;
}

QString Settings::getDatabase_userName(){
	m_settings->beginGroup("connection");
	QString val = m_settings->value("userName").toString();
	m_settings->endGroup();
	return val;
}

QString Settings::getDatabase_userPassword(){
	m_settings->beginGroup("connection");
	QString val = m_settings->value("userPassword").toString();
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getPrintFont
 * @return 
 */
QFont Settings::getPrintFont(){
	m_settings->beginGroup("print");
	QFont val( m_settings->value("font").toString() );
	m_settings->endGroup();
	return val;
}


/**
	Sauvegarde dans le fichier les donnees relatif a la base de donnees
  */
void Settings::setDatabase_bdd(const QString& bdd) {
	m_settings->beginGroup("connection");
	m_settings->setValue("bdd", bdd);
	m_settings->endGroup();
}

void Settings::setDatabase_hostName(const QString& hostName) {
	m_settings->beginGroup("connection");
	m_settings->setValue("hostName", hostName);
	m_settings->endGroup();
}

void Settings::setDatabase_port(const int& port) {
	m_settings->beginGroup("connection");
	m_settings->setValue("port",QString::number(port));
	m_settings->endGroup();
}

void Settings::setDatabase_databaseName(const QString& databaseName) {
	m_settings->beginGroup("connection");
	m_settings->setValue("databaseName", databaseName);
	m_settings->endGroup();
}

void Settings::setDatabase_userName(const QString& userName){
	m_settings->beginGroup("connection");
	m_settings->setValue("userName", userName);
	m_settings->endGroup();
}

void Settings::setDatabase_userPassword(const QString& userPassword) {
	m_settings->beginGroup("connection");
	m_settings->setValue("userPassword", userPassword);
	m_settings->endGroup();
}

/**
	Applique les valeurs par defaut de la bdd
  */
void Settings::setDatabase_default() {
	m_settings->beginGroup("connection");
	m_settings->setValue("bdd", "SQLITE");
	m_settings->setValue("hostName", "localhost");
	m_settings->setValue("databaseName", path_DataLocation + "/mcercle.db");
	m_settings->endGroup();
}


/**
 * @brief Settings::settingIsOk
 * @return 
 */
bool Settings::settingIsOk() {
	m_settings->beginGroup("main");
	bool val = false;
	val = m_settings->value("settingState").toBool();
	m_settings->endGroup();
	return val;
}

void Settings::setSettingState(bool state) {
	m_settings->beginGroup("main");
	m_settings->setValue("settingState", state);
	m_settings->endGroup();
}

/**
	Position des listebox pour les recherches
  */
void Settings::setPositionListSearchProduct(int pos) {
	m_settings->beginGroup("list");
	m_settings->setValue("searchProduct", pos);
	m_settings->endGroup();
}

/**
 * @brief Settings::getPositionListSearchProduct
 * @return 
 */
int Settings::getPositionListSearchProduct(){
	m_settings->beginGroup("list");
	int val = m_settings->value("searchProduct").toInt();
	m_settings->endGroup();
	return val;
}

/**
 * @brief setPrintFont
 * @param printFont
 */
void Settings::setPrintFont(const QString& printFont){
	m_settings->beginGroup("print");
	m_settings->setValue("font", printFont);
	m_settings->endGroup();
}

/**
 * @brief setDatebddSave
 * @param date
 */
void Settings::setDatebddSave(const QDate &date) {
	m_settings -> beginGroup("date");
	m_settings -> setValue("bddSave", date.toString(tr("dd-MM-yyyy")) );
	m_settings -> endGroup();
}

/**
 * @brief Settings::getDatebddSave
 * @return 
 */
QDate Settings::getDatebddSave(){
	m_settings->beginGroup("date");
	QDate date;
	date = QDate::fromString( m_settings->value("bddSave").toString(), tr("dd-MM-yyyy") );
	m_settings->endGroup();
	return date;
}

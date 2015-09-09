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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif

#include <QDate>
#include <QDir>
#include <QFile>
#include <QMessageBox>

Settings::Settings(QObject *parent) :
	QObject(parent)
{
	QString path;
	m_fileName = "/.mcercle";
	
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	path_DataLocation = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );

#else
	// Qt5 gere un dosssier du nom de lapplication
	path_DataLocation = QStandardPaths::writableLocation ( QStandardPaths::DataLocation );
#endif
	path = path_DataLocation+m_fileName;
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
	QFont val( m_settings->value("font","DejaVu Serif").toString() );
	val.setPointSize( m_settings->value("fontSize","9").toInt() );
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getCheckVersion
 * @return integer because Qt::CheckState is interger!
 */
int Settings::getCheckVersion(){
	m_settings->beginGroup("main");
	int val = false;
	val = m_settings->value("checkVersion", Qt::Checked).toInt();
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getTheme
 * @return 
 */
QString Settings::getTheme(){
	m_settings->beginGroup("main");
	QString val = m_settings->value("theme").toString();
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getUrl
 * @return
 */
QString Settings::getUrl(){
	m_settings->beginGroup("main");
	QString val = m_settings->value("url","https://raw.githubusercontent.com/cfdev/mcercle/master/version.txt").toString();
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getCode
 * @return
 */
QString Settings::getCode(){
	m_settings->beginGroup("main");
	QString val = m_settings->value("code","").toString();
	m_settings->endGroup();
	return val;
}

/**
 * @brief Settings::getKey
 * @return key
 */
QString Settings::getKey(){
	m_settings->beginGroup("main");
	QString val = m_settings->value("key","").toString();
	m_settings->endGroup();
	return val;
}

//*******************************************************************************//

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
	QString dbaseLocation;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	dbaseLocation = QDesktopServices::storageLocation ( QDesktopServices::DocumentsLocation );

#else
	// Qt5 gere un dosssier du nom de lapplication
	dbaseLocation = QStandardPaths::writableLocation ( QStandardPaths::DocumentsLocation );
#endif

	m_settings->beginGroup("connection");
	m_settings->setValue("bdd", "SQLITE");
	m_settings->setValue("hostName", "localhost");
	m_settings->setValue("databaseName", dbaseLocation + "/mcercle.db");
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
 * @brief Settings::setTheme
 * @param theme
 */
void Settings::setTheme(const QString& theme){
	m_settings->beginGroup("main");
	m_settings->setValue("theme", theme);
	m_settings->endGroup();
}

/**
 * @brief Settings::setCheckVersion
 * @param state integer because Qt::CheckState is interger!
 */
void Settings::setCheckVersion(int state) {
	m_settings->beginGroup("main");
	m_settings-> setValue("checkVersion", state);
	m_settings->endGroup();
}

/**
 * @brief setUrl
 * @param url
 */
void Settings::setUrl(const QString &url) {
	m_settings -> beginGroup("main");
	m_settings -> setValue("url", url );
	m_settings -> endGroup();
}

/**
 * @brief setCode
 * @param code
 */
void Settings::setCode(const QString &code) {
	m_settings -> beginGroup("main");
	m_settings -> setValue("code", code );
	m_settings -> endGroup();
}

/**
 * @brief setKey
 * @param key
 */
void Settings::setKey(const QString &key) {
	m_settings -> beginGroup("main");
	m_settings -> setValue("key", key );
	m_settings -> endGroup();
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
 * @param printFont and size
 */
void Settings::setPrintFont(const QString& printFont, const int& size){
	m_settings->beginGroup("print");
	m_settings->setValue("font", printFont);
	m_settings->setValue("fontSize", size);
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



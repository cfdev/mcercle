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

#include "settings.h"
#include <QDesktopServices>


Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_fileName = ".mcercle";
    path_DataLocation = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
    QString path = path_DataLocation+"\\"+m_fileName;
    m_settings = new QSettings(path,QSettings::IniFormat,this);
}

/**
    Recuperation des parametres de connexion pour la bdd
  */
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
    Sauvegarde dans le fichier les donnees relatif a la base de donnees
  */
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
    m_settings->setValue("hostName", "localhost");
    m_settings->setValue("port",QString::number(3050));
    m_settings->setValue("databaseName", path_DataLocation + "\\db_mcercle.fdb");
    m_settings->setValue("userName", "sysdba");
    m_settings->setValue("userPassword", "masterkey");
    m_settings->endGroup();
}


/**
  */
bool Settings::settingIsOk() {
    m_settings->beginGroup("main");
    bool val = m_settings->value("settingState").toBool();
    m_settings->endGroup();
    return val;
}

void Settings::setSettingState(bool state) {
    m_settings->beginGroup("main");
    m_settings->setValue("settingState", state);
    m_settings->endGroup();
}

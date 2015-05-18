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

#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTextCodec>
#include <QMessageBox>
#include <QTextCodec>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow m_win;

	//Load the translate lib file of Qt
	QString locale = QLocale::system().name();
	QString languages_path = QCoreApplication::applicationDirPath() + "/lang/";

	QTranslator translator, translatorApp;
	if (!translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
		translator.load("qt_" + locale, languages_path);
	}
	app.installTranslator(&translator);

	// Load the tranduction file
	if (!translatorApp.load("mcercle_" + locale, languages_path)) {
		// if error
		if (locale != "fr_FR") {
				// Use the english language for default
				translatorApp.load("mcercle_en", languages_path);
		}
	}
	app.installTranslator(&translatorApp);

	//Initization of application
	m_win.init();

	//Show application
	m_win.show();

	return app.exec();
}

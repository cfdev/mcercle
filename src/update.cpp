#include "update.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif

#include <QDate>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

#include "mcercle.h"
/**
 * @brief update::update Class qui gere si une nouvelle mise a jour est disponible
 * @param parent
 */
Update::Update(QObject *parent) :
	QObject(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	mtemp_location = QDesktopServices::storageLocation ( QDesktopServices::TempLocation );

#else
	mtemp_location = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
#endif

}

/**
 * @brief update::checkVersion
 * @return true si nouvelle version
 */
void Update::checkVersion(const QString& url){
	manager = new QNetworkAccessManager(this);

	connect(manager, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(replyFinished(QNetworkReply*)));

	qDebug() << "Update::checkVersion -> url = " << url;
	manager->get(QNetworkRequest(QUrl(url)));
}

/**
 * @brief Update::replyFinished
 * @param reply
 */
void Update::replyFinished (QNetworkReply *reply) {
	//Si erreur sur le telechargement
	if(reply->error()) {
		qDebug() << "Update::replyFinished -> ERROR!";
		qDebug() << reply->errorString();
	}
	//Sinon on recupere le fichier en memoire
	else 	{
		qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
		qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
		qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
		qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

		QString path = mtemp_location+"\\version.txt";
		QFile *file = new QFile(path);
		qDebug() << "File created: "+path;
		if(file->open(QFile::Append)) {
			file->write(reply->readAll());
			file->flush();
			file->close();
			// Analyse du fichier
			readFile(path);
		}
		delete file;
		//Supression du fichier temporaire
		if(QFile::exists ( path )){
			// Si le fichier existe deja on supprime
			if(!QFile::remove ( path ) ){
				QMessageBox::critical(0, tr("Erreur"),
									 tr("Impossible de remplacer le fichier de version!"));
			}
		}
	}
	reply->deleteLater();
}

/**
 * @brief Update::readFile
 * @param fichier telecharge
 * @param path chemin du fichier
 */
void Update::readFile(QString path) {
	QSettings *m_settings = new QSettings(path,QSettings::IniFormat,this);

	m_settings->beginGroup("version");
	QString sOnlineVersion = m_settings->value("num").toString();
	QString sOnlineLink = m_settings->value("link").toString();
	QString sOnlineLinkChangeLog = m_settings->value("linkChangeLog").toString();
	m_settings->endGroup();

	//Mise en forme des versions
	QString ActualVers = MCERCLE::Version;
	ActualVers.truncate(5);
	qDebug() << tr("ActualVers") << ActualVers;

	QString OnlineVersion = sOnlineVersion;
	OnlineVersion.truncate(5);
	qDebug() << tr("OnlineVersion") << OnlineVersion;

	//Test de la version
	if(ActualVers.toDouble() < OnlineVersion.toDouble()){
		QString mess =tr("<b>Nouvelle version de mcercle disponible!</b><br><br>");
		mess += tr("Nouvelle Version = <b>") + sOnlineVersion + tr("</b>");
		mess += tr("<ul>");
		mess += tr("<li><a href=") + sOnlineLinkChangeLog + tr(">Information sur les changements</a></li>");
		mess += tr("<li><a href=") + sOnlineLink + tr(">Télécharger la nouvelle version!</li>");
		mess += tr("</ul><hr>");
		mess += tr("Version actuelle = <b>") + MCERCLE::Version + tr("</b>");

		QMessageBox::information(0, tr("Mise à jour disponible"), mess);
	}
}

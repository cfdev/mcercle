#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDateTime>

class Update : public QObject
{
	Q_OBJECT
	public:
	explicit Update(QObject *parent = 0);
	void checkVersion();

	private:
	QString mtemp_location;
	QNetworkAccessManager *manager;

	void readFile(QString path);

	signals:
		public slots:
		void replyFinished (QNetworkReply *reply);
};

#endif // UPDATE_H

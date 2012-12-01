#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>


class Settings : public QObject
{
	Q_OBJECT

private:
	QString m_fileName;
	QSettings *m_settings;
	QString path_DataLocation;

public:
	explicit Settings(QObject *parent = 0);

	void setDatabase_bdd(const QString& bdd);
	void setDatabase_hostName(const QString& hostName);
	void setDatabase_port(const int& port);
	void setDatabase_databaseName(const QString& databaseName);
	void setDatabase_userName(const QString& userName);
	void setDatabase_userPassword(const QString& userPassword);
	void setDatabase_default();
	void setPositionListSearchProduct(int pos);
	void setPositionListSearchCustomer(int pos);

	QString getDatabase_bdd();
	QString getDatabase_hostName();
	int getDatabase_port();
	QString getDatabase_databaseName();
	QString getDatabase_userName();
	QString getDatabase_userPassword();
	int getPositionListSearchProduct();
	int getPositionListSearchCustomer();

	bool settingIsOk();
	void setSettingState(bool state);

	QString getDataLocation(){return path_DataLocation;}

signals:

public slots:

};

#endif // SETTINGS_H

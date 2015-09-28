#ifndef UPDATE_DB_H
#define UPDATE_DB_H

#include <QObject>
#include "dbase.h"

class update_db : public QObject
{
	Q_OBJECT
	public:
		explicit update_db(database *pdata, QObject *parent = 0);

		/* fonctions de mise a jour */
		bool upgradeToV2(QString *log);
		bool upgradeToV3(QString *log);
		bool upgradeToV4(QString *log);
		bool upgradeToV5(QString *log);
		bool upgradeToV6(QString *log);
		bool upgradeToV7(QString *log);
		bool upgradeToV8(QString *log);
		bool upgradeToV9(QString *log);

	private:
		database *m_data;

};

#endif // UPDATE_DB_H

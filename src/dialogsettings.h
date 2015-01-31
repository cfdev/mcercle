#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QLocale>

#include "settings.h"
#include "dbase.h"
#include "dialogtax.h"

namespace Ui {
	class DialogSettings;
}

class DialogSettings : public QDialog
{
	Q_OBJECT

public:
	explicit DialogSettings(Settings *s, database *pdata, QLocale &lang, QWidget *parent = 0);
	~DialogSettings();

private:
	Ui::DialogSettings *ui;

	DialogTax *m_DialogTax;
	Settings *m_Settings;
	database *m_data;
	QLocale m_lang;

	void loadInfoSettings();
	void loadInfoDatabase();
	void setDbaseEditState( bool state);

private slots:
	void on_pushButton_connect_clicked();
	void on_toolButton_BaseSelect_clicked();
	void on_pushButton_ClearImage_clicked();
	void on_pushButton_Logo_clicked();
	void on_buttonBox_accepted();	
	void on_checkBox_TAX_toggled(bool checked);
	
	signals:
	void askRefreshList();
};

#endif // DIALOGSETTINGS_H

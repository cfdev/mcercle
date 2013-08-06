#ifndef DIALOGSERVICESEDIT_H
#define DIALOGSERVICESEDIT_H

#include <QDialog>
#include "dbase.h"

namespace Ui {
	class DialogServicesEdit;
}

class DialogServicesEdit : public QDialog
{
	Q_OBJECT

public:
	explicit DialogServicesEdit(database *pdata, QWidget *parent = 0);
	~DialogServicesEdit();

private:
	service_common* m_servComm;
	tax* m_taxTable;
	bool m_tax;

	void setTitle(QString val);
	void listInterCommToTable();
	void loadValuesFormService();
	void loadEmptyValues();
	void loadTaxList();
	void setValuesToService();

	Ui::DialogServicesEdit *ui;

private slots:
	void checkConditions();
	void on_tableWidget_Inter_itemSelectionChanged();
	void on_pushButton_add_clicked();
	void on_pushButton_edit_clicked();
	void on_pushButton_del_clicked();
	void on_pushButton_close_clicked();
};

#endif // DIALOGSERVICESEDIT_H

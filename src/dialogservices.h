#ifndef DIALOGServices_H
#define DIALOGServices_H

#include <QDialog>
#include "dbase.h"

namespace Ui {
	class DialogServices;
}

class DialogServices : public QDialog
{
	Q_OBJECT

public:
	explicit DialogServices(database *pdata, QWidget *parent = 0);
	~DialogServices();

	void setTitle(QString val);
	void loadServiceCommList();
	void loadValuesFromService();
	void loadValuesFromServiceComm();
	void loadEmptyValues();

private:
	Ui::DialogServices *ui;
	database* m_data;
	service* m_serv;
	service_common* m_servComm;
	service_common::serviceCommList m_servCommlist;

	int m_idServ, m_idCustomer;
	bool m_tax;


private slots:
	void on_buttonBox_accepted();
	void checkConditions();

	void on_toolButton_EditServiceComm_clicked();
	void on_comboBox_servComm_currentIndexChanged(int index);
};

#endif // DIALOGServices_H

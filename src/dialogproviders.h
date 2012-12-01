#ifndef DIALOGPROVIDERS_H
#define DIALOGPROVIDERS_H

#include <QDialog>
#include "product.h"

namespace Ui {
	class DialogProviders;
}

class DialogProviders : public QDialog
{
	Q_OBJECT

public:
	explicit DialogProviders(product *prod, QWidget *parent = 0);
	~DialogProviders();

private:
	Ui::DialogProviders *ui;
	product *m_product;
	void listProvidersToTable(QString order, QString filter, QString field);

private slots:
	void on_pushButton_delete_clicked();
	void on_pushButton_edit_clicked();
	void on_tableWidget_providers_itemSelectionChanged();
	void on_pushButton_add_clicked();
	void on_buttonBox_accepted();
};

#endif // DIALOGPROVIDERS_H

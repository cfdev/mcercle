#ifndef DIALOGTAX_H
#define DIALOGTAX_H

#include <QDialog>
#include "tax.h"

namespace Ui {
class DialogTax;
}

class DialogTax : public QDialog
{
	Q_OBJECT

public:
	explicit DialogTax(tax *t, QWidget *parent = 0);
	~DialogTax();

private slots:
	void on_pushButton_add_clicked();

	void on_pushButton_edit_clicked();

	void on_pushButton_del_clicked();

	void on_tableWidget_itemSelectionChanged();

private:
	Ui::DialogTax *ui;
	tax *m_tax;

	void listTaxToTable(QString order, QString filter, QString field);
};

#endif // DIALOGTAX_H

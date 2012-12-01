#ifndef DIALOGINVOICELIST_H
#define DIALOGINVOICELIST_H

#include <QDialog>
#include "dbase.h"

namespace Ui {
class DialogInvoiceList;
}

class DialogInvoiceList : public QDialog
{
	Q_OBJECT

public:
	explicit DialogInvoiceList(QLocale &lang, database *pdata, QWidget *parent = 0);
	~DialogInvoiceList();

private slots:
	void on_dateEdit_dateChanged(const QDate &date);
	void on_pushButton_ok_clicked();
	void on_pushButton_print_clicked();

	// slots dimpression
	void on_paintPrinter(QPrinter *printer);

private:
	Ui::DialogInvoiceList *ui;
	database *m_data;
	invoice *m_invoice;
	bool m_isTax;
	QLocale m_lang;
	QDate m_date;

#define COL_COUNT 7

#define COL_ID 0
#define COL_DATE 1
#define COL_CODE 2
#define COL_CUSTOMER 3
#define COL_DESCRIPTION 4
#define COL_PRICE 5
#define COL_TYPE_PAYMENT 6


	void listInvoicesToTable(QDate mdate);
};

#endif // DIALOGINVOICELIST_H

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
	void on_toolButton_selAll_clicked();
	void on_toolButton_unSel_clicked();
	
	void on_pushButton_printAll_clicked();
	
	private:
	Ui::DialogInvoiceList *ui;
	database *m_data;
	invoice *m_invoice;
	bool m_isTax;
	QLocale m_lang;
	QDate m_date;

#define COL_COUNT 8

enum{COL_ID=0, COL_STATE, COL_DATE,
	 COL_CODE, COL_CUSTOMER, COL_DESCRIPTION,
	 COL_PRICE, COL_TYPE_PAYMENT};

	void listInvoicesToTable(QDate mdate);
	QList<int> list_of_InvoiceSelected();
};

#endif // DIALOGINVOICELIST_H

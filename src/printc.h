#ifndef PRINTC_H
#define PRINTC_H

#include <QObject>
#include <QLocale>

#include "dbase.h"

class Printc : public QObject
{
	Q_OBJECT
	public:
	explicit Printc(database *pdata, QLocale &lang, QObject *parent = 0);
	~Printc();
	
	void print_Proposal(const int &);
	void print_Invoice(const int &);
	void print_Service(const int &);
	
	private:
	QLocale m_lang;
	database *m_data;
	customer *m_cus;
	invoice *m_inv;
	proposal *m_pro;
	service *m_serv;
	
	void print_header(const QPainter &painter);
	void print_footer(const QPainter &painter);
	
	private slots:
	// slots dimpression

	void on_paintPrinterProposal(QPrinter *printer);
	void on_paintPrinterInvoice(QPrinter *printer);
	void on_paintPrinterService(QPrinter *printer);
};

#endif // PRINTC_H

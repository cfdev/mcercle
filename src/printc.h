#ifndef PRINTC_H
#define PRINTC_H

#include <QObject>
#include <QLocale>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QFont>

#include "dbase.h"

class Printc : public QObject
{
	Q_OBJECT
	public:
	Printc(database *pdata, QLocale &lang, QObject *parent = 0);
	~Printc();
	
	void print_Proposal(const int &);
	void print_Invoice(const int &);
	void print_Service(const int &);
	
	private:
	enum{T_PROPOSAL, T_INVOICE, T_SERVICE};
		
	QLocale m_lang;
	database *m_data;
	customer *m_cus;
	invoice *m_inv;
	proposal *m_pro;
	service *m_serv;
	QImage mlogo;
	QString mtextInfo, mtextidentity, mfooterTextInfo;
	
	// print
	qreal mLeft, mTop, mRight, mBottom;
	QFont mFont;
	qreal mwUtil;
	QRectF mpageRect, mRectContent;
	int mBlockHeight;
	
	void load_parameters(QPrinter *printer, QPainter &painter);
	QRectF print_header(QPainter &painter, int type);
	QRectF print_footer(QPainter &painter, QRectF rect, QString page);
	
	private slots:
	// slots dimpression
	void on_paintPrinterProposal(QPrinter *printer);
	void on_paintPrinterInvoice(QPrinter *printer);
	void on_paintPrinterService(QPrinter *printer);
};

#endif // PRINTC_H

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
	void print_InvoicesList(QList<int> listofId);
	void print_Service(const int &);
	
	private:
	enum{T_PROPOSAL, T_INVOICE, T_SERVICE};
#define OFFSET_BOT_TOTAL 275
		
	QLocale m_lang;
	database *m_data;
	customer *m_cus;
	invoice *m_inv;
	proposal *m_pro;
	service *m_serv;
	QImage mlogo;
	QString mtextInfo, mtextidentity, mfooterTextInfo, mBankTextID;
	
	typedef struct{
		QStringList designation;
		QList<qreal> tax;
		QList<int> discount;
		QList<qreal> price;
		QList<int> quantity;
		QList<qreal> totalPrice;
	}itemList;
	
	// print
	qreal mLeft, mTop, mRight, mBottom;
	QFont mFont;
	qreal mwUtil;
	QRectF mpageRect, mRectContent;
	int mBlockHeight;
	int mlinePerPage, mlinePerLastPage;
	
	void load_parameters(QPrinter *printer, QPainter &painter);
	void print_header(QPainter &painter, QRectF &rect, int type);
	void print_content(QPainter &painter, QRectF &rect, itemList Ilist, int &itemPrinted, int page, int NbOfpage);
	void print_total(QPainter &painter, QRectF &rect, itemList Ilist, qreal &totalPrice, int type);
	void print_reglement(QPainter &painter, QRectF &rect, qreal &totalPrice);
	QRectF get_RecFooter(QPainter &painter);
	void print_footer(QPainter &painter, QRectF &rect, QString page, QString NbOfpage);
	
	private slots:
	// slots dimpression
	void on_paintPrinterProposal(QPrinter *printer);
	void on_paintPrinterInvoice(QPrinter *printer);
	void on_paintPrinterService(QPrinter *printer);
};

#endif // PRINTC_H

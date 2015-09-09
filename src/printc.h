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
	void print_fileEmpty();
	
	private:
	enum{T_PROPOSAL, T_INVOICE, T_SERVICE, T_FILE};

//Largeur en %
#define WIDTH_DES 0.54
#define WIDTH_TAX 0.08
#define WIDTH_DIS 0.07
#define WIDTH_PRI 0.12
#define WIDTH_QTE 0.07
#define WIDTH_TOT 0.12
	
#define WIDTH_TOTAUX 0.375
#define SPACE_BORDER 20
#define OFFSET_BOT_TOTAL 1700 //275

#define DPI 300
	//1.2 low resolution
	//5 High resolution
#define PEN_WIDTH DPI/150

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
		QList<QImage> picture;
	}itemList;
	
	// print
	qreal mLeft, mTop, mRight, mBottom;
	QFont mFont;
	qreal mFontSize;
	qreal mwUtil;
	QRectF mpageRect, mRectContent;
	int mBlockHeight;
	int mlinePerPage, mlinePerLastPage;
	qreal mtotalPrice, mtotalTaxPrice;
	int mRoundedRect;
	bool mDrawLine;
	
	void load_customerInfos();
	void load_parameters(QPrinter *printer, QPainter &painter);
	void print_header(QPainter &painter, QRectF &rect, int type);
	void print_content(QPainter &painter, QRectF &rect, itemList Ilist, int &itemPrinted, int page, int NbOfpage);
	void print_total(QPainter &painter, QRectF &rect, itemList Ilist, int type);
	void print_reglement(QPainter &painter, QRectF &rect, const QString &typeP, const int &type);
	QRectF get_RecFooter(QPainter &painter);
	void print_footer(QPainter &painter, QRectF &rect, QString page, QString NbOfpage);
	void setRoundedRect(bool state);
	void setDrawLine(bool state);

	private slots:
	// slots dimpression
	void on_paintPrinterProposal(QPrinter *printer);
	void on_paintPrinterInvoice(QPrinter *printer);
	void on_paintPrinterService(QPrinter *printer);
	void on_paintPrinter_fileEmpty(QPrinter *printer);
};

#endif // PRINTC_H

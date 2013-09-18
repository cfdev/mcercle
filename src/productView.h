#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QLocale>

#include "dbase.h"

namespace Ui {
	class productView;
}

class productView : public QWidget {
	Q_OBJECT
public:
	productView(database *pdata, QLocale &lang, unsigned char type, QWidget *parent = 0);
	~productView();

	void listProducts(int page);
	int getRowCount();
	QString getSelectedProductName();
	qreal getSelectedProductPrice();
	int getSelectedProductID();

	void setIndexSearchProduct(int index);
	int getIndexSearchProduct();

	enum{PRODUCT_VIEW, INVOICE_VIEW};

protected:
	void changeEvent(QEvent *e);

private:
	Ui::productView *ui;
	database *m_data;
	QLocale m_lang;
	bool m_ShowObsoleteProduct;
	int m_prodPage, m_prodNbPage;
	QString m_prodfilter, m_prodfield;

	#define PRODUCTSMAX_PAGE 1000
	#define ID_ROW 0
	#define CODE_ROW 1
	#define NAME_ROW 2
	#define PRICE_ROW 4
	
	//info produit
	void listProductsFilter(int page, QString val);
	void listProductsToTable(int page, QString filter, QString field);
	QString InfoProduct();
	QString InfoProvider();

private slots:
	void on_toolButton_printInfo_clicked();
	void on_toolButton_print_clicked();
	void on_toolButton_remove_clicked();
	void on_lineEdit_searchProduct_returnPressed();
	void on_checkBox_viewDiscontinued_clicked(bool checked);
	void on_tableWidget_products_itemSelectionChanged();
	void on_toolButton_editProduct_clicked();
	void on_toolButton_addProduct_clicked();
	void on_toolButton_searchProduct_clicked();

	// slots dimpression
	void on_paintPrinterListingStock(QPrinter *printer);
	void on_paintPrinterInfoProduct(QPrinter *printer);

	void on_lineEdit_page_returnPressed();
	void on_toolButton_pagePrev_clicked();
	void on_toolButton_pageNext_clicked();

	void on_toolButton_clipboard_clicked();
	void on_tableWidget_products_itemDoubleClicked();

	void on_comboBoxFiltre_currentIndexChanged(int index);
	void on_lineEdit_searchProduct_textChanged(const QString &arg1);

public slots:
	void refreshProductsList();


};

#endif // PRODUCT_H

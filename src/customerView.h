#ifndef CUSTOMERVIEW_H
#define CUSTOMERVIEW_H

#include <QWidget>
#include <QLocale>
#include <QTableWidgetItem>
#include <QShortcut>

#include "dbase.h"

namespace Ui {
	class customerView;
}

class customerView : public QWidget {
	Q_OBJECT
public:
	customerView(database *pdata, QLocale &lang, QWidget *parent = 0);
	~customerView();

	QIcon getIconState(int state);
	QString getTextState(int state);

protected:
	void changeEvent(QEvent *e);

private:
	Ui::customerView *ui;
	database *m_data;
	QLocale m_lang;
	QString m_order;
	int m_colHeaderSelected;
	int m_custPage, m_custNbPage;
	int m_custId;
	QString m_custfilter, m_custfield;
	QShortcut* shortcutEstimate;

#define ID_ROW 0
#define LASTNAME_ROW 1

	//info clients
	void listCustomersFilter(int page, QString val);
	void listCustomersToTable(int page, QString filter, QString field);

	void loadInfoCustomer();
	QString InfoCustomer(int id);

	//Devis
	void listProposalsToTable(QString filter, QString field);
	//Services
	void listServicesToTable(QString filter, QString field);
	//Factures
	void listInvoicesToTable(QString filter, QString field);
#define CUSTOMERSMAX_PAGE 1000

private slots:
	void on_toolButton_addService_clicked();
	void on_toolButton_editService_clicked();
	void on_toolButton_delService_clicked();
	void on_tableWidget_Services_itemSelectionChanged();

	void on_lineEdit_Search_returnPressed();
	void on_toolButton_editProposal_clicked();
	void on_toolButton_addProposal_clicked();

	void on_toolBut_Add_clicked();
	void on_toolBut_Edit_clicked();
	void on_tableWidget_itemSelectionChanged();
	void on_toolBut_search_clicked();

	void on_toolButton_addInvoice_clicked();
	void on_toolButton_editInvoice_clicked();

	void on_toolButton_pagePrev_clicked();
	void on_toolButton_pageNext_clicked();
	void on_lineEdit_page_returnPressed();

	void on_toolButton_clipboard_clicked();
	void on_toolButton_delInvoice_clicked();
	void on_toolButton_delProposal_clicked();
	void on_toolButton_Del_clicked();

	void on_toolButton_printServ_clicked();
	void on_toolButton_printPro_clicked();
	void on_toolButton_printInv_clicked();

	void on_tableWidget_itemDoubleClicked();
	void on_tableWidget_Services_itemDoubleClicked();
	void on_tableWidget_Proposals_itemDoubleClicked();
	void on_tableWidget_Invoices_itemDoubleClicked();

	void on_lineEdit_Search_textChanged(const QString &arg1);	
	void on_comboBoxFiltre_currentIndexChanged(int index);

	// Keypress
	void copyEstimate();
	void cutEstimate();
	void pasteEstimate();

	void copyInvoice();
	void cutInvoice();
	void pasteInvoice();

	void on_toolButton_printPDFInv_clicked();

	void on_toolButton_printPDFPro_clicked();

	public slots:
	void refreshCustomersList();
	void listCustomers(int page);
	void listServices();
	void listProposals();
	void listInvoices();
};

#endif // CUSTOMER_H

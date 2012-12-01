
#ifndef DIALOGCATEGORY_H
#define DIALOGCATEGORY_H

#include <QDialog>
#include "product.h"

namespace Ui {
	class DialogCategory;
}

class DialogCategory : public QDialog
{
	Q_OBJECT

public:
	explicit DialogCategory(product *prod, QWidget *parent = 0);
	~DialogCategory();

private:
	Ui::DialogCategory *ui;
	product *m_product;
	QColor m_color;

	void listCategoriesToTable(QString order, QString filter, QString field);
	void setColorLabel(const QColor &color);


private slots:
	void on_toolButton_colorDefault_clicked();
	void on_pushButton_color_clicked();
	void on_pushButton_edit_clicked();
	void on_pushButton_add_clicked();
	void on_pushButton_del_clicked();
	void on_tableWidget_itemSelectionChanged();
	void checkConditions();
};

#endif // DIALOGCATEGORY_H

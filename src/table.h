#ifndef TABLE_H
#define TABLE_H

#include <QTableWidgetItem>

#define TABLE_BG_COLOR QColor(69,69,69,255)
#define TABLE_TXT_COLOR QColor(219,219,219,255)

class ItemOfTable : public QObject, public QTableWidgetItem {
	Q_OBJECT
	public:
	ItemOfTable(const QColor& colorBG, const QColor& colorTXT);
	
};

class Table : /*public QObject,*/ public QTableWidget {
	Q_OBJECT
	public:
	Table( QWidget *parent=0 );
	
	void raz();
	
	private:
	QWidget mparent;
};
#endif // TABLE_H

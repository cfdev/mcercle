#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include "dbase.h"

namespace Ui {
	class about;
}

class about : public QDialog {
	Q_OBJECT
public:
	about(database *pdata, QWidget *parent = 0);
	~about();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::about *ui;

private slots:
	void on_pushButton_don_clicked();
	void on_pushBut_Close_clicked();
	void on_pushButton_donFree_clicked();
	void on_pushButton_donmonthly_clicked();
};

#endif // ABOUT_H

#ifndef DIALOGPRINTCHOICE_H
#define DIALOGPRINTCHOICE_H

#include <QDialog>
#include <QPrinter>

namespace Ui {
	class DialogPrintChoice;
}

class DialogPrintChoice : public QDialog
{
	Q_OBJECT

public:
	explicit DialogPrintChoice(QPrinter * printer, QWidget *parent = 0);
	~DialogPrintChoice();
	int typePrint(){return m_typePrint;}
	QString pathFile(){return mpathFile;}
	QString path(){return mpath;}
	
	enum{PRINT_FILE , PRINT_PDF};
	
private slots:
	void on_radioButton_pdf_clicked();
	void on_radioButton_printer_clicked();
	void on_buttonBox_accepted();

	void on_toolButton_path_clicked();
	
	private:
	QPrinter *m_printer;
	Ui::DialogPrintChoice *ui;
	int m_typePrint;
	QString mpath, mpathFile;
	
};

#endif // DIALOGPRINTCHOICE_H

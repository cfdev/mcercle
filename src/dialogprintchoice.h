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

private slots:
	void on_radioButton_pdf_clicked();
	void on_radioButton_printer_clicked();

	void on_buttonBox_accepted();

private:
	QPrinter *m_printer;
	Ui::DialogPrintChoice *ui;
};

#endif // DIALOGPRINTCHOICE_H

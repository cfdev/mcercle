#include "dialogprintchoice.h"
#include "ui_dialogprintchoice.h"
#include <QDesktopServices>

DialogPrintChoice::DialogPrintChoice(QPrinter * printer, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogPrintChoice)
{
	ui->setupUi(this);

	m_printer = printer,
	ui->radioButton_printer->setChecked(true);
	ui->lineEdit_path->setEnabled(false);
	ui->lineEdit_path->setText( QDesktopServices::storageLocation ( QDesktopServices::HomeLocation )+'/'+
								m_printer->outputFileName() );
}

DialogPrintChoice::~DialogPrintChoice() {
	delete ui;
}

void DialogPrintChoice::on_radioButton_pdf_clicked() {
	ui->lineEdit_path->setEnabled(true);
}

void DialogPrintChoice::on_radioButton_printer_clicked() {
	ui->lineEdit_path->setEnabled(false);
}

/**
 * @brief DialogPrintChoice::on_buttonBox_accepted
 */
void DialogPrintChoice::on_buttonBox_accepted() {
	//impression physique
	if( ui->radioButton_printer->isChecked() ){
		m_typePrint = PRINT_FILE;
		mpathFile = "";
		m_printer -> setOutputFormat(QPrinter::NativeFormat);
		m_printer -> setOutputFileName(mpathFile);
	}
	//impression PDF
	else{
		m_typePrint = PRINT_PDF;
		mpathFile = ui->lineEdit_path->text();
		m_printer -> setOutputFormat(QPrinter::PdfFormat);
		m_printer -> setOutputFileName( ui->lineEdit_path->text() );
	}
}

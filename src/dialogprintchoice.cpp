#include "dialogprintchoice.h"
#include "ui_dialogprintchoice.h"

#include <QFileDialog>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif

DialogPrintChoice::DialogPrintChoice(QPrinter * printer, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogPrintChoice)
{
	ui->setupUi(this);
	setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	m_printer = printer,
	ui->radioButton_printer->setChecked(true);
	on_radioButton_printer_clicked();
	
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	mpath = QDesktopServices::storageLocation ( QDesktopServices::HomeLocation );
#else
	mpath = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
#endif	
	if( m_printer -> outputFileName().isEmpty() )
		mpathFile = mpath + "/FAxxx.pdf";
	else
		mpathFile = mpath + '/' + m_printer->outputFileName();
	
	ui->lineEdit_path->setText( mpathFile );
}

DialogPrintChoice::~DialogPrintChoice() {
	delete ui;
}

/**
 * @brief Active les controles pour l export fichier
 */
void DialogPrintChoice::on_radioButton_pdf_clicked() {
	ui -> lineEdit_path -> setEnabled(true);
	ui -> toolButton_path -> setEnabled(true);
}

/**
 * @brief Desactive les controles pour l export fichier
 */
void DialogPrintChoice::on_radioButton_printer_clicked() {
	ui -> lineEdit_path -> setEnabled(false);
	ui -> toolButton_path -> setEnabled(false);
}

/**
 * @brief DialogPrintChoice::on_buttonBox_accepted
 */
void DialogPrintChoice::on_buttonBox_accepted() {
	//impression physique
	mpathFile = ui->lineEdit_path->text();
	QFileInfo file( mpathFile );
	mpath = file.dir().absolutePath()+ '/';
	
	if( ui->radioButton_printer->isChecked() ){
		m_typePrint = PRINT_FILE;
		mpathFile = "";
		m_printer -> setOutputFormat(QPrinter::NativeFormat);
	}
	//impression PDF
	else{
		m_typePrint = PRINT_PDF;
		m_printer -> setOutputFormat(QPrinter::PdfFormat);
	}
	m_printer -> setOutputFileName(mpathFile);
}

/**
 * @brief Lance l explorer de fichier
 */
void DialogPrintChoice::on_toolButton_path_clicked() {
	
	QString filename = QFileDialog::getSaveFileName(this, "Emplacement du fichier", mpathFile.toStdString().c_str(), "*.pdf");
	if( !filename.isEmpty() ) {
		QFileInfo file( filename );
		mpath = file.dir().absolutePath()+ '/';
		ui -> lineEdit_path -> setText(filename);
	}
}

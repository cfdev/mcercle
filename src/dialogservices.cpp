/**
  This file is a part of mcercle
  Copyright (C) 2010-2013 Cyril FRAUSTI

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include "dialogservices.h"
#include "ui_dialogservices.h"
#include "dialogservicesedit.h"

#include <QMessageBox>

DialogServices::DialogServices(database *pdata, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogServices)
{
	ui->setupUi(this);
	setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	m_data = pdata;
	m_serv = m_data -> m_customer -> m_service;
	m_servComm = m_data -> m_customer -> m_serviceComm;
	Istax_ = m_data -> getIsTax();
	m_taxTable = pdata -> m_tax;
	m_lang = pdata -> lang();

	//Affiche ou pas les taxes
	ui->comboBox_tax -> setVisible(Istax_);
	ui->label_titletax -> setVisible(Istax_);
	if(Istax_)
		ui->label_tax -> setText( tr("HT") );
	else
		ui->label_tax -> setText( tr("TTC") );
	// Charge la liste des taxes dans la combo box
	loadTaxList();
	
	ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime());
  //  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	loadServiceCommList();
	//Event
	connect(ui->lineEdit_Name, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
}


DialogServices::~DialogServices()
{
	delete ui;
}

/**
	 Charge la liste des tax
*/
void DialogServices::loadTaxList() {
	//categories
	tax::taxList list;
	//Recuperation des donnees presentent dans la bdd
	m_taxTable->getTaxList(list, "TAX", "", "");
	ui->comboBox_tax->clear();
	
	for(unsigned int i=0; i<list.value.size(); i++){
		ui->comboBox_tax->addItem( m_lang.toString(list.value.at(i),'f',2) );
	}
}

/**
	 Test les conditions pour activer les boutons
  */
void DialogServices::checkConditions()
{
 /*   if(!ui->lineEdit_Name->text().isEmpty()){
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->pushButton_add->setEnabled(true);
		ui->pushButton_edit->setEnabled(true);
	}
	else {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		ui->pushButton_add->setEnabled(false);
		ui->pushButton_edit->setEnabled(false);
	}*/
}

/**
	Renseigne le titre
	@param titre de la fenetre
  */
void DialogServices::setTitle(QString val){
	ui->labelTitle->setText(val);
}


/**
	Sur la validation on renseigne la class intervention
  */
void DialogServices::on_buttonBox_accepted()
{
	//resize
	QString desc = ui->textEdit_Desc->toPlainText();
	if(desc.size() > 1000) desc.resize(1000);
	m_serv -> setName( ui->lineEdit_Name->text() );
	m_serv -> setDate( ui->dateTimeEdit->dateTime() );
	m_serv -> setPrice( ui->doubleSpinBox->value() );
	m_serv -> setTax( m_lang.toDouble(ui->comboBox_tax->currentText()) );
	m_serv -> setDescription( desc );
}


/**
	Charge les informations de l intervention
  */
void DialogServices::loadValuesFromService() {
	ui->lineEdit_Name->setText( m_serv->getName() );
	ui->dateTimeEdit->setDateTime( m_serv->getDate() );
	ui->doubleSpinBox->setValue( m_serv->getPrice() );
	ui->textEdit_Desc->setPlainText( m_serv->getDescription() );
	
	for(int i=0; i<ui->comboBox_tax->count(); i++){
		if(ui->comboBox_tax->itemText(i) == m_lang.toString(m_serv->getTax(),'f',2) ) {
			ui->comboBox_tax->setCurrentIndex(i);
			break;
		}
	}
}

/**
	Charge les informations de l intervention
  */
void DialogServices::loadValuesFromServiceComm() {
	ui->lineEdit_Name->setText( m_servComm->getName() );
	ui->dateTimeEdit->setDateTime( m_servComm->getDate() );
	ui->doubleSpinBox->setValue( m_servComm->getPrice() );
	ui->textEdit_Desc->setPlainText( m_servComm->getDescription() );
	
	for(int i=0; i<ui->comboBox_tax->count(); i++){
		if(ui->comboBox_tax->itemText(i) == m_lang.toString(m_servComm->getTax(),'f',2) ) {
			ui->comboBox_tax->setCurrentIndex(i);
			break;
		}
	}
}

/**
	efface les informations de l intervention
  */
void DialogServices::loadEmptyValues() {
	ui->lineEdit_Name->setText("");
	ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime() );
	ui->doubleSpinBox->setValue( 0 );
	ui->textEdit_Desc->setPlainText( "" );
}

/**
	Charge la liste des services communs
  */
void DialogServices::loadServiceCommList(){
	int select=-1;
	//efface la liste
	m_servCommlist.creationDate.clear();
	m_servCommlist.date.clear();
	m_servCommlist.description.clear();
	m_servCommlist.id.clear();
	m_servCommlist.name.clear();
	m_servCommlist.price.clear();
	m_servCommlist.tax.clear();
	//Recuperation des donnees presentent dans la bdd
	m_servComm->getServiceCommList(m_servCommlist, "NAME", "", "");
	ui->comboBox_servComm->clear();
	ui->comboBox_servComm->addItem( "" );
	for(int i=0; i<m_servCommlist.name.count(); i++){
		ui->comboBox_servComm->addItem( m_servCommlist.name.at(i) );
	}
	ui->comboBox_servComm->setCurrentIndex(select);
}


/**
  Event sur le changement du combo de lintervention generale
  */
void DialogServices::on_comboBox_servComm_currentIndexChanged(int index){
	if(index<=0){
		loadEmptyValues();
	}
	else{
		m_servComm->loadFromID( m_servCommlist.id.at(index-1) );
		loadValuesFromServiceComm();
	}
}

/**
	Dialog des interventions generales
  */
void DialogServices::on_toolButton_EditServiceComm_clicked()
{
	DialogServicesEdit *m_DServComm = new DialogServicesEdit(m_data);
	m_DServComm->setModal(true);
	m_DServComm->exec();

	// rafraichit la liste des interventions communes
	loadServiceCommList();
	delete m_DServComm;
}

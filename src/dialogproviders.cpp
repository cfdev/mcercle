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

#include "dialogproviders.h"
#include "ui_dialogproviders.h"
#include "dialogprovidersedit.h"
#include "table.h"

#include <QMessageBox>


DialogProviders::DialogProviders(product *prod, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogProviders)
{
	ui->setupUi(this);

	m_product = prod;
	//list les fournisseurs
	listProvidersToTable("NAME", "", "");
}

DialogProviders::~DialogProviders()
{
	delete ui;
}

/**
	Affiche les fournisseurs
*/
void DialogProviders::listProvidersToTable(QString order, QString filter, QString field)
{
	provider::ProviderList plist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget_providers->clear();
	for (int i=ui->tableWidget_providers->rowCount()-1; i >= 0; --i)
		ui->tableWidget_providers->removeRow(i);
	for (int i=ui->tableWidget_providers->columnCount()-1; i>=0; --i)
		ui->tableWidget_providers->removeColumn(i);

	ui->tableWidget_providers->setSortingEnabled(false);
	//Style de la table produit
	ui->tableWidget_providers->setColumnCount(4);
//    ui->tableWidget_providers->setColumnWidth(1,200);
	ui->tableWidget_providers->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_providers->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget_providers->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Nom") << tr("Tel") << tr("Fax") << tr("Email");
	ui->tableWidget_providers->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_product->m_provider->getProviderList(plist, order, filter, field);

	// list all products
	for(int i=0,j=0; i<plist.name.count();i++){
		ItemOfTable *item_NAME      = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_PHONENUMBER      = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_FAXNUMBER     = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);
		ItemOfTable *item_EMAIL     = new ItemOfTable(TABLE_BG_COLOR, TABLE_TXT_COLOR);

		item_NAME->setText(         plist.name.at(i));
		item_PHONENUMBER->setText(  plist.phoneNumber.at(i));
		item_FAXNUMBER->setText(    plist.faxNumber.at(i));
		item_EMAIL->setText(        plist.email.at(i));
		
		//definir le tableau
		ui->tableWidget_providers->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget_providers->setItem(j, 0, item_NAME);
		ui->tableWidget_providers->setItem(j, 1, item_PHONENUMBER);
		ui->tableWidget_providers->setItem(j, 2, item_FAXNUMBER);
		ui->tableWidget_providers->setItem(j, 3, item_EMAIL);
		j++;
	}
	ui->tableWidget_providers->setSortingEnabled(true);
	ui->tableWidget_providers->selectRow(0);

	//Si pas de produit on desactive les boutons
	if( ui->tableWidget_providers->rowCount() <=0 ) {
		ui->pushButton_edit->setEnabled(false);
		ui->pushButton_delete->setEnabled(false);
	}else{
		ui->pushButton_edit->setEnabled(true);
		ui->pushButton_delete->setEnabled(true);
	}
}

/**
	Fermeture du dialog
  */
void DialogProviders::on_buttonBox_accepted()
{
	this->close();
}

/**
	Ajouter un fournisseur
  */

void DialogProviders::on_pushButton_add_clicked()
{
	DialogProvidersEdit *m_DPE = new DialogProvidersEdit(m_product, PROVIDER_ADD);
	m_DPE->setModal(true);
	m_DPE->exec();
	if(m_DPE->result() == QDialog::Accepted)
	{
		//list les fournisseurs
		if( m_product->m_provider->create() ) listProvidersToTable("NAME", "", "");
	}
	delete m_DPE;
}

/**
	Selection d'un fournisseur
  */
void DialogProviders::on_tableWidget_providers_itemSelectionChanged()
{
	int m_index = ui->tableWidget_providers->currentRow();
	m_product->m_provider->loadFromName( ui->tableWidget_providers->item(m_index, 0)->text() );
}

/**
	Modifier le fournisseur
  */
void DialogProviders::on_pushButton_edit_clicked()
{
	DialogProvidersEdit *m_DPE = new DialogProvidersEdit(m_product, PROVIDER_EDIT);
	m_DPE->setModal(true);
	m_DPE->exec();
	if(m_DPE->result() == QDialog::Accepted)
	{
		// Recuperation des informations de clients
		if( m_product->m_provider->update() ) listProvidersToTable("NAME", "", "");
	}
	delete m_DPE;
}

/**
	Suppression d'un fournisseur
  */
void DialogProviders::on_pushButton_delete_clicked()
{
	int ret = QMessageBox::warning(this, tr("Attention"),
								   tr("Voulez-vous vraiment supprimer le fournisseur:<br><b>")
								   + m_product -> m_provider -> getName()
								   +"</b><br>Ceci supprimera aussi le fournisseur dans la liste des produits",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		// Avant il faut supprimer toutes les references a ce fournisseur
		// dans la table produits !!!!
		m_product -> m_provider -> remove();
		//list les fournisseurs
		listProvidersToTable("NAME", "", "");
	}
}

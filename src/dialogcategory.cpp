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

#include "dialogcategory.h"
#include "ui_dialogcategory.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColorDialog>

DialogCategory::DialogCategory(product *prod, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogCategory)
{
	ui->setupUi(this);
	m_product = prod;
	//list les categories
	listCategoriesToTable("NAME", "", "");
	//Event
	connect(ui->lineEdit_name, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
}


DialogCategory::~DialogCategory()
{
	delete ui;
}

/**
	 Test les conditions pour activer les boutons
  */
void DialogCategory::checkConditions()
{
	if(!ui->lineEdit_name->text().isEmpty()){
		ui->pushButton_add->setEnabled(true);
		ui->pushButton_edit->setEnabled(true);
	}
	else{
		ui->pushButton_add->setEnabled(false);
		ui->pushButton_edit->setEnabled(false);
	}
}

/**
	Affiche les categories
*/
void DialogCategory::listCategoriesToTable(QString order, QString filter, QString field)
{
	category::categoryList clist;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget->clear();
	for (int i=ui->tableWidget->rowCount()-1; i >= 0; --i)
		ui->tableWidget->removeRow(i);
	for (int i=ui->tableWidget->columnCount()-1; i>=0; --i)
		ui->tableWidget->removeColumn(i);

	ui->tableWidget->setSortingEnabled(false);
	//Style de la table produit
	ui->tableWidget->setColumnCount(1);
	ui->tableWidget->setColumnWidth(0,200);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Nom");
	ui->tableWidget->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_product->m_category->getcategoryList(clist, order, filter, field);

	// list all products
	for(int i=0,j=0; i<clist.name.count();i++){
		QTableWidgetItem *item_NAME      = new QTableWidgetItem();
		item_NAME->setText( clist.name.at(i) );
		//definir le tableau
		ui->tableWidget->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget->setItem(j, 0, item_NAME);
		j++;
	}
	ui->tableWidget->setSortingEnabled(true);
	ui->tableWidget->selectRow(0);
}


void DialogCategory::on_tableWidget_itemSelectionChanged()
{
	int m_index = ui->tableWidget->currentRow();
	m_product->m_category->loadFromName( ui->tableWidget->item(m_index, 0)->text() );
	//Charge les champs
	ui->lineEdit_name->setText(m_product->m_category->getName());
	if((m_product->m_category->getColor().isValid()) && (m_product->m_category->getColor() != QColor(0, 0, 0, 255)) ){
		setColorLabel(m_product->m_category->getColor()) ;
	}else
		on_toolButton_colorDefault_clicked();
}

/**
	Supprimer la categorie
  */
void DialogCategory::on_pushButton_del_clicked()
{
	int ret = QMessageBox::warning(this, tr("Attention"),
								   tr("Voulez-vous vraiment supprimer la categorie:<br><b>")
								   + m_product->m_category->getName()
								   +"</b><br>Ceci supprimera aussi la categorie dans la liste des produits",
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		// Avant il faut supprimer toutes les references a ce fournisseur
		// dans la table produits !!!!
		//TODO: avoir si ca pose pas de soucis pour les produits...?
		m_product->m_category->remove();
		//list les categories
		listCategoriesToTable("NAME", "", "");
		ui->lineEdit_name->setText("");
	}
}

/**
	Ajouter la categorie
  */
void DialogCategory::on_pushButton_add_clicked()
{
	if(m_product->m_category->isHere(ui->lineEdit_name->text())){
		QMessageBox::warning(this, tr("Attention"), tr("Cat&eacute;gorie d&eacute;ja pr&eacute;sente...<br>Merci de changer de nom") );
		return;
	}
	m_product->m_category->setName(ui->lineEdit_name->text());
	m_product->m_category->setColor( m_color );

	m_product->m_category->create();
	//list les categories
	listCategoriesToTable("NAME", "", "");
	ui->lineEdit_name->setText("");
}

/**
	Modifer la categorie
  */
void DialogCategory::on_pushButton_edit_clicked()
{
	/*if(m_product->m_category->isHere(ui->lineEdit_name->text())){
		QMessageBox::warning(this, tr("Attention"), tr("Cat&eacute;gorie d&eacute;ja pr&eacute;sente...<br>Merci de changer de nom") );
		return;
	}*/
	m_product->m_category->setName(ui->lineEdit_name->text());
	m_product->m_category->setColor( m_color );

	m_product->m_category->update();
	//list les categories
	listCategoriesToTable("NAME", "", "");
	ui->lineEdit_name->setText("");
}

/**
	Choisir une couleur pour la categorie
  */
void DialogCategory::on_pushButton_color_clicked()
{
	QColorDialog dcolor;
	dcolor.setOption(QColorDialog::DontUseNativeDialog, true);
	//on selectionne la couleur
	if( dcolor.exec() == QDialog::Accepted ){
		//change la couleur du bouton
		setColorLabel( dcolor.selectedColor() );
	}
}

/**
	Retablir la couleur
  */
void DialogCategory::on_toolButton_colorDefault_clicked()
{
	m_color = QColor();
	ui->label_color->setPalette(QPalette(m_color));
	ui->label_color->setAutoFillBackground(false);
}

/**
	Affiche la couleur
	@param colour a afficher
*/
void DialogCategory::setColorLabel(const QColor &color) {
	m_color = color;
	ui->label_color->setPalette(QPalette(m_color));
	ui->label_color->setAutoFillBackground(true);
}


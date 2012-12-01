#include "dialogtax.h"
#include "ui_dialogtax.h"

#include <QMessageBox>

DialogTax::DialogTax(tax *t, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogTax)
{
	m_tax = t;
	ui->setupUi(this);

	listTaxToTable("TAX","","");
}

DialogTax::~DialogTax()
{
	delete ui;
}


/**
	Affiche les tax
*/
void DialogTax::listTaxToTable(QString order, QString filter, QString field)
{
	tax::taxList list;

	//Clear les items, attention tjs utiliser la fonction clear()
	ui->tableWidget->clear();
	for (int i=ui->tableWidget->rowCount()-1; i >= 0; --i)
		ui->tableWidget->removeRow(i);
	for (int i=ui->tableWidget->columnCount()-1; i>=0; --i)
		ui->tableWidget->removeColumn(i);

	ui->tableWidget->setSortingEnabled(false);
	//Style de la table
	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setColumnWidth(0,25);
	ui->tableWidget->setColumnWidth(1,50);
	ui->tableWidget->setColumnWidth(2,200);
#ifdef QT_NO_DEBUG
	ui->tableWidget->setColumnHidden(0 , true); //cache la colonne ID ou DEBUG
#endif
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList titles;
	titles.clear();
	titles << tr("Id") << tr("Valeur") << tr("Description");
	ui->tableWidget->setHorizontalHeaderLabels( titles );

	//Recuperation des donnees presentent dans la bdd
	m_tax->getTaxList(list, order, filter, field);

	// list all tax
	for(unsigned int i=0,j=0; i<list.value.size();i++){
		QTableWidgetItem *item_ID      = new QTableWidgetItem();
		QTableWidgetItem *item_VAL      = new QTableWidgetItem();
		QTableWidgetItem *item_DESC      = new QTableWidgetItem();
		item_ID->setData(Qt::DisplayRole, list.id.at(i));
		item_VAL->setData(Qt::DisplayRole, list.value.at(i));
		item_DESC->setData(Qt::DisplayRole, list.description.at(i));
		//definir le tableau
		ui->tableWidget->setRowCount(j+1);
		//remplir les champs
		ui->tableWidget->setItem(j, 0, item_ID);
		ui->tableWidget->setItem(j, 1, item_VAL);
		ui->tableWidget->setItem(j, 2, item_DESC);
		j++;
	}
	ui->tableWidget->setSortingEnabled(true);
	ui->tableWidget->selectRow(0);
}

/**
	Ajoute une tax
  */
void DialogTax::on_pushButton_add_clicked()
{
	if(m_tax->isHere(ui->doubleSpinBox_tax->value())){
		QMessageBox::warning(this, tr("Attention"), tr("Tax d&eacute;ja pr&eacute;sente...<br>Merci de changer de valeur") );
		return;
	}
	m_tax->setValue(ui->doubleSpinBox_tax->value()  );
	m_tax->setDescription( ui->lineEdit_description->text() );

	m_tax->create();
	//list les tax
	listTaxToTable("TAX", "", "");
	ui->doubleSpinBox_tax->setValue(0);
	ui->lineEdit_description->setText("");
}

/**
	Modifie la tax
  */
void DialogTax::on_pushButton_edit_clicked()
{
	m_tax->setValue(ui->doubleSpinBox_tax->value()  );
	m_tax->setDescription( ui->lineEdit_description->text() );

	m_tax->update();
	//list les tax
	listTaxToTable("TAX", "", "");
	ui->doubleSpinBox_tax->setValue(0);
	ui->lineEdit_description->setText("");
}

/**
  Supprime la tax
  */
void DialogTax::on_pushButton_del_clicked()
{
	QString f;
	int ret = QMessageBox::warning(this, tr("Attention"),
								   tr("Voulez-vous vraiment supprimer la tax:<br><b>")
								   + f.setNum(m_tax->getValue(),'f',2),
								   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

	if(ret == QMessageBox::Yes){
		m_tax->remove();
		//list les tax
		listTaxToTable("TAX", "", "");
		ui->doubleSpinBox_tax->setValue(0);
		ui->lineEdit_description->setText("");
	}
}

/**
	Sur la selection dun item
  */
void DialogTax::on_tableWidget_itemSelectionChanged()
{
	int m_index = ui->tableWidget->currentRow();
	m_tax->loadFromID( ui->tableWidget->item(m_index, 0)->text().toInt() );
	//Charge les champs
	ui->lineEdit_description->setText(m_tax->getDescription() );
	ui->doubleSpinBox_tax->setValue( m_tax->getValue() );
}

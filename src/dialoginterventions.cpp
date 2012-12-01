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

#include "dialoginterventions.h"
#include "ui_dialoginterventions.h"

#include <QMessageBox>

DialogInterventions::DialogInterventions(intervention* inter, bool tax, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInterventions)
{
    ui->setupUi(this);
    m_inter = inter;

    //Affiche ou pas les taxes
    ui->comboBox_tax->setVisible(tax);
    if(tax) ui->label_tax->setText( tr("HT") );
    else     ui->label_tax->setText( tr("TTC") );
    ui->label_titletax->setVisible(tax);

    //sauvegarde les IDs
    m_idCustomer = m_inter->getIdCustomer();
    m_idInter = m_inter->getId();
    m_inter->setIdCustomer(0); //Mettre l intervention commun

    ui->groupBox_IntCom->setVisible(false);
    this->adjustSize();

    ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    //Event
    connect(ui->lineEdit_Desc, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
    connect(this, SIGNAL(accepted()), this, SLOT(setIdCustomer()));
    connect(this, SIGNAL(rejected()), this, SLOT(setIdCustomer()));

}

DialogInterventions::~DialogInterventions()
{
    delete ui;
}

/**
   Event sur la fermeture du dialog
 */
void DialogInterventions::closeEvent(QCloseEvent* event) {
    emit(setIdCustomer());
}

/**
    On restaure les IDs
*/
void DialogInterventions::setIdCustomer() {
    m_inter->setIdCustomer(m_idCustomer);
    m_inter->setId(m_idInter);
}

/**
     Test les conditions pour activer les boutons
  */
void DialogInterventions::checkConditions()
{
    if(!ui->lineEdit_Desc->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->pushButton_add->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    }
    else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->pushButton_add->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
    }
}

/**
    Renseigne le titre
    @param titre de la fenetre
  */
void DialogInterventions::setTitle(QString val){
    ui->labelTitle->setText(val);
}


/**
    Decouvre les champs pour les interventions communes
  */
void DialogInterventions::on_pushButton_more_clicked()
{
    ui->groupBox_IntCom->setVisible( !ui->groupBox_IntCom->isVisible() );
    /// TODO: A voir pour l ajustement de la fenetre!
    this->adjustSize();
    //Rafraichir la liste des interventions generales
    listInterCommToTable();
}

/**
    Sur la validation on renseigne la class intervention
  */
void DialogInterventions::on_buttonBox_accepted()
{
    m_inter->setDate( ui->dateTimeEdit->dateTime() );
    m_inter->setPrice( ui->doubleSpinBox->value() );
    m_inter->setDescription( ui->lineEdit_Desc->text() );
}

/**
    Affiche les interventions generales
*/
void DialogInterventions::listInterCommToTable()
{
    intervention::InterventionList ilist;

    //Clear les items, attention tjs utiliser la fonction clear()
    ui->tableWidget_Inter->clear();
    for (int i=ui->tableWidget_Inter->rowCount()-1; i >= 0; --i)
        ui->tableWidget_Inter->removeRow(i);
    for (int i=ui->tableWidget_Inter->columnCount()-1; i>=0; --i)
        ui->tableWidget_Inter->removeColumn(i);


    ui->tableWidget_Inter->setSortingEnabled(false);
    //Style de la table de l intervention
    ui->tableWidget_Inter->setColumnCount(3);
    ui->tableWidget_Inter->setColumnWidth(1,225);
    ui->tableWidget_Inter->setColumnWidth(2,100);
    ui->tableWidget_Inter->setColumnHidden(0, true); //On cache la colonne des ID
    ui->tableWidget_Inter->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Inter->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_Inter->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList titles;
    titles << tr("Id") << tr("Description") << tr("Prix") ;
    ui->tableWidget_Inter->setHorizontalHeaderLabels( titles );

    //Recuperation des donnees presentent dans la bdd
    m_inter->getInterventionList(ilist, 0, "DESCRIPTION", "", "");

    // list all customers
    for(unsigned int i=0; i<ilist.id.size(); i++){
        QTableWidgetItem *item_ID      = new QTableWidgetItem();
        QTableWidgetItem *item_PRICE     = new QTableWidgetItem();
        QTableWidgetItem *item_DESCRIPTION     = new QTableWidgetItem();

        item_ID->setData(Qt::DisplayRole, QString::number(ilist.id.at(i)));
        item_PRICE->setData(Qt::DisplayRole, ilist.price.at(i));
        item_DESCRIPTION->setData(Qt::DisplayRole, ilist.description.at(i));

        //definir le tableau
        ui->tableWidget_Inter->setRowCount(i+1);

        //remplir les champs
        ui->tableWidget_Inter->setItem(i, 0, item_ID);
        ui->tableWidget_Inter->setItem(i, 1, item_DESCRIPTION);
        ui->tableWidget_Inter->setItem(i, 2, item_PRICE);
    }
    ui->tableWidget_Inter->setSortingEnabled(true);

    //Si pas de produit on desactive les boutons
    if( ui->tableWidget_Inter->rowCount() <=0 ) {
        ui->pushButton_edit->setEnabled(false);
        ui->pushButton_del->setEnabled(false);
    }else{
        ui->pushButton_edit->setEnabled(true);
        ui->pushButton_del->setEnabled(true);
    }
}

/**
    Sur la selection d une intervention generales
  */
void DialogInterventions::on_tableWidget_Inter_itemSelectionChanged()
{
    int m_index = ui->tableWidget_Inter->currentRow();
    m_inter->loadFromID( ui->tableWidget_Inter->item(m_index, 0)->text().toInt() );
    //Charge les champs
    ui->doubleSpinBox->setValue( m_inter->getPrice() );
    ui->lineEdit_Desc->setText( m_inter->getDescription() );
}

/**
    Ajoute un intervention generale
  */
void DialogInterventions::on_pushButton_add_clicked()
{
    if( !m_inter->commIsHere(ui->lineEdit_Desc->text()) ) {
        m_inter->setPrice( ui->doubleSpinBox->value() );
        m_inter->setDescription( ui->lineEdit_Desc->text() );
        m_inter->create();
        //rafraichir la liste
        listInterCommToTable();
    }
    else{
        QMessageBox::warning(this, tr("Attention"), tr("Cat&eacute;gorie d&eacute;ja pr&eacute;sente...<br>Merci de changer de nom") );
        return;
    }
}

/**
      Supprime un intervention generale
  */
void DialogInterventions::on_pushButton_del_clicked()
{
    int ret = QMessageBox::warning(this, tr("Attention"),
                                   tr("Voulez-vous vraiment supprimer cette intervention:<br><b>")
                                   + m_inter->getDescription(),
                                   QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);

    if(ret == QMessageBox::Yes){
        m_inter->remove();
        //rafraichir la liste
        listInterCommToTable();
    }
}

/**
    Modification de l intervention generale
  */
void DialogInterventions::on_pushButton_edit_clicked()
{
/// TODO: Avoir
   // if( !m_inter->commIsHere(ui->lineEdit_Desc->text()) ) {
        m_inter->setPrice( ui->doubleSpinBox->value() );
        m_inter->setDescription( ui->lineEdit_Desc->text() );
        m_inter->update();
        //rafraichir la liste
        listInterCommToTable();
   // }
  /*  else{
        QMessageBox::warning(this, tr("Attention"), tr("Cat&eacute;gorie d&eacute;ja pr&eacute;sente...<br>Merci de changer de nom") );
        return;
    }*/
}


/**
    Charge les informations de l intervention
  */
void DialogInterventions::loadValuesFromInter() {
    ui->dateTimeEdit->setDateTime( m_inter->getDate() );
    ui->doubleSpinBox->setValue( m_inter->getPrice() );
    ui->lineEdit_Desc->setText( m_inter->getDescription() );
}



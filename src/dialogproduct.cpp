/**
  This file is a part of mcercle
  Copyright (C) 2010-2012 Cyril FRAUSTI

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

#include "dialogproduct.h"
#include "ui_dialogproduct.h"
#include "dialogproviders.h"
#include "dialogcategory.h"
#include <QPushButton>
#include <QValidator>
#include <QMessageBox>

DialogProduct::DialogProduct(QLocale &lang, product *p, bool tax, unsigned char type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProduct)
{
    ui->setupUi(this);
    m_type = type;
    m_product = p;
    m_lang = lang;
    // TAX
  /*  ui->comboBox_tax->addItem(tr("5,5%"));
    ui->comboBox_tax->addItem(tr("19,6%"));
    ui->comboBox_tax->setCurrentIndex(1);*/

    ui->pushButton_add_edit->setEnabled(false);
    //
    if(m_type == PRODUCT_ADD){
        clearEdits();
        ui->pushButton_add_edit->setText(tr("Ajouter"));
        ui->pushButton_add_edit->setIcon(QIcon(":/app/insert"));
    }else{
        ui->pushButton_add_edit->setText(tr("Modifier"));
        ui->pushButton_add_edit->setIcon(QIcon(":/app/Edit"));
    }

    //Contraintes sur les champs de saisie
    QIntValidator* m_valid_int = new QIntValidator(-99999,99999,this);
    ui->lineEdit_stock->setValidator(m_valid_int);
    ui->lineEdit_stockAlert->setValidator(m_valid_int);

    //Affiche ou pas les taxes
    ui->comboBox_tax->setVisible(tax);
    if(tax) ui->label_tax->setText( tr("HT") );
    else     ui->label_tax->setText( tr("TTC") );
    ui->label_titletax->setVisible(tax);

    //Chargement de la liste des fournisseurs
    loadProviderList();
    //Chargement de la liste des categories
    loadCategoryList();

    //Event
    connect(ui->lineEdit_code, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
    connect(ui->lineEdit_description, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
    connect(ui->lineEdit_stock, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
    connect(ui->lineEdit_stockAlert, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
}

DialogProduct::~DialogProduct()
{
    delete ui;
}

void DialogProduct::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
    Renseigne le titre
    @param titre de la fenetre
  */
void DialogProduct::setTitle(QString val){
    ui->labelTitle->setText(val);
}

/**
    Effacer les champs de saisies
  */
void DialogProduct::clearEdits(){
    ui->lineEdit_code->setText("");
    ui->lineEdit_description->setText("");
    ui->lineEdit_stock->setText("0");
    ui->lineEdit_stockAlert->setText("0");
    ui->doubleSpinBox_buyingPrice->setValue(0);
    ui->doubleSpinBox_price->setValue(0);
}

/**
     Test les conditions pour activer le bouton Ajouter/modifier
  */
void DialogProduct::checkConditions()
{
    if((!ui->lineEdit_code->text().isEmpty())
        && (!ui->lineEdit_description->text().isEmpty())
        && (!ui->lineEdit_stock->text().isEmpty())
        && (!ui->lineEdit_stockAlert->text().isEmpty())
        )
        ui->pushButton_add_edit->setEnabled(true);
    else
        ui->pushButton_add_edit->setEnabled(false);
}

/**
  Renseigne les informations produit
  */
void DialogProduct::loadValuesFormProduct(){
 //(CODE, QUANTITY, SELLING_PRICE, TAX, DESCRIPTION, STOCK, STOCK_WARNING, STATE)
    ui->lineEdit_code->setText( m_product->getCode() );
    ui->lineEdit_description->setText( m_product->getName() );

    //Prix Achat/Vente    
    ui->doubleSpinBox_buyingPrice->setValue( m_product->getBuyingPrice() );
    ui->doubleSpinBox_price->setValue( m_product->getSellingPrice() );

    // Tax
   /* float val = m_product->getTax();
    if(val == 5.5)ui->comboBox_tax->setCurrentIndex(0);
    else ui->comboBox_tax->setCurrentIndex(1);*/

    //Stock
    ui->lineEdit_stock->setText( QString::number(m_product->getStock()) );
    ui->lineEdit_stockAlert->setText( QString::number(m_product->getStockWarning()) );

    //State
    if( m_product->getState() == product::OK )ui->radioButton_OK->setChecked( true );
    else ui->radioButton_Discontinued->setChecked( true );
}


/**
    Edition des fournisseurs
  */
void DialogProduct::on_toolButton_editProvider_clicked()
{
    DialogProviders *m_DialogProviders = new DialogProviders(m_product);
    m_DialogProviders->setModal(true);
    m_DialogProviders->exec();
    delete m_DialogProviders;
    //Rechargement de la liste des fournisseurs
    loadProviderList();
}

/**
    Charge la liste des fournisseur
  */
void DialogProduct::loadProviderList(){
    int select=-1;
    QString name;
    //Fournisseurs
    provider::ProviderList plist;
    //Recuperation des donnees presentent dans la bdd
    m_product->m_provider->getProviderList(plist, "NAME", "", "");
    ui->comboBox_providers->clear();
    name = m_product->getProvider();
    for(int i=0; i<plist.name.count(); i++){
        //Si le fournisseur est celui courant on charge lindex
        if(name == plist.name.at(i)){
                select = i;
        }
        ui->comboBox_providers->addItem( plist.name.at(i) );
    }
    ui->comboBox_providers->setCurrentIndex(select);
}

/**
    Charge la liste des categories
  */
void DialogProduct::loadCategoryList(){
    int select=-1;
    QString name;
    //categories
    category::categoryList clist;
    //Recuperation des donnees presentent dans la bdd
    m_product->m_category->getcategoryList(clist, "NAME", "", "");
    ui->comboBox_categories->clear();
    name = m_product->getCategory();
    ui->comboBox_categories->addItem(""); // Vide pour pas de categorie
    for(int i=0; i<clist.name.count(); i++){
        //Si le fournisseur est celui courant on charge lindex
        if(name == clist.name.at(i)){
                select = i+1; //+1 pour le vide
        }
        ui->comboBox_categories->addItem( clist.name.at(i) );
    }
    if(m_type == PRODUCT_ADD)select = 0;
    ui->comboBox_categories->setCurrentIndex(select);
}

/**
    Sur le changement du combobox des fournisseurs
    on rechage l'objet
  */
void DialogProduct::on_comboBox_providers_currentIndexChanged(QString name)
{
    m_product->m_provider->loadFromName(name);
    m_product->setProviderId(m_product->m_provider->getId());
}


/**
    Sur le changement du combobox des categories
    on rechage l'objet
  */
void DialogProduct::on_comboBox_categories_currentIndexChanged(QString name)
{
    m_product->m_category->loadFromName(name);
    m_product->setCategoryId(m_product->m_category->getId());
}


/**
    Ajout ou modification
  */
void DialogProduct::on_pushButton_add_edit_clicked()
{
    //Test si un produit existe
    if(m_type == PRODUCT_ADD) {
        if( m_product->isHere(ui->lineEdit_code->text()) ){
        QMessageBox::warning(this, tr("Attention"), tr("Code Produit d&#233;ja pr&#233;sent...<br>Merci de changer") );
        return;
        }
    }

    //Quelques test Car les class par defaut ne suffisent pas
    //Attention a la langue pour les float
    float buyingPrice, SellingPrice;
    buyingPrice = ui->doubleSpinBox_buyingPrice->value();
    SellingPrice = ui->doubleSpinBox_price->value();

    if((buyingPrice > 999999) || (SellingPrice > 999999) ){
        QMessageBox::warning(this, tr("Attention"), tr("Valeur num&#233;rique d&#233;pass&#233;e:<br><b>999 999,00</b> MAX...<br><br>Merci de changer") );
        return;
    }

    //On charge lobjet product
    m_product->setCode( ui->lineEdit_code->text() );
    m_product->setName(ui->lineEdit_description->text() );

    m_product->setBuyingPrice( buyingPrice );
    m_product->setSellingPrice( SellingPrice );

    /*if(ui->comboBox_tax->currentIndex() == 0) m_product->setTax( 5.5 );
    else m_product->setTax( 19.6 );
*/
    m_product->setTax( 0 );

    m_product->setStock( ui->lineEdit_stock->text().toInt() );
    m_product->setStockWarning( ui->lineEdit_stockAlert->text().toInt() );

    if( ui->radioButton_OK->isChecked() )m_product->setState( product::OK );
    else m_product->setState( product::DISCONTINUED );

    //Ajouter un produit
    if(m_type == PRODUCT_ADD) {
        if( m_product->create() ){
            //Demande un rafraichissement de la liste des produits
            emit askRefreshList();
            clearEdits();
        }
    }
    //Modifier le produit
    else {
        if( m_product->update() ){
            //Demande un rafraichissement de la liste des produits
            emit askRefreshList();
            this->close();
        }
    }
}


/**
    Fermeture du dialog
  */
void DialogProduct::on_pushButton_close_clicked()
{
    this->close();
}

/**
    Edition Les categories
  */
void DialogProduct::on_toolButton_editCategorie_clicked()
{
    DialogCategory *m_DialogCategory = new DialogCategory(m_product);
    m_DialogCategory->setModal(true);
    m_DialogCategory->exec();
    delete m_DialogCategory;
    //Rechargement de la liste des categories
    loadCategoryList();
}


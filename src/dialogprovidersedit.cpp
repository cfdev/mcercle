#include "dialogprovidersedit.h"
#include "ui_dialogprovidersedit.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QPushButton>

DialogProvidersEdit::DialogProvidersEdit(product *prod, unsigned char type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProvidersEdit)
{
    ui->setupUi(this);
    m_product = prod;
    m_type = type;
    if(m_type == PROVIDER_ADD){
        setTitle(tr("Ajouter un fournisseur"));
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else{
        setTitle(tr("Modifier le frounisseur"));
        loadValuesFormProvider();
    }
    //Event
    connect(ui->lineEdit_name, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
}

DialogProvidersEdit::~DialogProvidersEdit()
{
    delete ui;
}

/**
     Test les conditions pour activer le bouton QDialogButtonBox::Ok
  */
void DialogProvidersEdit::checkConditions()
{
    if(!ui->lineEdit_name->text().isEmpty())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

/**
    Renseigne le titre
    @param titre de la fenetre
  */
void DialogProvidersEdit::setTitle(QString val){
    ui->GroupBox->setTitle(val);
}

/**
  Renseigne les informations du fournisseur
  */
void DialogProvidersEdit::loadValuesFormProvider(){
    //Charge les champs
    ui->lineEdit_name->setText(m_product->m_provider->getName());
    ui->lineEdit_email->setText(m_product->m_provider->getEmail());
    ui->lineEdit_phone->setText(m_product->m_provider->getPhoneNumber());
    ui->lineEdit_fax->setText(m_product->m_provider->getFaxNumber());
    ui->lineEdit_city->setText(m_product->m_provider->getCity());
    ui->lineEdit_zipCode->setText(m_product->m_provider->getZipCode());
    ui->lineEdit_country->setText(m_product->m_provider->getCountry());
    ui->lineEdit_add1->setText(m_product->m_provider->getAddress1());
    ui->lineEdit_add2->setText(m_product->m_provider->getAddress2());
    ui->lineEdit_add3->setText(m_product->m_provider->getAddress3());
    ui->lineEdit_contact->setText(m_product->m_provider->getContact());
}

/**
      Chargement des informations dans l objet provider sur l acceptation
      de la fenetre
  */
void DialogProvidersEdit::on_buttonBox_accepted()
{
    if(m_type == PROVIDER_ADD){
        //TODO: Test d'un doublon Avoir pour le faire hors de la fonction accept!!
        if(m_product->m_provider->isHere( ui->lineEdit_name->text() )){
            QMessageBox::warning(this, tr("Attention"), tr("Fournisseur d&eacute;ja pr&eacute;sent...<br>Merci de changer de nom") );
            this->reject();
            return;
        }
    }
    else{
        int id = m_product->m_provider->isHere( ui->lineEdit_name->text());
        //Si l ID existe et qu'il est different de celui charge
        //Alors le nom du fournisseur est deja prensent
        if( (id > 0) &&( id != m_product->m_provider->getId()) ){
            QMessageBox::warning(this, tr("Attention"), tr("Fournisseur d&eacute;ja pr&eacute;sent...<br>Merci de changer de nom") );
            this->reject();
            return;
        }
    }
    m_product->m_provider->setName( ui->lineEdit_name->text() );
    m_product->m_provider->setEmail(ui->lineEdit_email->text());
    m_product->m_provider->setPhone(ui->lineEdit_phone->text(),ui->lineEdit_fax->text());
    m_product->m_provider->setCity(ui->lineEdit_city->text());
    m_product->m_provider->setZipCode(ui->lineEdit_zipCode->text());
    m_product->m_provider->setCountry(ui->lineEdit_country->text());
    m_product->m_provider->setAddress(ui->lineEdit_add1->text(),
                           ui->lineEdit_add2->text(),
                           ui->lineEdit_add3->text()
                           );
    m_product->m_provider->setContact(ui->lineEdit_contact->text());
}

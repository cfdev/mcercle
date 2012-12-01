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

#include "dialogcustomer.h"
#include "ui_dialogcustomer.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QPushButton>

DialogCustomer::DialogCustomer(customer* c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomer)
{
    ui->setupUi(this);
    m_customer = c;
    ui->comboBoxGender->addItem(tr("M."));
    ui->comboBoxGender->addItem(tr("Mme"));
    int index=-1;
    if(m_customer->getType() == customer::PEOPLE_TYPE){
        QString gender = m_customer->getGender();
        if(gender == tr("M."))  index=0;
        if(gender == tr("Mme")) index=1;
    }

    ui->comboBoxGender->setCurrentIndex(index);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    //valid le particulier par defaut
    ui->radioButton_people->setChecked(true);
    on_radioButton_people_clicked();

    //Event
    connect(ui->lineEdit_LastName, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
    connect(ui->lineEdit_FirstName, SIGNAL(textChanged(const QString &)), this, SLOT(checkConditions()));
}

DialogCustomer::~DialogCustomer()
{
    delete ui;
}

void DialogCustomer::changeEvent(QEvent *e)
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
    Type de client particulier
  */
void DialogCustomer::on_radioButton_people_clicked()
{
    ui->lineEdit_FirstName->setVisible(true);
    ui->comboBoxGender->setVisible(true);
    ui->label_lastname->setVisible(true);
    ui->label_asterix_ltname->setVisible(true);
    ui->lineEdit_compagny->setVisible(false);
    ui->label_compagny->setVisible(false);
    ui->dateEdit->setVisible(true);
    ui->label_civ->setVisible(true);
    ui->label_date->setVisible(true);

    checkConditions();
}

/**
    Type de client societe
  */
void DialogCustomer::on_radioButton_compagny_clicked()
{
    ui->lineEdit_FirstName->setVisible(false);
    ui->label_lastname->setVisible(false);
    ui->label_asterix_ltname->setVisible(false);
    ui->lineEdit_compagny->setVisible(true);
    ui->label_compagny->setVisible(true);
    ui->comboBoxGender->setVisible(false);
    ui->dateEdit->setVisible(false);
    ui->label_civ->setVisible(false);
    ui->label_date->setVisible(false);

    checkConditions();
}

/**
  Renseigne les informations clients
  */
void DialogCustomer::loadValuesFormCustomer(){

    if(m_customer->getType() == customer::PEOPLE_TYPE){
        on_radioButton_people_clicked();
        ui->radioButton_people->setChecked(true);
        ui->lineEdit_FirstName->setText( m_customer->getFirstName() );
        ui->comboBoxGender->setEditText( m_customer->getGender() );
        ui->dateEdit->setDate(m_customer->getBirthday());
    }
    else{
        on_radioButton_compagny_clicked();
        ui->radioButton_compagny->setChecked(true);
        ui->lineEdit_compagny->setText( m_customer->getDesCompagny() );
    }

 //(CODE, LASTNAME, FIRSTNAME, GENDER, BIRTHDAY, ADDRESS1, ADDRESS2, ADDRESS3, ZIPCODE, CITY, PHONENUMBER, MOBILENUMBER, EMAIL, PROFESSION, COMMENTS)
    ui->lineEdit_LastName->setText( m_customer->getLastName() );

    ui->lineEdit_Add1->setText( m_customer->getAddress1() );
    ui->lineEdit_Add2->setText( m_customer->getAddress2() );
    ui->lineEdit_Add3->setText( m_customer->getAddress3() );

    ui->lineEdit_ZipCode->setText( m_customer->getZipCode() );
    ui->lineEdit_City->setText( m_customer->getCity() );
    ui->lineEdit_country->setText( m_customer->getCountry() );

    ui->lineEdit_PhoneNumber->setText( m_customer->getPhoneNumber() );
    ui->lineEdit_MobilePhone->setText( m_customer->getMobileNumber() );

    ui->lineEdit_Email->setText( m_customer->getEmail() );
    ui->lineEdit_Profession->setText( m_customer->getProfession() );

    ui->lineEdit_Comments->setText( m_customer->getComments() );
    ui->checkBox_promo->setChecked( m_customer->getSendingPubEmail() );
}

/**
    Renseigne le titre
    @param titre de la fenetre
  */
void DialogCustomer::setTitle(QString val){
    ui->labelTitle->setText(val);
}


/**
     Test les conditions pour activer le bouton QDialogButtonBox::Ok
  */
void DialogCustomer::checkConditions()
{
    if(ui->radioButton_people->isChecked()){
        if((!ui->lineEdit_LastName->text().isEmpty()) && (!ui->lineEdit_FirstName->text().isEmpty()))
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        else
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else{
        if(!ui->lineEdit_LastName->text().isEmpty())
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        else
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

/**
      Chargement des informations dans l objet customer
  */
void DialogCustomer::on_buttonBox_accepted()
{
    m_customer->setName(ui->lineEdit_FirstName->text(), ui->lineEdit_LastName->text());
    if(ui->radioButton_people->isChecked())
            m_customer->setType( customer::PEOPLE_TYPE );
    else    m_customer->setType( customer::COMPAGNY_TYPE );


    m_customer->setGender( ui->comboBoxGender->currentText() );
    m_customer->setBirthday( ui->dateEdit->date() );
    m_customer->setDesCompagny( ui->lineEdit_compagny->text() );

    m_customer->setAddress(ui->lineEdit_Add1->text(),
                           ui->lineEdit_Add2->text(),
                           ui->lineEdit_Add3->text());

    m_customer->setZipCode( ui->lineEdit_ZipCode->text() );
    m_customer->setCity( ui->lineEdit_City->text() );
    m_customer->setCountry( ui->lineEdit_country->text() );

    m_customer->setPhone(ui->lineEdit_PhoneNumber->text(), ui->lineEdit_MobilePhone->text());
    m_customer->setEmail( ui->lineEdit_Email->text() );
    m_customer->setProfession( ui->lineEdit_Profession->text() );
    m_customer->setComments( ui->lineEdit_Comments->text() );
    m_customer->setSendingPubEmail( ui->checkBox_promo->checkState() );
}


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

#include "dialogwaiting.h"
#include "ui_dialogwaiting.h"
#include <QPushButton>

DialogWaiting::DialogWaiting(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogWaiting)
{
	ui->setupUi(this);
	setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

DialogWaiting::~DialogWaiting()
{
	delete ui;
}

void DialogWaiting::setProgressBar(int val){
	ui->progressBar->setValue(val);
	if(val >= ui->progressBar->maximum()) {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	// Rafraichit l'affichage
	qApp->processEvents();
}

void DialogWaiting::upProgressBar(){
	int val = ui->progressBar->value();
	ui->progressBar->setValue( ++val );
	if(val >= ui->progressBar->maximum()) {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	// Rafraichit l'affichage
	qApp->processEvents();
}

void DialogWaiting::setProgressBarRange(int min, int max){
	ui->progressBar->setRange(min,max);
	//if infinity display disable the ok button
	if((min == 0) && (max == 0)){
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

void DialogWaiting::setTitle(const QString& val){
	QString title="<b> "+val+" </b>";
	ui->labelTitle->setText(title);
}

void DialogWaiting::setDetail(const QString& val){
	ui->label_detail->setText(val);
}

void DialogWaiting::refresh() {
	// Rafraichit l'affichage
	qApp->processEvents();
}

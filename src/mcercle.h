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
#ifndef _MCERCLE_H
#define _MCERCLE_H

namespace MCERCLE {
// Version et version de bdd supporte
	const QString Version = "15.04";
	const QString CodeName = "";
	const int Dbase_support = 8;

// Type paiement: espece, cheque, CB, TIP, virement, prelevement, autre
	const QString CASH ="CA";
	const QString CHECK ="CH";
	const QString CREDIT_CARD ="CC";
	const QString INTERBANK ="IB";
	const QString TRANSFER ="TR";
	const QString DEBIT ="DE";
	const QString OTHER ="OT";
	
	//DEVIS
	enum{PROPOSAL_WRITING, PROPOSAL_PROPOSED, PROPOSAL_VALIDATED};
	//FACTURE
	enum{INV_UNPAID , INV_PAID, INV_OVERDUE, INV_CANCEL};
	enum{TYPE_INV=0 , TYPE_PART, TYPE_CREDIT};
	//TYPE PRODUIT SERVICE
	const int SERVICE = 0;
	const int PRODUCT = 1;
			
	/*const QColor TABLE_BG_COLOR QColor(69,69,69,255);
	const QColor TABLE_TXT_COLOR QColor(219,219,219,255);*/
	
	enum UI {
		Dialog,
		Widget,
		Choice
	};
	
}
#endif

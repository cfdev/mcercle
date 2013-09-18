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

#include "table.h"

/**
 * @brief ItemOfTable::ItemOfTable
 * @param colorBG
 * @param colorTXT
 */
ItemOfTable::ItemOfTable(const QColor& colorBG, const QColor& colorTXT) {
	this -> setBackgroundColor( colorBG );
	this -> setTextColor( colorTXT );
}

/**
 * @brief Table::Table
 * @param parent
 */
Table::Table( QWidget *parent) : QTableWidget(parent) {
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/**
 * @brief Clear les items, attention tjs utiliser la fonction clear()
 */
void Table::raz() {
	clear();
	for (int i=rowCount()-1; i >= 0; --i)
		removeRow(i);
	for (int i=columnCount()-1; i>=0; --i)
		removeColumn(i);
}

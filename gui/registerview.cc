// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "registerview.h"
#include "definitions.h"
#include "util.h"
#include <QAbstractTableModel>
#include <QVector>

QVariant RegisterView::data(const QModelIndex &i, int role) const
{
	Qt::Alignment a;

	if (role == Qt::TextAlignmentRole) {
		a = Qt::AlignRight | Qt::AlignVCenter;
		return QVariant(static_cast<int>(a));
	}
	if (!i.isValid() || role != Qt::DisplayRole)
		return QVariant();
	return this->d[i.row()][i.column()];
}

QVariant RegisterView::headerData(int section, Qt::Orientation o, int role) const
{
	Qt::Alignment a;

	if (role == Qt::TextAlignmentRole) {
		a = Qt::AlignRight | Qt::AlignVCenter;
		return QVariant(static_cast<int>(a));
	}

	if (role != Qt::DisplayRole)
		return QVariant();

	if (o == Qt::Vertical) {
		return format_toggled_value(section * 4, this->is_hex);
	}
	return QVariant();
}

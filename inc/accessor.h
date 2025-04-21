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

#ifndef ACCESSOR_H
#define ACCESSOR_H
#include <iostream>

enum Accessor {
	IDLE,
	WRITE,
	MEM,
	EXEC,
	DCDE,
	FETCH,
	L1CACHE,
	SIDE,
};

std::ostream &operator<<(std::ostream &os, Accessor a);

#endif /* ACCESSOR_H_INCLUDED */

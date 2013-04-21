/*
avrprog - A Linux tool for the MikroElektronika (www.mikroe.com) AVRprog2 programming hardware.
Copyright (C) 2011  Andreas Hagmann, Embedded Computing Systems group - TU Wien

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "CEEPROMOptions.h"
#include <vector>

CEEPROMOptions::CEEPROMOptions(string options) : CMemoryOptions(options, BUFFER_OFFSET, vector<string>{".eeprom"}) {
	if (operation == READ && this->type == IMMEDIATE) {
		throw ProgramOptionsException("Cannot read from immediate value.");
	}
}

CEEPROMOptions::~CEEPROMOptions() {

}

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

#include "CFusesOptions.h"
#include "CFormat.h"
#include <vector>

CFusesOptions::CFusesOptions(string options) : CMemoryOptions(options, BUFFER_OFFSET, vector<string>{".fuse"}), lfuse(0), hfuse(0), efuse(0), numOfFuses(0) {
	// parse immediate values, write to buffer
	if ((operation == WRITE || operation == VERIFY) && this->type == IMMEDIATE) {
		switch (source.length()) {
		case 2:			// 1 fuse
			buffer = new uint8_t[1];
			buffer[0] = hexStringToByte(source.substr(0, 2));
			bufferLen = 1;
			break;
		case 5:			// 2 fuses
			buffer = new uint8_t[2];
			if (source.substr(2,1).compare(",") != 0) {
				throw ProgramOptionsException("Unknown fuse settings '" + source + "'.");
			}
			buffer[0] = hexStringToByte(source.substr(0, 2));
			buffer[1] = hexStringToByte(source.substr(3, 2));
			bufferLen = 2;
			break;
		case 8:			// 3 fuses
			buffer = new uint8_t[3];
			if (source.substr(2,1).compare(",") != 0) {
				throw ProgramOptionsException("Unknown fuse settings '" + source + "'.");
			}
			if (source.substr(5,1).compare(",") != 0) {
				throw ProgramOptionsException("Unknown fuse settings '" + source + "'.");
			}
			buffer[0] = hexStringToByte(source.substr(0, 2));
			buffer[1] = hexStringToByte(source.substr(3, 2));
			buffer[2] = hexStringToByte(source.substr(6, 2));
			bufferLen = 3;
			break;
		default:
			throw ProgramOptionsException("Unknown fuse settings '" + source + "'.");
			break;
		}
	}

	// parse fuse settings from buffer
	if (operation == WRITE || operation == VERIFY) {
		// check number of fuses
		if (bufferLen > 3 || bufferLen < 1) {
			throw ProgramOptionsException("Unknown fuse settings.");
		}

		numOfFuses = bufferLen;
		lfuse = buffer[0];
		if (bufferLen > 1)
			hfuse = buffer[1];
		if (bufferLen > 2)
			efuse = buffer[2];
	}
}

int CFusesOptions::getHfuse() {
	return hfuse;
}

int CFusesOptions::getEfuse() {
	return efuse;
}

int CFusesOptions::getNumOfFuses() {
	return numOfFuses;
}

int CFusesOptions::getLfuse() {
	return lfuse;
}

int CFusesOptions::hexStringToByte(string val) {
	int byte;

	byte = CFormat::hexStringToInt(val);
	if (byte < 0 || byte > 0xff) {
		throw ProgramOptionsException("Invalid fuse value '" + val + "'.");
	}
	return byte;
}

CFusesOptions::~CFusesOptions() {

}

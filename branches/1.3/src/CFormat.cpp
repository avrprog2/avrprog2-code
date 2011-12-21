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

#include "CFormat.h"
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

string CFormat::str(uint8_t *buffer, int len) {
	stringstream str;

	// break on 0x00 or 0xff
	for (int i=0; i<len && buffer[i]!=0 && buffer[i]!=0xff; i++) {
		str << buffer[i];
	}

	return str.str();
}

string CFormat::hex(uint8_t *buffer, int len) {
	stringstream str;

	for (int i=0; i<len; i++) {
		if (i % 16 == 0) str << endl;
		str.flags(ios::hex);
		str.width (2);
		str.fill('0');
		str << std::hex << (int)buffer[i] << " ";
	}

	return str.str();
}

string CFormat::intToString(int value) {
	stringstream str;

	str << value;
	return str.str();
}

string CFormat::intToHexString(int value) {
	stringstream str;

	str << std::hex << value;
	return str.str();
}

int CFormat::hexStringToInt(string val) {
	stringstream str;
	int v;

	// remove leading 0x
	if (val.substr(0, 2).compare("0x") == 0) {
		val = val.substr(2, val.size());
	}

	str << val;
	str.flags(ios::hex);
	str >> v;

	return v;
}

int CFormat::stringToInt(string val) {
	stringstream str;
	int v;

	// hex ?
	if (val.substr(0, 2).compare("0x") == 0) {
		return hexStringToInt(val);
	}

	str << val;
	str >> v;

	return v;
}

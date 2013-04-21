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

#include "CProgramOptions.h"
#include <boost/algorithm/string.hpp>
#include "CLArgumentException.h"

CProgramOptions::CProgramOptions(string options) {
	// parse options string
	// it should look like: (r|w|v):source

	// check delimiter
	string delimiter = options.substr(1, 1);
	if (delimiter.compare(":") != 0) {
		throw CLArgumentException("Invalid option '" + options + "'");
	}

	// check memory operation
	string operation = options.substr(0, 1);
	if (operation.compare("r") == 0) {
		this->operation = READ;
	}
	else if (operation.compare("w") == 0) {
		this->operation = WRITE;
	}
	else if (operation.compare("v") == 0) {
		this->operation = VERIFY;
	}
	else {
		throw CLArgumentException("Unsupported memory operation '" + operation + "'");
	}

	// check source (path or immediate value) and determine type
	this->source = options.substr(2, options.length());

	size_t dot = this->source.rfind('.');
	if (dot == this->source.npos) {
		type = IMMEDIATE;
	}
	else {
		string fileExtension = this->source.substr(dot+1, 4);
		boost::to_lower(fileExtension);

		if (fileExtension.compare("elf") == 0) {
			type = ELF;
		}
		else if ((fileExtension.compare("hex") == 0) || (fileExtension.compare("eep") == 0) || (fileExtension.compare("ihex") == 0)) {
			type = HEX;
		}
		else {
			throw ProgramOptionsException("Unsupported filetype '" + fileExtension + "'");
		}
	}
}

operation_t CProgramOptions::getOperation() {
	return operation;
}

string CProgramOptions::getPath() {
	return source;
}

filetype_t CProgramOptions::getType() {
	return type;
}

CProgramOptions::~CProgramOptions() {

}

ProgramOptionsException::ProgramOptionsException(string err) : ExceptionBase(err) {

}

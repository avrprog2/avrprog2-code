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
/*
 * CHexFile.cpp
 *
 *  Created on: 29.01.2011
 *      Author: root
 */

#include "CHexFile.h"
#include "config.h"
#include <bfd.h>
#include <iostream>

using namespace std;

CHexFile::CHexFile(string _path) : path(_path) {

}

void CHexFile::save(uint8_t *buffer, int size) {
	bfd *outputFile = NULL;
	asection *section;

	bfd_init();

	// open new file
	outputFile = bfd_openw(path.c_str(), "ihex");
	if (outputFile == NULL) {
		throw FileException("Could not write to '" + path + "'.\n" + bfd_errmsg(bfd_get_error()));
	}

	// set format to object
	if (!bfd_set_format(outputFile, bfd_object)) {
		throw FileException("Could not write to '" + path + "'.\n" + bfd_errmsg(bfd_get_error()));
	}

	// create a new section
	section = bfd_make_section_with_flags(outputFile, ".sec1", SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD);
	if (section == NULL) {
		throw FileException("Could not write to '" + path + "'.\n" + bfd_errmsg(bfd_get_error()));
	}

	// set size to fit to buffer
	if (!bfd_set_section_size(outputFile, section, size)) {
		throw FileException("Could not write to '" + path + "'.\n" + bfd_errmsg(bfd_get_error()));
	}

	// write content to the created section
	if (!bfd_set_section_contents(outputFile, section, buffer, 0, size)) {
		throw FileException("Could not write to '" + path + "'.\n" + bfd_errmsg(bfd_get_error()));
	}

	bfd_close(outputFile);
}

CHexFile::~CHexFile() {

}


FileException::FileException(string err) : MyException(err) {

}

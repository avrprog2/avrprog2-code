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
 * CMemoryOptions.cpp
 *
 *  Created on: 21.01.2011
 *      Author: root
 */

#include "CMemoryOptions.h"
#include <iostream>
#include <string.h>
#include <boost/foreach.hpp>
#include <cstring>
#include "CFormat.h"
#include "avrprog.h"
#include "COut.h"

using namespace std;


CMemoryOptions::CMemoryOptions(string options, offset_t offsetType, vector<string> sectionNames) : CProgramOptions(options), buffer(NULL), bufferLen(0) {
	char *target = NULL;
	int sectionCount = 0;
	bfd *inputFile;

	if (this->type == IMMEDIATE) {
		// nothing to do here
		return;
	}

	if (operation == WRITE || operation == VERIFY) {
		bfd_init();

		// open file
		inputFile = bfd_openr(this->source.c_str(), target);
		if (inputFile == NULL) {
			throw ProgramOptionsException("Could not open file '" + this->source + "'");
		}

		// check file format
		if (!bfd_check_format (inputFile, bfd_object)) {
			if (bfd_get_error () != bfd_error_file_ambiguously_recognized) {
				throw ProgramOptionsException("Incompatible file format in '" + this->source + "'");
			}
		}

		//printf("Entry point is at address %d\n",bfd_get_start_address(inputFile));

		// print sections
		//for (asection *section = inputFile->sections; section; section = section->next) {
		//	cout << section->name << endl;
		//	cout << section->flags << endl;
		//	//cout << section->output_section->name << endl << endl;
		//}

		if (operation == WRITE || operation == VERIFY) {
			asection *section;

			switch (this->type) {
			case IMMEDIATE:
				// do nothing
				break;
			case HEX:
				COut::d("Load hex file");
				// load all sections from an ihex file

				// check file format
				if (strcmp(bfd_get_target(inputFile), "ihex") != 0) {
					throw ProgramOptionsException("Incompatible file format in '" + this->source + "'. Expected an 'ihex' file.");
				}

				// loop sections
				for (section = inputFile->sections; section; section = section->next) {
					sectionCount++;
					// add section to buffer
					if (offsetType == SECTION_OFFSET)
						addSectionToBuffer(inputFile, section, section->lma);
					else
						addSectionToBuffer(inputFile, section, bufferLen);
				}

				if (sectionCount == 0) {
					throw ProgramOptionsException("No data found in '" + this->source + "'.");
				}

				break;
			case ELF:
				COut::d("Load elf file.");
				// load the given sections from an elf file
				BOOST_FOREACH(string sectionName, sectionNames) {
					sectionCount++;
					// get section
					section = bfd_get_section_by_name(inputFile, sectionName.c_str());
					if (section == NULL) {
						// the first section is mandatory
						if (sectionCount == 1) {
							throw ProgramOptionsException("No '" + (string)sectionName + "' section found in '" + this->source + "'.");
						}
						else {
							continue;
						}
					}
					if (offsetType == SECTION_OFFSET)
						addSectionToBuffer(inputFile, section, section->lma);
					else
						addSectionToBuffer(inputFile, section, bufferLen);
				}
				break;
			}
		}
		bfd_close(inputFile);
	}
}

void CMemoryOptions::addSectionToBuffer(bfd *inputFile, asection *section, int offset) {
	uint8_t *oldBuffer;
	int oldSize;

	oldBuffer = buffer;						// remember buffer
	oldSize = bufferLen;

	if (offset >= MAX_SECTION_OFFSET) {
		throw ProgramOptionsException("Cannot load sections above 0x" + CFormat::intToHexString(MAX_SECTION_OFFSET) + ".");
	}

	COut::d("\tAdd section: '" + (string)section->name + "' at 0x" + CFormat::intToHexString(offset) + ".");

	// determine new size and extend buffer
	if ((int)(offset + section->size) > bufferLen) {
		bufferLen = offset + section->size;

		buffer = new uint8_t[bufferLen];
		memcpy(buffer, oldBuffer, oldSize);		// copy old buffer
		delete[] oldBuffer;

		// initialize extended buffer
		memset(buffer + oldSize, 0xff, bufferLen - oldSize);
		COut::d("\tExtend buffer to: " + CFormat::intToString(bufferLen) + " bytes.");
	}

	// read section and add it to buffer
	if (!bfd_get_section_contents(inputFile, section, buffer+offset, 0, section->size)) {
		throw ProgramOptionsException("Error while reading '" + this->source + "'");
	}
}

uint8_t *CMemoryOptions::getBuffer() {
	return buffer;
}

int CMemoryOptions::getBufferSize() {
	return bufferLen;
}

CMemoryOptions::~CMemoryOptions() {
	if (buffer != NULL) {
		delete buffer;
	}
}

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

#ifndef CMEMORYOPTIONS_H_
#define CMEMORYOPTIONS_H_

#include "CProgramOptions.h"
#include <inttypes.h>
#include "config.h"
#include <bfd.h>
#include <vector>

/// Specifies the offset type, when reading ihex and elf files.
typedef enum {
	SECTION_OFFSET,		///< use the lma entry of the section
	BUFFER_OFFSET,		///< ignore the lma entry and write the section to the end of the current buffer
} offset_t;

/**
 * @brief	Parses command a line argument of a memory operations.
 *
 * Further it parses ihex and elf files to a byte buffer.
 *
 * For the parsing of the argument look at CProgramOptions.
 *
 * @throws	ProgramOptionsException on errors.
 */
class CMemoryOptions : public CProgramOptions {
public:
	/**
	 * @brief	Parses a command line argument and open the corresponding file.
	 *
	 * This class can read files in ihex and elf format.
	 *
	 * If a *.hex file was detected this class loads all sections into its buffer. When a *.elf file is given, it
	 * copies only the sections in \a sectionNames to the internal buffer.
	 *
	 * According to the given \a offsetType the lma entries in the sections are condidered (\a SECTION_OFFSET) or ignored (\a BUFFER_OFFSET).
	 *
	 * If neither a *hex nor a *.elf file was detected, this class does nothing.
	 *
	 * @param	options		Command line argument.
	 * @param	offsetType	Interpretation of lma entries.
	 * @param	sectionNames	List of section names which should be read from an *.elf file. The first section is mandatory, all others are ignored if they are not present in a file.
	 */
	CMemoryOptions(string options, offset_t offsetType, vector<string> sectionNames);
	virtual ~CMemoryOptions();

	/**
	 * @brief	Get a buffer with a binary representation of the file contents.
	 * @return	Pointer to the buffer. This pointer is valid as long as this object exists.
	 */
	uint8_t *getBuffer();

	/**
	 * @return	The number of bytes in \a buffer.
	 */
	int getBufferSize();

protected:
	uint8_t *buffer;
	int bufferLen;

private:
	/**
	 * @brief	Adds a the content of section to the buffer.
	 *
	 * This method automatically extends the size of the buffer such that
	 * the new section fits into it.
	 */
	void addSectionToBuffer(bfd *inputFile, asection *section, int offset);
};

#endif /* CMEMORYOPTIONS_H_ */

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

#ifndef CHEXFILE_H_
#define CHEXFILE_H_

#include <string>
#include <inttypes.h>
#include "MyException.h"

using namespace std;

/**
 * @brief	Writes a ihex file.
 * @throw	FileException on errors.
 */
class CHexFile {
public:
	/**
	 * @brief	Create a new hex file.
	 *
	 * @param	path	Path to the new file.
	 */
	CHexFile(string path);

	/**
	 * @brief	Save something to the file.
	 *
	 * The whole array is stored in one section of the hex file.
	 *
	 * Already existing files get overridden by this method.
	 *
	 * @param	buffer	Byte array of data, which should be stored.
	 * @param	size	Length of the \a buffer array.
	 */
	void save(uint8_t *buffer, int size);
	virtual ~CHexFile();

protected:
	string path;	///< path of the file
};

/**
 * @brief	Exception thrown by CHexFile
 */
class FileException : public MyException {
public:
	FileException(string err);
};

#endif /* CHEXFILE_H_ */

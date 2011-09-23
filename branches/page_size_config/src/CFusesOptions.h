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

#ifndef CFUSESOPTIONS_H_
#define CFUSESOPTIONS_H_

#include "CMemoryOptions.h"

using namespace std;

/**
 * @brief Parses command line arguments for fuse byte operations.
 *
 * See CMemoryOptions for more details.
 *
 * Additional to the parsing mechanisms of CMemoryOptions, this class can parse
 * immediate values. These values must look like:
 * @code
 * lfuse
 * @endcode
 * for one fuse byte.
 * @code
 * lfuse,hfuse
 * @endcode
 * for two fuse bytes.
 * @code
 * lfuse,hfuse,efuse
 * @endcode
 * for three fuse bytes.
 *
 * All values must be specified in hexadecimal form.
 *
 * @throw 	ProgramOptionsException on error.
 */
class CFusesOptions : public CMemoryOptions {
public:
	/**
	 * @param	option	Command line argument.
	 */
	CFusesOptions(string option);
	virtual ~CFusesOptions();

	/**
	 * @return	number of parsed fuse bytes
	 */
	int getNumOfFuses();

	/**
	 * @return	low fuse byte
	 */
	int getLfuse();

	/**
	 * @return	high fuse byte
	 */
	int getHfuse();

	/**
	 * @return	extended fuse byte
	 */
	int getEfuse();

protected:
	int lfuse;
	int hfuse;
	int efuse;
	int numOfFuses;

	/**
	 * @brief	Converts a hex string to an integer value.
	 *
	 * Further it checks if the value is between 0x00 and 0xff.
	 *
	 * @param	val	String representation of a hex value.
	 * @return	\a val as integer.
	 * @throw 	ProgramOptionsException if \a val is outside 0x00 and 0xff.
	 */
	int hexStringToByte(string val);
};

#endif /* CFUSESOPTIONS_H_ */

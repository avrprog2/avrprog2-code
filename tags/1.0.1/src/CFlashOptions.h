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

#ifndef CFLASHOPTIONS_H_
#define CFLASHOPTIONS_H_

#include "CMemoryOptions.h"

/**
 * @brief Parses command line arguments for flash memory operations.
 *
 * See CMemoryOptions for more details.
 *
 * @throw 	ProgramOptionsException on error.
 */
class CFlashOptions : public CMemoryOptions {
public:
	/**
	 * @param	options	Command line argument.
	 */
	CFlashOptions(string options);
	virtual ~CFlashOptions();
};

#endif /* CFLASHOPTIONS_H_ */

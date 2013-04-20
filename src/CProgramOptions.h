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

#ifndef CPROGRAMOPTIONS_H_
#define CPROGRAMOPTIONS_H_

#include "ExceptionBase.h"
#include <string>

/// memory operation types
typedef enum {
	READ,
	WRITE,
	VERIFY,
} operation_t;

/// argument source types
typedef enum {
	HEX,
	ELF,
	IMMEDIATE,
} filetype_t;

using namespace std;

/**
 * @brief	Class for parsing a command line argument.
 *
 * The argument has to look like:
 * @code
 * (r|w|v):value[hex|elf]
 * @endcode
 *
 * The class parses the operation (read, write or verify) and the
 * value type of the argument, which can be a path to a *.hex or *.elf,
 * or an immediate value.
 */
class CProgramOptions {
public:
	/**
	 * @brief	Parses the argument.
	 * @param	options	Commandline argument to parse.
	 */
	CProgramOptions(string options);
	virtual ~CProgramOptions();

	/**
	 * @brief	Get the operation type.
	 * @return	Type of memory operation.
	 */
	operation_t getOperation();

	/**
	 * @brief	Get the file path or immediate value as string according to \a getType().
	 * @return	String with a file path or immediate value.
	 */
	string getPath();

	/**
	 * @brief	Get the type (meaning) of the string which is returned by \a getPath().
	 * @return	Type of the command line argument value.
	 */
	filetype_t getType();
protected:
	filetype_t type;
	operation_t operation;
	string source;

};

/**
 * Exception thrown by CProgramOptions.
 */
class ProgramOptionsException : public ExceptionBase {
public:
	ProgramOptionsException(string err);
};

#endif /* CPROGRAMOPTIONS_H_ */

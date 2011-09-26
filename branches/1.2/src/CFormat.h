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

#ifndef CFORMAT_H_
#define CFORMAT_H_

#include <inttypes.h>
#include <string>

using namespace std;

/**
 * @brief	Helper class for input and output processing.
 */
class CFormat {
public:
	/**
	 * @brief	Converts a byte array to a readable string.
	 * @param	buffer	Byte array.
	 * @param	len		Lenght of the given array.
	 * @return	Content of buffer as string.
	 */
	static string str(uint8_t *buffer, int len);

	/**
	 * @brief	Converts a byte array to a string of hex values.
	 * @param	buffer	Byte array.
	 * @param	len		Lenght of the given array.
	 * @return	Content of buffer as string of hex values.
	 */
	static string hex(uint8_t *buffer, int len);

	/**
	 * @brief	Converts an integer value to a decimal string.
	 * @param	value Value to convert.
	 * @return	String representation of \a value.
	 */
	static string intToString(int value);

	/**
	 * @brief	Converts an integer value to a hex string.
	 * @param	value Value to convert.
	 * @return	String representation of \a value.
	 */
	static string intToHexString(int value);

	/**
	 * @brief	Converts a hex number presented as string to an integer value.
	 * @param	value hex value.
	 * @return	Integer value of \a value.
	 */
	static int hexStringToInt(string value);
	
	/**
	 * @brief	Converts a integer number presented as string to an integer value.
	 * @param	value integer value.
	 * @return	Integer value of \a value.
	 */
	static int stringToInt(string value);
};

#endif /* CFORMAT_H_ */

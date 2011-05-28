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

#ifndef COUT_H_
#define COUT_H_

#include <string>

using namespace std;

/**
 * @brief This class controls the output to stdout according to the actual debug level.
 */
class COut {
public:
	/**
	 * @brief	Output a debug level 1 message.
	 * @param	message	Output string.
	 */
	static void d(string message);

	/**
	 * @brief	Output a debug level 2 message.
	 * @param	message	Output string.
	 */
	static void dd(string message);

	/**
	 * @brief	Sets the debug level for all subsequent outputs.
	 * @param	level	Debug level 1, 2 or 0.
	 */
	static void setDebugLevel(int level);

	/**
	 * @brief	Get the actual debug level.
	 * @return	The actual debug level.
	 */
	static bool isSet(int level);

private:
	static int debugLevel;		///< store the actual debug level
};

#endif /* COUT_H_ */

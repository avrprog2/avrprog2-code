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

#ifndef CPROGRESSBAR_H_
#define CPROGRESSBAR_H_

/**
 * @brief	Draws a progressbar on stdout.
 */
class CProgressbar {
public:
	/**
	 * @brief	Initialize a new progress.
	 * @param	max	Number of steps of the visualized progress.
	 */
	CProgressbar(int max);
	virtual ~CProgressbar();

	/**
	 * @brief	The bar makes progress when this method is called.
	 */
	void step();

protected:
	static const int WIDTH = 80;	///< Width (in characters) of the drawn bar.
	int maxValue;
	int value;
	int drawn;
};

#endif /* CPROGRESSBAR_H_ */

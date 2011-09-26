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
/**
 * @file
 * @brief	Several definitions for avrprog.
 */

#ifndef AVRPROG_H_
#define AVRPROG_H_

#include "config.h"

#define VERSION_STRING "0.1"

/// USB vendor id
#define VENDOR_ID	0x3e1a
/// USB device id
#define DEVICE_ID	0x0200
/// USB interface number
#define INTERFACE	0

#ifndef WRITE_FUSES_SUPPORT
#define WRITE_FUSES_SUPPORT 1
#endif

#define AUTO_DETECT 0xff

/// memory type
typedef enum {
	FLASH,
	EEPROM,
} memory_t;

/// Directory for device description files.
// config directory, with trailing slash
#ifndef CONFIG_DIR
#define CONFIG_DIR	"/etc/avrprog/"
#endif

// user config directory, without the path to home
#ifndef HOME_CONFIG_DIR
#define HOME_CONFIG_DIR	".avrprog/"
#endif

/// An error is reported if more than MAX_READ_CYCLES read attempts failed when reading a memory page.
#define MAX_READ_CYCLES	1000

/// Communication timeout after which an error is reported.
#define USB_TIMEOUT		3000

/// Polling interval for the programmer to look if a page read has finished.
#define READ_PAGE_DELAY 3000

#define EMPTY_FLASH_BYTE	0xff
#define EMPTY_EEPROM_BYTE	0xff

/// Exit code if an error occurs during a verify operation.
#define VERIFY_ERROR_NUMBER	-2
/// Exit code for all other errors.
#define COMMON_ERROR_NUMBER	-1

/// Memory sections with a load address above MAX_SECTION_OFFSET lead to an error.
#define MAX_SECTION_OFFSET	0x800000

#endif /* AVRPROG_H_ */

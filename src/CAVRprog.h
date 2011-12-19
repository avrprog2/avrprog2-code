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

#ifndef CAVRPROG_H_
#define CAVRPROG_H_

#include "CAvrProgCommands.h"
#include "CAVRDevice.h"
#include "MyException.h"

typedef enum {
	FREQUENCY_AUTODETECT	= -1,
} frequency_t;

/**
 * @brief THE Programmer
 *
 * Provides methods for read, write, verify and erase operations on a specific device.
 * The device is specified through a device description file.
 *
 * Mainly this class does only checks on memory sizes and limits and uses the CAvrProgCommands class
 * for interaction with the hardware.
 *
 * @throw	ProgrammerException on errors.
 */
class CAVRprog : public CAvrProgCommands {
public:
	/**
	 * @param	device		see CUSBCommunication::CUSBCommunication
	 */
	CAVRprog(string device);
	virtual ~CAVRprog();

	/**
	* @brief	Connect to target mcu
	*
	* Must be called before any operations on the target can be performed.
	*
	* @param	deviceFile	name of the target mcu
	* @param	frequency	device frequency in Hz
	*/
	void connect(string deviceFile, int frequency);

	/**
	 * @brief	Writes to flash memory.
	 * @param	buffer	A buffer array.
	 * @param	size	Length of the buffer array.
	 */
	void writeFlash(uint8_t *buffer, int size);

	/**
	 * @brief	Writes to eeprom memory.
	 * @param	buffer	A buffer array.
	 * @param	size	Length of the buffer array.
	 */
	void writeEEPROM(uint8_t *buffer, int size);

	/**
	 * @brief	Writes fuses, for controllers with 3 fuse bytes.
	 * @param	lfuse	Low fuse byte.
	 * @param	hfuse	High fuse byte.
	 * @param	efuse	Extended fuse byte.
	 * @param	numOfFuses	Number of fuse bytes to write.
	 */
	void writeFuses(uint8_t lfuse, uint8_t hfuse, uint8_t efuse, int numOfFuses);

	/**
	 * @brief	Reads the content of flash memory.
	 *
	 * This method allocates enough memory to store the read content and sets buffer to the first element.
	 * The caller is responsible for freeing (\c delete[]) the buffer memory.
	 *
	 * @param	buffer	Pointer to a buffer array.
	 * @return	Number of bytes read.
	 * @return	Filled buffer.
	 */
	int readFlash(uint8_t **buffer);

	/**
	 * @brief	Reads the content of eeprom memory.
	 *
	 * This method allocates enough memory to store the read content and sets buffer to the first element.
	 * The caller is responsible for freeing (\c delete[]) the buffer memory.
	 *
	 * @param	buffer	Pointer to a buffer array.
	 * @return	Number of bytes read.
	 * @return	Filled buffer.
	 */
	int readEEPROM(uint8_t **buffer);

	/**
	 * @brief	Reads the content of flash memory.
	 *
	 * This method allocates enough memory to store all fuse bytes.
	 *
	 * - buffer[0] will contain the low fuse byte.\n
	 * - buffer[1] will contain the high fuse byte.\n
	 * - buffer[2] will contain the extended fuse byte.\n
	 *
	 * The caller is responsible for freeing (\c delete[]) the buffer memory.
	 *
	 * @param	buffer	Pointer to a buffer array.
	 * @return	Number of fuse bytes.
	 * @return	Filled buffer.
	 */
	int readFuses(uint8_t **buffer);

	/**
	 * @brief	Verifies the content of flash memory against the given buffer.
	 *
	 * Reads the whole flash memory and compares it with the given buffer.
	 * Before the comparison takes place, the buffer is filled with
	 * EMPTY_FLASH_BYTE up to the size of flash memory.
	 *
	 * @param	buffer	Buffer array.
	 * @param	size	Length of buffer array.
	 * @return	true if buffer an flash are equal.
	 * @return	false otherwise.
	 */
	bool verifyFlash(uint8_t *buffer, int size);

	/**
	 * @brief	Verifies the content of flash memory against the given buffer.
	 *
	 * Reads \a size bytes from flash memory and compares it with the given buffer.
	 *
	 * @param	buffer	Buffer array.
	 * @param	size	Length of buffer array.
	 * @return	true if buffer an flash are equal.
	 * @return	false otherwise.
	 */
	bool fastVerifyFlash(uint8_t *buffer, int size);

	/**
	 * @brief	Verifies the content of eeprom memory against the given buffer.
	 *
	 * Reads the whole eeprom memory and compares it with the given buffer.
	 * Before the comparison takes place, the buffer is filled with
	 * EMPTY_EEPROM_BYTE up to the size of eeprom memory.
	 *
	 * @param	buffer	Buffer array.
	 * @param	size	Length of buffer array.
	 * @return	true if buffer an eeprom are equal.
	 * @return	false otherwise.
	 */
	bool verifyEEPROM(uint8_t *buffer, int size);

	/**
	 * @brief	Verifies the content of eeprom memory against the given buffer.
	 *
	 * Reads \a size bytes from eeprom memory and compares it with the given buffer.
	 *
	 * @param	buffer	Buffer array.
	 * @param	size	Length of buffer array.
	 * @return	true if buffer an eeprom are equal.
	 * @return	false otherwise.
	 */
	bool fastVerifyEEPROM(uint8_t *buffer, int size);

	/**
	 * @brief	Verifies the fuse bytes against the given buffer.
	 *
	 * Reads \a size number of fuse bytes and compares it with the given buffer.
	 *
	 * - buffer[0] must contain the low fuse byte.\n
	 * - buffer[1] must contain the high fuse byte.\n
	 * - buffer[2] must contain the extended fuse byte.\n
	 *
	 * @param	buffer	Buffer array.
	 * @param	size	Number of fuse bytes.
	 * @return	true if all fuse bytes are equal.
	 * @return	false otherwise.
	 */
	bool verifyFuses(uint8_t *buffer, int size);

protected:
	CAVRDevice *device;		///< target device description
};

/**
 * @brief Exception thrown by CAVRprog
 */
class ProgrammerException : public MyException {
public:
	ProgrammerException(string err);
};

#endif /* CAVRPROG_H_ */

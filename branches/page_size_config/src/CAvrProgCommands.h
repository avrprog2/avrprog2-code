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

#ifndef CAVRPROGCOMMANDS_H_
#define CAVRPROGCOMMANDS_H_

#include "CUSBCommunication.h"
#include "avrprog.h"

/**
 * @brief	Contains all low level commands to programmer hardware and maps them to higher level commands.
 * @throw	CommandException
 * @throw	ChecksumException
 *
 * This class does no checks on memory sizes etc.
 *
 * Implementation details of the USB communication  are described in the *.cpp file.
 */
class CAvrProgCommands: public CUSBCommunication {
public:
	/**
	 * @brief	Start a session with the hardware programmer.
	 */
	CAvrProgCommands();
	virtual ~CAvrProgCommands();

	/**
	 * @brief	Connect to target mcu
	 *
	 * Must be called before any operations on the target can be performed.
	 * @param	socket		Socket of the expected device.
	 */
	void connect(socket_t socket);

	/**
	 * @brief	Reads the device signature
	 *
	 * @return	device signature
	 */
	uint32_t getDeviceSignature();

	/**
	 * @brief	Perform a chip erase.
	 */
	void chipErase();

	/**
	 * @brief	Write to flash memory.
	 * @param	buffer	Byte array with the content to write.
	 * @param	size	Length of \a buffer.
	 */
	void writeFlash(uint8_t *buffer, int size, int pageSize);

	/**
	 * @brief	Write fuse bytes.
	 * @param	lfuse	Low fuse byte.
	 * @param	hfuse	High fuse byte.
	 * @param	efuse	Extended fuse byte.
	 */
	void writeFuses(uint8_t lfuse, uint8_t hfuse, uint8_t efuse);

	/**
	 * @brief	Write fuse bytes.
	 * @param	lfuse	Low fuse byte.
	 * @param	hfuse	High fuse byte.
	 */
	void writeFuses(uint8_t lfuse, uint8_t hfuse);


	/**
	 * @brief	Write to eeprom memory.
	 * @param	buffer	Byte array with the content to write.
	 * @param	size	Length of \a buffer.
	 */
	void writeEEPROM(uint8_t *buffer, int size, int pageSize);

	/**
	 * @brief	Read the content of flash memory.
	 *
	 * The caller is responsible to free (delete[]) the buffer.
	 *
	 * @param	size	Number of bytes to read.
	 * @return	Pointer to the first element of the buffer with the read content.
	 */
	uint8_t *readFlash(int size, int pageSize);

	/**
	 * @brief	Read the content of eeprom memory.
	 *
	 * The caller is responsible to free (delete[]) the buffer.
	 *
	 * @param	size	Number of bytes to read.
	 * @return	Pointer to the first element of the buffer with the read content.
	 */
	uint8_t *readEEPROM(int size, int pageSize);

	/**
	 * @brief	Read the fuse bytes.
	 *
	 * The returned buffer contains the fuse bytes in the following order:
	 * - buffer[0] will contain the low fuse byte.\n
	 * - buffer[1] will contain the high fuse byte.\n
	 * - buffer[2] will contain the extended fuse byte.\n
	 *
	 * The caller is responsible to free (delete[]) the buffer.
	 *
	 * @param	size	Number fuse bytes to read.
	 * @return	Pointer to the first element of the buffer with the read content.
	 */
	uint8_t *readFuses(int size);

	/**
	 * @brief	Set the frequency of the target device
	 *
	 * @param frequency	Device frequency on Hz. If the value is smaller 0x100
	 * 					it is sent to the programmer without processing.
	 */
	void setProgrammingSpeed(int frequency);


private:
//	static const int PAGE_SIZE = 256;		///< size of a flash memory page
//	static const int SECTION_SIZE = 256/4;	///< size of a eeprom memory section (a section can be programmed with one USB transfer)

	// constants
	typedef enum {
		ACTIVATE	= 0x01,
		DEACTIVATE	= 0x02,
	} programmer_action_t;

	typedef enum {
		SETUP_COMMAND_SIZE	= 7,
		DATA_COMMAND_SIZE = 256,
	} setup_command_t;

	typedef enum {
		INFO_NAME		= 0x01,
		INFO_VERSION	= 0x02,
	} programmer_info_t;

	// private functions are documented in the *.cpp file
	void checkDevice();
	uint8_t *readMemory(int size, memory_t mem);
	void selectSocket(socket_t socket);
	void setExtendedAddress();
	uint8_t *readMemoryPage(int pageNumber, int pageSize, memory_t mem);
	void programmerInfo(programmer_info_t info, uint8_t **retBuffer, uint8_t *retLen);
	void programmer(programmer_action_t action);
	void delayMs(uint8_t time);
	bool detectDevice(bool reportError);
	void executeCommands(uint8_t *setupCommand, uint8_t numOfCommands, uint8_t *data);
	uint16_t checksum(uint8_t *buffer, int size);
	void writeFlashPage(uint8_t *buffer, int page, int pageSize);
	void writeEEPROMSection(uint8_t *buffer, int offset, int pageSize);
	bool isEmptyPage(uint8_t *buffer);
	bool trySocket(socket_t socket);
};

/**
 * Exception thrown by CAvrProgCommands
 */
class CommandException : public MyException {
public:
	CommandException(string err);
};

/**
 * Exception thrown by CAvrProgCommands
 */
class ChecksumException : public MyException {
public:
	ChecksumException(string err);
};

#endif /* CAVRPROGCOMMANDS_H_ */

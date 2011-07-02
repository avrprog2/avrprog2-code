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

#include "CAvrProgCommands.h"

#include <iostream>
#include "CFormat.h"
#include <cstring>
#include <cmath>
#include "avrprog.h"
#include "CProgressbar.h"
#include "COut.h"

using namespace std;

/*
 * Some things to the USB programmer and its commands:
 *
 * This class is split to higher level functions (all public methods) and
 * low level functions (all private methods).
 *
 * Checksums:
 * Most of the isochronous transfers are protected with a checksum. The value of this checksum is the 16 bit
 * wide sum of all bytes in the transfer buffer (Which is usually 256 bytes long).
 *
 * Program flow:
 * Operations with the programmer can be divided into the following steps:
 * - Connect to the USB device (this is already done in CUSBCommunication())
 * - Initialize the USB programmer (in this step the version number and other things are read, done by the constructor)
 * - select the programming pins (this is done by selectSocket())
 * - enable the programming pins (programmer, Probably this command switches the analog switches on the bigavr board)
 * - check if a device is present (works with detectDevice())
 * - read the device signature (getDeviceSignature())
 * - In some operations the original programmer waits for some time (delayMs()), however this has no effect with the tested mcus and is omitted here.
 * - perform all desired operations (read, write,...). Details to each operation are given at the top of each method.
 * - disable the programmer
 *
 * The author does not exactly know the meaning/effect of all low level commands, hence some commands are not closer described.
 */

CAvrProgCommands::CAvrProgCommands() {
	uint8_t *buffer;
	uint8_t len;

	programmerInfo(INFO_NAME, &buffer, &len);		// read device name
	COut::d("Programmer Name: " + CFormat::str(buffer, len));

	programmerInfo(INFO_VERSION, &buffer, &len);	// read device version
	COut::d("Programmer Version: " + CFormat::hex(buffer, len));
}

// public function

void CAvrProgCommands::connect(socket_t socket) {
	// the autodetection feature is not in the original programmer
	if (socket == AUTO_DETECT) {
		cout << "Autodetect programming pins..." << endl;
		// try to find the right socket
		if (trySocket(TQFP64) == true) {
			socket = TQFP64;
		}
		else if (trySocket(TQFP100) == true) {
			socket = TQFP100;
		}
		else {
			throw CommandException("No device found during autodetection of programming pins.");
		}
	}

	selectSocket(socket);
	programmer(ACTIVATE);
	detectDevice(false);			// in the original programmer checkDevice is called in front of each action
	// further it performs a chip erase and writes default fuses before any other action
	// this all is omitted here
}

void CAvrProgCommands::chipErase() {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//detectDevice(false);

	uint8_t command[] = {0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t data[] = {	0xac, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	executeCommands(command, 4, data);

	//delayMs(0x14);
}

/*
 * This method partitions the passed buffer into pages and fills the last page
 * with EMPTY_FLASH_BYTES.
 * Then each page is transferred with writeFlashPage()
 * A progressbar informs the user about the progress of this operation
 */
void CAvrProgCommands::writeFlash(uint8_t *buffer, int size) {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//detectDevice(false);

	uint8_t lastPage[PAGE_SIZE];
	int sizeOfLastPage;			// size of the last page without empty (0xff) bytes
	int page;
	int numOfPages;				// without the last page

	numOfPages = size / PAGE_SIZE;
	sizeOfLastPage = size - PAGE_SIZE * numOfPages;

	// copy the last page and fill it up with 0xff
	memcpy(lastPage, buffer+PAGE_SIZE*numOfPages, sizeOfLastPage);
	memset(lastPage+sizeOfLastPage, EMPTY_FLASH_BYTE, PAGE_SIZE-sizeOfLastPage);

	CProgressbar progressbar(numOfPages+1);

	for (page=0; page<numOfPages; page++) {
		writeFlashPage(&(buffer[page*PAGE_SIZE]), page);
		progressbar.step();
	}
	writeFlashPage(lastPage, page);
	progressbar.step();

	//delayMs(0x14);
}

/*
 * This method partitions the passed buffer into sections (128 bytes long) and fills the last section
 * with EMPTY_EEPROM_BYTES.
 * Then each section is transferred with writeEEPROMSection()
 * A progressbar informs the user about the progress of this operation
 */
void CAvrProgCommands::writeEEPROM(uint8_t *buffer, int size) {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//detectDevice(false);

	uint8_t lastSection[SECTION_SIZE];
	int sizeOfLastSection;			// size of the last section without empty (0xff) bytes
	int section;
	int numOfSections;				// without the last section

	numOfSections = size / SECTION_SIZE;
	sizeOfLastSection = size - SECTION_SIZE * numOfSections;

	// copy the last section and fill it up with 0xff
	memcpy(lastSection, buffer+SECTION_SIZE*numOfSections, sizeOfLastSection);
	memset(lastSection+sizeOfLastSection, EMPTY_EEPROM_BYTE, SECTION_SIZE-sizeOfLastSection);

	CProgressbar progressbar(numOfSections+1);

	for (section=0; section<numOfSections; section++) {
		writeEEPROMSection(&(buffer[section*SECTION_SIZE]), section*SECTION_SIZE);
		progressbar.step();
	}
	writeEEPROMSection(lastSection, section*SECTION_SIZE);
	progressbar.step();

	//delayMs(0x14);
}

void CAvrProgCommands::writeFuses(uint8_t lfuse, uint8_t hfuse, uint8_t efuse) {
	// original AVRprog does an erase before writing fuses
	// furthermore it writes default fuses, before programming the new ones

	uint8_t command[] = {0x02, 0x04, 0x00, 0x00, 0x00, 0x09, 0x00};
	uint8_t data[] = {	0xac, 0xa0, 0x00, 0xe1, 0xac, 0xa8, 0x00, 0x99, 0xac, 0xa4, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	data[3] = lfuse;
	data[7] = hfuse;
	data[11] = efuse;

	COut::dd("Set fuses");

	executeCommands(command, 3, data);
}

uint8_t *CAvrProgCommands::readFlash(int size) {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//checkDevice();
	return readMemory(size, FLASH);

	//delayMs(0x14);
}

uint8_t *CAvrProgCommands::readEEPROM(int size) {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//checkDevice();
	return readMemory(size, EEPROM);

	//delayMs(0x14);
}

/*
 * Reads 'size' fuse bytes. Currently only controllers with 3 fuses are supported.
 *
 * The fuses are returned in a byte array.
 */
uint8_t *CAvrProgCommands::readFuses(int size) {
	// the commented functions are sent by the original programmer
	//delayMs(0x14);

	//checkDevice();

	int len;
	uint8_t *buffer = NULL;
	uint8_t *fuses;

	uint8_t command[] = {0x02, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00};
	uint8_t data[] = {	0x58, 0x00, 0x00, 0x50, 0x00, 0x00, 0x58, 0x08, 0x00, 0x50, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	if (size != 3) {
		throw CommandException("Only MCUs with 3 fuse bytes are supported.");
	}

	executeCommands(command, 4, data);

	len = 256;
	iso_read(3, &buffer, &len);

	COut::dd("Command read fuses returned " + CFormat::hex(buffer, len));

	// check length
	if (len != size + 1) {
		throw CommandException("Error while reading fuses.");
	}
	if (buffer[0] != 0xff) {
		throw CommandException("Error while reading fuses.");
	}

	fuses = new uint8_t[size];
	memcpy(fuses, buffer+1, size);

	return fuses;

	//delayMs(0x14);
}

// internal functions

/*
 * helper function to check if a page in the buffer is empty
 * It is not necessary to transfer empty pages. In some cases this speeds up the programming procedure.
 */
bool CAvrProgCommands::isEmptyPage(uint8_t *buffer) {
	bool empty;

	// check if page is empty
	empty = true;
	for (int i=0; i<PAGE_SIZE; i++) {
		if (buffer[i] != EMPTY_FLASH_BYTE) {
			empty = false;
			break;
		}
	}

	return empty;
}

/*
 * write a page to flash memory
 * this method takes the page content as array and the page number as integer
 *
 * A page usually consists of 256 bytes and is transfered in the following steps:
 * - send the page content
 * - send a command which contains the page number and a checksum
 * - read the response
 */
void CAvrProgCommands::writeFlashPage(uint8_t *code, int page) {
	uint8_t *buffer = NULL;
	uint16_t checksum;
	// todo: check command with different mcus
	uint8_t command[] = {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x05};
	int len;


	if (isEmptyPage(code) == true) {
		return;
	}

	checksum = this->checksum(code, PAGE_SIZE);

	command[2] = (checksum>>8) & 0xff;	// assign checksum
	command[1] = (checksum>>0) & 0xff;

	command[5] = (page>>0) & 0xff;		// assign page number
	command[6] = (page>>8) & 0xff;
	command[7] = (page>>16) & 0xff;

	// continue writing
	if (page > 0) {
		command[3] = 1;
	}

	iso_write(3, code, PAGE_SIZE);

	// when sending page 512 switch addressing to extended
	if (page == 512) {
		setExtendedAddress();
	}

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Write flash page " + CFormat::intToString(page) + " returned " + CFormat::hex(buffer, len));

	// check return data
	if (len != 1) {
		throw CommandException("Error while writing page (" + CFormat::intToString(page) + ") to flash memory.");
	}
	else if (buffer[0] != 0x00) {
		throw CommandException("Error while writing page (" + CFormat::intToString(page) + ") to flash memory.");
	}
}

/*
 * This function reads 'size' bytes from 'mem'.
 *
 * The read procedure consists of many page reads, this reads are performed by readMemoryPage().
 * This functions further copies the single pages into one buffer and returns this buffer.
 */
uint8_t *CAvrProgCommands::readMemory(int size, memory_t mem) {
	uint8_t *pageBuffer;
	int numOfPages;

	// extend size to next full page
	if ((size % PAGE_SIZE) != 0) {
		size += PAGE_SIZE - (size % PAGE_SIZE);
	}

	uint8_t *buffer = new uint8_t[size];

	numOfPages = size / PAGE_SIZE;

	CProgressbar progressbar(numOfPages);

	for (int page = 0; page < numOfPages; page++) {
		pageBuffer = readMemoryPage(page, mem);

		memcpy(buffer+page*PAGE_SIZE, pageBuffer, PAGE_SIZE);
		progressbar.step();
	}

	return buffer;
}

/*
 * search for a target mcu in socket
 */
bool CAvrProgCommands::trySocket(socket_t socket) {
	bool found;

	selectSocket(socket);
	programmer(ACTIVATE);					// enable programmer
	found = detectDevice(true);				// check if device is present
	programmer(DEACTIVATE);					// disable programmer
	return found;
}

/*
 * With this info the programmer chip selects the correct programming pins.
 */
void CAvrProgCommands::selectSocket(socket_t socket) {
	uint8_t command[] = {0x04, 0x00};

	command[1] = (uint8_t)socket;
	int_write(2, command, sizeof(command));

	COut::dd("Command 040x (select socket " + CFormat::intToString((uint8_t)socket) + ")");
}

/*
 * is sent when programming large devices
 * issued when sending the page number 512
 */
void CAvrProgCommands::setExtendedAddress() {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x0b, 0x01};

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Command 0b01 returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 1) {
		throw CommandException("Error while executing Command 0b01");
	}
	else if (buffer[0] != 0x00) {
		throw CommandException("Error while executing Command 0b01");
	}
}

/*
 * device frequency
 * MHz	value
 * 1	4b
 * 2	23
 * 4	0f
 * 6	08
 * 8	05
 * 10	03
 * 12	02
 * 16	01
 *
 * lowest	ff
 * highest	01
 */
void CAvrProgCommands::setProgrammingSpeed(int frequency) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x05, 0x05};

	if (frequency < 0x100) {	// use raw value
		cout << "Note: Use raw frequency value for programming speed." << endl;
	}
	else {						// calculate programming speed from device frequency
		frequency = round(97.83 * pow((double)frequency/1000000.0, -1.52));
	}

	if (frequency < 1) {
		frequency = 1;
	}
	if (frequency > 0xff) {
		frequency = 0xff;
	}

	command[1] = frequency;
	COut::d("Set programming speed to " + CFormat::intToString(frequency));

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Set programming speed returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 1) {
		throw CommandException("Error while setting programming speed.");
	}
	else if (buffer[0] != 0x00) {
		throw CommandException("Error while setting programming speed.");
	}
}

/*
 * reads programmer info
 */
void CAvrProgCommands::programmerInfo(programmer_info_t info, uint8_t **retBuffer, uint8_t *retLen) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x10, 0x00};

	command[1] = info;

	int_write(2, command, sizeof(command));
	len = 256;
	int_read(2, &buffer, &len);

	COut::dd("Read info returned " + CFormat::hex(buffer, len));

	*retBuffer = buffer;
	*retLen = len;
}

/*
 * activate/deactivate programmer (probably this switches the analog switches)
 */
void CAvrProgCommands::programmer(programmer_action_t action) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x01, 0x01};

	command[1] = action;

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Command 0101 returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 1) {
		throw CommandException("Error while executing Command 0101");
	}
	else if (buffer[0] != 0x00) {
		throw CommandException("Error while executing Command 0101");
	}
}

/*
 * delay, is sent before and after some action (erase, program), is not used in the current configuration
 */
void CAvrProgCommands::delayMs(uint8_t ms) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x0e, 0x00};

	command[1] = ms;

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Delay ms returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 1) {
		throw CommandException("Error while delaying " + CFormat::intToString(ms) + "ms.");
	}
	else if (buffer[0] != 0x01) {
		throw CommandException("Error while delaying " + CFormat::intToString(ms) + "ms.");
	}
}

/*
 * setup and execute commands
 *
 * This is used for special command, which have not an own command number
 * on endpoint 2.
 * The command itself is sent as data to endpoint 3.
 *
 * @param	setupCommand	(begins always with 0x02) is a buffer of size SETUP_COMMAND_SIZE and contains information
 * 							of the following commands
 * @param	numOfCommands	number of commands in data
 * @param	data			is a buffer of lenth DATA_COMMAND_SIZE, which contains the commands
 */
void CAvrProgCommands::executeCommands(uint8_t *setupCommand, uint8_t numOfCommands, uint8_t *data) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x03, 0x00, 0x00, 0x00};
	uint16_t checksum;

	command[1] = numOfCommands;

	checksum = this->checksum(data, DATA_COMMAND_SIZE);

	command[2] = (checksum >> 0) & 0xff;
	command[3] = (checksum >> 8) & 0xff;

	int_write(2, setupCommand, SETUP_COMMAND_SIZE);

	// send data
	iso_write(3, data, DATA_COMMAND_SIZE);

	// execute
	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Execute command returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 1) {
		throw CommandException("Error while sending execute command.");
	}
	else if (buffer[0] == 0x81) {
		throw ChecksumException("Execute command returned without success.");
	}
	else if (buffer[0] != 0x00) {
		throw ChecksumException("Error while sending execute command.");
	}
}

/*
 * read device signature
 */
uint32_t CAvrProgCommands::getDeviceSignature() {
	int len;
	uint8_t *buffer;

	uint8_t command[] = {0x02, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00};
	uint8_t data[] = {	0x30, 0x00, 0x00, 0x30, 0x00, 0x01, 0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	COut::dd("Get device signature");

	executeCommands(command, 3, data);

	len = 256;
	iso_read(3, &buffer, &len);

	COut::dd("Get device signature returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 3) {
		throw CommandException("Error while reading device Signature.");
	}

	return buffer[0]<<16 | buffer[1]<<8 | buffer[2]<<0;
}

/*
 * reads 0x53 0x00 if device is present
 * if reportError is true, the outgoing of the command is returned instead of throwing an exception
 */
bool CAvrProgCommands::detectDevice(bool reportError) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t command[] = {0x02, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00};
	uint8_t data[] = {	0xac, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	executeCommands(command, 1, data);

	len = 256;
	iso_read(3, &buffer, &len);
	COut::dd("Command 0301 read returned " + CFormat::hex(buffer, len));

	// check data
	if (len != 2) {
		if (reportError == true) return false;
		throw CommandException("Error while executing Command 0301 read");
	}
	else if (buffer[0] != 0x53) {
		if (reportError == true) return false;
		throw CommandException("Error while executing Command 0301 read (No Device found!)");
	}

	return true;
}

/*
 * calculate the checksum (16 bit sum of all bytes in buffer)
 */
uint16_t CAvrProgCommands::checksum(uint8_t *buffer, int size) {
	uint16_t checksum = 0;
	for (int i=0; i<size; i++) {
		checksum += buffer[i];
	}
	return checksum;
}

/*
 * low level functions to write eeprom memory
 *
 * A section usually consists of 128 bytes and is transfered in the following steps:
 * - send the section content (in this step 256 bytes are sent, where only the first 64 bytes (SECTION_SIZE) contain real data)
 * - send a command which contains the memory address of the section and a checksum
 * - read the response
 */
void CAvrProgCommands::writeEEPROMSection(uint8_t *code, int offset) {
	uint8_t section[PAGE_SIZE];
	uint8_t *buffer;
	uint16_t checksum;
	uint8_t command[] = {0x09, 0x00, 0x00, 0x00, 0x00, SECTION_SIZE, 0x00, 0x09};
	int len;

	// copy section to buffer
	memcpy(section, code, SECTION_SIZE);

	checksum = this->checksum(section, PAGE_SIZE);

	command[2] = (checksum>>8) & 0xff;		// assign checksum
	command[1] = (checksum>>0) & 0xff;

	command[4] = (offset>>8) & 0xff;		// assign offset
	command[3] = (offset>>0) & 0xff;

	iso_write(3, section, PAGE_SIZE);

	int_write(2, command, sizeof(command));
	len = 1;
	int_read(2, &buffer, &len);

	COut::dd("Write eeprom section returned " + CFormat::hex(buffer, len));

	// check return data
	if (len != 1) {
		throw CommandException("Error while writing section to eeprom memory");
	}
	else if (buffer[0] != 0x00) {
		throw CommandException("Error while writing section to eeprom memory");
	}
}

/*
 * Low level functions for reading a memory page
 *
 * A page (256 bytes long) is read in the following steps:
 * - send a command to the programmer (this is the only thing that differs between readings from flash and eeprom)
 * - Poll the response until the length of the response is 256 byte.
 *
 * Between two polling attempts the program sleeps for READ_PAGE_DELAY us
 */
uint8_t *CAvrProgCommands::readMemoryPage(int pageNumber, memory_t mem) {
	int len;
	uint8_t *buffer = NULL;
	uint8_t *command = NULL;
	uint8_t commandFlash[] = {0x08, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t commandEEPROM[] = {0x0a, 0x00, 0x00, 0x00, 0x01};
	int commandSize;
	int count;

	switch (mem) {
	case FLASH:
		command = commandFlash;
		commandSize = sizeof(commandFlash);
		command[2] = (pageNumber >> 0) & 0xff;
		command[3] = (pageNumber >> 8) & 0xff;
		command[4] = (pageNumber >> 16) & 0xff;		// only guessed
		break;
	case EEPROM:
		command = commandEEPROM;
		commandSize = sizeof(commandEEPROM);
		command[2] = (pageNumber >> 0) & 0xff;
		command[3] = (pageNumber >> 8) & 0xff;
		break;
	}

	// when reading page 512 issue setExtendedAddress
	if (pageNumber == 512) {
		setExtendedAddress();
	}

	int_write(2, command, commandSize);
	//COut::dd("Command 0800 (read page)");

	count = MAX_READ_CYCLES;
	do {
		usleep(READ_PAGE_DELAY);
		len = 256;
		iso_read(3, &buffer, &len);

		// check data
		if (len == PAGE_SIZE) {
			break;
		}
		count--;
	} while (count != 0);

	if (count == 0) {
		throw CommandException("Error while reading page " + CFormat::intToString(pageNumber));
	}

	// Debugging output
	if (COut::isSet(2)) {
		if (isEmptyPage(buffer)) {
			COut::dd("Read page (" + CFormat::intToString(pageNumber) + ") returned (after " + CFormat::intToString(MAX_READ_CYCLES - count) + " tries): empty page");
		}
		else {
			COut::dd("Read page (" + CFormat::intToString(pageNumber) + ") returned (after " + CFormat::intToString(MAX_READ_CYCLES - count) + " tries) " + CFormat::hex(buffer, len));
		}
	}

	return buffer;
}

CAvrProgCommands::~CAvrProgCommands() {
	programmer(DEACTIVATE);
}

CommandException::CommandException(string err) : MyException(err) {

}

ChecksumException::ChecksumException(string err) : MyException(err) {

}

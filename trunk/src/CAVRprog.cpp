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

#include "CAVRprog.h"
#include <cstring>

CAVRprog::CAVRprog(string deviceFile, int frequency) : CAVRDevice(deviceFile) {
	// init programmer
	programmer = new CAvrProgCommands(_deviceSignature, _socket, frequency);
}

void CAVRprog::connect() {
	programmer->connect();
}

void CAVRprog::chipErase() {
	programmer->chipErase();
}

void CAVRprog::writeFlash(uint8_t *buffer, int size) {
	if (size > _flashSize) {
		throw ProgrammerException("Not enough flash memory.");
	}

	programmer->writeFlash(buffer, size);
}

void CAVRprog::writeEEPROM(uint8_t *buffer, int size) {
	if (size > _eepromSize) {
		throw ProgrammerException("Not enough eeprom memory.");
	}

	programmer->writeEEPROM(buffer, size);
}

void CAVRprog::writeFuses(uint8_t lfuse, uint8_t hfuse, uint8_t efuse) {
	if (3 != _fusesSize) {
		throw ProgrammerException("Fuses Error.");
	}

	programmer->writeFuses(lfuse, hfuse, efuse);
}

void CAVRprog::writeFuses(uint8_t lfuse, uint8_t hfuse) {
	if (2 != _fusesSize) {
		throw ProgrammerException("Fuses Error.");
	}

	throw ProgrammerException("MCUs with 2 fuse bytes are not supported.");
}

void CAVRprog::writeFuses(uint8_t lfuse) {
	if (1 != _fusesSize) {
		throw ProgrammerException("Fuses Error.");
	}

	throw ProgrammerException("MCUs with 1 fuse byte are not supported.");
}

int CAVRprog::readFlash(uint8_t **buffer) {
	int lastData = -1;
	*buffer = programmer->readFlash(_flashSize);

	// cut off empty flash memory
	for (int i=0; i<_flashSize; i++) {
		if ((*buffer)[i] != EMPTY_FLASH_BYTE) {
			lastData = i;
		}
	}

	return lastData + 1;
}

int CAVRprog::readEEPROM(uint8_t **buffer) {
	int lastData = -1;
	*buffer = programmer->readEEPROM(_eepromSize);

	// cut off empty eeprom memory
	for (int i=0; i<_eepromSize; i++) {
		if ((*buffer)[i] != EMPTY_EEPROM_BYTE) {
			lastData = i;
		}
	}

	return lastData + 1;
}

int CAVRprog::readFuses(uint8_t **buffer) {
	*buffer = programmer->readFuses(_fusesSize);

	return _fusesSize;
}

bool CAVRprog::verifyFlash(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > _flashSize) {
		return false;
	}

	uint8_t flashFile[_flashSize];
	uint8_t *flashContent = programmer->readFlash(_flashSize);

	// extend buffer to flash size
	memcpy(flashFile, buffer, size);
	memset(flashFile+size, EMPTY_FLASH_BYTE, _flashSize-size);

	if (memcmp(flashFile, flashContent, _flashSize) != 0) {
		equal = false;
	}

	delete[] flashContent;

	return equal;
}

bool CAVRprog::fastVerifyFlash(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > _flashSize) {
		return false;
	}

	uint8_t *flashContent = programmer->readFlash(size);

	if (memcmp(buffer, flashContent, size) != 0) {
		equal = false;
	}

	delete[] flashContent;

	return equal;
}

bool CAVRprog::verifyEEPROM(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > _eepromSize) {
		return false;
	}

	uint8_t eepromFile[_eepromSize];
	uint8_t *eepromContent = programmer->readEEPROM(_eepromSize);

	// extend buffer to flash size
	memcpy(eepromFile, buffer, size);
	memset(eepromFile+size, EMPTY_FLASH_BYTE, _eepromSize-size);

	if (memcmp(eepromFile, eepromContent, _eepromSize) != 0) {
		equal = false;
	}

	delete[] eepromContent;

	return equal;
}

bool CAVRprog::fastVerifyEEPROM(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > _eepromSize) {
		return false;
	}

	uint8_t *eepromContent = programmer->readEEPROM(size);

	if (memcmp(buffer, eepromContent, size) != 0) {
		equal = false;
	}

	delete[] eepromContent;

	return equal;
}

bool CAVRprog::verifyFuses(uint8_t *buffer, int size) {
	bool equal = true;
	uint8_t *fusesContent = programmer->readFuses(_fusesSize);

	if (size != _fusesSize) {
		equal = false;
	}
	else if (memcmp(buffer, fusesContent, _fusesSize) != 0) {
		equal = false;
	}

	delete[] fusesContent;

	return equal;
}

CAVRprog::~CAVRprog() {
	delete programmer;
}

ProgrammerException::ProgrammerException(string err) : MyException(err) {

}

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
#include <iostream>
#include "CFormat.h"
#include "COut.h"

using namespace std;

const int frequencies[] = {128000, 1000000, 4000000, 8000000, 16000000};	///< frequencies for speed autodetection

CAVRprog::CAVRprog() : CAvrProgCommands(), device(NULL) {

}

void CAVRprog::connect(string deviceFile, int frequency) {
	bool error = false;
	unsigned int f;
	uint32_t deviceSignature;

	if (frequency < 0) {						// autodetect programming frequency
		setProgrammingSpeed(frequencies[0]);	// set low frequency and increase it later
	}
	else {
		setProgrammingSpeed(frequency);
	}

	if (deviceFile.size() == 0) {				// autodetect device
		CAvrProgCommands::connect(AUTO_DETECT);
		cout << "Autodetect target device..." << endl;
		uint32_t signature = getDeviceSignature();
		device = new CAVRDevice(signature);
	}
	else {
		device = new CAVRDevice(deviceFile);
		CAvrProgCommands::connect(device->socket());


	}

	deviceSignature = getDeviceSignature();

	COut::d("Device Signature: 0x" + CFormat::intToHexString(deviceSignature));

	//check device signature
	if (deviceSignature != device->deviceSignature()) {
		throw ProgrammerException("Wrong device signature: expected 0x" + CFormat::intToHexString(device->deviceSignature()) + " but found 0x" + CFormat::intToHexString(deviceSignature) + ".");
	}

	cout << "Connected to '" << device->name() << "'." << endl;

	if (frequency < 0) {				// autodetect programming frequency
		cout << "Autodetect programming frequency..." << endl;

		// increase the frequency until an error occurs or the maximum is reached
		for (f=0; f<sizeof(frequencies)/sizeof(int); f++) {
			setProgrammingSpeed(frequencies[f]);

			if (getDeviceSignature() != device->deviceSignature()) {
				error = true;
				break;
			}
		}

		if (error == true) {			// set lower frequency
			if (f == 0) {
				throw ProgrammerException("Autodetect device frequency failed.");
			}
			else {
				setProgrammingSpeed(frequencies[f-1]);
			}
		}

		cout << "Set frequency to " << (double)(frequencies[f-1]/1000000.0) << "MHz." << endl;
	}
}

void CAVRprog::writeFlash(uint8_t *buffer, int size) {
	if (size > device->flashSize()) {
		throw ProgrammerException("Not enough flash memory.");
	}

	CAvrProgCommands::writeFlash(buffer, size, device->flashPageSize());
}

void CAVRprog::writeEEPROM(uint8_t *buffer, int size) {
	if (size > device->eepromSize()) {
		throw ProgrammerException("Not enough eeprom memory.");
	}

	CAvrProgCommands::writeEEPROM(buffer, size);
}

void CAVRprog::writeFuses(uint8_t lfuse, uint8_t hfuse, uint8_t efuse, int numOfFuses) {
	if (numOfFuses != device->fusesSize()) {
		throw ProgrammerException("Fuses Error. Expected " + CFormat::intToString(device->fusesSize()) + " fuse bytes.");
	}

	CAvrProgCommands::writeFuses(lfuse, hfuse, efuse, numOfFuses);
}

int CAVRprog::readFlash(uint8_t **buffer) {
	int lastData = -1;
	*buffer = CAvrProgCommands::readFlash(device->flashSize());

	// cut off empty flash memory
	for (int i=0; i<device->flashSize(); i++) {
		if ((*buffer)[i] != EMPTY_FLASH_BYTE) {
			lastData = i;
		}
	}

	return lastData + 1;
}

int CAVRprog::readEEPROM(uint8_t **buffer) {
	int lastData = -1;
	*buffer = CAvrProgCommands::readEEPROM(device->eepromSize());

	// cut off empty eeprom memory
	for (int i=0; i<device->eepromSize(); i++) {
		if ((*buffer)[i] != EMPTY_EEPROM_BYTE) {
			lastData = i;
		}
	}

	return lastData + 1;
}

int CAVRprog::readFuses(uint8_t **buffer) {
	*buffer = CAvrProgCommands::readFuses(device->fusesSize());

	return device->fusesSize();
}

bool CAVRprog::verifyFlash(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > device->flashSize()) {
		return false;
	}

	uint8_t flashFile[device->flashSize()];
	uint8_t *flashContent = CAvrProgCommands::readFlash(device->flashSize());

	// extend buffer to flash size
	memcpy(flashFile, buffer, size);
	memset(flashFile+size, EMPTY_FLASH_BYTE, device->flashSize()-size);

	if (memcmp(flashFile, flashContent, device->flashSize()) != 0) {
		equal = false;
	}

	delete[] flashContent;

	return equal;
}

bool CAVRprog::fastVerifyFlash(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > device->flashSize()) {
		return false;
	}

	uint8_t *flashContent = CAvrProgCommands::readFlash(size);

	if (memcmp(buffer, flashContent, size) != 0) {
		equal = false;
	}

	delete[] flashContent;

	return equal;
}

bool CAVRprog::verifyEEPROM(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > device->eepromSize()) {
		return false;
	}

	uint8_t eepromFile[device->eepromSize()];
	uint8_t *eepromContent = CAvrProgCommands::readEEPROM(device->eepromSize());

	// extend buffer to flash size
	memcpy(eepromFile, buffer, size);
	memset(eepromFile+size, EMPTY_FLASH_BYTE, device->eepromSize()-size);

	if (memcmp(eepromFile, eepromContent, device->eepromSize()) != 0) {
		equal = false;
	}

	delete[] eepromContent;

	return equal;
}

bool CAVRprog::fastVerifyEEPROM(uint8_t *buffer, int size) {
	bool equal = true;

	if (size > device->eepromSize()) {
		return false;
	}

	uint8_t *eepromContent = CAvrProgCommands::readEEPROM(size);

	if (memcmp(buffer, eepromContent, size) != 0) {
		equal = false;
	}

	delete[] eepromContent;

	return equal;
}

bool CAVRprog::verifyFuses(uint8_t *buffer, int size) {
	bool equal = true;
	uint8_t *fusesContent = CAvrProgCommands::readFuses(device->fusesSize());

	if (size != device->fusesSize()) {
		equal = false;
	}
	else if (memcmp(buffer, fusesContent, device->fusesSize()) != 0) {
		equal = false;
	}

	delete[] fusesContent;

	return equal;
}

CAVRprog::~CAVRprog() {
	if (device != NULL) {
		delete device;
	}
}

ProgrammerException::ProgrammerException(string err) : MyException(err) {

}

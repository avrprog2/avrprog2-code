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
 * @brief	The starting point of the application.
 *
 * @mainpage
 * @copydoc readme
 * - @ref readme "Readme"
 * - @ref avrprog2 "Manpage"
 */

#include <iostream>
#include <cstdlib>
#include <inttypes.h>
#include <stdio.h>
#include "CAVRprog.h"
#include "CFormat.h"
#include <iostream>
#include <string>
#include <sstream>
#include <getopt.h>
#include "avrprog.h"
#include "CEEPROMOptions.h"
#include "CFlashOptions.h"
#include "CFusesOptions.h"
#include "CHexFile.h"
#include "MyException.h"
#include "CLArgumentException.h"
#include "COut.h"

using namespace std;

/**
 * @brief	Displays a usage message.
 */
void usage() {
	cout << endl;
	cout << "Usage: " << PACKAGE_NAME << " (--mcu | -m) (<mcutype> | <file>.xml | list)"<< endl;
	cout << "	[--help | -h] [--version] [-d] [-d]"									<< endl;
	cout << "	[(--frequency | -f) <frequency>]"										<< endl;
	cout << "	[--erase]"																<< endl;
	cout << "	[--flash (r|w|v):<file>]"												<< endl;
	cout << "	[--eeprom (r|w|v):<file>]"												<< endl;
	cout << "	[--fuses (r|w|v):(<file> | <lfuse>[,<hfuse>[,<efuse>]])]"				<< endl;
	cout << endl;
	cout << "Option Description:"														<< endl;
	cout << "  --mcu, -m <mcutype>     Specify the mcu type by name."					<< endl;
	cout << "            <file>.xml    Specify the path to a device configuration file."<< endl;
	cout << "            list          Get a list of supported devices." 				<< endl;
	cout << "  --help, -h              Display this usage message." 					<< endl;
	cout << "  --version               Print version informations." 					<< endl;
	cout << "  -d                      Print more information."							<< endl;
	cout << "                          Specified a second time prints even more information." << endl;
	cout << "  --frequency, -f <freq>  Device frequency in Hz. (If the value is smaller than" << endl;
	cout << "                          0x100, it is passed directly to the programmer.)"<< endl;
	cout << "  --erase                 Perform a chip erase."							<< endl;
	cout << "  --flash (r|w|v):<file>  Perform the given operation on flash memory." 	<< endl;
	cout << "                          r    Read memory and save it to file."			<< endl;
	cout << "                          w    Write content from file to memory." 		<< endl;
	cout << "                          v    Verify the memory content against file."	<< endl;
	cout << "  --eeprom (r|w|v):<file> Perform the given operation on eeprom memory." 	<< endl;
	cout << "  --fuses (r|w|v):(<file> | <lfuse>[,<hfuse>[,<efuse>]]) "					<< endl;
	cout <<	"                          Perform the given operation on fuse bytes." 		<< endl;
	cout <<	"                          Values have to be specified in hex format (without 0x)." << endl;
	cout <<	"Calling " << PACKAGE_NAME << " without any memory operations will reset the target device." << endl;
	cout << endl;
	cout << "For more information type 'man " << PACKAGE_NAME << "'."					<< endl;
}

int main(int argc, char** argv) {
	CAVRprog *prog = NULL;
	uint8_t *buffer = NULL;
	int size;
	CHexFile *hexFile = NULL;
	int returnValue = 0;
	stringstream freqConversion;
	int frequency = 8000000;

	// actions
	bool printHelp = false;
	bool printVersion = false;
	int debug = 0;
	bool verify = false;
	bool chipErase = false;
	string flash = "";
	string eeprom = "";
	string fuses = "";
	string mcu = "";
	CFlashOptions *flashOptions = NULL;
	CEEPROMOptions *eepromOptions = NULL;
	CFusesOptions *fusesOptions = NULL;

	int c;
	int index = 0;
	struct option options[] = {
			{"help",		no_argument,		NULL, 'h'},
			{"version",		no_argument,		NULL, 'V'},
			{"debug",		no_argument,		NULL, 'd'},
			{"mcu",			required_argument,	NULL, 'm'},
			{"verify",		no_argument,		NULL, 'v'},
			{"frequency",	required_argument,	NULL, 'f'},
			{"erase",		no_argument,		NULL, 'E'},
			{"flash",		required_argument,	NULL, 'F'},
			{"eeprom",		required_argument,	NULL, 'P'},
			{"fuses",		required_argument,	NULL, 'U'},
			{0, 0, 0, 0}
	};

	try {
		// parse command line arguments
		while ((c = getopt_long(argc, argv, "hdvf:m:", options, &index)) != EOF) {
			switch(c) {
			case 'h':
				printHelp = true;
				break;
			case 'V':
				printVersion = true;
				break;
			case 'd':
				debug++;
				break;
			case 'm':
				if (mcu.size() != 0) throw CLArgumentException("mcu was already specified.");
				if (optarg[0] == '-') throw CLArgumentException("mcu requires an argument.");
				mcu = optarg;
				break;
			case 'v':
				verify = true;
				break;
			case 'f':
				if (optarg[0] == '-') throw CLArgumentException("frequency requires an argument.");
				freqConversion << optarg;
				freqConversion >> frequency;
				break;
			case 'E':
				chipErase = true;
				break;
			case 'F':
				if (flash.size() != 0) throw CLArgumentException("flash was already specified.");
				if (optarg[0] == '-') throw CLArgumentException("flash requires an argument.");
				flash = optarg;
				break;
			case 'P':
				if (eeprom.size() != 0) throw CLArgumentException("eeprom was already specified.");
				if (optarg[0] == '-') throw CLArgumentException("eeprom requires an argument.");
				eeprom = optarg;
				break;
			case 'U':
				if (fuses.size() != 0) throw CLArgumentException("fuses was already specified.");
				if (optarg[0] == '-') throw CLArgumentException("fuses requires an argument.");
				fuses = optarg;
				break;
			case '?':
				throw CLArgumentException("");
				break;
			}
		}

		// check for remaining arguments
		if (optind != argc)	throw CLArgumentException("Invalid argument '" + (string)argv[optind] + "'.");

		// print debuf info
		COut::setDebugLevel(debug);

		COut::d(PACKAGE_STRING);
		COut::d("System configuration directory: " + (string)CONFIG_DIR);
		COut::d("User configuration directory: ~/" + (string)HOME_CONFIG_DIR);
#if WRITE_FUSES_SUPPORT
#else
		COut::d("Writing of fuse bytes is disabled");
#endif
		COut::d("");

		// execute actions without hardware access
		// help, version, list
		// after one of these actions was executed, the program exits
		if (printHelp == true) {
			usage();
			return returnValue;
		}

		if (printVersion == true) {
			cout << PACKAGE_STRING << endl;
			return returnValue;
		}

		if (mcu.compare("list") == 0) {
			CAVRDevice::listDevices();
			return returnValue;
		}

		// check required arguments
		if (mcu.size() == 0) throw CLArgumentException("No mcu (-m) type specified.");

		if (flash.size() != 0) {
			COut::d("Prepare buffer for flash operations.");
			flashOptions = new CFlashOptions(flash);
			if (flashOptions->getOperation() == WRITE) {
				chipErase = true;
			}
			COut::d("");
		}
		if (eeprom.size() != 0) {
			COut::d("Prepare buffer for eeprom operations.");
			eepromOptions = new CEEPROMOptions(eeprom);
			if (eepromOptions->getOperation() == WRITE) {
				chipErase = true;
			}
			COut::d("");
		}
		if (fuses.size() != 0) {
			COut::d("Prepare buffer for fuse bytes operations.");
			fusesOptions = new CFusesOptions(fuses);
			COut::d("");
		}

		prog = new CAVRprog(mcu, frequency);
		prog->connect();
		cout << "Connected to '" << prog->name() << "'." << endl;

		// this is only for output
		if (fusesOptions == NULL && flashOptions == NULL && eepromOptions == NULL && chipErase == false) {
			cout << "Reset device..." << endl;
		}

		// perform chip erase
		if (chipErase == true) {
			cout << endl << "Chip erase..." << endl;
			prog->chipErase();
		}

		// perform fuses actions
		if (fusesOptions != NULL) {
			switch (fusesOptions->getOperation()) {
			case WRITE:
				cout << endl << "Write fuse bytes..." << endl;
#if WRITE_FUSES_SUPPORT
				if (fusesOptions->getNumOfFuses() == 3) {
					prog->writeFuses(fusesOptions->getLfuse(), fusesOptions->getHfuse(), fusesOptions->getEfuse());
				}
				else if (fusesOptions->getNumOfFuses() == 2) {
					prog->writeFuses(fusesOptions->getLfuse(), fusesOptions->getHfuse());
				}
				else {
					prog->writeFuses(fusesOptions->getLfuse());
				}
				cout << prog->fusesSize() << " fuses byte written" << endl;

				if (verify == true) {
					cout << endl << "Verify fuse bytes..." << endl;
					if (prog->verifyFuses(fusesOptions->getBuffer(), fusesOptions->getBufferSize()) == false) {
						throw MyException("Verify fuse bytes failed.");
					}
					else {
						cout << "OK, " << prog->fusesSize() << " fuse bytes verified" << endl;
					}
				}
#else
				cout << "This version does not support writing of fuse bytes.";
#endif
				break;
			case READ:
				cout << endl << "Read fuse bytes..." << endl;
				size = prog->readFuses(&buffer);
				switch (fusesOptions->getType()) {
				case HEX:
					hexFile = new CHexFile(fusesOptions->getPath());
					hexFile->save(buffer, size);
					delete hexFile;
					break;
				case IMMEDIATE:
					cout << "\tlfuse: " << "0x" << CFormat::intToHexString(buffer[0]) << endl;
					if (size > 1)
						cout << "\thfuse: " << "0x" << CFormat::intToHexString(buffer[1]) << endl;
					if (size > 2)
						cout << "\tefuse: " << "0x" << CFormat::intToHexString(buffer[2]) << endl;
					break;
				case ELF:
					throw ProgramOptionsException("Reads fuse bytes into *.elf files is not supported.");
					break;
				}
				delete[] buffer;
				cout << prog->fusesSize() << " fuse bytes read" << endl;
				break;
			case VERIFY:
				cout << endl << "Verify fuse bytes..." << endl;
				if (prog->verifyFuses(fusesOptions->getBuffer(), fusesOptions->getBufferSize()) == false) {
					cout << "failed";
					returnValue = VERIFY_ERROR_NUMBER;
				}
				else {
					cout << "OK";
				}
				cout << ", " << prog->fusesSize() << " fuse bytes verified" << endl;
				break;
			}
		}

		// perform flash actions
		if (flashOptions != NULL) {
			switch (flashOptions->getOperation()) {
			case WRITE:
				cout << endl << "Write to flash memory..." << endl;
				prog->writeFlash(flashOptions->getBuffer(), flashOptions->getBufferSize());
				cout << flashOptions->getBufferSize() << " bytes written" << endl;

				if (verify == true) {
					cout << endl << "Verify flash memory..." << endl;
					if (prog->fastVerifyFlash(flashOptions->getBuffer(), flashOptions->getBufferSize()) == false) {
						throw MyException("Verify flash failed.");
					}
					else {
						cout << "OK, " << flashOptions->getBufferSize() << " bytes verified" << endl;
					}
				}
				break;
			case READ:
				if (flashOptions->getType() != HEX) {
					throw ProgramOptionsException("Only reads into *.hex files are supported.");
				}
				cout << endl << "Read from flash memory..." << endl;
				size = prog->readFlash(&buffer);
				hexFile = new CHexFile(flashOptions->getPath());
				hexFile->save(buffer, size);
				delete hexFile;
				delete[] buffer;
				cout << size << " bytes read" << endl;
				break;
			case VERIFY:
				cout << endl << "Verify flash memory..." << endl;
				if (prog->verifyFlash(flashOptions->getBuffer(), flashOptions->getBufferSize()) == false) {
					cout << "failed";
					returnValue = VERIFY_ERROR_NUMBER;
				}
				else {
					cout << "OK";
				}
				cout << ", " << flashOptions->getBufferSize() << " bytes verified" << endl;
				break;
			}
		}

		// perform eeprom actions
		if (eepromOptions != NULL) {
			switch (eepromOptions->getOperation()) {
			case WRITE:
				cout << endl << "Write to eeprom memory..." << endl;
				prog->writeEEPROM(eepromOptions->getBuffer(), eepromOptions->getBufferSize());
				cout << eepromOptions->getBufferSize() << " bytes written" << endl;

				if (verify == true) {
					cout << endl << "Verify eeprom memory..." << endl;
					if (prog->fastVerifyEEPROM(eepromOptions->getBuffer(), eepromOptions->getBufferSize()) == false) {
						//cout << "failed" << endl;
						throw MyException("Verify eeprom failed.");
					}
					else {
						cout << "OK, " << eepromOptions->getBufferSize() << " bytes verified" << endl;
					}
				}
				break;
			case READ:
				if (eepromOptions->getType() != HEX) {
					throw ProgramOptionsException("Only reads into *.hex files are supported.");
				}
				cout << endl << "Read from eeprom memory..." << endl;
				size = prog->readEEPROM(&buffer);
				hexFile = new CHexFile(eepromOptions->getPath());
				hexFile->save(buffer, size);
				delete hexFile;
				delete[] buffer;
				cout << size << " bytes read" << endl;
				break;
			case VERIFY:
				cout << endl << "Verify eeprom memory..." << endl;
				if (prog->verifyEEPROM(eepromOptions->getBuffer(), eepromOptions->getBufferSize()) == false) {
					cout << "failed";
					returnValue = VERIFY_ERROR_NUMBER;
				}
				else {
					cout << "OK";
				}
				cout << ", " << eepromOptions->getBufferSize() << " bytes verified" << endl;
				break;
			}
		}
	}
	catch (ChecksumException &e) {
		cerr << "Checksum error: ";
		cerr << e.what() << endl;
		cerr << "Exiting..." << endl;
		returnValue = COMMON_ERROR_NUMBER;
	}
	catch (USBCommunicationException &e) {
		cerr << "USB communication error: ";
		cerr << e.what() << endl;
		cerr << "Exiting..." << endl;
		returnValue = COMMON_ERROR_NUMBER;
	}
	catch (CLArgumentException &e) {
		cerr << e.what() << endl;
		usage();
		returnValue = COMMON_ERROR_NUMBER;
	}
	catch (DeviceNotFoundException &e) {
		cerr << e.what() << endl;
		try {
			CAVRDevice::listDevices();
		}
		catch (DeviceException &e) {
			cerr << e.what() << endl;
		}
		cerr << "Exiting..." << endl;
		returnValue = COMMON_ERROR_NUMBER;
	}
	catch (MyException &e) {
		cerr << e.what() << endl;
		cerr << "Exiting..." << endl;
		returnValue = COMMON_ERROR_NUMBER;
	}

	// cleanup
	if (prog != NULL) {
		delete prog;
	}
	if (flashOptions != NULL) {
		delete flashOptions;
	}
	if (eepromOptions != NULL) {
		delete eepromOptions;
	}
	if (fusesOptions != NULL) {
		delete fusesOptions;
	}

	return returnValue;
}
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

#include "CAVRDevice.h"
#include "avrprog.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include "CFormat.h"
#include "COut.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost::property_tree;

CAVRDevice::CAVRDevice(string deviceFile) {
	string deviceFilePath;		// path from which the file is opened
	string configPath;			// path to device description file in CONFIG_DIR
	string homeConfigPath;		// path to device description file in HOME_CONFIG_DIR
	ptree propetries;			// property map to read xml device description file
	string deviceSignature;
	string socket;

	if (deviceFile.size() == 0) {
		throw DeviceNotFoundException("Device description file '" + deviceFile + "' not found.");
	}

	// create filepath in config dir
	configPath = CONFIG_DIR;
	configPath.append(deviceFile + ".xml");
	
	// create filepath in home dir
	homeConfigPath = getHomeDir() + HOME_CONFIG_DIR;
	homeConfigPath.append(deviceFile + ".xml");

	// check if a path is given
	string extension = "";
	unsigned int dot = deviceFile.rfind('.');
	if (dot != deviceFile.npos) {
		extension = deviceFile.substr(dot+1, 3);
	}
	if ((extension.compare("xml") == 0) && is_regular_file(deviceFile)) {
		deviceFilePath = deviceFile;
	}
	// search device file in home config directory
	else if (is_regular_file(homeConfigPath)) {
		deviceFilePath = homeConfigPath;
	}
	// search device file in config directory
	else if (is_regular_file(configPath)) {
		deviceFilePath = configPath;
	}
	else {
		throw DeviceNotFoundException("Device description file '" + deviceFile + "' not found.");
	}

	try {
		COut::d("Infos from device description file '" + deviceFilePath + "':");
		read_xml(deviceFilePath, propetries);

		// read device name
		_name = propetries.get<string>("device.name");
		COut::d("\tDevice name: " + _name);

		// read flash size
		_flashSize = propetries.get<int>("device.flashSize");
		if (_flashSize <= 0) {
			throw DeviceException("Invalid flash size in device description file.");
		}
		COut::d("\tSize of flash memory: " + CFormat::intToString(_flashSize) + " bytes");

		// read eeprom size
		_eepromSize = propetries.get<int>("device.eepromSize");
		if (_eepromSize <= 0) {
			throw DeviceException("Invalid eeprom size in device description file.");
		}
		COut::d("\tSize of eeprom memory: " + CFormat::intToString(_eepromSize) + " bytes");

		// read number of fuse bytes
		_fusesSize = propetries.get<int>("device.numOfFuses");
		if (_fusesSize <= 0 || _fusesSize > 3) {
			throw DeviceException("Invalid number of fuses in device description file.");
		}
		COut::d("\tNumber of fuse bytes: " + CFormat::intToString(_fusesSize));

		// read device signature
		deviceSignature = propetries.get<string>("device.signature");
		_deviceSignature = CFormat::hexStringToInt(deviceSignature);
		if (_deviceSignature <= 0) {
			throw DeviceException("Invalid device signature in device description file.");
		}
		COut::d("\tDevice signature: 0x" + CFormat::intToHexString(_deviceSignature));

		// read socket
		socket = propetries.get("device.socket", "auto");
		if (socket.compare("TQFP64") == 0) {
			_socket = TQFP64;
		}
		else if (socket.compare("TQFP100") == 0) {
			_socket = TQFP100;
		}
#ifdef SOCKET_AUTODETECTION
		else {
			_socket = AUTO_DETECT;
		}
#else
		throw DeviceException("Invalid socket in device description file.");
#endif
		COut::d("\tSocket: " + socket);
		COut::d("");
	}
	catch (std::exception &e) {
		throw DeviceException("Error while reading device file.\n" + (string)e.what());
	}
}

int CAVRDevice::flashSize() {
	return _flashSize;
}

int CAVRDevice::eepromSize() {
	return _eepromSize;
}

int CAVRDevice::fusesSize() {
	return _fusesSize;
}

string CAVRDevice::name() {
	return _name;
}

uint32_t CAVRDevice::deviceSignature() {
	return _deviceSignature;
}

socket_t CAVRDevice::socket() {
	return _socket;
}

string CAVRDevice::getHomeDir() {
	char const* home = getenv("HOME");
	string ret = home + (string)"/";
	
	return ret;
}

void CAVRDevice::listDevices() {
	bool configDirFound = false;
	string homeConfigDir;
	
	homeConfigDir = getHomeDir() + HOME_CONFIG_DIR;
	
	try {
		cout << "List of supported mcu types:" << endl;
		// list all *.xml files in CONFIG_DIR
		if (is_directory(CONFIG_DIR)) {
			configDirFound = true;
			cout << "system wide:" << endl;
			directory_iterator endIter;
			for (directory_iterator dirIter(CONFIG_DIR); dirIter != endIter; dirIter++) {
				if (is_regular_file(dirIter->status())) {
					if (dirIter->path().extension().compare(".xml") == 0) {
						cout << "\t" << dirIter->path().stem() << endl;
					}
				}
			}
		}
		
		// list all *.xml files in HOME_CONFIG_DIR
		if (is_directory(homeConfigDir)) {
			configDirFound = true;
			cout << "user defined:" << endl;
			directory_iterator endIter;
			for (directory_iterator dirIter(homeConfigDir); dirIter != endIter; dirIter++) {
				if (is_regular_file(dirIter->status())) {
					if (dirIter->path().extension().compare(".xml") == 0) {
						cout << "\t" << dirIter->path().stem() << endl;
					}
				}
			}
		}
		
		if (configDirFound == false) {
			throw DeviceException("Configuration directory '" + (string)CONFIG_DIR + "' does not exist.");
		}
	}
	catch (const std::exception &e) {
		throw DeviceException((string)"Error while searching for device descriptions files.\n" + e.what());
	}
}

CAVRDevice::~CAVRDevice() {

}

DeviceException::DeviceException(string err) : MyException(err) {

}

DeviceNotFoundException::DeviceNotFoundException(string err) : MyException(err) {

}

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

#ifndef CAVRDEVICE_H_
#define CAVRDEVICE_H_

#include <inttypes.h>
#include <string>
#include "avrprog.h"
#include "MyException.h"

using namespace std;

/**
 * @brief	Represents a microcontroller device.
 *
 * The information is parsed from a device description file in *.xml format,
 * which looks like the following example:
 * @code
 * <device>
 *   <name>ATmega 128</name>
 *   <signature>0x1e9702</signature>
 *   <flashSize>4096</flashSize>
 *   <eepromSize>4096</eepromSize>
 *   <numOfFuses>3</numOfFuses>
 *   <socket>TQFP64</socket>
 * </device>
 * @endcode
 * Allowed sockets are TQFP64 and TQFP100. If another or no socket is given, autodetection gets enabled.
 *
 * @throw	DeviceException if an error occur.
 *
 */
class CAVRDevice {
public:
	/**
	 * @param	deviceFile Name of a device description file in the CONFIG_DIR, or a path to a *.xml file.
	 * @throw	DeviceNotFoundException if the specified device description file does not exists.
	 */
	CAVRDevice(string deviceFile);

	/**
	 * @param	deviceSignature	signature of a device in the CONFIG_DIR.
	 * @throw	DeviceNotFoundException if the specified device description file does not exists.
	 */
	CAVRDevice(uint32_t deviceSignature);

	virtual ~CAVRDevice();

	/**
	 * @return	Size of flash memory.
	 */
	int flashSize();

	/**
	 * @return	Size of eeprom memory.
	 */
	int eepromSize();

	/**
	 * @return	Number of fuse bytes.
	 */
	int fusesSize();

	/**
	 * @return	Name of the microcontroller device.
	 */
	string name();

	/// Maximum number of fuse bytes.
	static const int MAX_FUSE_SIZE	= 3;
	/// Size of device id.
	static const int DEVICE_ID_SIZE	= 3;

	/**
	 * @return	Signature of the microcontroller device.
	 */
	uint32_t deviceSignature();

	/**
	 * @return	Physical socket of the microcontroller device.
	 */
	socket_t socket();

	/**
	 * @brief	Displays a list of all device description files in the CONFIG_DIR directory.
	 */
	static void listDevices();

protected:
	int _flashSize;
	int _eepromSize;
	int _fusesSize;
	uint32_t _deviceSignature;
	socket_t _socket;
	string _name;

	/**
	 * @return	the path to the users home deirectory
	 */
	static string getHomeDir();

private:
	void openDevicefile(string deviceFile);
	bool hasDeviceSignature(uint32_t deviceSignature, string deviceFile);
};

/**
 * Exception thrown by CAVRDevice.
 */
class DeviceException : public MyException {
public:
	DeviceException(string err);
};

/**
 * Exception thrown by CAVRDevice.
 */
class DeviceNotFoundException : public MyException {
public:
	DeviceNotFoundException(string err);
};

#endif /* CAVRDEVICE_H_ */

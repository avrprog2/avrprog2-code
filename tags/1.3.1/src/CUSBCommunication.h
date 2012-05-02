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

#ifndef CUSBCOMMUNICATION_H_
#define CUSBCOMMUNICATION_H_

#include <libusb-1.0/libusb.h>
#include <string>
#include "avrprog.h"
#include "MyException.h"

using namespace std;

/**
 * @brief	This class contains all things to communicate with an USB device with libusb.
 * @throw	USBCommunicationException on errors.
 */
class CUSBCommunication {
public:
	/**
	 * @brief	Initialize libusb
	 *
	 * Open a connection to the programmer hardware. Identifies the programmer by
	 * VENDOR_ID and DEVICE_ID and optionally by it the USB bus and device id.
	 *
	 * @param	device		USB bus and device id of the programmer hardware in the format "bus:device".
	 * 						If this parameter is the empty string, the bus and device number
	 * 						are ignored, and the first appropriate hardware in the device list is used.
	 */
	CUSBCommunication(string device);
	virtual ~CUSBCommunication();
	static const int BUFFER_LEN = 256;	///< Size of the internal buffer. This is also the limit of bytes that can be read with one transfer.

	/**
	 * @brief	Print a list of available avrprog2 devices
	 */
	static void print_device_list();

	/**
	 * @brief	Isochronous read transfer.
	 *
	 * Performs an isochronous read transfer. The method blocks until the transfer has finished and returns
	 * a pointer to the first element of the read buffer and the number of read bytes.
	 *
	 * The pointer to the buffer is valid until the next USB operation is performed.
	 *
	 * @param	endpoint	USB endpoint number of the transfer.
	 * @param	buffer		After  the transfer this pointer points to the first element of the read buffer.
	 * @param	len			Number of bytes to read. After the transfer this parameter includes the number of bytes really read.
	 * @return	Pointer to the read buffer in \a buffer
	 * @return	Number of read bytes in \a len.
	 */
	void iso_read(int endpoint, uint8_t **buffer, int *len);

	/**
	 * @brief	Isochronous write transfer.
	 *
	 * Performs an isochronous write transfer. The method blocks until the transfer has finished
	 * 	 *
	 * @param	endpoint	USB endpoint number of the transfer.
	 * @param	data		Byte array which should be transfered.
	 * @param	len			Size of the \a data array.
	 */
	void iso_write(int endpoint, uint8_t *data, int len);

	/**
	 * @brief	Interrupt read transfer.
	 *
	 * Performs an interrupt read transfer. The method blocks until the transfer has finished and returns
	 * a pointer to the first element of the read buffer and the number of read bytes.
	 *
	 * The pointer to the buffer is valid until the next USB operation is performed.
	 *
	 * @param	endpoint	USB endpoint number of the transfer.
	 * @param	buffer		After  the transfer this pointer points to the first element of the read buffer.
	 * @param	len			Number of bytes to read. After the transfer this parameter includes the number of bytes really read.
	 * @return	Pointer to the read buffer in \a buffer
	 * @return	Number of read bytes in \a len.
	 */
	void int_read(int endpoint, uint8_t **buffer, int *len);

	/**
	 * @brief	Interrupt write transfer.
	 *
	 * Performs an interrupt write transfer. The method blocks until the transfer has finished
	 * 	 *
	 * @param	endpoint	USB endpoint number of the transfer.
	 * @param	data		Byte array which should be transfered.
	 * @param	len			Size of the \a data array.
	 */
	void int_write(int endpoint, uint8_t *data, int len);
private:
	libusb_context *context;
	libusb_device_handle *dev;
	struct libusb_transfer *transfer;
	int isoReceivedLen;
	bool error;
	string errorMsg;

	uint8_t buffer[BUFFER_LEN];

	// The following methods are a wrappers from the asynchronous (non blocking) libusb isochrounous transfer function to a synchrounsous (blocking) one.
	static void callback(struct libusb_transfer *transfer);
	void iso_transfer(int endpoint, uint8_t *buffer, int *len);
};

/**
 * @brief	Exception thrown by CUSBCommunication
 */
class USBCommunicationException : public MyException {
public:
	USBCommunicationException(string err);
};

#endif /* CUSBCOMMUNICATION_H_ */

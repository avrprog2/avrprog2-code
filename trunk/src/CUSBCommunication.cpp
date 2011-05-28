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

#include "CUSBCommunication.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "CFormat.h"
#include "COut.h"

using namespace std;

CUSBCommunication::CUSBCommunication() {
	int ret;

	COut::d("Connect to usb device...");
	COut::dd("\tVendor ID: 0x" + CFormat::intToHexString(VENDOR_ID));
	COut::dd("\tDevice ID: 0x" + CFormat::intToHexString(DEVICE_ID));
	COut::dd("\tInterface: " + CFormat::intToHexString(INTERFACE));
	COut::dd("");

	// init libusb
	libusb_init(&context);

	// search programmer device
	dev = libusb_open_device_with_vid_pid(context, VENDOR_ID, DEVICE_ID);
	if (dev == NULL) {
		throw USBCommunicationException("Device not found");
	}

	// configure the device
	ret = libusb_set_configuration(dev, 1);
	if (ret != LIBUSB_SUCCESS) {
		throw USBCommunicationException("Setting Configuration failed");
	}

	// connect to interface 0
	ret = libusb_claim_interface(dev, INTERFACE);
	if (ret != LIBUSB_SUCCESS) {
		throw USBCommunicationException("Claiming Interface failed");
	}
}

void CUSBCommunication::int_read(int endpoint, uint8_t **buffer, int *len) {
	int err;
	stringstream str;
	int urbLen;

	if (*len > BUFFER_LEN) {
		throw USBCommunicationException("Cannot transfer " + CFormat::intToString(*len) + " bytes in one transfer. (Limit is " + CFormat::intToString(BUFFER_LEN) + " bytes.)");
	}

	urbLen = *len;

	err = libusb_interrupt_transfer(dev, endpoint | LIBUSB_ENDPOINT_IN, this->buffer, urbLen, len, TIMEOUT);
	if (err != LIBUSB_SUCCESS) {
		str << err;
		throw USBCommunicationException("Error (" + str.str() + ") while read (interrupt)");
	}
	*buffer = this->buffer;
}

void CUSBCommunication::int_write(int endpoint, uint8_t *buffer, int len) {
	int err;
	stringstream str;
	int transfered;

	err = libusb_interrupt_transfer(dev, endpoint | LIBUSB_ENDPOINT_OUT, buffer, len, &transfered, TIMEOUT);
	if (err != LIBUSB_SUCCESS) {
		str << err;
		throw USBCommunicationException("Error (" + str.str() + ") while write (interrupt)");
	}
}

void CUSBCommunication::iso_read(int endpoint, uint8_t **buffer, int *len) {
	if (*len > BUFFER_LEN) {
		throw USBCommunicationException("Cannot transfer " + CFormat::intToString(*len) + " bytes in one transfer. (Limit is " + CFormat::intToString(BUFFER_LEN) + " bytes.)");
	}
	iso_transfer(endpoint | LIBUSB_ENDPOINT_IN, this->buffer, len);
	*buffer = this->buffer;
}

void CUSBCommunication::iso_write(int endpoint, uint8_t *buffer, int len) {
	iso_transfer(endpoint | LIBUSB_ENDPOINT_OUT, buffer, &len);
}

void CUSBCommunication::iso_transfer(int endpoint, uint8_t *buffer, int *len) {
	int numOfPackets;
	int err;

	numOfPackets = *len / libusb_get_max_iso_packet_size(libusb_get_device(dev), endpoint);
	if (numOfPackets == 0) {
		numOfPackets = 1;
	}

	// prepare
	transfer = libusb_alloc_transfer(numOfPackets+1);
	if (transfer == NULL) {
		throw USBCommunicationException("Error while allocating Transfer");
	}


	transfer->flags = LIBUSB_TRANSFER_FREE_TRANSFER;

	libusb_fill_iso_transfer(transfer, dev, endpoint, buffer, *len, numOfPackets, callback, this, TIMEOUT);
	libusb_set_iso_packet_lengths(transfer, libusb_get_max_iso_packet_size(libusb_get_device(dev), endpoint));

	// start transfer
	err = libusb_submit_transfer(transfer);
	if (err != LIBUSB_SUCCESS) {
		throw USBCommunicationException("Error (" + CFormat::intToString(err) + ") transmitting Transfer");
	}

	// wait
	error = false;
	err = libusb_handle_events(context);
	if (err != LIBUSB_SUCCESS) {
		throw USBCommunicationException("Error (" + CFormat::intToString(err) + ") while handling Events)");
	}

	if (error == true) {
		throw USBCommunicationException(errorMsg);
	}

	buffer = this->buffer;
	*len = this->isoReceivedLen;
}

void CUSBCommunication::callback(struct libusb_transfer *transfer) {
	// regenerate "this" pointer named self
	CUSBCommunication *self;
	self = (CUSBCommunication*)transfer->user_data;

	if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
		//cout << transfer->num_iso_packets << endl;
		//cout << transfer->iso_packet_desc[0].length << endl;

		self->isoReceivedLen = 0;
		// check each socket of the transfer
		for (int i=0; i<transfer->num_iso_packets; i++) {
			//printf("%d: len: %d, alen %d, state: %d\n", 100, transfer->iso_packet_desc[i].length,transfer->iso_packet_desc[i].actual_length, transfer->iso_packet_desc[i].status);
			if (transfer->iso_packet_desc[i].actual_length > 0) {
				self->isoReceivedLen += transfer->iso_packet_desc[i].actual_length;
				if (transfer->iso_packet_desc[i].status != LIBUSB_TRANSFER_COMPLETED ) {
					// throwing an exception in the callback leads to crashes
					self->error = true;
					self->errorMsg = "Error during isochronous usb transfer.";
					//self->errorMsg = "Error (" + CFormat::intToString(transfer->iso_packet_desc[i].status) + ") in Packet " + CFormat::intToString(i) + ".";
				}

			}
			else {
				break;
			}
		}
	}
	else {
		// throwing an exception in the callback leads to crashes
		self->error = true;
		self->errorMsg = "Error (" + CFormat::intToString(transfer->status) + ") while waiting for isochronous transfer.";
	}
}

CUSBCommunication::~CUSBCommunication() {
	COut::d("Close usb connection...");

	if (dev != NULL) {
		libusb_release_interface(dev, INTERFACE);
		libusb_close(dev);
	}
	if (context != NULL) {
		libusb_exit(context);
	}
}

USBCommunicationException::USBCommunicationException(string err) : MyException(err) {

}


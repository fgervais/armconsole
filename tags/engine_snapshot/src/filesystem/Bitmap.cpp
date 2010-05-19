/*
 * Bitmap.cpp
 *
 *  Created on: Mar 25, 2010
 *      Author: fgervais
 */

#include "Bitmap.h"
#include "Debug.h"

Bitmap::Bitmap(const XCHAR *path) {
	this->path = path;

	magic = 0;
	header = 0;
	infoHeader = 0;
	data = 0;
	loaded = 0;
}

Bitmap::~Bitmap() {
	/* Free memore */
	delete magic;
	delete header;
	delete infoHeader;
	delete[] data;
}

/**
 * Load the bitmap file to memory.
 *
 * @return 0 if successful. 1 otherwise.
 */
uint8_t Bitmap::load() {
	FRESULT f_err_code;
	UINT byteRead;
	uint8_t* ptr;

	f_err_code = f_open(&handle, path, FA_OPEN_EXISTING | FA_READ);

	if(f_err_code == 0) {
		//Debug::writeLine("Bitmap File opened");
	}
	else {
		Debug::writeLine("Failed to open file");
		return 1;
	}

	magic = new Magic();
	f_read(&handle, (void*)magic, sizeof(Magic), &byteRead);

	if(magic->content[0] != 'B' || magic->content[1] != 'M') {
		Debug::writeLine("File invalid");
		return 1;
	}

	header = new Header();
	f_read(&handle, (void*)header, sizeof(Header), &byteRead);

	infoHeader = new InfoHeader();
	f_read(&handle, (void*)infoHeader, sizeof(InfoHeader), &byteRead);

	uint32_t dataLength = infoHeader->height*infoHeader->width;
	data = new uint32_t[dataLength];

	// Compute padding
	uint32_t padLength = 0;
	while((((infoHeader->width * (infoHeader->bpp/8))+padLength) % 4) != 0) {
		padLength++;
	}

	ptr = (uint8_t*)&data[dataLength];

	// Get at the end of the last line
	ptr -= infoHeader->width*4;

	for(int32_t i=0; i<infoHeader->height; i++) {
		// Fill a line
		for(int32_t j=0; j<infoHeader->width; j++) {
			// Bitmap is stored as BRG format so we have to invert it
			f_read(&handle, (void*)(ptr+2), 1, &byteRead);
			f_read(&handle, (void*)(ptr+1), 1, &byteRead);
			f_read(&handle, (void*)(ptr), 1, &byteRead);
			ptr += 4;
		}
		// Go up 2 lines
		ptr -= 2*infoHeader->width*4;
		// Skip padding
		f_lseek(&handle, handle.fptr + padLength);
	}

	f_close(&handle);
	loaded = 1;
	return 0;
}

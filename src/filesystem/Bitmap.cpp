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
	uint32_t ptr;

	f_err_code = f_open(&handle, "0:mmx1.bmp", FA_OPEN_EXISTING | FA_READ);

	if(f_err_code == 0) {
		Debug::writeLine("File opened");
	}
	else {
		Debug::writeLine("Failed to open file");
		return 1;
	}

	ptr = 0xA0080000;
	magic = (Magic*)ptr;
	f_read(&handle, (void*)ptr, sizeof(Magic), &byteRead);

	ptr += 4;
	header = (Header*)ptr;
	f_read(&handle, (void*)ptr, sizeof(Header), &byteRead);

	ptr += sizeof(Header);
	infoHeader = (InfoHeader*)ptr;
	f_read(&handle, (void*)ptr, sizeof(InfoHeader), &byteRead);

	ptr += sizeof(InfoHeader);
	data = (uint32_t*)ptr;
	uint32_t dataLength = header->size-header->offset;

	// Compute padding
	uint32_t padLength = 0;
	while((((infoHeader->width * (infoHeader->bpp/8))+padLength) % 4) != 0) {
		padLength++;
	}

	/*for(uint32_t i=0; i<dataLength; i+=3) {


		// Bitmap is stored as BRG format
		f_read(&handle, (void*)(ptr+2), 1, &byteRead);
		f_read(&handle, (void*)(ptr+1), 1, &byteRead);
		f_read(&handle, (void*)(ptr), 1, &byteRead);
		ptr += 4;

		//if(i>0 && (i % ((infoHeader->width * (infoHeader->bpp/8)))) == 0) {
		if(i > 0 && ((i/3) % 0)) {
			f_lseek(&handle, handle.fptr + padLength);
		}
	}*/

	ptr += infoHeader->height*infoHeader->width*4;
	ptr -= infoHeader->width*4;

	for(int32_t i=0; i<infoHeader->height; i++) {
		for(int32_t j=0; j<infoHeader->width; j++) {
			f_read(&handle, (void*)(ptr+2), 1, &byteRead);
			f_read(&handle, (void*)(ptr+1), 1, &byteRead);
			f_read(&handle, (void*)(ptr), 1, &byteRead);
			ptr += 4;
		}
		ptr -= 2*infoHeader->width*4;
		f_lseek(&handle, handle.fptr + padLength);
	}

	f_close(&handle);
	return 0;
}

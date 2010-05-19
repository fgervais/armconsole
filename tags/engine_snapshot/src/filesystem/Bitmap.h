/*
 * Bitmap.h
 *
 *  Created on: Mar 25, 2010
 *      Author: fgervais
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "File.h"
#include "ff.h"
#include <stdint.h>

/**
 * Class used to handle Bitmap files.
 *
 * Memory alignment note:
 * The structures used are mean to work on a ARM7TDMI-S
 * architecture and *may not* work on different MCU due
 * to the way memory access are handled. In the case of
 * the ARM7TDMI-S, 8bits, 16bits and 32 bits memory
 * access can be done as long as 32bits access are word
 * aligned and 16bits access half word aligned. If your
 * MCU cannot do 16bits access, These data structures will
 * most likely result in a bus exception.
 *
 * Ref: ARM7TDMI-S Technical Reference Manual
 *
 * 1.1.2 Memory access
 * The ARM7TDMI-S processor has a Von Neumann architecture, with a single 32-bit
 * data bus carrying both instructions and data. Only load, store, and swap
 * instructions can access data from memory.
 *
 * Data can be 8-bit bytes, 16-bit halfwords, or 32-bit words. Words must be
 * aligned to 4-byte boundaries. Halfwords must be aligned to 2-byte boundaries.
 */
class Bitmap: public File {
public:
	/*
	 * These structures are greatly inspired from Wikipedia and
	 * Paul Bourke work (http://local.wasp.uwa.edu.au/~pbourke/dataformats/bmp/)
	 */
	struct Magic {
		uint8_t content[2];
	};

	struct Header {
		uint32_t size;				/* File size in bytes          */
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t offset;			/* Offset to image data, bytes */
	};

	/*
	 * The compression types supported by BMP are listed below :
	 *
	 * 0 - no compression
	 * 1 - 8 bit run length encoding
	 * 2 - 4 bit run length encoding
	 * 3 - RGB bitmap with mask
	 */
	struct InfoHeader {
		uint32_t size;				/* Header size in bytes      */
		int32_t width;				/* Width of image            */
		int32_t height;				/* Height of image           */
		uint16_t planes;			/* Number of colour planes   */
		uint16_t bpp;				/* Bits per pixel            */
		uint32_t compression;		/* Compression type          */
		uint32_t imageSize;			/* Image size in bytes       */
		int32_t xResolution;		/* Pixels per meter          */
		int32_t yResolution;		/* Pixels per meter          */
		uint32_t nColours;			/* Number of colours         */
		uint32_t importantColours;	/* Important colours         */
	};

	Bitmap(const XCHAR *path);
	virtual ~Bitmap();

	Magic* getMagic() { return magic; }
	Header* getHeader() { return header; };
	InfoHeader* getInfoHeader() { return infoHeader; }
	uint32_t* getData() { return data; }
	uint8_t isLoaded() { return loaded; }

	uint8_t load();

private:
	Magic* magic;
	Header* header;
	InfoHeader* infoHeader;
	uint32_t* data;

	FIL handle;
	const XCHAR *path;
	uint8_t loaded;
};

#endif /* BITMAP_H_ */

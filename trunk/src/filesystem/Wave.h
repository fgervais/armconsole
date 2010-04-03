/*
 * Wave.h
 *
 *  Created on: Apr 2, 2010
 *      Author: fgervais
 */

#ifndef WAVE_H_
#define WAVE_H_

#include "File.h"
#include "ff.h"
#include <stdint.h>

class Wave: public File {
public:
	struct RIFFHeader {
		uint8_t magic[4];			/* "RIFF" */
		uint32_t size;				/* This is usually file size - 8. */
	};

	struct WAVEHeader {
		uint8_t magic1[4];			/* "WAVE" */
		uint8_t magic2[4];			/* "fmt " */
		uint32_t size;				/* This is usually 16 (or 0x10). */
		uint16_t format;			/* PCM (0x01) */
		uint16_t numberOfChannel;	/* mono (0x01) or stereo (0x02) */
		uint32_t sampleRate;		/* Sample rate. */
		uint32_t bytePerSec;		/* Bytes/Second */
		uint16_t blockAlignment;	/* Block alignment */
		uint16_t bitPerSample;		/* Bits/Sample */
	};

	struct DataHeader {
		uint8_t magic[4];			/* "data" */
		uint32_t size;				/* Number of bytes of data is included in the data section. */
	};

	Wave(const XCHAR *path);
	virtual ~Wave();

	RIFFHeader* getRIFFHeader() { return riffHeader; }
	WAVEHeader* getWAVEHeader() { return waveHeader; }
	DataHeader* getDataHeader() { return dataHeader; }
	int16_t* getData() { return data; }
	uint8_t isLoaded() { return loaded; }

	uint8_t load();

private:
	FIL handle;
	const XCHAR *path;
	uint8_t loaded;

	RIFFHeader* riffHeader;
	WAVEHeader* waveHeader;
	DataHeader* dataHeader;
	int16_t* data;
};

#endif /* WAVE_H_ */

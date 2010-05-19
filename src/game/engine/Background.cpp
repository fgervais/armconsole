/*
 * Background.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#include "Background.h"
#include "Environment.h"
#include "VideoMemory.h"
#include "VisibleArea.h"
#include "Bitmap.h"
#include "LPC2478.h"
#include "DMAChannel.h"
#include "DMAConfiguration.h"
#include "Debug.h"

Background::Background(Bitmap* bitmap, uint32_t width, uint32_t height, Environment* environment) {
	this->bitmap = bitmap;
	this->height = height;
	this->width = width;

	this->environment = environment;

	if(!bitmap->isLoaded()) {
		bitmap->load();
	}

	uint32_t dmaControl = 480;		// Transfer size
	dmaControl |= (4<<12);	// Source burst size = 32
	dmaControl |= (4<<15);	// Destination burst size = 32
	dmaControl |= (2<<18);	// Source width = 32 bits
	dmaControl |= (2<<21);	// Destination width = 32 bits
	dmaControl |= (1<<26);	// Source increment
	dmaControl |= (1<<27);	// Destination increment

	// Create LLI for DMA transfer
	lli = new DMACCxLLI*[height];
	for(uint32_t i=0; i<height; i++) {
		lli[i] = new DMACCxLLI();
		lli[i]->DMACCxControl = dmaControl;
	}

}

Background::~Background() {

}

/**
 * Background rendering function.
 *
 * This is quite a critical function since it's the hardest thing the console
 * has to do on every cycle.
 *
 * This function has been optimized but not without some drawback.
 * First, the background must repeat every 256 pixels and has to be
 * 768 pixel wide. That means that the same image repeat 3 times.
 *
 * Furthermore, the background will be rendered only once vertically.
 *
 * @param videoMemory
 */
void Background::render(VideoMemory* videoMemory) {
	VisibleArea* visibleArea = environment->getVisibleArea();

	/*
	 * Calculate the visible area of the background according to the speed multiplier.
	 *
	 * We do this because the background normally scroll slower than the layer
	 * where the hero is.
	 */
	uint32_t renderX1 = visibleArea->x >> 1;
	//uint32_t renderY1 = visibleArea->y1 >> 1;

	//uint32_t renderWidth = visibleArea->x2 - visibleArea->x1;
	//uint32_t renderHeight = visibleArea->y2 - visibleArea->y1;

	// These are safe initial guess
	//uint32_t renderMaskX1 = renderX1 % width;
	//uint32_t renderMaskY1 = renderY1 % height;
	uint32_t renderMaskX1 = renderX1 & 255;

	// Draw the image on the screen
	uint32_t videoMemoryWidth = videoMemory->getWidth();
	uint32_t* videoMemoryPointer = videoMemory->getPointer();

	/* Generic version */
	/*for (uint32_t i=0; i<renderHeight; i++) {
		for (uint32_t j=0; j<renderWidth; j++) {
			videoMemoryPointer[i*videoMemoryWidth + j]
			                   = bitmap->getData()[((i+renderMaskY1) % height)*width + ((j+renderMaskX1) % width)];
		}
	}*/

	/* Optimized version */
	/*uint32_t* image = bitmap->getData();
	for (uint32_t i=0; i<renderHeight; i++) {
		for (uint32_t j=0; j<renderWidth; j++) {
			videoMemoryPointer[i*videoMemoryWidth + j]
							   = image[i*width + ((j+renderMaskX1) & (width-1))];
		}
	}*/

	/* Optimized version 2 */
	/*uint32_t* image = bitmap->getData();
	uint32_t widthm1 = width-1;
	for (uint32_t i=0; i<height; i++) {
		for (uint32_t j=0; j<renderWidth; j++) {
			videoMemoryPointer[i*videoMemoryWidth + j]
							   = image[i*width + ((j+renderMaskX1) & widthm1)];
		}
	}*/

	/* DMA version 1 */
	/*DMAChannel* dma = LPC2478::getDMA1();

	DMAConfiguration dmaConfig;

	uint32_t* image = bitmap->getData();

	lli[0]->DMACCxSrcAddr = (uint32_t)&(image[renderMaskX1]);
	lli[0]->DMACCxDestAddr = (uint32_t)&(videoMemoryPointer[0]);

	for(uint32_t i=1; i<height; i++) {
		lli[i]->DMACCxSrcAddr = (uint32_t)&(image[i*width + renderMaskX1]);
		lli[i]->DMACCxDestAddr = (uint32_t)&(videoMemoryPointer[i*videoMemoryWidth]);
		lli[i-1]->DMACCxLLI = (uint32_t)lli[i];
	}
	lli[height-1]->DMACCxLLI = 0;*/

	/* DMA version 2 */

	DMAChannel* dma = LPC2478::getDMA1();
	DMAConfiguration dmaConfig;

	uint32_t casted_image = (uint32_t)bitmap->getData();
	uint32_t casted_videoMemoryPointer = (uint32_t)videoMemoryPointer;
	uint32_t* struct_pointer;
	uint32_t* previous_lli;

	casted_image += renderMaskX1<<2;

	struct_pointer = (uint32_t*)lli[0];
	*struct_pointer = casted_image;
	*(struct_pointer+1) = casted_videoMemoryPointer;

	for(uint32_t i=1; i<height; i++) {
		previous_lli = struct_pointer+2;
		struct_pointer = (uint32_t*)lli[i];

		*previous_lli = (uint32_t)struct_pointer;

		casted_image += width<<2;
		*struct_pointer = casted_image;

		casted_videoMemoryPointer += videoMemoryWidth<<2;
		*(struct_pointer+1) = casted_videoMemoryPointer;
	}
	*(struct_pointer+2) = 0;

	dmaConfig.lli = lli[0];

	dma->configure(dmaConfig);
	dma->enable();

	while(dma->isEnabled());
}

void Background::set(Environment* environment) {
	this->environment = environment;
}

/* PRIVATE FUNCTIONS */

/*
 * Memory.cpp
 *
 *  Created on: Jan 24, 2010
 *      Author: fgervais
 */

#include "Memory.h"

Memory::Memory(void* poolLocation, uint32_t poolSize) {
	this->poolLocation = (uint8_t*)poolLocation;
	this->poolSize = poolSize;

	freeMemoryPointer = (uint8_t*)poolLocation;
}

Memory::~Memory() {
	free();
}

void* Memory::allocate(uint32_t size) {
	return allocate(size, 4);
}

void* Memory::allocate(uint32_t size, uint32_t align) {
	MemoryChunk* chunk;

	// Check if alignment is correct
	if(((uint32_t)freeMemoryPointer & (align-1)) != 0) {
		// Align pointer
		freeMemoryPointer = (uint8_t*)(((uint32_t)freeMemoryPointer + align) & ~(align-1));
	}

	chunk = new MemoryChunk();

	return 0;
}

void Memory::free() {

}

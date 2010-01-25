/*
 * Memory.h
 *
 *  Created on: Jan 24, 2010
 *      Author: fgervais
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>

class Memory;

/**
 * This class is used to manage a memory pool.
 *
 * It is meant to be used with memory space outside
 * of the malloc (sbrk) range. For example, a USB
 * controller may have some memory of it own which is
 * outside  of the heap space.
 *
 * On the other hand, this class will not protect
 * you against using some memory inside the heap.
 * If you do so, at some point in your program, it
 * is possible that a malloced memory chuck include
 * the memory used by this pool.
 *
 * So if you want to use this class to manage a memory
 * block inside the heap, make sure that is is allocated
 * for you first.
 *
 * Something like this should be safe :
 *
 * int* buffer = new int[32];
 * Memory* memoryBlock = new Memory(buffer, sizeof(int) * 32);
 */
class Memory {
public:
	Memory(void* poolLocation, uint32_t poolSize);
	virtual ~Memory();

	void* alloc(uint32_t size);
	void* alloc(uint32_t size, uint32_t align);
	void dealloc(void* ptr);

private:
	Memory* usbMemory;
};

#endif /* MEMORY_H_ */

/*
 * MemoryPool.h
 *
 *  Created on: Jul 4, 2010
 *      Author: fgervais
 */

#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include <stdint.h>

class MemoryPool {
public:
	MemoryPool();
	virtual ~MemoryPool();

	uint8_t free;
	void* head;
	uint32_t size;
};

#endif /* MEMORYPOOL_H_ */

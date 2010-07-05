/*
 * MemoryPool.cpp
 *
 *  Created on: Jul 4, 2010
 *      Author: fgervais
 */

#include "MemoryPool.h"

MemoryPool::MemoryPool() {
	free = 1;
	head = 0;
	size = 0;
}

MemoryPool::~MemoryPool() {

}

#include "EntityAllocator.h"

#include <stdlib.h>


EntityAllocator::EntityAllocator()
{
	m_buffer = malloc(8 * 1024 * 1024);
	m_pointer = 0;
}

void* EntityAllocator::allocate(unsigned int size)
{
	void* ptr = reinterpret_cast<char*>(m_buffer) + m_pointer;
	m_pointer += size;
	return ptr;
}

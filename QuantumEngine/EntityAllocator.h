#pragma once


class EntityAllocator
{
public:

	static EntityAllocator& getInstance() { static EntityAllocator instance; return instance; }

	template<class EntityClass>
	EntityClass* allocate() {
		return reinterpret_cast<EntityClass*>(this->allocate(sizeof(EntityClass)));
	}

	void* allocate(unsigned int size);

private:

	EntityAllocator();


private:

	void* m_buffer;
	unsigned int m_pointer;
};

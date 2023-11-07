#pragma once

#include "QuEntity.h"

class PhysicsWorld;



class QuWorld : public QuEntity
{
public:

	QuWorld();
	~QuWorld();

	//virtual void ExecuteProcedure();

	inline PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld; }

	inline bool isOpen() const { return m_isOpen; }
	inline void setOpen(bool b) { m_isOpen = b; }


private:

	PhysicsWorld* m_physicsWorld;

	bool m_isOpen;
};

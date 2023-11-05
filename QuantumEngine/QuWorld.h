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


private:

	PhysicsWorld* m_physicsWorld;
};

#pragma once

#include "QuEntity.h"

class RigidBody;
class PhysicsShape;



class QuEntityPhysicsCollider
{
public:

	QuEntityPhysicsCollider();


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;
};

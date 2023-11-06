#pragma once

class PhysicsShape;



class CollisionAgent
{
public:

	virtual bool getOverlapping(RigidBody* a, RigidBody* b) const = 0;
};

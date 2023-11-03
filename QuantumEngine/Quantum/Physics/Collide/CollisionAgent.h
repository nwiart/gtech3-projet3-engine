#pragma once

class PhysicsShape;



struct ShapeKeyPair
{

};



class CollisionAgent
{
public:

	static void registerCollisionAgents();

	static CollisionAgent* getCollisionAgent(PhysicsShape* a, PhysicsShape* b);


public:

	virtual bool getOverlapping(RigidBody* a, RigidBody* b) const = 0;
};

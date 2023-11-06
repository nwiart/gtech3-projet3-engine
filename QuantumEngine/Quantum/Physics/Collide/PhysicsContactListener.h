#pragma once

class RigidBody;



struct CollisionEvent
{
	RigidBody* m_rigidBodyA;
	RigidBody* m_rigidBodyB;
};



class PhysicsContactListener
{
public:

	virtual void onCollisionAdded(const CollisionEvent& info) { }

	virtual void onCollisionRemoved(const CollisionEvent& info) { }
};

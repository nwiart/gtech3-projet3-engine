#pragma once

#include "Quantum/Physics/RayCast.h"

#include <vector>
#include <map>

class RigidBody;
class PhysicsShape;
class CollisionAgent;



struct PhysicsWorldCinfo
{
	DirectX::XMFLOAT3 m_gravity;
	float m_broadphaseSize;
	int m_broadphaseDivisions;

	PhysicsWorldCinfo();
};



class PhysicsWorld
{
public:

	PhysicsWorld(const PhysicsWorldCinfo& info);

	void step(float deltaTime);


	void addRigidBody(RigidBody* rb);
	void removeRigidBody(RigidBody* rb);

	bool rayCast(RayHitResult& outHit, DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, float length);
	bool rayCast(RayHitResult& outHit, DirectX::FXMVECTOR begin, DirectX::FXMVECTOR end);
	bool rayCast(RayHitResult& outHit, const Ray& ray);


	void registerCollisionAgents();

	CollisionAgent* getCollisionAgent(PhysicsShape* a, PhysicsShape* b);



private:

	std::vector<RigidBody*> m_dynamicRigidBodies;
	std::vector<RigidBody*> m_staticRigidBodies;

	std::map<uint32_t, CollisionAgent*> m_collisionAgents;

	DirectX::XMFLOAT3 m_gravity;
};

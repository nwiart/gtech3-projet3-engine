#pragma once

#include "Quantum/Physics/RayCast.h"

#include <vector>

class RigidBody;



struct PhysicsWorldCinfo
{
	DirectX::XMFLOAT3 m_gravity;
	float m_broadphase;

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



private:

	std::vector<RigidBody*> m_dynamicRigidBodies;
	std::vector<RigidBody*> m_staticRigidBodies;

	DirectX::XMFLOAT3 m_gravity;
};

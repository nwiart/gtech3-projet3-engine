#include "QuWorld.h"

#include "Quantum/Physics/PhysicsWorld.h"



QuWorld::QuWorld()
{
	PhysicsWorldCinfo info;
	info.m_gravity = DirectX::XMFLOAT3(0.0F, -0.0F, 0.0F);

	m_physicsWorld = new PhysicsWorld(info);
	m_physicsWorld->registerCollisionAgents();

	m_isOpen = false;
}

QuWorld::~QuWorld()
{
	delete m_physicsWorld;
}

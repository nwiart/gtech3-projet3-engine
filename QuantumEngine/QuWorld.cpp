#include "QuWorld.h"

#include "Quantum/Physics/PhysicsWorld.h"



QuWorld::QuWorld()
{
	PhysicsWorldCinfo info;
	info.m_gravity = DirectX::XMFLOAT3(0.0F, -0.0F, 0.0F);

	m_physicsWorld = new PhysicsWorld(info);
	m_physicsWorld->registerCollisionAgents();
}

QuWorld::~QuWorld()
{
	delete m_physicsWorld;
}


void QuWorld::markForDeletion(QuEntity* e)
{
	m_markedForDelete.push_back(e);
}


void QuWorld::deletePendingEntities()
{
	for (QuEntity* e : m_markedForDelete) {
		// Move children up.
		for (QuEntity* c = e->m_FirstChild; c; c = c->m_Sibling) {
			c->AttachToParent(e->m_Parent);
		}

		e->removeAllAttachments();

		delete e;
	}

	m_markedForDelete.clear();
}

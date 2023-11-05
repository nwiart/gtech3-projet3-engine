#include "QuEntityPhysicsTriggerSphere.h"

#include "QuWorld.h"

#include "Quantum/Physics/PhysicsWorld.h"
#include "Quantum/Physics/RigidBody.h"
#include "Quantum/Physics/Collide/PhysicsShapeSphere.h"
#include "Quantum/Physics/Collide/PhysicsContactListener.h"



class MyTriggerListener : public PhysicsContactListener
{
public:

	virtual void onCollisionAdded(const CollisionEvent& event) override
	{
		QuEntityPhysicsTriggerSphere* myTrigger = reinterpret_cast<QuEntityPhysicsTriggerSphere*>(event.m_rigidBodyA->getUserData() );
		if (myTrigger) {
			
			myTrigger->m_overlappingBodies.push_back(event.m_rigidBodyB);
			myTrigger->onTriggerEnter(reinterpret_cast<QuEntity*>(event.m_rigidBodyB->getUserData()));
		}
	}

	virtual void onCollisionRemoved(const CollisionEvent& event) override
	{
		QuEntityPhysicsTriggerSphere* myTrigger = reinterpret_cast<QuEntityPhysicsTriggerSphere*>( event.m_rigidBodyA->getUserData() );
		if (myTrigger) {

			myTrigger->onTriggerLeave(reinterpret_cast<QuEntity*>(event.m_rigidBodyB->getUserData()));
			myTrigger->m_overlappingBodies.erase(std::remove(myTrigger->m_overlappingBodies.begin(), myTrigger->m_overlappingBodies.end(), event.m_rigidBodyB));
		}
	}
};



QuEntityPhysicsTriggerSphere::QuEntityPhysicsTriggerSphere(float radius)
	: m_radius(radius)
{
	m_rigidBody = 0;
	m_shape = 0;
	m_listener = 0;
}

void QuEntityPhysicsTriggerSphere::OnSpawn(QuWorld* world)
{
	// Create shape and rigid body.
	m_shape = new PhysicsShapeSphere(m_radius);

	RigidBodyCinfo info;
	info.m_motionType = MOTION_STATIC;
	info.m_shape = m_shape;
	DirectX::XMStoreFloat4(&info.m_position, this->getWorldPosition());
	DirectX::XMStoreFloat4(&info.m_rotation, this->getWorldRotation());

	m_rigidBody = new RigidBody(info);

	// Set this entity as user data.
	m_rigidBody->setUserData(this);

	// Register contact listener.
	m_listener = new MyTriggerListener();
	m_rigidBody->addContactListener(m_listener);

	world->getPhysicsWorld()->addRigidBody(m_rigidBody);
}

void QuEntityPhysicsTriggerSphere::OnUpdate(const Timer& timer)
{
	for (RigidBody* rb : m_overlappingBodies) {
		QuEntity* e = reinterpret_cast<QuEntity*>(rb->getUserData());
		this->onTriggerStay(e);
	}
}

void QuEntityPhysicsTriggerSphere::OnDestroy(QuWorld* world)
{
	if (m_rigidBody) {
		world->getPhysicsWorld()->removeRigidBody(m_rigidBody);

		delete m_rigidBody;
		delete m_shape;
		delete m_listener;

		m_rigidBody = 0;
		m_shape = 0;
		m_listener = 0;
	}
}

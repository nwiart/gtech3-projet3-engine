#include "QuEntityPhysicsCollider.h"

#include "QuWorld.h"

#include "Quantum/Physics/PhysicsWorld.h"
#include "Quantum/Physics/RigidBody.h"
#include "Quantum/Physics/Collide/PhysicsShapeSphere.h"
#include "Quantum/Physics/Collide/PhysicsContactListener.h"

using namespace DirectX;



class MyCollideListener : public PhysicsContactListener
{
public:

	virtual void onCollisionAdded(const CollisionEvent& event) override
	{
		QuEntityPhysicsCollider* myRb = reinterpret_cast<QuEntityPhysicsCollider*>(event.m_rigidBodyA->getUserData());
		if (myRb) {

			myRb->onCollide(reinterpret_cast<QuEntity*>(event.m_rigidBodyB->getUserData()));
		}
	}
};



QuEntityPhysicsCollider::QuEntityPhysicsCollider(float radius, EMotionType motionType)
	: m_rigidBody(0)
	, m_shape(0)
	, m_radius(radius)
	, m_motionType(motionType)
{
	m_linearVelocity = { 0, 0, 0, 0 };
}

void QuEntityPhysicsCollider::applyImpulse(FXMVECTOR f)
{
	XMVECTOR newVel = XMVectorAdd(f, XMLoadFloat4(&m_linearVelocity));
	XMStoreFloat4(&m_linearVelocity, newVel);

	if (m_rigidBody) {
		m_rigidBody->applyImpulse(f);
	}
}

void QuEntityPhysicsCollider::setPosition(FXMVECTOR f)
{
	XMFLOAT3 pos; XMStoreFloat3(&pos, f);
	QuEntity::setPosition(pos);

	if (m_rigidBody) {
		m_rigidBody->setPosition(f);
	}
}


void QuEntityPhysicsCollider::OnSpawn(QuWorld* world)
{
	m_shape = new PhysicsShapeSphere(m_radius);

	RigidBodyCinfo info;
	info.m_motionType = m_motionType;
	info.m_shape = m_shape;
	XMStoreFloat4(&info.m_position, XMLoadFloat3(&this->GetTransform().getPosition()));
	m_rigidBody = new RigidBody(info);

	m_listener = new MyCollideListener();
	m_rigidBody->setUserData(this);
	m_rigidBody->addContactListener(m_listener);

	m_rigidBody->applyImpulse(XMLoadFloat4(&m_linearVelocity));

	world->getPhysicsWorld()->addRigidBody(m_rigidBody);
}

void QuEntityPhysicsCollider::OnUpdate(const Timer& timer)
{
	if (m_rigidBody) {
		XMVECTOR v = XMLoadFloat4(&m_rigidBody->getPosition());
		this->setPosition(v);
	}
}

void QuEntityPhysicsCollider::OnDestroy(QuWorld* world)
{
	if (m_rigidBody)
	{
		world->getPhysicsWorld()->removeRigidBody(m_rigidBody);

		delete m_rigidBody;
		delete m_shape;
		delete m_listener;

		m_rigidBody = 0;
		m_shape = 0;
		m_listener = 0;
	}
}

XMVECTOR QuEntityPhysicsCollider::GetLinearVelocity()
{
	return XMLoadFloat4(&m_rigidBody->getLinearVelocity());
}

void QuEntityPhysicsCollider::setLinearVelocity(DirectX::FXMVECTOR p)
{
	m_rigidBody->setLinearVelocity(p);
}

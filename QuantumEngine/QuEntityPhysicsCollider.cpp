#include "QuEntityPhysicsCollider.h"

#include "QuWorld.h"

#include "Quantum/Physics/PhysicsWorld.h"
#include "Quantum/Physics/RigidBody.h"
#include "Quantum/Physics/Collide/PhysicsShapeSphere.h"

using namespace DirectX;



QuEntityPhysicsCollider::QuEntityPhysicsCollider()
	: m_rigidBody(0)
	, m_shape(0)
{
	
}

void QuEntityPhysicsCollider::OnSpawn(QuWorld* world)
{
	m_shape = new PhysicsShapeSphere(0.5F);

	RigidBodyCinfo info;
	info.m_motionType = MOTION_KEYFRAMED;
	info.m_shape = m_shape;
	XMStoreFloat4(&info.m_position, XMLoadFloat3(&this->GetTransform().getPosition()));
	m_rigidBody = new RigidBody(info);

	m_rigidBody->setUserData(this);

	world->getPhysicsWorld()->addRigidBody(m_rigidBody);
}

void QuEntityPhysicsCollider::OnUpdate(const Timer& timer)
{
	if (m_rigidBody) {
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, XMLoadFloat4(&m_rigidBody->getPosition()));
		this->setPosition(pos);
	}
}

void QuEntityPhysicsCollider::OnDestroy(QuWorld* world)
{
	if (m_rigidBody)
	{
		world->getPhysicsWorld()->removeRigidBody(m_rigidBody);

		delete m_rigidBody;
		delete m_shape;

		m_rigidBody = 0;
		m_shape = 0;
	}
}

#include "Quantum/Math/Math.h"
#include "RigidBody.h"

using namespace DirectX;



RigidBodyCinfo::RigidBodyCinfo()
	: m_motionType(MOTION_STATIC)
	, m_shape(0)
{

}



RigidBody::RigidBody(const RigidBodyCinfo& info)
	: m_world(0)
	, m_shape(info.m_shape)
	, m_motionType(info.m_motionType)
{
	assert(m_shape != 0);

	m_initiallyStatic = (info.m_motionType != MOTION_DYNAMIC);

	XMVECTOR zero = XMVectorZero();
	XMStoreFloat4(&m_position, XMLoadFloat4(&info.m_position));
	XMStoreFloat4(&m_linearVelocity, zero);
}

void RigidBody::setPosition(FXMVECTOR p)
{
	XMStoreFloat4(&m_position, p);
}

void RigidBody::setRotation(FXMVECTOR p)
{
	XMStoreFloat4(&m_rotation, p);
}

void RigidBody::setLinearVelocity(FXMVECTOR p)
{
	XMStoreFloat4(&m_linearVelocity, p);
}

void RigidBody::applyImpulse(FXMVECTOR force)
{
	XMVECTOR vel = XMLoadFloat4(&m_linearVelocity);
	vel = XMVectorAdd(vel, force);
	XMStoreFloat4(&m_linearVelocity, vel);
}

void RigidBody::setMotionType(EMotionType mt)
{
	assert(!m_initiallyStatic);

	m_motionType = mt;
}

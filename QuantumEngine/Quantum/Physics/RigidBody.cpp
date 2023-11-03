#include "Quantum/Math/Math.h"
#include "RigidBody.h"

using namespace DirectX;



RigidBody::RigidBody(const RigidBodyCinfo& info)
	: m_world(0)
	, m_shape(info.m_shape)
	, m_motionType(info.m_motionType)
{
	assert(m_shape != 0);
}

void RigidBody::setPosition(FXMVECTOR p)
{

}

void RigidBody::setRotation(FXMVECTOR p)
{

}

void RigidBody::setLinearVelocity(FXMVECTOR p)
{
	XMStoreFloat4(&m_linearVelocity, p);
}

void RigidBody::applyImpulse(FXMVECTOR force)
{
	XMVECTOR vel = XMLoadFloat4(&m_linearVelocity);
	XMVectorAdd(vel, force);
	XMStoreFloat4(&m_linearVelocity, vel);
}

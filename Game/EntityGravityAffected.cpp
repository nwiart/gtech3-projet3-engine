#include "EntityGravityAffected.h"

#include "QuEntityPhysicsCollider.h"



EntityGravityAffected::EntityGravityAffected(QuEntityPhysicsCollider* rigidBodyEntity)
	: m_rigidBodyEntity(rigidBodyEntity)
{
	m_localGravity = DirectX::XMFLOAT3(0, 0, 0);
}

EntityGravityAffected::~EntityGravityAffected()
{

}

void EntityGravityAffected::OnUpdate(const Timer& t)
{
	if (m_rigidBodyEntity) {
		DirectX::XMVECTOR deltaVec = DirectX::XMVectorReplicate(t.getDeltaTime());
		deltaVec = DirectX::XMVectorMultiply(deltaVec, DirectX::XMLoadFloat3(&m_localGravity));

		m_rigidBodyEntity->applyImpulse(deltaVec);
	}
}

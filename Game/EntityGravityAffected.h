#pragma once

#include <QuEntity.h>

class QuEntityPhysicsCollider;



class EntityGravityAffected : public QuEntity
{
public:

	EntityGravityAffected(QuEntityPhysicsCollider* rigidBodyEntity);
	virtual ~EntityGravityAffected();

	virtual void OnUpdate(const Timer& t) override;

	inline QuEntityPhysicsCollider* getRigidBodyEntity() const { return m_rigidBodyEntity; }

	inline const DirectX::XMFLOAT3& getLocalGravity() const { return m_localGravity; }
	inline void setLocalGravity(DirectX::FXMVECTOR g) { DirectX::XMStoreFloat3(&m_localGravity, g); }


private:

	QuEntityPhysicsCollider* m_rigidBodyEntity;

	DirectX::XMFLOAT3 m_localGravity;
};

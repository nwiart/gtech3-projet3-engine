#pragma once

#include "QuEntity.h"

#include "Quantum/Physics/RigidBody.h"

class RigidBody;
class PhysicsShape;



class QuEntityPhysicsCollider : public QuEntity
{
public:

	QuEntityPhysicsCollider(float radius, EMotionType motionType);

	void applyImpulse(DirectX::FXMVECTOR f);

	void setPosition(DirectX::FXMVECTOR f);
	void setRotation(DirectX::FXMVECTOR f);

	virtual void onCollide(QuEntity* e) { }


	DirectX::XMVECTOR GetLinearVelocity();
	void setLinearVelocity(DirectX::FXMVECTOR p);

	virtual void OnSpawn(QuWorld* world) final override;
	virtual void OnUpdate(const Timer& timer) final override;
	virtual void OnDestroy(QuWorld* world) final override;


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;
	class MyCollideListener* m_listener;

	DirectX::XMFLOAT4 m_linearVelocity;

	float m_radius;
	EMotionType m_motionType;
};

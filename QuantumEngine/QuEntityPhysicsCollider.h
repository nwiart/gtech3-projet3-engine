#pragma once

#include "QuEntity.h"

#include "Quantum/Physics/RigidBody.h"

#include "QuEntityRenderModel.h"

class RigidBody;
class PhysicsShape;



class QuEntityPhysicsCollider : public QuEntity
{
public:

	QuEntityPhysicsCollider(float radius, EMotionType motionType);

	void applyImpulse(DirectX::FXMVECTOR f);


	virtual void OnSpawn(QuWorld* world) final override;
	virtual void OnUpdate(const Timer& timer) final override;
	virtual void OnDestroy(QuWorld* world) final override;

	inline void SetRenderModel(QuEntityRenderModel* renderModel) { m_renderModel = renderModel; }
	inline QuEntityRenderModel* GetRenderModel() { return m_renderModel; }

protected:
	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;

	DirectX::XMFLOAT4 m_linearVelocity;

	float m_radius;
	EMotionType m_motionType;

	QuEntityRenderModel* m_renderModel;
};

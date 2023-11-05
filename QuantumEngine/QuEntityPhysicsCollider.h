#pragma once

#include "QuEntity.h"

class RigidBody;
class PhysicsShape;



class QuEntityPhysicsCollider : public QuEntity
{
public:

	QuEntityPhysicsCollider();

	virtual void OnSpawn(QuWorld* world) final override;

	virtual void OnUpdate(const Timer& timer) final override;

	virtual void OnDestroy(QuWorld* world) final override;


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;
};

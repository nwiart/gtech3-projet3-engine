#pragma once

#include "QuEntity.h"

#include <vector>

class RigidBody;
class PhysicsShape;



class QuEntityPhysicsTriggerSphere : public QuEntity
{
	friend class MyTriggerListener;

public:

	QuEntityPhysicsTriggerSphere(float radius);

	virtual void onTriggerEnter(QuEntity* e) { }
	virtual void onTriggerStay(QuEntity* e) { }
	virtual void onTriggerLeave(QuEntity* e) { }


	virtual void OnSpawn(QuWorld* world) override;
	virtual void OnUpdate(const Timer& timer) override;
	virtual void OnDestroy(QuWorld* world) override;


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;

	class MyTriggerListener* m_listener;

	std::vector<RigidBody*> m_overlappingBodies;

	float m_radius;
};

#include "EntityGravityField.h"

#include "EntityGravityAffected.h"
#include <QuEntityPhysicsCollider.h>

using namespace DirectX;



void EntityGravityField::onTriggerEnter(QuEntity* e)
{
	EntityGravityAffected* g = e->findSubEntity<EntityGravityAffected>();
	if (g) {
		XMVECTOR diff = XMVectorSubtract(this->getWorldPosition(), g->getRigidBodyEntity()->getWorldPosition());
		diff = XMVector3Normalize(diff);
		diff = XMVectorMultiply(diff, XMVectorReplicate(m_intensity));

		g->setLocalGravity(diff);
	}
}

void EntityGravityField::onTriggerLeave(QuEntity* e)
{
	EntityGravityAffected* g = e->findSubEntity<EntityGravityAffected>();
	if (g) {
		g->setLocalGravity(XMVectorSet(-10, 0, 0, 0));
	}
}

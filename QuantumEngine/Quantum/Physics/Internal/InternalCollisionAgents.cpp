#include "Quantum/Physics/Physics.h"
#include "Quantum/Physics/Collide/CollisionAgent.h"

#include "Quantum/Physics/RigidBody.h"

#include "Quantum/Physics/Collide/PhysicsShapeSphere.h"

using namespace DirectX;



class CollisionAgent_SphereSphere : public CollisionAgent
{
public:

	virtual bool getOverlapping(RigidBody* a, RigidBody* b) const override
	{
		PhysicsShapeSphere* shapeA = reinterpret_cast<PhysicsShapeSphere*>(a->getShape());
		PhysicsShapeSphere* shapeB = reinterpret_cast<PhysicsShapeSphere*>(b->getShape());

		XMVECTOR posA = XMLoadFloat4(&a->getPosition());
		XMVECTOR posB = XMLoadFloat4(&b->getPosition());
		float dist = XMVectorGetX(XMVector3Length(XMVectorSubtract(posB, posA)));

		return dist < shapeA->getRadius() + shapeB->getRadius();
	}
};

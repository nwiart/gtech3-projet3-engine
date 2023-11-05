#include "PhysicsShapeSphere.h"

#include "Quantum/Math/AABB.h"



void PhysicsShapeSphere::getAabb(Quantum::AABB& out) const
{
	DirectX::XMVECTOR pr, nr;
	pr = DirectX::XMVectorReplicate(m_radius);
	nr = DirectX::XMVectorNegate(pr);

	out = Quantum::AABB(nr, pr);
}

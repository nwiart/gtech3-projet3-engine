#include "PhysicsShape.h"


#include "Quantum/Math/Math.h"
#include "Quantum/Math/AABB.h"

using namespace Quantum;
using namespace DirectX;



PhysicsShape::PhysicsShape(EShapeType t)
	: m_type(t)
{

}


void PhysicsShape::getAabb(AABB& out) const
{
	XMVECTOR zero = XMVectorZero();

	out = AABB(zero, zero);
}

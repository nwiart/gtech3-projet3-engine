#pragma once

#include "Quantum/Physics/Collide/PhysicsShape.h"



class PhysicsShapeSphere : public PhysicsShape
{
public:

	PhysicsShapeSphere(float radius)
		: PhysicsShape(SHAPE_SPHERE)
		, m_radius(radius)
	{

	}

	inline float getRadius() const { return m_radius; }

	virtual void getAabb(Quantum::AABB& out) const override;


private:

	float m_radius;
};

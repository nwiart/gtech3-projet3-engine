#pragma once

#include "Quantum/Physics/Collide/PhysicsShape.h"



class PhysicsShapeSphere : public PhysicsShape
{
public:

	PhysicsShapeSphere(float radius)
		: m_radius(radius)
	{

	}

	inline float getRadius() const { return m_radius; }


private:

	float m_radius;
};

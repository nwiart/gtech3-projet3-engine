#pragma once



class PhysicsShapeSphere
{
public:

	PhysicsShapeSphere(float radius);

	inline float getRadius() const { return m_radius; }


private:

	float m_radius;
};

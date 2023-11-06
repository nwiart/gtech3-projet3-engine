#pragma once

namespace Quantum { class AABB; }



enum EShapeType
{
	SHAPE_BOX,
	SHAPE_SPHERE,

	//SHAPE_CAPSULE,
	//SHAPE_CYLINDER,
	//SHAPE_CONVEX,
};



class PhysicsShape
{
public:

	PhysicsShape(EShapeType t);

	inline EShapeType getType() const { return m_type; }

	virtual void getAabb(Quantum::AABB& out) const;


private:

	EShapeType m_type;
};

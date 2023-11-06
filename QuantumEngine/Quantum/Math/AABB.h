#pragma once

#include "Quantum/Math/Math.h"



namespace Quantum
{


class AABB
{
public:

	AABB(DirectX::FXMVECTOR minPoint, DirectX::FXMVECTOR maxPoint);
	AABB(const AABB& other);

	void set(const AABB& other);

	void translate(DirectX::FXMVECTOR t);

		/// Rotates this AABB's points around the origin, and constructs another AABB
		/// containing the transformed points.
	void rotate(DirectX::FXMVECTOR quat);

	bool overlaps(const AABB& other) const;


	inline const DirectX::XMFLOAT3& getMin() const { return m_min; }
	inline const DirectX::XMFLOAT3& getMax() const { return m_max; }


	void operator=(const AABB& other);


private:

	DirectX::XMFLOAT3 m_min;
	DirectX::XMFLOAT3 m_max;
};



} // namespace Quantum

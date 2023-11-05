#pragma once

#include "Quantum/Math/Math.h"

class VertexBuffer;
class IndexBuffer;


class Model
{
public:

	Model();
	~Model();

	inline VertexBuffer* GetVertexBuffer() const { return VB; }
	inline IndexBuffer* GetIndexBuffer() const { return IB; }
	inline unsigned int GetNumberTriangle() const { return m_numTriangles; }

	void SetNumTriangle(unsigned int numTriangle);

	inline const DirectX::XMFLOAT3& getDimensionsMin() const { return m_dimensionsMin; }
	inline const DirectX::XMFLOAT3& getDimensionsMax() const { return m_dimensionsMax; }

	void setDimensions(DirectX::FXMVECTOR aabbMin, DirectX::FXMVECTOR aabbMax );



private:

	unsigned int m_numTriangles;

	VertexBuffer* VB;
	IndexBuffer* IB;

		/// Object-space AABB describing a rough bounding volume.
		/// Used for culling and collisions.
	DirectX::XMFLOAT3 m_dimensionsMin;
	DirectX::XMFLOAT3 m_dimensionsMax;
};

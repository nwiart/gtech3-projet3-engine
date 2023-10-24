#include "Transform.h"

using namespace Quantum;
using namespace DirectX;



Transform::Transform()
{
	this->setIdentity();
}

void Transform::setIdentity()
{
	m_position = { 0.0F, 0.0F, 0.0F };
	m_rotation = { 0.0F, 0.0F, 0.0F, 1.0F };
	m_scale    = { 1.0F, 1.0F, 1.0F };
}

const XMFLOAT4X4& Transform::toMatrix()
{
	static XMVECTOR zero = XMVectorZero();
	static XMVECTOR idqu = XMQuaternionIdentity();

	if (m_dirty) {
		XMVECTOR translate = XMLoadFloat3(&m_position);
		XMVECTOR rotate = XMLoadFloat4(&m_rotation);
		XMVECTOR scale = XMLoadFloat3(&m_scale);
		XMMATRIX result;

		m_dirty = false;
		result = XMMatrixTransformation(zero, idqu, scale, zero, rotate, translate);

		XMStoreFloat4x4(&m_cachedLocalTransform, result);
	}

	return m_cachedLocalTransform;
}

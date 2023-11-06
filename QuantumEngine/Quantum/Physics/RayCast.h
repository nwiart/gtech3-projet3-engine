#pragma once



struct Ray
{
	DirectX::XMFLOAT4 m_directionAndLength;
	DirectX::XMFLOAT3 m_origin;
};

struct RayHitResult
{
	class RigidBody* m_collider;

	DirectX::XMFLOAT3 m_hitLocation;
	float m_hitParameter;
};

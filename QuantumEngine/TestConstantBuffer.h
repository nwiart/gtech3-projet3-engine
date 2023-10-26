#pragma once

#include <DirectXMath.h>


struct TestConstantBuffer
{
	DirectX::XMFLOAT4X4 viewProjection; /// Combined view & projection matrices (camera).

	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4 cameraDir;

	DirectX::XMFLOAT4 DirColors;
	DirectX::XMFLOAT4 DirDirection;
};

struct ObjectConstantBuffer
{
	DirectX::XMFLOAT4X4 world;   /// World space transform of the object.
};

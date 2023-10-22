#pragma once

#include <DirectXMath.h>


struct TestConstantBuffer
{
	DirectX::XMMATRIX viewProjection; /// Combined view & projection matrices (camera).
};

struct ObjectConstantBuffer
{
	DirectX::XMMATRIX world;   /// World space transform of the object.
};

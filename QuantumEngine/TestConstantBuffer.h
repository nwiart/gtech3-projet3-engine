#pragma once

#include <DirectXMath.h>


#define MAX_POINT_LIGHT 8

struct TestConstantBuffer
{
	DirectX::XMFLOAT4X4 viewProjection; /// Combined view & projection matrices (camera).

	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4 cameraDir;

	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DirColors;
	DirectX::XMFLOAT4 DirDirection;

	DirectX::XMFLOAT4 PointLightPosition[MAX_POINT_LIGHT];
	DirectX::XMFLOAT4 PointLightColor[MAX_POINT_LIGHT];
	DirectX::XMFLOAT4 PointLightAttenuation[MAX_POINT_LIGHT];
};

struct ObjectConstantBuffer
{
	DirectX::XMFLOAT4X4 world;   /// World space transform of the object.

	DirectX::XMFLOAT4X4 pad0;
	DirectX::XMFLOAT4X4 pad1;
	DirectX::XMFLOAT4X4 pad2;
};

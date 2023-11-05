#include "FrustumTest.h"

using namespace DirectX;



void frustumBuild(Frustum& frustum, FXMVECTOR cameraPos, FXMVECTOR cameraTarget, FXMVECTOR cameraRight, GXMVECTOR cameraUp, float fov, float aspect)
{
	float halfVertical = tan(XMConvertToRadians(fov * 0.5F));
	float halfHorizontal = halfVertical * aspect;

	XMVECTOR cameraForward = XMVectorSubtract(cameraTarget, cameraPos);
	XMVECTOR cameraHalfRight = XMVectorScale(cameraRight, halfHorizontal);
	XMVECTOR cameraHalfUp = XMVectorScale(cameraUp, halfVertical);

	frustum[0] = XMPlaneFromPointNormal(cameraPos, XMVector3Normalize(XMVector3Cross(cameraUp, cameraForward + cameraHalfRight)));
	frustum[1] = XMPlaneFromPointNormal(cameraPos, XMVector3Normalize(XMVector3Cross(cameraForward + cameraHalfUp, cameraRight)));
	frustum[2] = XMPlaneFromPointNormal(cameraPos, XMVector3Normalize(XMVector3Cross(cameraForward - cameraHalfRight, cameraUp)));
	frustum[3] = XMPlaneFromPointNormal(cameraPos, XMVector3Normalize(XMVector3Cross(cameraRight, cameraForward - cameraHalfUp)));
}

bool frustumSphereIntersect(const Frustum& frustum, FXMVECTOR sphereCenter, float sphereRadius)
{
	// Test each plane if sphere is lying entirely on its positive side.
	for (int i = 0; i < 4; ++i) {
		float dist = XMVectorGetX(XMPlaneDot(frustum[i], sphereCenter));
		if (dist > sphereRadius) {
			return false;
		}
	}

	// Sphere intersects.
	return true;
}

#pragma once

#include "Quantum/Math/Math.h"



// Not counting zNear and zFar planes.
// We're not storing the near and far planes, as we just assume the frustum extends to infinity.
typedef DirectX::XMVECTOR Frustum[4];



void frustumBuild(Frustum& out, DirectX::FXMVECTOR cameraPos, DirectX::FXMVECTOR cameraTarget, DirectX::FXMVECTOR cameraRight, DirectX::GXMVECTOR cameraUp, float fov, float aspect);

bool frustumSphereIntersect(const Frustum& frustum, DirectX::FXMVECTOR sphereCenter, float sphereRadius);

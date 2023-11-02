#pragma once
#include "QuEntity.h"
#include "Graphics.h"

class QuEntityCamera : public QuEntity
{
public:

	Graphics* graphics;

	QuEntityCamera();

	void setCamera(float camYaw, float camPitch, int renderWidth, int renderHeight);

	float m_camYaw;
	float m_camPitch;
	int m_renderWidth;
	int m_renderHeight;
};

#include "QuEntityCamera.h"

QuEntityCamera::QuEntityCamera()
{
}

void QuEntityCamera::setCamera(float camYaw, float camPitch, int renderWidth, int renderHeight)
{
	m_camYaw = camYaw;
	m_camPitch = CamPitch;
	m_renderWidth = renderWidth;
	m_renderHeight = renderHeight;
}

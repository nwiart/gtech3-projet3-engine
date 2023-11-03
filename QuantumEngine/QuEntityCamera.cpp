#include "stdafx.h"
#include "QuEntityCamera.h"

#include "Graphics.h"



QuEntityCamera::QuEntityCamera()
	: m_fov(70.0F)
{

}

void QuEntityCamera::ExecuteProcedure()
{
	Graphics::getInstance().setCamera(this);
}

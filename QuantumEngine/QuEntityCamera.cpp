#include "stdafx.h"
#include "QuEntityCamera.h"

#include "Graphics.h"



QuEntityCamera::QuEntityCamera()
{

}

void QuEntityCamera::ExecuteProcedure()
{
	Graphics::getInstance().setCamera(this);
}

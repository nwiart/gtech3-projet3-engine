#include "stdafx.h"
#include "QuEntityRenderSkybox.h"

#include "Graphics.h"



QuEntityRenderSkybox::QuEntityRenderSkybox()
{
	m_texture = 0;
}

void QuEntityRenderSkybox::ExecuteProcedure()
{
	Graphics::getInstance().setSkybox(this);
}

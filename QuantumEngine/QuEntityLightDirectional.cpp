#include "QuEntityLightDirectional.h"
#include "stdafx.h"
#include "Graphics.h"


QuEntityLightDirectional::QuEntityLightDirectional()
{
	m_ambientColor = 0x102038ff;
	m_color.fromFloat4(1.0F, 0.85F, 0.7F, 1.0F);

	m_intensity = 1.0F;
}

QuEntityLightDirectional::~QuEntityLightDirectional()
{
}

void QuEntityLightDirectional::ExecuteProcedure()
{
	Graphics::getInstance().setDirectionalLight(this);
}

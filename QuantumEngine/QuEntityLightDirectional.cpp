#include "QuEntityLightDirectional.h"
#include "stdafx.h"
#include "Graphics.h"


QuEntityLightDirectional::QuEntityLightDirectional()
{
	m_ColorR = 1.0F;
	m_ColorG = 0.85F;
	m_ColorB = 0.7F;
	m_Intensity = 1.0F;

	m_ambientColor = 0x102038;
}

QuEntityLightDirectional::~QuEntityLightDirectional()
{
}

void QuEntityLightDirectional::ExecuteProcedure()
{
	Graphics::getInstance().UpdateDirectionalLight(this);
}

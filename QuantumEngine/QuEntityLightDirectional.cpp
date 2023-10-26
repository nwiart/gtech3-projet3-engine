#include "QuEntityLightDirectional.h"
#include "stdafx.h"
#include "Graphics.h"

QuEntityLightDirectional::QuEntityLightDirectional()
{
	m_ColorR = 0.8F;
	m_ColorG = 0.4F;
	m_ColorB = 0.2F;
	m_Intensity = 1.0F;

}

QuEntityLightDirectional::~QuEntityLightDirectional()
{
}

void QuEntityLightDirectional::ExecuteProcedure()
{
	Graphics::getInstance().UpdateDirectionalLight(this);
	this->GetTransform().ApplyRotation(XMQuaternionRotationRollPitchYaw(0, 0.1, 0));

}

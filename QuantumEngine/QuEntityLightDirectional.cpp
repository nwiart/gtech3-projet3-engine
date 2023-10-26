#include "QuEntityLightDirectional.h"
#include "stdafx.h"
#include "Graphics.h"

QuEntityLightDirectional::QuEntityLightDirectional()
{
	m_ColorR = 1.0F;
	m_ColorG = .0F;
	m_ColorB = .0F;
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

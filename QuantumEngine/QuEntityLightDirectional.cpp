#include "QuEntityLightDirectional.h"
#include "stdafx.h"
#include "Graphics.h"

QuEntityLightDirectional::QuEntityLightDirectional(float R, float G, float B, float I)
{
	m_ColorR = R;
	m_ColorG = G;
	m_ColorB = B;
	m_Intensity = I;

}

QuEntityLightDirectional::~QuEntityLightDirectional()
{
}

void QuEntityLightDirectional::ExecuteProcedure()
{
	Graphics::getInstance().UpdateDirectionalLight(this);
	this->GetTransform().ApplyRotation(XMQuaternionRotationRollPitchYaw(0, 0, 0));

}

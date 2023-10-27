#pragma once

#include "QuEntity.h"

class QuEntityLightDirectional : public QuEntity
{

public:
	QuEntityLightDirectional(float,float,float,float);
	~QuEntityLightDirectional();

	float getColorR() { return m_ColorR; }
	float getColorG() { return m_ColorG; }
	float getColorB() { return m_ColorB; }
	float getIntensity() { return m_Intensity; }

	virtual void ExecuteProcedure();

private:
	float m_ColorR;
	float m_ColorG;
	float m_ColorB;

	float m_Intensity;


};



#pragma once

#include "QuEntity.h"

class QuEntityPointLIght : public QuEntity
{

public:
	QuEntityPointLIght();
	~QuEntityPointLIght();

	float getColorR() { return m_ColorR; }
	float getColorG() { return m_ColorG; }
	float getColorB() { return m_ColorB; }
	float getIntensity() { return m_Intensity; }

	unsigned int getAmbientColor() const { return m_ambientColor; }

	virtual void ExecuteProcedure();

private:
	float m_ColorR;
	float m_ColorG;
	float m_ColorB;

	float m_Intensity;

	unsigned int m_ambientColor;
};


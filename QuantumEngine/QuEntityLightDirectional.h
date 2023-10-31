#pragma once

#include "QuEntity.h"

#include "Quantum/Types/Color.h"


class QuEntityLightDirectional : public QuEntity
{
public:

	QuEntityLightDirectional();
	~QuEntityLightDirectional();

	const Quantum::Color& getAmbientColor() const { return m_ambientColor; }
	const Quantum::Color& getColor() const { return m_color; }
	float getIntensity() const { return m_intensity; }

	void setAmbientColor(const Quantum::Color& c) { m_ambientColor = c; }
	void setColor(const Quantum::Color& c) { m_color = c; }
	void setIntensity(float i) { m_intensity = i; }

private:

	virtual void ExecuteProcedure();


private:

	Quantum::Color m_ambientColor;
	Quantum::Color m_color;

	float m_intensity;
};


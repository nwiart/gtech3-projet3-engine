
#pragma once

#include "QuEntity.h"
#include "Quantum/Types/Color.h"

class QuEntityLightPoint : public QuEntity
{

public:

	QuEntityLightPoint();
	~QuEntityLightPoint();

	const Quantum::Color& getColor() const { return m_color; }
	float getIntensity() const { return m_intensity; }
	DirectX::XMFLOAT3 getAttenuation() const { return m_attenuation; }
	

	void setColor(const Quantum::Color& c) { m_color = c; }
	void setIntensity(float i) { m_intensity = i; }


private:


	virtual void ExecuteProcedure();


private:

	DirectX::XMFLOAT3 m_attenuation;
	Quantum::Color m_color;

	float m_intensity;
};


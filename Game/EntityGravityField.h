#pragma once

#include <QuEntityPhysicsTriggerSphere.h>



class EntityGravityField : public QuEntityPhysicsTriggerSphere
{
public:

	EntityGravityField(float intensity)
		: QuEntityPhysicsTriggerSphere(10.0F), m_intensity(intensity)
	{

	}

	virtual void onTriggerStay(QuEntity* e) override;

	virtual void onTriggerLeave(QuEntity* e) override;


private:

		/// Force of attraction in m/s�.
	float m_intensity;
};

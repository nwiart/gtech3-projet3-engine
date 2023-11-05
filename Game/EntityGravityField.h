#pragma once

#include <QuEntityPhysicsTriggerSphere.h>



class EntityGravityField : public QuEntityPhysicsTriggerSphere
{
public:

	EntityGravityField(float intensity);

	virtual void onTriggerEnter(QuEntity* e) override;

	virtual void onTriggerLeave(QuEntity* e) override;


private:

		/// Force of attraction in m/s².
	float m_intensity;
};

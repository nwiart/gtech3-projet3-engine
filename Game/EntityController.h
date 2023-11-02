#pragma once

#include "QuEntityCamera.h"


class EntityController : public QuEntityCamera
{
public:

	EntityController();

	void OnUpdate(Timer timer) override;

	void UpdateCamera(float dt);


private:

	float m_camYaw;
	float m_camPitch;

	int mouseLastStateX;
	int mouseLastStateY;
};

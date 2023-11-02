#pragma once

#include "QuEntityCamera.h"


class EntityController : public QuEntityCamera
{
public:

	EntityController();

	void OnUpdate(Timer timer) override;

	static inline EntityController& Get() { static EntityController m_entityControl; return m_entityControl; }

	void UpdateCamera(float dt);

	float m_camYaw;
	float m_camPitch;

	int mouseLastStateX;
	int mouseLastStateY;
};

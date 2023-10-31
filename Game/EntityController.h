#pragma once
#include "InputCallback.h"

class EntityController : public InputCallback
{
public:

	int mouseLastStateX;
	int mouseLastStateY;

	//Graphics* m_graphics;

	void OnKeyDown(WPARAM wparam) override;
	void SetCamera();

	float camYaw = 0.0f;
	float camPitch = 0.0f;
};


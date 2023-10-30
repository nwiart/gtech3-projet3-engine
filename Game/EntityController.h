#pragma once
#include "InputCallback.h"

class EntityController : public InputCallback
{
public:

	EntityController();

	int mouseLastStateX;
	int mouseLastStateY;

	Graphics* m_graphics;

	void OnKeyDown(WPARAM wparam) override;
	void SetCamera(Graphics* graphics);
};

